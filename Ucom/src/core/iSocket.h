#pragma once

#include <afxsock.h>
#include "UBase.h"
#include "DeConsole.h"

#define ISOCKET_READBUFF_MAX 4086

typedef struct
{
	CString strIP;
	void *hSocket;
} nSocketPara;


// client客户端类，用于ISocket的server模式的动态创建
class nSocket :public CAsyncSocket
{
private:
	HWND hwParent;
	HWND hwTopParent;
	nSocketPara mPara;
public:
	nSocket(HWND pParent, HWND pTopParent) {
		hwParent = pParent;
		hwTopParent = pTopParent;
	}
	~nSocket();

	void SetSocketHandle(void *hSock) {
		mPara.hSocket = hSock;
	}
	void OnReceive(int nErrorCode);

	void OnSend(int nErrorCode);

	void OnConnect(int nErrorCode);

	void OnClose(int nErrorCode);

	void PostMsgIsOpen(bool isOpen);
	void GetClientInfo(void);
	int UnblockRead(CString &dataStr);
};

// 连接到本地server的client类
class iSocket :public CAsyncSocket
{
private:
	//是否可以读写操作
	bool isSocketOpen;
	//父窗口句柄用于向非主窗口发送消息
	HWND hwParent;
	HWND hwTopParent;
public:
	iSocket();
	~iSocket();
	CArray<nSocket *> nClient;
	void SetParentHWND(HWND pParent, HWND pTopParent) {
		hwParent = pParent;
		hwTopParent = pTopParent;
	}
	void DelFromClient(nSocket *mclient);
	void DelAllClient(void);

	void OnReceive(int nErrorCode);

	void OnAccept(int nErrorCode);

	void OnConnect(int nErrorCode);

	void OnClose(int nErrorCode);

	void PostMsgIsOpen(bool isOpen);
	int UnblockRead(CString &dataStr);
	int UnblockRead(CString & dataStr, CString & infoStr);
	int UnblockSend(const CString & dataStr)
	{
		return Send((LPCTSTR)dataStr, dataStr.GetLength());
	}

	bool IsScoketOpen(void) {
		return isSocketOpen;
	}
	// 主要用于UDP情况下，需要面板手动打开关闭
	void SetIsOpen(bool isopen) {
		isSocketOpen = isopen;
	}
};