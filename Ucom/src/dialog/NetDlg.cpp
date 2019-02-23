// NetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "NetDlg.h"
#include "afxdialogex.h"

/*************** 控件id ***************/
// 协议-目的\本地IP-目的端口-本地端口
#define NET_IDC_CBB_TYPE			IDC_CbNetType
#define NET_IDC_IP_IPDST			IDC_IPDst
#define NET_IDC_EDT_PORT_SER		IDC_EditNetPortServer
#define NET_IDC_EDT_PORT_LOC		IDC_EditNetPortLocal
// TCP Server模式下链接的客户端列表
#define NET_IDC_CBB_CLIENT		IDC_CbClient
// 协议帮助信息-打开状态图片-打开按钮
#define NET_IDC_TXT_INFO			IDC_TxtIPInfo
#define NET_IDC_BTN_OPEN			IDC_BtnNetConnect
#define NET_IDC_PIC_STAT			IDC_NPicUartStatus


// CNetDlg 对话框

IMPLEMENT_DYNAMIC(CNetDlg, CDialog)

CNetDlg::CNetDlg(CWnd* pParent)
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
	ON_BN_CLICKED(NET_IDC_BTN_OPEN, &CNetDlg::OnBtnConnect)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(NET_IDC_EDT_PORT_SER, &CNetDlg::OnEditPortSendComplete)
	ON_MESSAGE(WM_NETDLG_MSG, &CNetDlg::OnMyReceiveMsg)
	ON_CBN_SELCHANGE(NET_IDC_CBB_TYPE, &CNetDlg::OnSelNetType)
	ON_CBN_SELCHANGE(NET_IDC_CBB_CLIENT, &CNetDlg::OnSelChangeCient)
	ON_EN_KILLFOCUS(NET_IDC_EDT_PORT_LOC, &CNetDlg::OnEditPortRecvComplete)
	ON_STN_CLICKED(NET_IDC_PIC_STAT, &CNetDlg::OnClickedNpicuartstatus)
END_MESSAGE_MAP()


BOOL CNetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	curSender = -1;
	InitPanel();

	LoadRegConfig();
	//切换禁用
	OnSelNetType();
	// 设置其消息句柄
	mSocket.SetParentHWND(GetSafeHwnd(), hwTopParent);

	((CComboBox*)GetDlgItem(NET_IDC_CBB_CLIENT))->InsertString(0, "所有连接");
	return TRUE;
}

void CNetDlg::InitPanel(void)
{
	CComboBox *pComBox = (CComboBox *)GetDlgItem(NET_IDC_CBB_TYPE);

	pComBox->ResetContent();
	pComBox->InsertString(0, "UDP");
	pComBox->InsertString(0, "TCP Server");
	pComBox->InsertString(0, "TCP Client");
	//选择TCP Client"位为默认
	pComBox->SetCurSel(0);
	// 端口最大65535，限制为5位
	((CEdit *)GetDlgItem(NET_IDC_EDT_PORT_SER))->LimitText(5);
	((CEdit *)GetDlgItem(NET_IDC_EDT_PORT_LOC))->LimitText(5);
}

