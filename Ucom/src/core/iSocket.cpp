#include "stdafx.h"
#include "iSocket.h"

// :CAsyncSocket()
iSocket::iSocket()
{
	AfxSocketInit();
	// 初始为1个当超过后再动态分配
	nClient.SetSize(0,1);
}


iSocket::~iSocket()
{
}

// 从客户数据列表里面移除
void iSocket::DelFromClient(nSocket *mclient)
{
	int size = nClient.GetSize();
	for (int i = 0; i < size; i++) {
		if (nClient[i] == mclient) {
			delete nClient[i];
			nClient.RemoveAt(i);
			// 再无客户端关闭可读写操作
			if (size == 1) {
				isSocketOpen = false;
			}
			return;
		}
	}
}

// 从客户数据列表里面移除
void iSocket::DelAllClient(void)
{
	int size = nClient.GetSize();
	// 关闭可读写操作
	isSocketOpen = false;
	for (int i = 0; i < size; i++)
	{
		nClient[i]->Close();
		delete nClient[i];
	}

	nClient.RemoveAll();
}

void iSocket::OnReceive(int nErrorCode)
{
	::PostMessage(hwTopParent, WM_MYONRECVMSG, W_NET_RECV, 0);
	CAsyncSocket::OnReceive(nErrorCode);
}


// Server模式下客户端连入
void iSocket::OnAccept(int nErrorCode)
{
	TRACE("On Accept:%d\n", nErrorCode);
	if (!nErrorCode) {
		// 只要有就设置为开启
		isSocketOpen = true;

		//构造一个新的空套接字
		nSocket *sockRecv = new nSocket(hwParent, hwTopParent);
		sockRecv->SetSocketHandle(sockRecv);
		//接收连接，若失败删除new的套接字
		if (0 == Accept(*sockRecv))
		{
			delete sockRecv;
		}
		else
		{
			::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_SUBNET | WL_UCOM_OPEN, 0);
		}
	}
	CAsyncSocket::OnAccept(nErrorCode);
}

void iSocket::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		PostMsgIsOpen(true);
	}
	else
	{
		CString errorStr;
		// 发送关闭消息，并关闭
		PostMsgIsOpen(false);
		Close();
		switch (nErrorCode)
		{
		case WSAEADDRINUSE:
			errorStr = _T("地址被占用");break;
		case WSAEADDRNOTAVAIL:
			errorStr = _T("地址本机不可用");break;
		case WSAECONNREFUSED:
			errorStr = _T("连接被拒绝");break;
		case WSAEDESTADDRREQ:
			errorStr = _T("未输入地址");break;
		case WSAEFAULT:
			errorStr = _T("无效地址");break;
		case WSAEINVAL:
			errorStr = _T("无效参数");break;
		case WSAEISCONN:
			errorStr = _T("连接被占用");break;
		case WSAENETUNREACH:
			errorStr = _T("地址不可及"); break;
		case WSAENOBUFS:
			errorStr = _T("套接字内存不足"); break;
		case WSAENOTCONN:
			errorStr = _T("未建立连接\n");break;
		case WSAETIMEDOUT:
			errorStr = _T("建立连接超时"); break;
		default:
			errorStr.Format("错误码-%d", nErrorCode);
			break;
		}
		AfxMessageBox(("连接失败:"+errorStr));
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void iSocket::OnClose(int nErrorCode)
{
	PostMsgIsOpen(false);
}

void iSocket::PostMsgIsOpen(bool isOpen)
{
	isSocketOpen = isOpen;
	if (isOpen)
	{
		::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_NET | WL_UCOM_OPEN, 0);
		::SendMessage(hwParent, WM_NETDLG_MSG, WL_UCOM_OPEN, 0);
	}
	else
	{
		::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_NET | WL_UCOM_CLOSE, 0);
		::SendMessage(hwParent, WM_NETDLG_MSG, WL_UCOM_CLOSE, 0);
	}
}

int iSocket::UnblockRead(CString & dataStr)
{
	TCHAR buff[ISOCKET_READBUFF_MAX];
	int nRead;
	nRead = Receive(buff, ISOCKET_READBUFF_MAX);
	switch (nRead)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("Socket读取错误!"));
			Close();
		}
		break;
	default:
		dataStr.GetBufferSetLength(nRead);
		memcpy(dataStr.GetBuffer(0), buff, nRead);
	}
	return nRead;
}


int iSocket::UnblockRead(CString & dataStr, CString & infoStr)
{
	TCHAR buff[ISOCKET_READBUFF_MAX];
	int nRead;
	UINT nPort;
	nRead = ReceiveFrom(buff, ISOCKET_READBUFF_MAX, infoStr, nPort);
	infoStr.Format("\n【From: %s:%d】\n", infoStr, nPort);
	switch (nRead)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("Socket读取错误!"));
			Close();
		}
		break;
	default:
		dataStr.GetBufferSetLength(nRead);
		memcpy(dataStr.GetBuffer(0), buff, nRead);
	}
	return nRead;
}


/* nSocket */

nSocket::~nSocket()
{
}

void nSocket::OnReceive(int nErrorCode)
{
	// 多实例会干扰
	::PostMessage(hwTopParent, WM_MYONRECVMSG, W_SUBNET_RECV, (LPARAM)&mPara);
}

void nSocket::OnSend(int nErrorCode)
{
	TRACE("Frist Sub Send\n");
	//第一次连接时候都会有一次send触发，这时候获得ip和端口，关闭send消息触发
	//这权且当连接成功的消息处理
	GetClientInfo();
	AsyncSelect(FD_READ | FD_CONNECT | FD_CLOSE | FD_ACCEPT);
	PostMsgIsOpen(true);
}


void nSocket::OnConnect(int nErrorCode)
{
	PostMsgIsOpen(!nErrorCode);
}

void nSocket::OnClose(int nErrorCode)
{
	PostMsgIsOpen(false);
}

void nSocket::GetClientInfo(void)
{
	UINT port;
	GetPeerName(mPara.strIP, port);
	mPara.strIP.Format("%s:%d", mPara.strIP, port);
}

void nSocket::PostMsgIsOpen(bool isOpen)
{
	if (isOpen)
	{
		::SendMessage(hwParent, WM_NETDLG_MSG, W_SUBNET_OPEN, (LPARAM)&mPara);
	}
	else
	{
		::SendMessage(hwParent, WM_NETDLG_MSG, W_SUBNET_CLOSE, (LPARAM)&mPara);
	}
}

int nSocket::UnblockRead(CString & dataStr)
{
	TCHAR buff[ISOCKET_READBUFF_MAX];
	int nRead;
	nRead = Receive(buff, ISOCKET_READBUFF_MAX);
	switch (nRead)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("Socket读取错误!"));
			Close();
		}
		break;
	default:
		dataStr.GetBufferSetLength(nRead);
		memcpy(dataStr.GetBuffer(0), buff, nRead);
	}
	return nRead;
}

