#pragma once

#include <afxstr.h>


// register global message
// at *.h file: afx_msg LRESULT OnMyReceiveMsg(WPARAM wParam, LPARAM lParam);
// at *.cpp file: ON_REGISTERED_MESSAGE(WM_MYONRECVMSG, OnMyReceiveMsg)
// implement this method in the class
const UINT WM_MYONRECVMSG = ::RegisterWindowMessage(_T("ONRECEIVE"));

#define WH_MASK			0xFFFF0000
#define WL_MASK			0x0000FFFF
#define WKIND_MASK		0x000F0000
#define WH_UCOM_UART	0x00010000
#define WH_UCOM_NET		0x00020000
#define WH_UCOM_SUBNET	0x00120000

#define WL_UCOM_OPEN	0x00000001
#define WL_UCOM_CLOSE	0x00000002
#define WL_UCOM_RECV	0x00000003

#define W_UART_RECV		0x00010003
#define W_NET_RECV		0x00020003
#define W_SUBNET_RECV	0x00120003

#define W_SUBNET_OPEN	0x00120004
#define W_SUBNET_CLOSE	0x00120005


// custom message
#define WM_NETDLG_MSG  (WM_USER + 2)
#define WM_UARTDLG_MSG  (WM_USER + 3)


// abstract of async send and read
class UcomBase
{
public:
	UcomBase() {
	}
	~UcomBase() {

	}
	virtual int AsyncSend(const CString &dataStr) = 0;
	virtual int AsyncRead(CString &dataStr, CString & infoStr, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool IsRWable (void) = 0;
};