// 根据模式进行输入控件的禁用和启用
void CNetDlg::EnableIPIn(int type)
{
	bool ipin, client, portServer, portLocal;
	switch (type)
	{
	// TCP Client
	case NETDLG_TCP_CLIENT:
		ipin = true;
		portServer = true;
		portLocal = false;
		client = false;
		curSender = -1;
		SetTips("本地端口自动分配");
		break;
	// TCP Server
	case NETDLG_TCP_SERVER:
		ipin = true;
		portServer = false;
		portLocal = true;
		client = true;
		SetTips("发送数据前先选客户端 ↑");
		curSender = 0;
		break;
	// UDP Client and Server
	case NETDLG_UDP:
		ipin = true;
		portServer = true;
		portLocal = true;
		client = false;
		SetTips("监听自己设本地端口");
		break;
	default:
		break;
	}
	GetDlgItem(NET_IDC_IP_IPDST)->EnableWindow(ipin);
	GetDlgItem(NET_IDC_CBB_CLIENT)->EnableWindow(client);
	GetDlgItem(NET_IDC_EDT_PORT_SER)->EnableWindow(portServer);
	GetDlgItem(NET_IDC_EDT_PORT_LOC)->EnableWindow(portLocal);
}
// 连接状态切换
void CNetDlg::SwPanel(bool choose)
{
	if (choose) {
		SetDlgItemText(NET_IDC_BTN_OPEN, _T("断开"));
		ChangeBmpPic(NET_IDC_PIC_STAT, IDB_SwOn);
	}
	else
	{
		SetDlgItemText(NET_IDC_BTN_OPEN, _T("连接"));
		ChangeBmpPic(NET_IDC_PIC_STAT, IDB_SwOff);
	}

	choose = !choose;
	GetDlgItem(NET_IDC_CBB_TYPE)->EnableWindow(choose);
	switch (typeSel)
	{
	case NETDLG_TCP_CLIENT:
		GetDlgItem(NET_IDC_EDT_PORT_SER)->EnableWindow(choose);
		GetDlgItem(NET_IDC_IP_IPDST)->EnableWindow(choose);
		break;
	case NETDLG_TCP_SERVER:
		GetDlgItem(NET_IDC_IP_IPDST)->EnableWindow(choose);
		GetDlgItem(NET_IDC_EDT_PORT_LOC)->EnableWindow(choose);
		break;
	case NETDLG_UDP:
		GetDlgItem(NET_IDC_EDT_PORT_LOC)->EnableWindow(choose);
		break;
	default:
		break;
	}
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
	CIPAddressCtrl *pIPAddr = (CIPAddressCtrl*)GetDlgItem(NET_IDC_IP_IPDST);
	pIPAddr->GetAddress(IP0, IP1, IP2, IP3);
	strIP.Format(_T("%d.%d.%d.%d"), IP0, IP1, IP2, IP3);
}
// 从注册表载入设置
void CNetDlg::LoadRegConfig()
{
	CString netConfig;
	DWORD dwIP, dwTypeSel;
	UINT wPortServer, wPortLoacl;
	CIPAddressCtrl *pIPAddr = (CIPAddressCtrl*)GetDlgItem(NET_IDC_IP_IPDST);
	CComboBox *pComBox = (CComboBox*)GetDlgItem(NET_IDC_CBB_TYPE);

	netConfig = AfxGetApp()->GetProfileString("Config", "netConfig", "NULL");
	if (netConfig != "NULL")
	{
		sscanf_s(netConfig, "%d-%d-%d-%d", &dwIP, &dwTypeSel, &wPortServer, &wPortLoacl);

		pIPAddr->SetAddress(dwIP);
		SetDlgItemInt(NET_IDC_EDT_PORT_SER, wPortServer);
		SetDlgItemInt(NET_IDC_EDT_PORT_LOC, wPortLoacl);
		pComBox->SetCurSel(dwTypeSel);
	}
	else
	{
		// 默认设置
		pIPAddr->SetAddress(127, 0, 0, 1);
		SetDlgItemText(NET_IDC_EDT_PORT_SER, "8000");
		SetDlgItemText(NET_IDC_EDT_PORT_LOC, "9000");
		pComBox->SetCurSel(0);
	}

}
// 写设置到注册表
void CNetDlg::WriteRegData(void)
{
	CString netConfig;
	DWORD dwIP;
	UINT wPortServer, wPortLoacl;
	CIPAddressCtrl *pIPAddr = (CIPAddressCtrl*)GetDlgItem(NET_IDC_IP_IPDST);
	CComboBox *pComBox = (CComboBox*)GetDlgItem(NET_IDC_CBB_TYPE);

	((CIPAddressCtrl*)GetDlgItem(NET_IDC_IP_IPDST))->GetAddress(dwIP);
	wPortServer = GetDlgItemInt(NET_IDC_EDT_PORT_SER);
	wPortLoacl = GetDlgItemInt(NET_IDC_EDT_PORT_LOC);
	netConfig.Format("%d-%d-%d-%d", dwIP, pComBox->GetCurSel(), wPortServer, wPortLoacl);
	AfxGetApp()->WriteProfileString("Config", "netConfig", netConfig);
}

