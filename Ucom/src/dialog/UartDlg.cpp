// UartDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "UartDlg.h"
#include "afxdialogex.h"

/*************** �ؼ�id ***************/
// ���ں�-������-����λ��-ֹͣλ-У��
#define UART_IDC_CBB_PORT	IDC_CbUartPort
#define UART_IDC_CBB_BAUD	IDC_CbUartBaudrate
#define UART_IDC_CBB_BITS	IDC_CbUartDatabit
#define UART_IDC_CBB_STOP	IDC_CbUartStopbit
#define UART_IDC_CBB_ECC		IDC_CbUartECC
// DTR-RTS���������ź� DSR-CTS�Է������ź�
#define UART_IDC_CKB_DTR		IDC_CkbDTR
#define UART_IDC_CKB_RTS		IDC_CkbRTS
#define UART_IDC_CKB_DSR		IDC_CkbDSR
#define UART_IDC_CKB_CTS		IDC_CkbCTS
// ��״̬ͼƬ-�򿪰�ť
#define UART_IDC_PIC_STAT	IDC_PicUartStatus
#define UART_IDC_BTN_OPEN	IDC_BtnOpen

// CUartDlg �Ի���

IMPLEMENT_DYNAMIC(CUartDlg, CDialog)

CUartDlg::CUartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UART, pParent)
{

}

CUartDlg::~CUartDlg()
{
}

void CUartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUartDlg, CDialog)
	ON_CBN_DROPDOWN(UART_IDC_CBB_PORT, OnDropdownCbUartPort)
	ON_CBN_SELENDOK(UART_IDC_CBB_PORT, OnSelendokCbUartPort)
	ON_BN_CLICKED(UART_IDC_BTN_OPEN, OnBtnOpen)
	ON_WM_PAINT()
	ON_STN_CLICKED(UART_IDC_PIC_STAT, &CUartDlg::OnStnClickedPicuartstatus)
	ON_MESSAGE(WM_UARTDLG_MSG, &CUartDlg::OnMyReceiveMsg)

	ON_BN_CLICKED(UART_IDC_CKB_DTR, &CUartDlg::OnBnClickedCkbdtr)
	ON_BN_CLICKED(UART_IDC_CKB_RTS, &CUartDlg::OnBnClickedCkbrts)
END_MESSAGE_MAP()



BOOL CUartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	isWorking = false;
	InitPanel();
	// װ��ע�������
	LoadRegConfig();
	SwPanel(isWorking);

	mUart.SetParentHWND(GetSafeHwnd(), hwTopParent);
	return TRUE;
}


void CUartDlg::OnPaint()
{
	CPaintDC dc(this);
	// ������С����ָ�����ͼƬ����ʾ�����
	//SwPanel(isWorking);
	ChangeBmpPic(UART_IDC_PIC_STAT, isWorking ? IDB_SwOn : IDB_SwOff);
}


void CUartDlg::InitPanel(void)
{
	CComboBox *pComBox;

	pComBox = (CComboBox *)GetDlgItem(UART_IDC_CBB_PORT);
	pComBox->ResetContent();
	//��ȡ�����б�
	mUart.GetComList(pComBox);
	//Ĭ��ѡ���һ��
	pComBox->SetCurSel(0);

	//��Ӳ�����
	pComBox = (CComboBox*)GetDlgItem(UART_IDC_CBB_BAUD);
	TCHAR BaudrateTable[][7] = { "460800", "230400", "194000", "115200", "57600", "56000", "38400"
								, "19200", "14400", "9600", "4800", "2400", "1200" };
	for (int i = 0; i < (sizeof(BaudrateTable) / sizeof(BaudrateTable[0])); i++)
	{
		pComBox->InsertString(0, BaudrateTable[i]);
	}

	//ѡ��115200ΪĬ��
	pComBox->SetCurSel(pComBox->FindString(-1, "115200"));

	//����λ
	pComBox = (CComboBox*)GetDlgItem(UART_IDC_CBB_BITS);
	pComBox->InsertString(0, "8");
	pComBox->InsertString(0, "7");
	pComBox->InsertString(0, "6");
	pComBox->InsertString(0, "5");

	pComBox->SetCurSel(3);//ѡ��8λΪĬ��

						  //ֹͣλ
	pComBox = (CComboBox*)GetDlgItem(UART_IDC_CBB_STOP);
	pComBox->InsertString(0, "2");
	pComBox->InsertString(0, "1.5");
	pComBox->InsertString(0, "1");

	pComBox->SetCurSel(0);//ѡ��115200λΪĬ��

	pComBox = (CComboBox*)GetDlgItem(UART_IDC_CBB_ECC);
	pComBox->InsertString(0, "even");
	pComBox->InsertString(0, "odd ");
	pComBox->InsertString(0, "none");
	//ѡ����У��ΪĬ��
	pComBox->SetCurSel(0);
}

