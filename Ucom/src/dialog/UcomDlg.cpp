// UcomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "UcomDlg.h"
#include "afxdialogex.h"

#include "DeConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GRAPH_TAB_INDEX 0
#define ENCODER_TAB_INDEX 1
#define DATA_WATCH_TAB_INDEX 2
#define MULTI_SEND_TAB_INDEX 3

// CUcomDlg �Ի���


CUcomDlg::CUcomDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUcomDlg::IDD, pParent)
, DataRx(), DataTx()
, MultiSendDlg(this, &uBase)
{
	//{{AFX_DATA_INIT(CUcomDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	// ���ó���ͼ��
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CUcomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUcomDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnSend, OnBtnSend)
	ON_BN_CLICKED(IDC_BtnClearRecv, OnBtnClearRecv)
	ON_BN_CLICKED(IDC_BtnClearSend, OnBtnClearSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CkbSendOnTime, OnCkbSendOnTime)
	ON_EN_CHANGE(IDC_EdbSendDelay, OnChangeEdbSendDelay)
	ON_BN_CLICKED(IDC_CkbFrameNewLine, OnCkbFrameNewLine)
	ON_BN_CLICKED(IDC_BtnClearnCnt, OnBtnClearnCnt)
	ON_BN_CLICKED(IDC_CkbDispHex, OnCkbDispHex)
	ON_EN_CHANGE(IDC_EditTxData, OnChangeEditTxData)
	ON_BN_CLICKED(IDC_CkbSendHex, OnCkbSendHex)
	ON_BN_CLICKED(IDC_BtnIsDispRx, OnBnClickedBtnisdisprx)

	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BtnToolBox, &CUcomDlg::OnBnClickedBtntoolbox)
	ON_COMMAND(ID_MeuDevManger, &CUcomDlg::OnMeudevmanger)
	ON_COMMAND(ID_MeuCalc, &CUcomDlg::OnMeucalc)
	ON_COMMAND(ID_MeuNotepad, &CUcomDlg::OnMeunotepad)
	ON_COMMAND(ID_MenuReg, &CUcomDlg::OnMenureg)
	ON_COMMAND(ID_MenuCmd, &CUcomDlg::OnMenucmd)
	ON_COMMAND(ID_MenuServ, &CUcomDlg::OnMenuserv)
	ON_BN_CLICKED(IDC_BtnWinSize, &CUcomDlg::OnBnClickedBtnwinsize)
	ON_BN_CLICKED(IDC_BtnBackColor, &CUcomDlg::OnBnClickedBtnbackcolor)
	ON_BN_CLICKED(IDC_BtnFontColor, &CUcomDlg::OnBnClickedBtnfontcolor)
	//	ON_COMMAND(IDR_MENU1, &CUcomDlg::OnToolsBoxMenu)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BtnFont, &CUcomDlg::OnBnClickedBtnfont)
	ON_BN_CLICKED(IDC_CkbRxFullClc, &CUcomDlg::OnBnClickedCkbrxfullclc)
	ON_EN_CHANGE(IDC_EdbFullBytes, &CUcomDlg::OnChangeEdbfullbytes)
	ON_BN_CLICKED(IDC_BtnSaveRx, &CUcomDlg::OnBnClickedBtnsaverx)
	ON_BN_CLICKED(IDC_BtnClearAll, &CUcomDlg::OnBnClickedBtnclearall)
	ON_BN_CLICKED(IDC_BtnHelp, &CUcomDlg::OnBnClickedBtnhelp)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABEx, &CUcomDlg::OnSelchangeTabex)
	ON_BN_CLICKED(IDC_BtnEncoder, &CUcomDlg::OnBnClickedBtnencoder)
	ON_BN_CLICKED(IDC_BtnSendFile, &CUcomDlg::OnBnClickedBtnsendfile)
	ON_REGISTERED_MESSAGE(WM_MYONRECVMSG, &CUcomDlg::OnMyReceiveMsg)
//	ON_WM_CHAR()
ON_BN_CLICKED(IDC_CkbCMD, &CUcomDlg::OnBnClickedCkbcmd)
ON_WM_SIZE()
ON_WM_GETMINMAXINFO()
ON_NOTIFY(TCN_SELCHANGE, IDC_TABSrc, &CUcomDlg::OnSelchangeTabsrc)
END_MESSAGE_MAP()

