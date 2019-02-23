#pragma once

#include "iUart.h"


// CUartDlg 对话框
class CUartDlg : public CDialog, public UcomBase
{
	DECLARE_DYNAMIC(CUartDlg)

private:
	HANDLE hRxThread;	// 接收线程句柄
	HWND hwTopParent;	// 顶层窗口句柄
	bool IsUartPortAvailable(void);
	//面板是否处于工作状态
	bool isWorking;
	iUart mUart;
	void ClearFlowCtrlStats(void);
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

	void SethwTopParent(HWND pTopParent) {
		hwTopParent = pTopParent;
	}
	int AsyncSend(const CString &dataStr) {
		if (!isWorking)
			return -1;
		return mUart.UnblockSend(dataStr);
	}
	int AsyncRead(CString &dataStr, CString & infoStr, WPARAM wParam, LPARAM lParam) {
		if (!isWorking)
			return -1;
		return mUart.UnblockRead(dataStr);
	}
	bool IsRWable(void) {
		return isWorking;
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
	afx_msg void OnBnClickedCkbdtr();
	afx_msg void OnBnClickedCkbrts();
	afx_msg LRESULT OnMyReceiveMsg(WPARAM wParam, LPARAM lParam);
};
