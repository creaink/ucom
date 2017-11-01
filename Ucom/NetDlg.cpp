// NetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "NetDlg.h"
#include "afxdialogex.h"


// CNetDlg 对话框

IMPLEMENT_DYNAMIC(CNetDlg, CDialog)

bool CNetDlg::netSocketIsOpen = false;
iSocket CNetDlg::mSocket;

CNetDlg::CNetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NET, pParent)
{


}

CNetDlg::~CNetDlg()
{

}

void CNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetDlg, CDialog)
	//ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CNetDlg::OnIpnFieldchangedIpaddress1)
	ON_BN_CLICKED(IDC_BtnNetConnect, &CNetDlg::OnBtnConnect)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_EditNetPort, &CNetDlg::OnEditPortComplete)
	ON_MESSAGE(WM_NETDLG_MSG, &CNetDlg::OnMyReceiveMsg)
	ON_CBN_SELCHANGE(IDC_CbNetType, &CNetDlg::OnSelNetType)
	ON_CBN_SELCHANGE(IDC_CbClient, &CNetDlg::OnSelChangeCient)
END_MESSAGE_MAP()


BOOL CNetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	mSocket.SetParentHWND(GetSafeHwnd());
	InitPanel();

	LoadRegConfig();
	//切换禁用
	OnSelNetType();
	return TRUE;
}

void CNetDlg::InitPanel(void)
{
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbNetType);

	pComBox->ResetContent();
	pComBox->InsertString(0, "UDP Server");
	pComBox->InsertString(0, "UDP Client");
	pComBox->InsertString(0, "TCP Server");
	pComBox->InsertString(0, "TCP Client");
	//选择TCP Client"位为默认
	pComBox->SetCurSel(0);
	// 端口最大65535，限制为5位
	((CEdit *)GetDlgItem(IDC_EditNetPort))->LimitText(5);
}

void CNetDlg::EnableIPIn(bool isEnable)
{
	GetDlgItem(IDC_IPDst)->EnableWindow(isEnable);
	GetDlgItem(IDC_CbClient)->EnableWindow(!isEnable);
}

void CNetDlg::SwPanel(bool choose)
{
	if (choose) {
		SetDlgItemText(IDC_BtnNetConnect, _T("断开"));
		ChangeBmpPic(IDC_NPicUartStatus, IDB_SwOn);
	}
	else
	{
		SetDlgItemText(IDC_BtnNetConnect, _T("连接"));
		ChangeBmpPic(IDC_NPicUartStatus, IDB_SwOff);
	}

	choose = !choose;
	GetDlgItem(IDC_EditNetPort)->EnableWindow(choose);
	GetDlgItem(IDC_CbNetType)->EnableWindow(choose);
	GetDlgItem(IDC_IPDst)->EnableWindow(choose);
}


void CNetDlg::ChangeBmpPic(int PicCtrlID, unsigned short nPicID)
{
	CBitmap bitmap;
	HBITMAP hBmp;
	CStatic *pStatic = (CStatic*)GetDlgItem(PicCtrlID);

	bitmap.LoadBitmap(nPicID);				// 将位图IDB_BITMAP1加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄

	pStatic->SetBitmap(hBmp);				// 设置图片控件
}

void CNetDlg::GetDstIPStr(CString &strIP)
{
	BYTE IP0, IP1, IP2, IP3;
	CIPAddressCtrl *pIPAddr = (CIPAddressCtrl*)GetDlgItem(IDC_IPDst);
	pIPAddr->GetAddress(IP0, IP1, IP2, IP3);
	strIP.Format(_T("%d.%d.%d.%d"), IP0, IP1, IP2, IP3);
}

void CNetDlg::LoadRegConfig()
{
	CString netConfig;
	DWORD dwIP, dwTypeSel;
	UINT wPort;
	CIPAddressCtrl *pIPAddr = (CIPAddressCtrl*)GetDlgItem(IDC_IPDst);
	CComboBox *pComBox = (CComboBox*)GetDlgItem(IDC_CbNetType);

	netConfig = AfxGetApp()->GetProfileString("Config", "netConfig", "NULL");
	if (netConfig != "NULL")
	{
		sscanf_s(netConfig, "%d-%d-%d", &dwIP, &dwTypeSel, &wPort);
		netConfig.Format("%d",wPort);

		pIPAddr->SetAddress(dwIP);
		SetDlgItemInt(IDC_EditNetPort, wPort);
		pComBox->SetCurSel(dwTypeSel);
	}
	else
	{
		// 默认设置
		pIPAddr->SetAddress(127, 0, 0, 1);
		SetDlgItemText(IDC_EditNetPort, "8000");
		pComBox->SetCurSel(0);
	}

}

void CNetDlg::WriteRegData(void)
{
	CString netConfig;
	DWORD dwIP;
	UINT wPort;
	CIPAddressCtrl *pIPAddr = (CIPAddressCtrl*)GetDlgItem(IDC_IPDst);
	CComboBox *pComBox = (CComboBox*)GetDlgItem(IDC_CbNetType);

	((CIPAddressCtrl*)GetDlgItem(IDC_IPDst))->GetAddress(dwIP);
	wPort = GetDlgItemInt(IDC_EditNetPort);

	netConfig.Format("%d-%d-%d", dwIP, pComBox->GetCurSel(), wPort);
	AfxGetApp()->WriteProfileString("Config", "netConfig", netConfig);
}