// CUcomDlg ��Ϣ�������

BOOL CUcomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect1, rect2;

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	// ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	SetWindowText(_T("Ucom v1.1.0 �ᴮ��"));

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


#ifdef USING_CONSLOE
	InitDebugConsole();
#endif
	isNewLineSend = false;
	isDispHex = false;
	isSendHex = false;
	isDispRx = true;
	isCmdMode = false;
	isWorking = false;
	encoderMode = 0;
	rxCnt = 0;
	txCnt = 0;

	// ���ڷָ�������ʼ��
	hzSplitter.SubclassDlgItem(IDC_BtnSplitter, this);
	hzSplitter.SetType(CS_HORZ);

	hzSplitter.AddToTopOrLeftCtrls(IDC_RichRx);
	hzSplitter.AddToBottomOrRightCtrls(IDC_EditTxData);
	hzSplitter.AddToBottomOrRightCtrls(IDC_TxtSendCnt, SPF_TOP);
	hzSplitter.AddToBottomOrRightCtrls(IDC_TxtRecvCnt, SPF_TOP);

	// ����Դ�Ӵ��ڳ�ʼ��
	NetDlg.SethwTopParent(GetSafeHwnd());
	UartDlg.SethwTopParent(GetSafeHwnd());
	InitTabEx();
	InitTabSrc();

	SetDlgItemInt(IDC_EdbSendDelay, 1000);
	SetDlgItemInt(IDC_EdbFullBytes, 5120);

	LoadRegConfig();
	SetRichLineSpace();

	cmdNextPointer = 0;
	cmdDispPointer = 0;

	// �����С�����ֵ�������ȡ��ֵ�г���
	// ��ȡ�ؼ������Ļ����������
	GetDlgItem(IDC_TABEx)->GetWindowRect(&rect1);

	// �����շ���
	GetDlgItem(IDC_GrpRS)->GetWindowRect(&rect2);
	rect1.left = (rect2.right + rect1.left) / 2 + 2;

	GetWindowRect(&rect2);
	rect1.right = rect2.right;
	widthEx = rect1.Width();
	heightDlgMin = rect1.Height() + 55; // ����55

	GetDlgItem(IDC_BtnSplitter)->GetWindowRect(&rect2);
	widthDlgMin = rect2.left + 120; // ����120

	GetDlgItem(IDC_EditTxData)->GetWindowRect(&rect2);
	LargerMode = 0;
	//�޸�״ֵ̬�˹�����������Ϊ����ģʽ
	isLarge = false;

	OnBnClickedBtnwinsize();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CUcomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ
HCURSOR CUcomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//д��ע�������
void CUcomDlg::WriteRegData(void)
{
	CHARFORMAT cf;
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));
	pRich->GetSelectionCharFormat(cf);
	AfxGetApp()->WriteProfileInt("Config", "TextColor", cf.crTextColor);
	//û�л�ȡ����ɫ�ķ���ֻ���ñ�����¼
	AfxGetApp()->WriteProfileInt("Config", "GroundColor", backgroudColor);

	TRACE("Color Reg Finish %d\n", backgroudColor);
}

// װ��ע���������������Ϣ
void CUcomDlg::LoadRegConfig()
{
	CRichEditCtrl *pRich = (CRichEditCtrl*)GetDlgItem(IDC_RichRx);
	UINT tmp = AfxGetApp()->GetProfileInt("Config", "TextColor", 0xFF000000);
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);;
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;

	if (tmp == 0xFF000000)
	{
		//��������Ĭ������
		backgroudColor = RGB(242, 241, 215);
		cf.crTextColor = RGB(0, 0, 0);

		pRich->SetDefaultCharFormat(cf);
		pRich->SetBackgroundColor(false, backgroudColor);
		//�����������������Ϣ
		OnBnClickedBtnhelp();
	}
	else
	{
		//��ɫ����
		cf.crTextColor = AfxGetApp()->GetProfileInt("Config", "TextColor", RGB(0, 0, 0));
		pRich->SetDefaultCharFormat(cf);
		backgroudColor = AfxGetApp()->GetProfileInt("Config", "GroundColor", RGB(242, 241, 215));
		pRich->SetBackgroundColor(false, backgroudColor);
	}
}

