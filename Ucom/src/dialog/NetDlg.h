#pragma once


#include "UBase.h"
#include "iSocket.h"

// CNetDlg 对话框
enum NetDlg_Type
{
	netdlg_tcpserver = 0,
	netdlg_tcpclient = 1,
	netdlg_udp = 2
};
#define NETDLG_TCP_CLIENT 0
#define NETDLG_TCP_SERVER 1
#define NETDLG_UDP		  2

class CNetDlg : public CDialog, public UcomBase
{
	DECLARE_DYNAMIC(CNetDlg)

private:
	//接收线程句柄
	HANDLE hRxThread;
	iSocket mSocket;
	bool isWorking;
	int curSender;	// 当前发送的客户端
	int typeSel;
	CString infoServer, infoLocal;
	HWND hwTopParent;
public:
	CNetDlg(CWnd* pParent = NULL);
	virtual ~CNetDlg();
	void SethwTopParent(HWND pTopParent) {
		hwTopParent = pTopParent;
	}

	void InitPanel(void);
	void SwPanel(bool choose);
	void EnableIPIn(int type);
	void ChangeBmpPic(int PicCtrlID, unsigned short nPicID);
	void SetTips(CString tips);
	void GetDstIPStr(CString &str);

	void LoadRegConfig();
	// 父窗口关闭时候OnClose需要调用保存一下信息
	void WriteRegData(void);

	//client列表处理
	void DelClient(nSocketPara *nPara);
	void AddClient(nSocketPara *nPara);
	void CleanClient(void);
	
	void OpenSocket(void);

	int AsyncSend(const CString &dataStr);
	int AsyncRead(CString &dataStr, CString & infoStr, WPARAM wParam, LPARAM lParam);
	bool IsRWable(void) {
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
	afx_msg void OnEditPortSendComplete();
	afx_msg LRESULT CNetDlg::OnMyReceiveMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelNetType();
	afx_msg void OnSelChangeCient();
	afx_msg void OnEditPortRecvComplete();
	afx_msg void OnClickedNpicuartstatus();
};