// 删除下拉菜单中的客户端，关闭该socket并释放内存
void CNetDlg::DelClient(nSocketPara *nPara)
{
	CComboBox *pComBox = (CComboBox*)GetDlgItem(NET_IDC_CBB_CLIENT);

	int sel = pComBox->FindString(-1, nPara->strIP);
	int curSel = pComBox->GetCurSel();
	if (sel != CB_ERR)
	{
		pComBox->DeleteString(sel);
		if (curSel == sel) {
			curSel--;
		}
		pComBox->SetCurSel(curSel);
	}
	// 最后尝试删除客户端socket
	mSocket.DelFromClient((nSocket *)nPara->hSocket);
}

// 增加下拉菜单中的客户端，记录该socket的内存
void CNetDlg::AddClient(nSocketPara *nPara)
{
	CComboBox *pComBox = (CComboBox*)GetDlgItem(NET_IDC_CBB_CLIENT);

	mSocket.nClient.Add((nSocket *)nPara->hSocket);
	// 在末尾插入对应nClient.Add次序
	pComBox->InsertString(-1, nPara->strIP);
	// 第一个客户端连接时候选择该客户端
	if (pComBox->GetCount() == 2)
		pComBox->SetCurSel(1);
}

// 删除所以客户端下拉菜单项，关闭所有客户端socket并释放内存
void CNetDlg::CleanClient(void)
{
	CComboBox *pComBox = (CComboBox*)GetDlgItem(NET_IDC_CBB_CLIENT);
	int count = pComBox->GetCount();
	for (int i = 1; i < count; i++)
	{
		pComBox->DeleteString(1);
	}
	// 关闭客户端socket并释放内存
	mSocket.DelAllClient();
}


void CNetDlg::OpenSocket(void)
{
	// 发往主界面的打开消息这里不操作，异步会存在打开失败，在异步程序里发送成功
	// 发往主界面的关闭消息这里需要操作一下，但是也可以在异步里主动关闭并SwPanel
	// 主界面的应该防止过度打开和关闭
	if (isWorking == false)
	{
		CString strIP;
		int nPortServer = GetDlgItemInt(NET_IDC_EDT_PORT_SER);
		int nPortLocal = GetDlgItemInt(NET_IDC_EDT_PORT_LOC);
		typeSel = ((CComboBox*)GetDlgItem(NET_IDC_CBB_TYPE))->GetCurSel();
		long lEvent = FD_READ | FD_CONNECT | FD_CLOSE | FD_ACCEPT;
		GetDstIPStr(strIP);
		switch (typeSel)
		{
		case NETDLG_TCP_CLIENT:	//TCP client
			// 创建时候Port为0是让win自动生成一个端口, 由于无法解决本地端口快速释放, 不能指定端口
			// SO_REUSEADDR无效，暂时不能指定本地端口连接
			mSocket.Create(0, SOCK_STREAM, lEvent);
			mSocket.Connect((LPCTSTR)strIP, nPortServer);
			break;
		case NETDLG_TCP_SERVER:	//TCP server
			mSocket.Create(nPortLocal, SOCK_STREAM);
			if (mSocket.Listen() == 0) {
				TRACE("Server Err:%d\n", mSocket.GetLastError());
				AfxMessageBox(_T("监听失败"));
				return;
			}
			break;
		case NETDLG_UDP: //UDP client
			mSocket.Create(nPortLocal, SOCK_DGRAM);
			//mSocket.Bind(nPortLocal);
			// UDP无连接，需要手动通知打开成功
			::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_NET | WL_UCOM_OPEN, 0);
			mSocket.SetIsOpen(true);
			break;
		default:
			break;
		}
		isWorking = true;
	}
	else
	{
		isWorking = false;
		mSocket.Close();
		// udp 模式
		if (typeSel == 2) {
			mSocket.SetIsOpen(false);
		}
		CleanClient();
		::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_NET | WL_UCOM_CLOSE, 0);
	}
	SwPanel(isWorking);

	CString strIP;
}

