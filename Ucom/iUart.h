#pragma once

#define WM_COMM_RX_MSG  (WM_USER + 2)

typedef struct
{
	//串口句柄
	HANDLE *commHandle;
	//线程终止标志
	int stopFlag;
}ThreadPara;

///线程请求
//主动：请求退出
#define RT_REQ_EXIT 0
//被动：可以退出
#define RT_PRE_EXIT 1
//被动：不可以退出
#define RT_NOT_EXIT 2
//被动：退出成功
#define RT_SUC_EXIT 3


class iUart
{
private:
	HANDLE hUartCom;
	DCB uartConfig;
	CString ComName;

	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;
	ThreadPara mThreadPara;
	void CleanCommErr(void);
public:

	iUart(){
		hUartCom = NULL;
		mThreadPara.commHandle = &hUartCom;
	}
	~iUart();

	void ClosePort(void){
		CloseHandle(hUartCom);
		hUartCom = NULL;
		TRACE("Close PortHandle\n");
	}

	bool OpenCom(bool isBlockMode = true);
	bool ConfigUart(CString comName, DCB mConfig);

	void GetComList(CComboBox *cblist);

	int WriteCString(const CString &cBuffer);
	CString ReadCString(void);

	int UnblockRead(CString &dataStr);
	int UnblockSend(const CString &dataStr);

	HANDLE GetHandle(void){
		return hUartCom;
	}
	HANDLE* GetHandleAddr(void){
		return &hUartCom;
	}

	bool isConnected(void){
		if (hUartCom == NULL)
			return false;
		else
			return true;
	}
	ThreadPara *GetThreadStartPara(void){
		mThreadPara.stopFlag = RT_PRE_EXIT;
		return &mThreadPara;
	}

};


UINT RxThreadFunc(LPVOID mThreadPara);

