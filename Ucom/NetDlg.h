#pragma once


#include "UBase.h"
#include "iSocket.h"

// CNetDlg 对话框

class CNetDlg : public CDialog, public UcomBase
{
	DECLARE_DYNAMIC(CNetDlg)

private:
	//接收线程句柄
	HANDLE hRxThread;
	//bool IsSocketAvailable(void);
	static iSocket mSocket;
	static bool netSocketIsOpen;
	//static iUart mUart;

public:
	CNetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNetDlg();

	void InitPanel(void);
	void SwPanel(bool choose);
	void EnableIPIn(bool isDisable);
	void ChangeBmpPic(int PicCtrlID, unsigned short nPicID);
	void SetTips(CString tips) {
		SetDlgItemText(IDC_TxtIPInfo, tips);
	}
	void GetDstIPStr(CString &str);

	void LoadRegConfig();
	// 父窗口关闭时候OnClose需要调用保存一下信息
	void WriteRegData(void);

	//client列表处理
	void DelClient(nSocketPara *nPara);
	void AddClient(nSocketPara *nPara);
	void CleanClient(void);

	void OpenSocket(void);

	//static int AsyncSend(const CString &dataStr) {
	//	return mSocket.UnblockSend(dataStr);
	//}
	//static int AsyncRead(CString &dataStr) {
	//	return mSocket.UnblockRead(dataStr);
	//}
	//int VirtualSend(CString &dataStr);
	//static bool isOpen(void) {
	//	return mSocket.IsScoketOpen();
	//}
	int AsyncSend(const CString &dataStr) {
		return mSocket.UnblockSend(dataStr);
	}
	int AsyncRead(CString &dataStr) {
		return mSocket.UnblockRead(dataStr);
}
	int VirtualSend(CString &dataStr);
	bool isOpen(void) {
		return mSocket.IsScoketOpen();
	}

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnConnect();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditPortComplete();
	afx_msg LRESULT CNetDlg::OnMyReceiveMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelNetType();
	afx_msg void OnSelChangeCient();
};