// �������ݷ��Ϳ��������
void CUcomDlg::SendEditBoxData(void)
{
	CString tmpStr("\r\n");
	int tmpCnt;
	//����
	if (isNewLineSend == TRUE)
	{
		tmpCnt = 1 + uBase->AsyncSend(DataTx.GetCStrData() + tmpStr);
	}
	else
	{
		tmpCnt = uBase->AsyncSend(DataTx.GetCStrData());
	}

	if (tmpCnt > 0)
	{
		txCnt += tmpCnt;
		//ˢ�¼���
		tmpStr.Format(_T("���ͣ�%d Bytes"), txCnt);
		SetDlgItemText(IDC_TxtSendCnt, tmpStr);
	}
}

//ˢ�½��տ�
void CUcomDlg::ReflashRecvEdit(void)
{
	CString tmpStr;
	CRichEditCtrl *pRich = (CRichEditCtrl *)GetDlgItem(IDC_RichRx);

	static int cnt=0,lastRxCnt=0;
	//�����Ͱ�rxFlashPeriod = 50ms����,���г�20

	lastRxCnt += DataRx.GetLength();
	if (++cnt == 10)
	{
		//ˢ�¼���
		tmpStr.Format(_T("���գ�%d Bytes ,���ʣ�%d Bps"), rxCnt, lastRxCnt*2);
		SetDlgItemText(IDC_TxtRecvCnt, tmpStr);
		cnt = 0;
		lastRxCnt = 0;
	}

	if (DataRx.GetLength() != 0)
	{
		//��Limt�������У������ֵ���
		if (isRxFullClc && (pRich->GetTextLength() >limitBytes))
		{
			pRich->SetWindowTextA("");
		}
		if (isDispRx)
		{
			//���ѡ�����,���±༭������
			pRich->SetSel(-1, -1);
			if (isDispHex)
			{
				pRich->ReplaceSel(DataRx.GetCStrData());
			}
			else
			{
				switch (encoderMode)
				{
				case 0:
					pRich->ReplaceSel(DataRx.GetCStrData());
					break;
				case 1:
					pRich->ReplaceSel(EncoderDlg.Utf8toAscii(DataRx.GetCStrData()));
					break;
				case 2:
					pRich->ReplaceSel(EncoderDlg.Unicode2Ascii(DataRx.GetCStrData()));
					break;
				default:
					break;
				}
			}
		}
		DataRx.ClearData();
	}
}

// ��ʱ���ص�������ˢ����ʾ���ݺ��Զ�����
void CUcomDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case AUTO_SEND_TIMER_ID:
		SendEditBoxData();
		break;
	case FLASH_RX_EDITBOX_TIMER_ID:
		ReflashRecvEdit();
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


void CUcomDlg::OnBtnClearnCnt()
{
	CString tmpStr;
	rxCnt = 0;
	txCnt = 0;

	// �������
	tmpStr.Format(_T("���գ�%d Bytes ,���ʣ�0 Bps"), rxCnt);
	SetDlgItemText(IDC_TxtRecvCnt, tmpStr);

	tmpStr.Format(_T("�������ݣ�%d Bytes"), txCnt);
	SetDlgItemText(IDC_TxtSendCnt, tmpStr);
}


void CUcomDlg::OnCkbSendHex()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbSendHex))
	{
		// AT ģʽ�� HEX ���ͻ���
		if (isCmdMode) {
			CButton *pCkb = (CButton *)GetDlgItem(IDC_CkbSendHex);
			pCkb->SetCheck(BST_UNCHECKED);
		}
		else
		{
			isSendHex = TRUE;
			SetDlgItemText(IDC_EditTxData, "");
		}
	}
	else
	{
		isSendHex = FALSE;
	}

	DataTx.ClearData();
}