int CNetDlg::AsyncSend(const CString & dataStr)
{
	if (!mSocket.IsScoketOpen())
		return -1;
	if (typeSel != NETDLG_UDP) {
		if (curSender == -1) {
			// TCP client
			return mSocket.Send((LPCTSTR)dataStr, dataStr.GetLength());
		}
		else
		{
			// TCP server 客户端是否有效
			if (mSocket.nClient.GetSize() >= curSender)
			{
				if (curSender == 0)
				{
					// 发送给全部连接的client
					int cnt = 0;
					for (int i = 1; i <= mSocket.nClient.GetSize(); i++)
					{
						cnt += mSocket.nClient[i - 1]->Send((LPCTSTR)dataStr, dataStr.GetLength());
					}
					return cnt;
				}
				else
				{
					return mSocket.nClient[curSender - 1]->Send((LPCTSTR)dataStr, dataStr.GetLength());
				}
			}
			else
				return -1;
		}
	}
	else
	{
		CString strIP;
		int nPortServer = GetDlgItemInt(NET_IDC_EDT_PORT_SER);
		GetDstIPStr(strIP);
		return mSocket.SendTo((LPCTSTR)dataStr, dataStr.GetLength(),nPortServer, strIP);
	}
	return 0;
}

int CNetDlg::AsyncRead(CString & dataStr, CString & infoStr, WPARAM wParam, LPARAM lParam)
{
	if (!mSocket.IsScoketOpen())
		return -1;

	if ((wParam&WH_MASK) == WH_UCOM_SUBNET) {
		nSocketPara *sParam = (nSocketPara *)lParam;
		infoStr.Format("\n【From: %s】\n",sParam->strIP);
		return ((nSocket *)sParam->hSocket)->UnblockRead(dataStr);
	}
	else
	{
		if (typeSel == NETDLG_UDP) {
			return mSocket.UnblockRead(dataStr, infoStr);
		}
		else
		{
			return mSocket.UnblockRead(dataStr);
		}
	}
	return 0;
}


// CNetDlg 消息处理程序


void CNetDlg::OnBtnConnect()
{
	OpenSocket();
}

//点击图片打开
void CNetDlg::OnClickedNpicuartstatus()
{
	OpenSocket();
}

void CNetDlg::OnPaint()
{
	CPaintDC dc(this);
	if (isWorking)
		ChangeBmpPic(NET_IDC_PIC_STAT, IDB_SwOn);
	else
		ChangeBmpPic(NET_IDC_PIC_STAT, IDB_SwOff);
}


void CNetDlg::OnEditPortSendComplete()
{
	UINT port = GetDlgItemInt(NET_IDC_EDT_PORT_SER);
	if (port > 0xFFFF)
		SetDlgItemInt(NET_IDC_EDT_PORT_SER, 0xFFFF);
}

void CNetDlg::OnEditPortRecvComplete()
{
	UINT port = GetDlgItemInt(NET_IDC_EDT_PORT_LOC);
	if (port > 0xFFFF)
		SetDlgItemInt(NET_IDC_EDT_PORT_LOC, 0xFFFF);
}

void CNetDlg::OnSelNetType()
{
	typeSel = ((CComboBox*)GetDlgItem(NET_IDC_CBB_TYPE))->GetCurSel();
	EnableIPIn(typeSel);
}

void CNetDlg::OnSelChangeCient()
{
	int clientSel = ((CComboBox*)GetDlgItem(NET_IDC_CBB_CLIENT))->GetCurSel();
	if (curSender >= 0) {
		curSender = clientSel;
	}
}

// 处理来自通信部分发来的界面改变消息
afx_msg LRESULT CNetDlg::OnMyReceiveMsg(WPARAM wParam, LPARAM lParam)
{
	CString strIP;
	UINT nPort;
	nSocketPara *sParam = (nSocketPara *)lParam;
	switch (wParam)
	{
	case WL_UCOM_CLOSE:
		isWorking = false;
		SwPanel(false);
		SetTips("");
		break;
	case WL_UCOM_OPEN:
		isWorking = true;
		mSocket.GetSockName(strIP, nPort);
		strIP.Format("Local:%s:%d\n", strIP, nPort);
		SetTips(strIP);
		break;
	// server模式的客户端的连入和断开
	case W_SUBNET_CLOSE:
		TRACE("Client Close:%s\n", sParam->strIP);
		DelClient(sParam);
		break;
	case W_SUBNET_OPEN:
		TRACE("Client Open:%s\n", sParam->strIP);
		AddClient(sParam);
	default:
		break;
	}
	return true;
}

void CNetDlg::SetTips(CString tips) {
	SetDlgItemText(NET_IDC_TXT_INFO, tips);
}