//�ӿؼ��еõ�������Ϣ�ַ���
DCB CUartDlg::GetUartConfigDCB(void)
{
	DCB configDCB;
	CString tmpStr, UartConfig;
	CComboBox *pCombox;

	//�趨���ڲ���
	pCombox = (CComboBox*)GetDlgItem(UART_IDC_CBB_BAUD);
	pCombox->GetLBText(pCombox->GetCurSel(), tmpStr);
	configDCB.BaudRate = _ttoi(tmpStr);

	pCombox = (CComboBox*)GetDlgItem(UART_IDC_CBB_ECC);
	configDCB.Parity = pCombox->GetCurSel();

	pCombox = (CComboBox*)GetDlgItem(UART_IDC_CBB_BITS);
	pCombox->GetLBText(pCombox->GetCurSel(), tmpStr);
	configDCB.ByteSize = atoi(tmpStr);

	pCombox = (CComboBox*)GetDlgItem(UART_IDC_CBB_STOP);
	configDCB.StopBits = pCombox->GetCurSel();

	return configDCB;
}

void CUartDlg::ChangeBmpPic(int PicCtrlID, unsigned short nPicID)
{
	CBitmap bitmap;
	HBITMAP hBmp;
	CStatic *pStatic = (CStatic*)GetDlgItem(PicCtrlID);

	bitmap.LoadBitmap(nPicID);				// ��λͼIDB_BITMAP1���ص�bitmap
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // ��ȡbitmap����λͼ�ľ��

	pStatic->SetBitmap(hBmp);				// ����ͼƬ�ؼ�
}

//�л����״̬
void CUartDlg::SwPanel(bool choose)
{
	ChangeBmpPic(UART_IDC_PIC_STAT, choose ? IDB_SwOn : IDB_SwOff);
	SetDlgItemText(UART_IDC_BTN_OPEN, choose ? _T("�رմ���") : _T("�򿪴���"));
	choose = !choose;
	GetDlgItem(UART_IDC_CBB_BAUD)->EnableWindow(choose);
	GetDlgItem(UART_IDC_CBB_BITS)->EnableWindow(choose);
	GetDlgItem(UART_IDC_CBB_STOP)->EnableWindow(choose);
	GetDlgItem(UART_IDC_CBB_ECC)->EnableWindow(choose);
	GetDlgItem(UART_IDC_CBB_PORT)->EnableWindow(choose);
}

bool CUartDlg::IsUartPortAvailable(void)
{
	CString comInfo;
	CComboBox *pCombox = (CComboBox *)GetDlgItem(UART_IDC_CBB_PORT);
	pCombox->GetLBText(pCombox->GetCurSel(), comInfo);

	return comInfo.Left(3) == _T("COM");
}

void CUartDlg::OpenUart()
{
	CComboBox *pCombox = (CComboBox *)GetDlgItem(UART_IDC_CBB_PORT);
	CString strtmp;

	if (isWorking == false)
	{
		//���ռ�þ�ȡ��ռ��
		if (mUart.isConnected())
			mUart.ClosePort();

		if (IsUartPortAvailable())
		{
			pCombox->GetLBText(pCombox->GetCurSel(), strtmp);
			mUart.ConfigUart(strtmp, GetUartConfigDCB());
		}
		else
		{
			AfxMessageBox(_T("������Ч,��ˢ��"));
			return;
		}

		if (!mUart.OpenCom())
		{
			AfxMessageBox(_T("�򿪴���ʧ��"));
			return;
		}

		//���������߳�
		hRxThread = AfxBeginThread(RxThreadFunc, mUart.GetThreadStartPara(), THREAD_PRIORITY_NORMAL);
		if (hRxThread == NULL)
		{
			TRACE("Rx Listenner Thread Created Failed");
			return;
		}

		isWorking = true;
		//���ʹ���Ϣ
		//::SendMessage(::AfxGetMainWnd()->m_hWnd, WM_COMM_RX_MSG, 1, 0);
		//GetParent()->SendMessage(WM_COMM_RX_MSG, 1, 0);
		::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_UART | WL_UCOM_OPEN, 0);
	}
	else if (isWorking == true)
	{
		//����λ��־λ����ر�ʱ����մ���
		isWorking = false;

		//���͹ر���Ϣ
		::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_UART | WL_UCOM_CLOSE, 0);

		TerminateThread(hRxThread, 0);
		ClearFlowCtrlStats();
		//�رմ���
		if (mUart.isConnected())
			mUart.ClosePort();
	}
	// �л�ͼ�ꡢ��ֹ״̬����ť�ı�
	SwPanel(isWorking);
}


//д��ע�������
void CUartDlg::WriteRegData(void)
{
	CComboBox *pCombox;
	CString strtmp;
	pCombox = (CComboBox *)GetDlgItem(UART_IDC_CBB_PORT);
	//���ں�
	pCombox->GetLBText(pCombox->GetCurSel(), strtmp);
	AfxGetApp()->WriteProfileString("Config", "ComName", strtmp);

	strtmp.Empty();
	strtmp.Format("%d-%d-%d-%d",
		((CComboBox*)GetDlgItem(UART_IDC_CBB_BAUD))->GetCurSel()
		, ((CComboBox*)GetDlgItem(UART_IDC_CBB_BITS))->GetCurSel()
		, ((CComboBox*)GetDlgItem(UART_IDC_CBB_STOP))->GetCurSel()
		, ((CComboBox*)GetDlgItem(UART_IDC_CBB_ECC))->GetCurSel());
	//��������
	AfxGetApp()->WriteProfileString("Config", "BDSE", strtmp);
}