// ������������ݸ�ʽ����
void CUcomDlg::OnChangeEditTxData()
{
	CString strtmp;
	char *pCh;

	GetDlgItemText(IDC_EditTxData, strtmp);
	CEdit *pEb = (CEdit *)GetDlgItem(IDC_EditTxData);
	if (isSendHex)
	{
		bool isPure = true;
		pCh = strtmp.GetBuffer(0);
		while (*pCh != '\0')
		{
			// ��ֹ���������ַ�
			if (!TextBank::isHexChar(*pCh))
			{
				int pos = strtmp.Find(*pCh);
				strtmp = strtmp.Left(pos);
				SetDlgItemText(IDC_EditTxData, strtmp);
				pEb->SetSel(pos, pos, TRUE);
				isPure = FALSE;
				break;
			}
			pCh++;
		}

		if (isPure)
		{
			// 16���Ƹ�ʽ����
			DataTx.ReString(strtmp, TRUE);
		}
		else
		{
			HWND hWnd = ::GetLastActivePopup(GetSafeHwnd()); //���Ҽ����MessageBox
			if (hWnd != GetSafeHwnd())
			{
				::EndDialog(hWnd, 0);
			}
			DataTx.ClearData();
			AfxMessageBox(_T("��������Ч��HEX��ʽ!\n��Χ��('0-9','a-f','A-F')\n�ֽڼ�����ÿո����"));
		}
	}
	else
	{
		DataTx.ReString(strtmp);

		// ����CMDģʽ�س�����(���ݰ����س�)���������͵��������������ʽ�ӵ����տ��������
		if (isCmdMode && (strtmp.Find('\n') != -1))
		{
			CRichEditCtrl *pRich = (CRichEditCtrl*)GetDlgItem(IDC_RichRx);
			// ���ָ�ʽ�ṹ
			CHARFORMAT2 cf;
			// ȡ���ı���ǰ���ֵĸ�ʽ
			pRich->GetSelectionCharFormat(cf);
			// ɾ���س�
			strtmp = DataTx.GetCStrData();
			strtmp.Remove('\r');
			strtmp.Remove('\n');
			// ��¼��ʷ����
			cmdHistory[cmdNextPointer].Empty();
			cmdHistory[cmdNextPointer] = strtmp;

			if (++cmdNextPointer == MAX_CMD_HISTORY)
			{
				cmdNextPointer = 0;
				cmdDispPointer = MAX_CMD_HISTORY - 1;
			}
			else
			{
				cmdDispPointer = cmdNextPointer;
			}
			// �Զ���ATָ�����ϻ��з�
			strtmp.Append("\r\n");
			uBase->AsyncSend(strtmp);
			txCnt += strtmp.GetLength();

			cf.dwEffects &= ~CFE_AUTOCOLOR;
			cf.crTextColor = RGB(240, 60, 60);
			cf.dwEffects |= CFM_ITALIC | CFM_BOLD;
			// ���ı���ʽ��Ϣ���õ��ı���ǰ�ı�
			cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_COLOR;

			int preLen = pRich->GetWindowTextLengthA();
			pRich->SetSel(-1, -1);
			pRich->ReplaceSel("\r\n" + strtmp);

			if (pRich->GetLineCount() == 2)
			{
				pRich->SetSel(0, pRich->GetTextLength() - 1);//ͷ�������ж�
			}
			else
			{
				pRich->SetSel(pRich->LineIndex(pRich->GetLineCount() - 2) - 1,
								pRich->LineIndex(pRich->GetLineCount() - 1));
			}

			pRich->SetSelectionCharFormat(cf);//������ɫ
			pRich->SetSel(-1, 0);

			OnBtnClearSend();
			//ˢ�¼���
			strtmp.Format(_T("���ͣ�%d Bytes"), txCnt);
			SetDlgItemText(IDC_TxtSendCnt, strtmp);
		}
	}
}

