#include "stdafx.h"
#include "iUart.h"

#include "DeConsole.h"

#define XON 0x11
#define XOFF 0x13

iUart::~iUart()
{
	if (isConnected())
		CloseHandle(hUartCom);
}


//以文件方式打开串口
bool iUart::OpenCom(bool isBlockMode){
	HANDLE hCom;

	//打开10以上的串口
	ComName = "\\\\.\\"+ComName;
	if (isBlockMode)
	{
		hCom = CreateFile(ComName,//COM口  
			GENERIC_READ | GENERIC_WRITE, //允许读和写  
			0, //独占方式  
			NULL,
			OPEN_EXISTING, //打开而不是创建  
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,//异步方式
			NULL);

		//设置非堵塞参数
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		//设置非堵塞参数
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	else
	{
		hCom = CreateFile(ComName,//COM口  
			GENERIC_READ | GENERIC_WRITE, //允许读和写  
			0, //独占方式  
			NULL,
			OPEN_EXISTING, //打开而不是创建  
			0, //同步方式
			NULL);
	}


	//获取失败
	if (hCom == INVALID_HANDLE_VALUE)
	{
		TRACE("Open %s Fail\n",ComName);
		return false;
	}

	SetCommMask(hCom, EV_RXCHAR | EV_TXEMPTY);//设置事件驱动的类型
	SetupComm(hCom, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是1024

	COMMTIMEOUTS TimeOuts;
	//设定读超时  
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//在读一次输入缓冲区的内容后读操作就立即返回，  
	//而不管是否读入了要求的字符
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时
	hUartCom=hCom;
	TRACE("h:%p\n",hUartCom);
	
	/* 串口配置结构体 */
	DCB dcb;
	GetCommState(hCom, &dcb);
	///设置详情参看DCB串口结构体
	dcb.BaudRate = uartConfig.BaudRate;
	dcb.ByteSize = uartConfig.ByteSize;
	dcb.Parity = uartConfig.Parity;
	dcb.StopBits = uartConfig.StopBits;

	TRACE("open: baud:%d,size:%d,stop:%d,ecc:%d\n", uartConfig.BaudRate,
		uartConfig.ByteSize, uartConfig.StopBits, uartConfig.Parity);
	
	//是否监控CTS(clear-to-send)信号来做输出流控
	//当设置为true时： 若CTS为低电平，则数据发送将被挂起，直至CTS变为高
	dcb.fOutxCtsFlow = false;
	
	// 本机DSR<-对方DTR
	// 本机DTR->对方DSR
	// 本机RTS->对方CTS
	// 本机CTS<-对方RTS
	//RTS_CONTROL_DISABLE/ENABLE打开设备时置RTS信号为什么电平
	//可以调用EscapeCommFunction函数来改变RTS线电平状态
	//RTS_CONTROL_HANDSHAKE 允许RTS信号握手，此时应用程序不能调用EscapeCommFunction函数
	//有空间接收数据时，驱动程序置RTS为高以接收，否则置低
	//RTS_CONTROL_TOGGLE 有字节要发送时RTS变高，当所有缓冲字节已被发送完毕后，RTS变低
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	//RTS的改变影响对方的CTS
	//当设置为true时若CTS为低电平(对方RTS低)则数据发送将被挂起，直至CTS变为高
	dcb.fOutxCtsFlow = false;
	
	//本机DTR->对方DSR，使用和RTS和RTS一样
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fOutxDsrFlow = false;

	// 设置流控制信号 SETRTS,CLRRTS,SETDTR,CLRDTR
	// EscapeCommFunction(mUart.GetHandle(), SETRTS);
	// 读取需要GetCommMask设置，然后WaitCommEvent等待，需要开一个线程


	//指定是否允许二进制模式，Win32 API不支持非二进制模式传输，应设true 
	dcb.fBinary = true;
	//接收0字符，为TRUE时，接收时自动去掉空（0值）字节 
	dcb.fNull = false;

	//关掉输入输出的 XON/XOFF流量控制
	dcb.fInX = false;
	dcb.fOutX = false;
	//dcb.XonChar = XON; //指定XON，请求发送方继续发送时的字符 0x11  
	//dcb.XoffChar = XOFF; //指定XOFF，请求发送方停止发送时的字符 0x13  
	//dcb.XonLim = 50;
	//dcb.XoffLim = 50;

	if (SetCommState(hUartCom, &dcb) == 0)
	{
		TRACE("Com Config fail\n");
		ClosePort();
		return false;
	}

	//清除接收和发送缓冲区
	PurgeComm(hUartCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return true;
}

bool iUart::ConfigUart(CString comName,DCB mConfig)
{
	ComName = comName;

	uartConfig.BaudRate = mConfig.BaudRate;
	uartConfig.ByteSize = mConfig.ByteSize;
	uartConfig.Parity = mConfig.Parity;
	uartConfig.StopBits = mConfig.StopBits;

	return true;
}


void iUart::GetComList(CComboBox *cblist)
{
	HKEY hKey;
	int   i = 0;
	TCHAR   portName[256], commName[256];
	DWORD   dwLong, dwSize;
	// 根据注册表来刷新
	//打开串口注册表  
	int rtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey);
	if (rtn == ERROR_SUCCESS)
	{
		while (TRUE)
		{
			dwLong = dwSize = sizeof(portName);
			memset(portName, 0, sizeof(portName));
			memset(commName, 0, sizeof(commName));

			rtn = RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize);
			//枚举串口
			if (rtn == ERROR_NO_MORE_ITEMS)
				break; 
			//插入列表
			cblist->AddString(commName);
			i++;
		}

		RegCloseKey(hKey);
	}
	//添加刷新串口选项
	cblist->AddString("刷新串口");
}

int iUart::WriteCString(const CString &cBuffer)
{
	unsigned long dwBytesWrite;
	BOOL bWriteStat;

	if (!isConnected())
		return false;

	bWriteStat = WriteFile(hUartCom, cBuffer, cBuffer.GetAllocLength(), &dwBytesWrite, NULL);
	//错误返回 -1
	if (!bWriteStat)
		return -1;
	//返回发送字节
	return dwBytesWrite;
}


CString iUart::ReadCString(void)
{
	unsigned long dwBytesRead;
	BOOL bReadStat;
	COMSTAT ComStat;
	DWORD dwErrorFlags;

	CString dataStr;
	dataStr.Empty();

	if (!isConnected())
	{
		TRACE("Read:no connect\n");
		return dataStr;
	}

	ClearCommError(hUartCom, &dwErrorFlags, &ComStat);
	//cbInQue返回在串行驱动程序输入队列中的字符数
	dwBytesRead = ComStat.cbInQue;

	if (dwBytesRead == 0)
	{
		TRACE("Read:no data\n");
		return dataStr;
	}

	dataStr.GetBufferSetLength(dwBytesRead);
	bReadStat = ReadFile(hUartCom, dataStr.GetBuffer(0), dwBytesRead, &dwBytesRead, NULL);
	if (!bReadStat)
	{
		TRACE("Read:read failed\n");
		dataStr.Empty();
		return dataStr;
	}

	PurgeComm(hUartCom, PURGE_RXABORT | PURGE_RXCLEAR);
	return dataStr;
}


UINT RxThreadFunc(LPVOID mThreadPara)
{
	OVERLAPPED os;
	DWORD dwMask, dwTrans;
	COMSTAT ComStat;
	DWORD dwErrorFlags;

	ThreadPara *pPara = (ThreadPara *)mThreadPara;
	HANDLE hComm = *(pPara->commHandle);
	if (hComm == NULL){
		TRACE("Handle not correct\n");
		return -1;
	}

	memset(&os, 0, sizeof(OVERLAPPED));
	os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (os.hEvent == NULL)
	{
		TRACE("hEvent Failed\n");
		return (UINT)-1;
	}
	TRACE("Rx Listenner Thread Start\n");

	while (1)
	{
		ClearCommError(hComm, &dwErrorFlags, &ComStat);
	
		if (ComStat.cbInQue)
		{
			///有数据发送消息给UI线程
			//发送消息过程中触发线程退出，错误?
			::PostMessage(HWND_BROADCAST, WM_MYONRECVMSG, W_UART_RECV, 0);
			//::SendMessage(::AfxGetMainWnd()->m_hWnd, WM_COMM_RX_MSG, 1, 0);
		}
		dwMask = 0;
		//等待事件
		if (!WaitCommEvent(hComm, &dwMask, &os))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				GetOverlappedResult(hComm, &os, &dwTrans, TRUE);
			}
			else
			{
				CloseHandle(os.hEvent);
				return(UINT)-1;
			}
		}

	}
	CloseHandle(os.hEvent);
	TRACE("Rx Listenner Thread Stop\n");
	return EXIT_SUCCESS;
}


