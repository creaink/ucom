#pragma once

#include "iUart.h"


// CUartDlg 对话框
class CUartDlg : public CDialog
{
	DECLARE_DYNAMIC(CUartDlg)

private:

	//接收线程句柄
	HANDLE hRxThread;
	bool IsUartPortAvailable(void);
	static bool uartPortIsOpen;
	static iUart mUart;

public:

	void InitPanel(void);
	void SwPanel(bool choose);
	void ChangeBmpPic(int PicCtrlID, unsigned short nPicID);

	void OpenUart();

	void OnDropdownCbUartPort();
	void OnSelendokCbUartPort();

	DCB GetUartConfigDCB(void);
	void LoadRegConfig();
	void GetRegData(CString &comName, CString &dcbConfig);
	// 父窗口关闭时候OnClose需要调用保存一下信息
	void WriteRegData(void);


	static int AsyncSend(const CString &dataStr) {
		return mUart.UnblockSend(dataStr);
	}
	static int AsyncRead(CString &dataStr) {
		return mUart.UnblockRead(dataStr);
	}
	static bool isOpen(void) {
		return uartPortIsOpen;
	}
public:
	CUartDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUartDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnOpen();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnStnClickedPicuartstatus();
};