// ������������Ի������Ϣ
afx_msg LRESULT CUcomDlg::OnMyReceiveMsg(WPARAM wParam, LPARAM lParam)
{
	CString dataStr,infoStr;

	TRACE("Main MSG:%x,%x\n", wParam, lParam);
	if ((wParam&WL_MASK) == WL_UCOM_RECV && (wParam&WKIND_MASK) == curDataSrc)
	{
		// ��Ϣ����Ҫ�����ȡ����
		uBase->AsyncRead(dataStr, infoStr, wParam, lParam);
		// �Ƿ���¶�����Ϣ
		if (infoStr.GetLength() != 0 && lastInfo != infoStr)
		{
			DataRx.AppendString(infoStr);
			lastInfo = infoStr;
		}
		TRACE("RecSize:%d\n", dataStr.GetAllocLength());
		DataWatchDlg.AddItem(dataStr);
		// ������ʾ����ˢ������
		rxCnt += dataStr.GetAllocLength();
		DataRx.AppendString(dataStr, isDispHex);
		GraphDlg.AddDataString(dataStr);
	}
	else if ((wParam&WL_MASK) == WL_UCOM_OPEN)
	{
		if (!isWorking)
		{
			isWorking = true;
			SetDelaySend();
			// �趨���տ��ı�ˢ��ʱ�� ����24Hz
			SetTimer(FLASH_RX_EDITBOX_TIMER_ID, rxFlashPeriod, NULL);
		}
	}
	else if ((wParam&WL_MASK) == WL_UCOM_CLOSE)
	{
		if (isWorking)
		{
			isWorking = false;
			KillTimer(AUTO_SEND_TIMER_ID); // �Զ����͹رն�ʱ��
			KillTimer(FLASH_RX_EDITBOX_TIMER_ID); // �رս�����ʾ��ʱ��
			// ����ر�ʱ����δˢ�µ������ֶ�ˢ��
			if (DataRx.GetLength())
			{
				ReflashRecvEdit();
			}
		}
	}
	return 0;
}

// ���ڹرմ���д��һЩ�û����ݵ�ע���
void CUcomDlg::OnClose()
{
	WriteRegData();
	UartDlg.WriteRegData();
	NetDlg.WriteRegData();
	CDialog::OnClose();
}

// ��չ��ť�ص�����
void CUcomDlg::OnBnClickedBtnwinsize()
{
	CRect rectOr;
	GetWindowRect(&rectOr);
	isLarge = !isLarge;
	if (isLarge)
	{
		LargerMode = 2;
		SetWindowPos(NULL, 0, 0, rectOr.Width() - widthEx, rectOr.Height(), SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "̽��>>");
	}
	else
	{
		LargerMode = 1;
		SetWindowPos(NULL, 0, 0, rectOr.Width() + widthEx, rectOr.Height(),SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "����<<");
	}
}

// ���ý��������ı��м��
void CUcomDlg::SetRichLineSpace(void)
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	PARAFORMAT2 pf;
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);

	memset(&pf, 0, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(PARAFORMAT2);
	pf.dwMask = PFM_LINESPACING;
	pf.bLineSpacingRule = 4;   // 0��5���ֱ��Ӧword�еĵ����оࡢ1.5���оࡢ2���оࡢ����1���趨����Сֵ���౶�о�

	pRich->GetDefaultCharFormat(cf);
	pf.dyLineSpacing = cf.yHeight * 14 / 10;	// ����word�а���20������bLineSpacingRule����3��4��5ʱ��Ч

	pRich->SetSel(0, -1);
	pRich->SetParaFormat(pf);
	pRich->SetSel(-1, 0);
}

// ������������Ϊ�ļ�
void CUcomDlg::OnBnClickedBtnsaverx()
{
	CFileDialog fDlg(
		false, // FALSEΪ�洢�ļ�
		NULL, // Ĭ����չ��
		"UcomRecvData.txt", // Ĭ���ļ���
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // ����ֻ����ѡ�Ϳ����ɸ�д����
		_T("�ı��ļ�(*.txt)|*.txt||"), // ��׺����
		NULL);
	CString strtmp;
	GetDlgItemText(IDC_RichRx, strtmp);
	if (IDOK == fDlg.DoModal())
	{
		CStdioFile stdFile;
		stdFile.Open(fDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		stdFile.WriteString(strtmp);
		stdFile.Close();
	}
}

// ��ʼ����չѡ�
void CUcomDlg::InitTabEx(void)
{
	CRect rect;
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABEx);
	pTab->InsertItem(GRAPH_TAB_INDEX, "����ͼ��");
	pTab->InsertItem(ENCODER_TAB_INDEX, "�������");
	pTab->InsertItem(DATA_WATCH_TAB_INDEX, "���ռ���");
	pTab->InsertItem(MULTI_SEND_TAB_INDEX, "��������");

	GraphDlg.Create(IDD_GRAPH, pTab);
	pTab->GetClientRect(&rect);
	rect.top += 27;
	GraphDlg.MoveWindow(&rect);
	GraphDlg.ShowWindow(true);

	EncoderDlg.Create(IDD_ENCODER, pTab);
	pTab->GetClientRect(&rect);
	rect.top += 27;
	EncoderDlg.MoveWindow(&rect);
	EncoderDlg.ShowWindow(false);

	DataWatchDlg.Create(IDD_WATCH, pTab);
	pTab->GetClientRect(&rect);
	rect.top += 27;
	DataWatchDlg.MoveWindow(&rect);
	DataWatchDlg.ShowWindow(false);

	MultiSendDlg.Create(IDD_XSEND, pTab);
	pTab->GetClientRect(&rect);
	rect.top += 27;
	MultiSendDlg.MoveWindow(&rect);
	MultiSendDlg.ShowWindow(false);
}

