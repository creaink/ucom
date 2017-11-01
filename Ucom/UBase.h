#pragma once

#include <afxstr.h>

// 通信接口
typedef  int(*AsyncSendX)(const CString &);
typedef  int(*AsyncReadX)(CString &);
typedef  bool(*IsOpenX)(void);


// 注册全局消息
// 在h文件里面afx_msg LRESULT OnMyReceiveMsg(WPARAM wParam, LPARAM lParam);
// 在cpp文件里	ON_REGISTERED_MESSAGE(WM_MYONRECVMSG, OnMyReceiveMsg)
// 并且在类里面实现该方法
const UINT WM_MYONRECVMSG = ::RegisterWindowMessage(_T("ONRECEIVE"));

#define WH_MASK			0xFFFF0000
#define WL_MASK			0x0000FFFF
#define WH_UCOM_UART	0x00010000
#define WH_UCOM_NET		0x00020000
#define WH_UCOM_SUBNET	0x00030000

#define WL_UCOM_OPEN	0x00000001
#define WL_UCOM_CLOSE	0x00000002
#define WL_UCOM_RECV	0x00000003

#define W_UART_RECV		0x00010003
#define W_NET_RECV		0x00020003
#define W_SUBNET_RECV	0x00030003

#define W_SUBNET_OPEN	0x00030004
#define W_SUBNET_CLOSE	0x00030005


#define WM_NETDLG_MSG  (WM_USER + 2)


class UcomBase
{
public:
	UcomBase() {
	}
	~UcomBase() {

	}
	virtual int AsyncSend(const CString &dataStr)=0;
	virtual int AsyncRead(CString &dataStr)=0;
	virtual bool isOpen(void)=0;
};
