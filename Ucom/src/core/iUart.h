#pragma once

#include "UBase.h"

#define IUART_BUFF_MAX 1024

typedef struct
{
	// serial-port HANDLE
	HANDLE *commHandle;
	// unused
	HWND hwParent;
	HWND hwTopParent;
}ThreadPara;


class iUart
{
private:
	HANDLE hUartCom;
	DCB uartConfig;
	CString ComName;

	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;
	ThreadPara mThreadPara;

	HWND hwParent;
	HWND hwTopParent;
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

	void SetParentHWND(HWND pParent, HWND pTopParent)
	{
		hwParent = pParent;
		hwTopParent = pTopParent;
	}

	HANDLE GetHandle(void)
	{
		return hUartCom;
	}

	HANDLE* GetHandleAddr(void)
	{
		return &hUartCom;
	}

	bool isConnected(void)
	{
		if (hUartCom == NULL)
			return false;
		else
			return true;
	}

	ThreadPara *GetThreadStartPara(void)
	{
		mThreadPara.hwParent = hwParent;
		mThreadPara.hwTopParent = hwTopParent;
		return &mThreadPara;
	}

};


UINT RxThreadFunc(LPVOID mThreadPara);
