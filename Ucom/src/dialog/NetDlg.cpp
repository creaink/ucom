// NetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "NetDlg.h"
#include "afxdialogex.h"

/*************** �ؼ�id ***************/
// Э��-Ŀ��\����IP-Ŀ�Ķ˿�-���ض˿�
#define NET_IDC_CBB_TYPE			IDC_CbNetType
#define NET_IDC_IP_IPDST			IDC_IPDst
#define NET_IDC_EDT_PORT_SER		IDC_EditNetPortServer
#define NET_IDC_EDT_PORT_LOC		IDC_EditNetPortLocal
// TCP Serverģʽ�����ӵĿͻ����б�
#define NET_IDC_CBB_CLIENT		IDC_CbClient
// Э�������Ϣ-��״̬ͼƬ-�򿪰�ť
#define NET_IDC_TXT_INFO			IDC_TxtIPInfo
#define NET_IDC_BTN_OPEN			IDC_BtnNetConnect
#define NET_IDC_PIC_STAT			IDC_NPicUartStatus


// CNetDlg �Ի���

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
	// �л�����
	OnSelNetType();
	// ��������Ϣ���
	mSocket.SetParentHWND(GetSafeHwnd(), hwTopParent);

	((CComboBox*)GetDlgItem(NET_IDC_CBB_CLIENT))->InsertString(0, "��������");
	return TRUE;
}

void CNetDlg::InitPanel(void)
{
	CComboBox *pComBox = (CComboBox *)GetDlgItem(NET_IDC_CBB_TYPE);

	pComBox->ResetContent();
	pComBox->InsertString(0, "UDP");
	pComBox->InsertString(0, "TCP Server");
	pComBox->InsertString(0, "TCP Client");
	// ѡ��TCP Client"λΪĬ��
	pComBox->SetCurSel(0);
	// �˿����65535������Ϊ5λ
	((CEdit *)GetDlgItem(NET_IDC_EDT_PORT_SER))->LimitText(5);
	((CEdit *)GetDlgItem(NET_IDC_EDT_PORT_LOC))->LimitText(5);
}

// ����ģʽ��������ؼ��Ľ��ú�����
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
		SetTips("���ض˿��Զ�����");
		break;
	// TCP Server
	case NETDLG_TCP_SERVER:
		ipin = true;
		portServer = false;
		portLocal = true;
		client = true;
		SetTips("��������ǰ��ѡ�ͻ��� ��");
		curSender = 0;
		break;
	// UDP Client and Server
	case NETDLG_UDP:
		ipin = true;
		portServer = true;
		portLocal = true;
		client = false;
		SetTips("�����Լ��豾�ض˿�");
		break;
	default:
		break;
	}
	GetDlgItem(NET_IDC_IP_IPDST)->EnableWindow(ipin);
	GetDlgItem(NET_IDC_CBB_CLIENT)->EnableWindow(client);
	GetDlgItem(NET_IDC_EDT_PORT_SER)->EnableWindow(portServer);
	GetDlgItem(NET_IDC_EDT_PORT_LOC)->EnableWindow(portLocal);
}
// ����״̬�л�
void CNetDlg::SwPanel(bool choose)
{
	if (choose)
	{
		SetDlgItemText(NET_IDC_BTN_OPEN, _T("�Ͽ�"));
		ChangeBmpPic(NET_IDC_PIC_STAT, IDB_SwOn);
	}
	else
	{
		SetDlgItemText(NET_IDC_BTN_OPEN, _T("����"));
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

	bitmap.LoadBitmap(nPicID);				// ��λͼIDB_BITMAP1���ص�bitmap
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // ��ȡbitmap����λͼ�ľ��

	pStatic->SetBitmap(hBmp);				// ����ͼƬ�ؼ�
}

void CNetDlg::GetDstIPStr(CString &strIP)
{
	BYTE IP0, IP1, IP2, IP3;
	CIPAddressCtrl *pIPAddr = (CIPAddressCtrl*)GetDlgItem(NET_IDC_IP_IPDST);
	pIPAddr->GetAddress(IP0, IP1, IP2, IP3);
	strIP.Format(_T("%d.%d.%d.%d"), IP0, IP1, IP2, IP3);
}
// ��ע�����������
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
		// Ĭ������
		pIPAddr->SetAddress(127, 0, 0, 1);
		SetDlgItemText(NET_IDC_EDT_PORT_SER, "8000");
		SetDlgItemText(NET_IDC_EDT_PORT_LOC, "9000");
		pComBox->SetCurSel(0);
	}

}
// д���õ�ע���
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