// ��ʼ������Դѡ�
void CUcomDlg::InitTabSrc(void)
{
	CRect rect;
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABSrc);
	pTab->InsertItem(0, "����");
	pTab->InsertItem(1, "����");

	pTab->GetClientRect(&rect);
	rect.top += 28;
	rect.left += 2;
	UartDlg.Create(IDD_UART, pTab);
	UartDlg.MoveWindow(&rect);
	UartDlg.ShowWindow(true);

	pTab->GetClientRect(&rect);
	rect.top += 28;
	rect.left += 2;
	NetDlg.Create(IDD_NET, pTab);
	NetDlg.MoveWindow(&rect);
	NetDlg.ShowWindow(false);
	// �л�����Դ
	SwitchCurDataSrc(WH_UCOM_UART);
}

// �л������շ�Դ
void CUcomDlg::SwitchCurDataSrc(UINT dataSrc)
{
	CString tmp;
	switch (dataSrc)
	{
	case WH_UCOM_UART:
		curDataSrc = WH_UCOM_UART;
		uBase = (UcomBase*)&UartDlg;
		break;
	case WH_UCOM_NET:
		curDataSrc = WH_UCOM_NET;
		uBase = (UcomBase*)&NetDlg;
		// socket����л������ݲ��������µ�msg����Ҫ��ȡ���һ��
		uBase->AsyncRead(tmp, tmp, 0, 0);
		break;
	default:
		break;
	}
}


// �л�ѡ�
void CUcomDlg::OnSelchangeTabex(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABEx);
	int sel = pTab->GetCurSel();
	*pResult = 0;

	GraphDlg.ShowWindow(sel== GRAPH_TAB_INDEX);
	EncoderDlg.ShowWindow(sel == ENCODER_TAB_INDEX);
	DataWatchDlg.ShowWindow(sel == DATA_WATCH_TAB_INDEX);
	MultiSendDlg.ShowWindow(sel == MULTI_SEND_TAB_INDEX);
}

// ����Դtab�л�
void CUcomDlg::OnSelchangeTabsrc(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABSrc);
	int sel = pTab->GetCurSel();
	*pResult = 0;

	switch (sel)
	{
	case 0:
		UartDlg.ShowWindow(true);
		NetDlg.ShowWindow(false);
		SwitchCurDataSrc(WH_UCOM_UART);
		break;
	case 1:
		UartDlg.ShowWindow(false);
		NetDlg.ShowWindow(true);
		SwitchCurDataSrc(WH_UCOM_NET);
		break;
	default:
		break;
	}
}

// �л�������ʾ����
void CUcomDlg::OnBnClickedBtnencoder()
{
	if (++encoderMode == 3)
		encoderMode = 0;
	switch (encoderMode)
	{
	case 0:
		SetDlgItemText(IDC_BtnEncoder, "  ���ձ��룺ASCII");
		break;
	case 1:
		SetDlgItemText(IDC_BtnEncoder, "  ���ձ��룺UTF-8");
		break;
	case 2:
		SetDlgItemText(IDC_BtnEncoder, "  ���ձ��룺Unicode");
		break;
	default:
		break;
	}

}

// �س����˳�����
void CUcomDlg::OnOK()
{
	//CDialog::OnOK();
	return;
}


void CUcomDlg::OnBnClickedBtnsendfile()
{
	if (!uBase->IsRWable()) return;
	CSendFile SendFileDlg(this, &uBase);
	SendFileDlg.DoModal();
}

