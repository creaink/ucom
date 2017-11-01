#pragma once

#include <afxsock.h>
#include "UBase.h"
#include "DeConsole.h"


typedef struct
{
	CString strIP;
	void *hSocket;
} nSocketPara;

class nSocket :public CAsyncSocket
{
private:
	HWND hwParent;
	nSocketPara mPara;
public:
	nSocket(HWND pParent);
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
	//int UnblockSend(const CString & dataStr)
	//{
	//	return Send((LPCTSTR)dataStr, dataStr.GetLength());
	//}
};


class iSocket :public CAsyncSocket
{
private:
	bool isSocketOpen;
	HWND hwParent;
public:
	iSocket();
	~iSocket();
	CArray<nSocket *> nClient;
	void SetParentHWND(HWND pParent) {
		hwParent = pParent;
	}
	void DelFromClient(nSocket *mclient);
	void DelAllClient(void);

	void OnReceive(int nErrorCode);

	void OnSend(int nErrorCode);

	void OnAccept(int nErrorCode);

	void OnConnect(int nErrorCode);

	void OnClose(int nErrorCode);

	void PostMsgIsOpen(bool isOpen);
	int UnblockRead(CString &dataStr);
	int UnblockSend(const CString & dataStr)
	{
		return Send((LPCTSTR)dataStr, dataStr.GetLength());
	}
	bool IsScoketOpen(void) {
		return isSocketOpen;
	}
};