void CUartDlg::GetRegData(CString &comName, CString &dcbConfig)
{
	comName = AfxGetApp()->GetProfileString("Config", "ComName", "NULL");
	dcbConfig = AfxGetApp()->GetProfileString("Config", "BDSE", "NULL");
	TRACE(comName + dcbConfig);
}

void CUartDlg::LoadRegConfig()
{
	CString comName, dcbConfig;
	CComboBox *pComBox = (CComboBox*)GetDlgItem(UART_IDC_CBB_PORT);

	GetRegData(comName, dcbConfig);
	if (comName != "NULL")
	{
		int sel = pComBox->FindString(-1, comName);
		//û�иô��ڣ����߲��Ǵ��ں�ʹ��Ĭ�ϣ���ʼ��ֵ��
		if (sel != CB_ERR && comName[0] == 'C')
			pComBox->SetCurSel(sel);

		int baudSel = 0, dataSel = 0, stopSel = 0, eccSel = 0;

		sscanf_s(dcbConfig, "%d-%d-%d-%d", &baudSel, &dataSel, &stopSel, &eccSel);
		((CComboBox*)GetDlgItem(UART_IDC_CBB_BAUD))->SetCurSel(baudSel);
		((CComboBox*)GetDlgItem(UART_IDC_CBB_BITS))->SetCurSel(dataSel);
		((CComboBox*)GetDlgItem(UART_IDC_CBB_STOP))->SetCurSel(stopSel);
		((CComboBox*)GetDlgItem(UART_IDC_CBB_ECC))->SetCurSel(eccSel);
	}
}

// CUartDlg ��Ϣ�������


//�˿��������ȿ���
void CUartDlg::OnDropdownCbUartPort()
{
	int DropSize;
	CComboBox *pComBox = (CComboBox *)GetDlgItem(UART_IDC_CBB_PORT);
	DropSize = pComBox->GetItemHeight(0)*pComBox->GetCount();
	pComBox->SetDroppedWidth(DropSize);
}

//ѡȡ���һ��ˢ�¶���
void CUartDlg::OnSelendokCbUartPort()
{
	CComboBox *pComBox = (CComboBox *)GetDlgItem(UART_IDC_CBB_PORT);
	int sel = pComBox->GetCurSel();
	int cnt = pComBox->GetCount();

	CString str;
	//ĩβˢ�´���
	if (sel == (cnt - 1))
	{
		//clear all
		pComBox->ResetContent();
		mUart.GetComList((CComboBox *)GetDlgItem(UART_IDC_CBB_PORT));

		//ѡȡ��һ��
		pComBox->SetCurSel(0);
	}
}


void CUartDlg::OnBtnOpen()
{
	OpenUart();
}



void CUartDlg::OnStnClickedPicuartstatus()
{
	OpenUart();
}


void CUartDlg::OnBnClickedCkbdtr()
{
	if (isWorking)
	{
		if (BST_CHECKED == IsDlgButtonChecked(UART_IDC_CKB_DTR))
			EscapeCommFunction(mUart.GetHandle(), SETDTR);
		else
			EscapeCommFunction(mUart.GetHandle(), CLRDTR);
	}
	else
	{
		((CButton *)GetDlgItem(UART_IDC_CKB_DTR))->SetCheck(BST_UNCHECKED);
	}
}


void CUartDlg::OnBnClickedCkbrts()
{
	if (isWorking)
	{
		if (BST_CHECKED == IsDlgButtonChecked(UART_IDC_CKB_RTS))
			EscapeCommFunction(mUart.GetHandle(), SETRTS);
		else
			EscapeCommFunction(mUart.GetHandle(), CLRRTS);
	}
	else
	{
		((CButton *)GetDlgItem(UART_IDC_CKB_RTS))->SetCheck(BST_UNCHECKED);
	}
}


// �������ĵ���Ϣ������Ҫ����UART�ź��߱仯
afx_msg LRESULT CUartDlg::OnMyReceiveMsg(WPARAM wParam, LPARAM lParam)
{
	if (lParam & 0xA0)
		((CButton *)GetDlgItem(UART_IDC_CKB_DSR))->SetCheck(BST_CHECKED);
	else
		((CButton *)GetDlgItem(UART_IDC_CKB_DSR))->SetCheck(BST_UNCHECKED);

	if (lParam & 0x10)
		((CButton *)GetDlgItem(UART_IDC_CKB_CTS))->SetCheck(BST_CHECKED);
	else
		((CButton *)GetDlgItem(UART_IDC_CKB_CTS))->SetCheck(BST_UNCHECKED);

	return 1;
}

void CUartDlg::ClearFlowCtrlStats(void)
{
	((CButton *)GetDlgItem(UART_IDC_CKB_DTR))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(UART_IDC_CKB_RTS))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(UART_IDC_CKB_DSR))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(UART_IDC_CKB_CTS))->SetCheck(BST_UNCHECKED);
}
