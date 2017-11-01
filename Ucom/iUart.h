#pragma once

#include "UBase.h"

typedef struct
{
	//´®¿Ú¾ä±ú
	HANDLE *commHandle;
	//Î´Ê¹ÓÃ
	int other;
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
		mThreadPara.other = 0;
		return &mThreadPara;
	}

};


UINT RxThreadFunc(LPVOID mThreadPara);