// ɾ�������˵��еĿͻ��ˣ��رո�socket���ͷ��ڴ�
void CNetDlg::DelClient(nSocketPara *nPara)
{
	CComboBox *pComBox = (CComboBox*)GetDlgItem(NET_IDC_CBB_CLIENT);

	int sel = pComBox->FindString(-1, nPara->strIP);
	int curSel = pComBox->GetCurSel();
	if (sel != CB_ERR)
	{
		pComBox->DeleteString(sel);
		if (curSel == sel)
		{
			curSel--;
		}
		pComBox->SetCurSel(curSel);
	}
	// �����ɾ���ͻ���socket
	mSocket.DelFromClient((nSocket *)nPara->hSocket);
}

// ���������˵��еĿͻ��ˣ���¼��socket���ڴ�
void CNetDlg::AddClient(nSocketPara *nPara)
{
	CComboBox *pComBox = (CComboBox*)GetDlgItem(NET_IDC_CBB_CLIENT);

	mSocket.nClient.Add((nSocket *)nPara->hSocket);
	// ��ĩβ�����ӦnClient.Add����
	pComBox->InsertString(-1, nPara->strIP);
	// ��һ���ͻ�������ʱ��ѡ��ÿͻ���
	if (pComBox->GetCount() == 2)
		pComBox->SetCurSel(1);
}

// ɾ�����Կͻ��������˵���ر����пͻ���socket���ͷ��ڴ�
void CNetDlg::CleanClient(void)
{
	CComboBox *pComBox = (CComboBox*)GetDlgItem(NET_IDC_CBB_CLIENT);
	int count = pComBox->GetCount();
	for (int i = 1; i < count; i++)
	{
		pComBox->DeleteString(1);
	}
	// �رտͻ���socket���ͷ��ڴ�
	mSocket.DelAllClient();
}


void CNetDlg::OpenSocket(void)
{
	// ����������Ĵ���Ϣ���ﲻ�������첽����ڴ�ʧ�ܣ����첽�����﷢�ͳɹ�
	// ����������Ĺر���Ϣ������Ҫ����һ�£�����Ҳ�������첽�������رղ�SwPanel
	// �������Ӧ�÷�ֹ���ȴ򿪺͹ر�
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
			// ����ʱ��PortΪ0����win�Զ�����һ���˿�, �����޷�������ض˿ڿ����ͷ�, ����ָ���˿�
			// SO_REUSEADDR��Ч����ʱ����ָ�����ض˿�����
			mSocket.Create(0, SOCK_STREAM, lEvent);
			mSocket.Connect((LPCTSTR)strIP, nPortServer);
			break;
		case NETDLG_TCP_SERVER:	//TCP server
			mSocket.Create(nPortLocal, SOCK_STREAM);
			if (mSocket.Listen() == 0)
			{
				TRACE("Server Err:%d\n", mSocket.GetLastError());
				AfxMessageBox(_T("����ʧ��"));
				return;
			}
			break;
		case NETDLG_UDP: //UDP client
			mSocket.Create(nPortLocal, SOCK_DGRAM);
			//mSocket.Bind(nPortLocal);
			// UDP�����ӣ���Ҫ�ֶ�֪ͨ�򿪳ɹ�
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
		// udp ģʽ
		if (typeSel == 2)
		{
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
			// TCP server �ͻ����Ƿ���Ч
			if (mSocket.nClient.GetSize() >= curSender)
			{
				if (curSender == 0)
				{
					// ���͸�ȫ�����ӵ�client
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
			{
				return -1;
			}
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
		infoStr.Format("\n��From: %s��\n",sParam->strIP);
		return ((nSocket *)sParam->hSocket)->UnblockRead(dataStr);
	}
	else
	{
		if (typeSel == NETDLG_UDP)
		{
			return mSocket.UnblockRead(dataStr, infoStr);
		}
		else
		{
			return mSocket.UnblockRead(dataStr);
		}
	}
	return 0;
}


// CNetDlg ��Ϣ�������


void CNetDlg::OnBtnConnect()
{
	OpenSocket();
}

// ���ͼƬ��
void CNetDlg::OnClickedNpicuartstatus()
{
	OpenSocket();
}

void CNetDlg::OnPaint()
{
	CPaintDC dc(this);
	ChangeBmpPic(NET_IDC_PIC_STAT, isWorking ? IDB_SwOn : IDB_SwOff);
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

// ��������ͨ�Ų��ַ����Ľ���ı���Ϣ
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
	// server ģʽ�Ŀͻ��˵�����ͶϿ�
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

BOOL CNetDlg::PreTranslateMessage(MSG* pMsg)
{
	// ���λس�/ESC������ֹ�Ի����˳�
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		}
	}
	return __super::PreTranslateMessage(pMsg);
}