// 删除下拉菜单中的客户端，关闭该socket并释放内存
void CNetDlg::DelClient(nSocketPara *nPara)
{
	CComboBox *pComBox = (CComboBox*)GetDlgItem(IDC_CbClient);

	mSocket.DelFromClient((nSocket *)nPara->hSocket);
	int sel = pComBox->FindString(-1, nPara->strIP);
	if (sel != CB_ERR)
	{
		pComBox->DeleteString(sel);
	}
}

// 增加下拉菜单中的客户端，记录该socket的内存
void CNetDlg::AddClient(nSocketPara *nPara)
{
	CComboBox *pComBox = (CComboBox*)GetDlgItem(IDC_CbClient);

	mSocket.nClient.Add((nSocket *)nPara->hSocket);
	// 在末尾插入对应nClient.Add次序
	pComBox->InsertString(-1, nPara->strIP);
	if (pComBox->GetCount() == 1)
		pComBox->SetCurSel(0);
}

// 删除所以客户端下拉菜单项，关闭所有客户端socket并释放内存
void CNetDlg::CleanClient(void)
{
	CComboBox *pComBox = (CComboBox*)GetDlgItem(IDC_CbClient);
	int count = pComBox->GetCount();
	for (int i = 0; i < count; i++)
	{
		pComBox->DeleteString(0);
	}
	// 关闭客户端socket并释放内存
	mSocket.DelAllClient();
}


void CNetDlg::OpenSocket(void)
{
	// 发往主界面的打开消息这里不操作，异步会存在打开失败，在异步程序里发送成功
	// 发往主界面的关闭消息这里需要操作一下，但是也可以在异步里主动关闭并SwPanel
	// 主界面的应该防止过度打开和关闭
	if (netSocketIsOpen == false)
	{
		CString strIP;
		int nPort = GetDlgItemInt(IDC_EditNetPort);
		int typeSel = ((CComboBox*)GetDlgItem(IDC_CbNetType))->GetCurSel();
		long lEvent = FD_READ | FD_CONNECT | FD_CLOSE | FD_ACCEPT;
		GetDstIPStr(strIP);
		switch (typeSel)
		{
		case 0:	//TCP client
			// 创建时候Port为0是让win自动生成一个端口
			mSocket.Create(0, SOCK_STREAM, lEvent);
			mSocket.Connect((LPCTSTR)strIP, nPort);
			break;
		case 1:	//TCP server
			mSocket.Create(nPort, SOCK_STREAM);
			mSocket.Listen();
			break;
		default:
			break;
		}
		netSocketIsOpen = true;
	}
	else
	{
		netSocketIsOpen = false;
		mSocket.Close();
		CleanClient();
		::PostMessage(HWND_BROADCAST, WM_MYONRECVMSG, WH_UCOM_NET | WL_UCOM_OPEN, 0);
	}
	SwPanel(netSocketIsOpen);

	CString strIP;
}

int CNetDlg::VirtualSend(CString & dataStr)
{

	return 0;
}




// CNetDlg 消息处理程序


void CNetDlg::OnBtnConnect()
{
	OpenSocket();
}


void CNetDlg::OnPaint()
{
	CPaintDC dc(this);
	SwPanel(netSocketIsOpen);
}


void CNetDlg::OnEditPortComplete()
{
	UINT port = GetDlgItemInt(IDC_EditNetPort);
	if (port > 0xFFFF)
		SetDlgItemInt(IDC_EditNetPort, 0xFFFF);
}

void CNetDlg::OnSelNetType()
{
	int typeSel = ((CComboBox*)GetDlgItem(IDC_CbNetType))->GetCurSel();
	typeSel = typeSel % 2;
	EnableIPIn(!typeSel);
	if (typeSel == 1)
	{
		SetTips("发送消息先选客户端");
	}
}

void CNetDlg::OnSelChangeCient()
{
	int typeSel = ((CComboBox*)GetDlgItem(IDC_CbClient))->GetCurSel();

}


afx_msg LRESULT CNetDlg::OnMyReceiveMsg(WPARAM wParam, LPARAM lParam)
{
	CString strIP;
	UINT nPort;
	nSocketPara *sParam = (nSocketPara *)lParam;
	switch (wParam)
	{
	case WL_UCOM_CLOSE:
		netSocketIsOpen = false;
		SwPanel(false);
		SetTips("");
		break;
	case WL_UCOM_OPEN:
		netSocketIsOpen = true;
		mSocket.GetSockName(strIP, nPort);
		strIP.Format("%s:%d", strIP, nPort);
		SetTips(strIP);
		break;
	case W_SUBNET_CLOSE:
		cout << "dlg close:" << sParam->strIP << endl;
		DelClient(sParam);
		break;
	case W_SUBNET_OPEN:
		cout << "dlg open:" << sParam->strIP << endl;
		AddClient(sParam);
	default:
		break;
	}
	return true;
}