//配合线程方式的读取
int iUart::UnblockRead(CString &dataStr)
{
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	DWORD dwBytesRead;
	BOOL bReadStatus;

	ClearCommError(hUartCom, &dwErrorFlags, &ComStat);
	//cbInQue返回在串行驱动程序输入队列中的字符数

	dwBytesRead = ComStat.cbInQue;
	/* 关闭时候的一次读取 无符号dwBytesRead会特别大GetBufferSetLength出现参数错误*/
	if ((int)dwBytesRead <= 0)
		return 0;
	dataStr.GetBufferSetLength(dwBytesRead);
	//读取
	bReadStatus = ReadFile(hUartCom, dataStr.GetBuffer(0), dwBytesRead, &dwBytesRead, &m_osRead);

	if (!bReadStatus)
	{
		//如果重叠操作未完成,等待直到操作完成
		if (GetLastError() == ERROR_IO_PENDING)
		{
			GetOverlappedResult(hUartCom, &m_osRead, &dwBytesRead, TRUE);
			m_osRead.Offset = 0;
		}
		else
		{
			dwBytesRead = 0;
		}
	}
	//返回读取个数
	return dwBytesRead;
}


int iUart::UnblockSend(const CString &dataStr)
{
	BOOL bWriteStatus;
	COMSTAT ComStat;
	DWORD dwErrorFlags, dwLength;

	ClearCommError(hUartCom, &dwErrorFlags, &ComStat);
	if (dwErrorFlags>0)
	{
		TRACE("Unblock Write Failed\n");
		PurgeComm(hUartCom, PURGE_TXABORT | PURGE_TXCLEAR);
		return 0;
	}
	m_osWrite.Offset = 0;

	//dwLength = dataStr.GetAllocLength();
	//append方式添加就得GetLength才能的出来正确计数（GetAllocLength有bug），好像GetLength不是以'\0'来算的
	dwLength = dataStr.GetLength();
	bWriteStatus = WriteFile(hUartCom, (LPCTSTR)dataStr, dwLength, &dwLength, &m_osWrite);

	if (!bWriteStatus)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			//如果重叠操作未完成,等待直到操作完成
			GetOverlappedResult(hUartCom, &m_osWrite, &dwLength, TRUE);
		}
		else
			dwLength = 0;
	}
	return dwLength;
}