// ����Ԥ����
BOOL CUcomDlg::PreTranslateMessage(MSG* pMsg)
{
	CEdit *pEdit= (CEdit*)GetDlgItem(IDC_EditTxData);
	int len;
	// ����������ѡ�񰴼��������ҽ����ڷ��Ϳ�
	if (pMsg->message == WM_KEYDOWN && GetDlgItem(IDC_EditTxData) == GetFocus())
	{
		if ((GetKeyState(VK_CONTROL) & 0x80))
		{
			// Ctrl + Enter send
			if (GetKeyState(VK_RETURN) & 0x80)
			{
				OnBtnSend();
				return true;
			}
			else
			{
				switch (pMsg->wParam)
				{
					// ֧��������ڵ�ctrl+aȫѡ����
					case 'a':
					case 'A':
						pEdit->SetSel(0, -1); return true;
						break;
					default:
						break;
				}
			}
		}

		// ATģʽ�ĸ߼�����
		if (isCmdMode)
		{
			// �����л���ʷ����
			switch (pMsg->wParam)
			{
				case VK_UP:
					if (--cmdDispPointer == -1)
						cmdDispPointer = MAX_CMD_HISTORY - 1;
					SetDlgItemText(IDC_EditTxData, cmdHistory[cmdDispPointer]);
					TRACE("CMD_U:%d\n", cmdDispPointer);

					len = pEdit->GetWindowTextLengthA();
					pEdit->SetSel(len, len);
					pEdit->SetFocus();
					break;
				case VK_DOWN:
					if (++cmdDispPointer >= MAX_CMD_HISTORY)
						cmdDispPointer = 0;
					SetDlgItemText(IDC_EditTxData, cmdHistory[cmdDispPointer]);
					TRACE("CMD_D:%d\n", cmdDispPointer);

					len = pEdit->GetWindowTextLengthA();
					pEdit->SetSel(len, len);
					pEdit->SetFocus();
					break;
				// tab��ȫAT+
				case VK_TAB:
					pEdit->SetSel(0, 0);
					pEdit->ReplaceSel("AT+");
					len = pEdit->GetWindowTextLengthA();
					pEdit->SetSel(len, len);
					pEdit->SetFocus();
					return true;
				default:
					break;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// ATģʽ�л�
void CUcomDlg::OnBnClickedCkbcmd()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbCMD))
	{
		if (isSendHex != true)
		{
			isCmdMode = TRUE;
		}
		else
		{
			// ��֧��HEXģʽ��cmd�Ի�
			CButton *pCkb = (CButton *)GetDlgItem(IDC_CkbCMD);
			pCkb->SetCheck(BST_UNCHECKED);
		}
	}
	else
		isCmdMode = FALSE;
}
// ���ڶ�̬�仯
void CUcomDlg::OnSize(UINT nType, int cx, int cy)
{
	static int lastCx = 0, lastCy = 0;

	CDialog::OnSize(nType, cx, cy);
	if (LargerMode)
	{
		lastCx = cx;
		LargerMode = 0;
		return;
	}


	if (nType == SIZE_RESTORED)
	{
		int dX = cx - lastCx;
		int dY = cy - lastCy;

		ChangeItemSize(IDC_RichRx, dX, 0, true);
		ChangeItemSize(IDC_EditTxData, dX, dY, true);
		ChangeItemSize(IDC_GrpRS, dX, dY, true);

		ChangeItemSize(IDC_TABEx, dX, 0, false);

		ChangeItemSize(IDC_BtnSendFile, dX, dY, false);
		ChangeItemSize(IDC_BtnToolBox, dX, dY, false);
		ChangeItemSize(IDC_BtnHelp, dX, dY, false);
		ChangeItemSize(IDC_BtnSend, dX, dY, false);
		ChangeItemSize(IDC_BtnWinSize, dX, dY, false);

		ChangeItemSize(IDC_BtnSplitter, dX, 0, true);


		hzSplitter.SetBottomLimit(dY);
		//step=1;
	}
	// ������С������
	if (cx != 0)
		lastCx = cx;
	if (cy != 0)
		lastCy = cy;
}

// �϶����ڱ䶯����ֵ�ص�
void CUcomDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.y = heightDlgMin;

	if (isLarge)
	{
		lpMMI->ptMinTrackSize.x = widthDlgMin;
	}
	else
	{
		lpMMI->ptMinTrackSize.x = widthDlgMin + widthEx;
	}
	CDialog::OnGetMinMaxInfo(lpMMI);
}
