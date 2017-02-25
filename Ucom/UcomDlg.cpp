// UcomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "UcomDlg.h"
#include "afxdialogex.h"

#include "DeConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUcomDlg 对话框


CUcomDlg::CUcomDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUcomDlg::IDD, pParent)
, DataRx(), DataTx(), mUart()
, MultiSendDlg(this, &uartPortIsOpen,mUart.GetHandleAddr())
{
	//{{AFX_DATA_INIT(CUcomDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//设置程序图标
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CUcomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUcomDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnOpen, OnBtnOpen)
	ON_BN_CLICKED(IDC_BtnSend, OnBtnSend)
	ON_CBN_DROPDOWN(IDC_CbUartPort, OnDropdownCbUartPort)
	ON_CBN_SELENDOK(IDC_CbUartPort, OnSelendokCbUartPort)
	ON_BN_CLICKED(IDC_Launch, OnLaunch)
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
	ON_MESSAGE(WM_COMM_RX_MSG, &CUcomDlg::OnRxMsgProc)


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
//	ON_WM_CHAR()
ON_BN_CLICKED(IDC_CkbCMD, &CUcomDlg::OnBnClickedCkbcmd)
END_MESSAGE_MAP()


// CUcomDlg 消息处理程序

BOOL CUcomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	SetWindowText(_T("Ucom v1.02 轻串口 长春理工大学电子学会"));

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


#ifdef USING_CONSLOE
	InitDebugConsole();
#endif

	hRxThread = NULL;
	uartPortIsOpen = false;
	isNewLineSend = false;
	isDispHex = false;
	isSendHex = false;
	isDispRx = true;
	isCmdMode = false;
	encoderMode = 0;
	rxCnt = 0;
	txCnt = 0;

	InitTabEx();

	InitCbBuart();
	//装载注册表数据
	LoadRegConfig();
	SetDlgItemInt(IDC_EdbSendDelay, 1000);
	SetDlgItemInt(IDC_EdbFullBytes, 5120);

	
	DisableCbUart(FALSE);

	SetRichLineSpace();
	OnBnClickedBtnwinsize();

	cmdNextPointer = 0;
	cmdDispPointer = 0;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUcomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUcomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUcomDlg::ChangeBmpPic(int PicCtrlID, unsigned short nPicID)
{
	CBitmap bitmap;
	HBITMAP hBmp;
	CStatic *pStatic = (CStatic*)GetDlgItem(PicCtrlID);

	bitmap.LoadBitmap(nPicID);				// 将位图IDB_BITMAP1加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄

	pStatic->SetBitmap(hBmp);				// 设置图片控件
}

//从空间得到配置信息字符串
DCB CUcomDlg::GetUartConfigDCB(void)
{
	DCB configDCB;
	CString tmpStr, UartConfig;
	CComboBox *pCombox;

	//设定串口参数
	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartBaudrate);
	pCombox->GetLBText(pCombox->GetCurSel(), tmpStr);
	configDCB.BaudRate = _ttoi(tmpStr);

	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartECC);
	configDCB.Parity = pCombox->GetCurSel();

	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartDatabit);
	pCombox->GetLBText(pCombox->GetCurSel(), tmpStr);
	configDCB.ByteSize = atoi(tmpStr);

	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartStopbit);
	configDCB.StopBits = pCombox->GetCurSel();

	return configDCB;
}

bool CUcomDlg::IsUartPortAvailable(void)
{
	CString comInfo;
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	pCombox->GetLBText(pCombox->GetCurSel(), comInfo);

	if (comInfo.Left(3) == _T("COM"))
		return TRUE;
	else
		return FALSE;
}


void CUcomDlg::OpenUart()
{
	CButton *pButton;
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	CString strtmp;

	if (uartPortIsOpen == FALSE)
	{
		//如果占用就取消占用
		if (mUart.isConnected())
			mUart.ClosePort();

		if (IsUartPortAvailable())
		{
			pCombox->GetLBText(pCombox->GetCurSel(), strtmp);
			mUart.ConfigUart(strtmp, GetUartConfigDCB());
		}
		else
		{
			AfxMessageBox(_T("串口无效,请刷新"));
			return;
		}

		if (!mUart.OpenCom())
		{
			AfxMessageBox(_T("打开串口失败"));
			return;
		}

		//创建接收线程
		hRxThread = AfxBeginThread(RxThreadFunc, mUart.GetThreadStartPara(), THREAD_PRIORITY_NORMAL);
		if (hRxThread == NULL){
			TRACE("Rx Listenner Thread Created Failed");
			return;
		}

		pButton = (CButton*)GetDlgItem(IDC_BtnOpen);
		SetDlgItemText(IDC_BtnOpen, _T("关闭串口"));
		ChangeBmpPic(IDC_PicUartStatus, IDB_SwOn);

		DisableCbUart(TRUE);
		uartPortIsOpen = TRUE;
		SetDelaySend();
		//设定接收框文本刷新时间 人眼24Hz
		SetTimer(FLASH_RX_EDITBOX_TIMER_ID, rxFlashPeriod, NULL);
	}
	else if (uartPortIsOpen == TRUE)
	{
		//先置位标志位避免关闭时候接收错误
		uartPortIsOpen = FALSE;
		KillTimer(AUTO_SEND_TIMER_ID);//自动发送关闭定时器
		KillTimer(FLASH_RX_EDITBOX_TIMER_ID);//关闭接收显示定时器

		//结束接收线程
		//mUart.CloseWaitThread();
		TerminateThread(hRxThread, 0);

		SetDlgItemText(IDC_BtnOpen, _T("打开串口"));
		ChangeBmpPic(IDC_PicUartStatus, IDB_SwOff);
		DisableCbUart(FALSE);
		//关闭串口
		if (mUart.isConnected())
			mUart.ClosePort();
	}
}

void CUcomDlg::OnBtnOpen()
{
	OpenUart();
}

void CUcomDlg::InitCbBuart(void)
{
	CComboBox *pComBox;

	pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	pComBox->ResetContent();
	//获取串口列表
	mUart.GetComList(pComBox);
	//默认选择第一个
	pComBox->SetCurSel(0);

	//添加波特率
	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartBaudrate);
	TCHAR BaudrateTable[][7] = { "460800", "230400", "194000", "115200", "57600", "56000", "38400"
		, "19200", "14400", "9600", "4800", "2400", "1200" };
	for (int i = 0; i < (sizeof(BaudrateTable) / sizeof(BaudrateTable[0])); i++)
		pComBox->InsertString(0, BaudrateTable[i]);

	//选择115200为默认
	pComBox->SetCurSel(pComBox->FindString(-1, "115200"));

	//数据位
	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartDatabit);
	pComBox->InsertString(0, "8");
	pComBox->InsertString(0, "7");
	pComBox->InsertString(0, "6");
	pComBox->InsertString(0, "5");

	pComBox->SetCurSel(3);//选择8位为默认

	//停止位
	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartStopbit);
	pComBox->InsertString(0, "2");
	pComBox->InsertString(0, "1.5");
	pComBox->InsertString(0, "1");

	pComBox->SetCurSel(0);//选择115200位为默认

	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartECC);
	pComBox->InsertString(0, "even");
	pComBox->InsertString(0, "odd ");
	pComBox->InsertString(0, "none");
	//选择无校验为默认
	pComBox->SetCurSel(0);
	TRACE("Add ok");
}



bool CUcomDlg::InitRegData(void)
{
	CString tmp = AfxGetApp()->GetProfileString("Config", "BDSE", "NULL");
	if (tmp == "NULL")
		return false;
	else
		return true;
}

void CUcomDlg::WriteRegData(void)
{
	CComboBox *pCombox;
	CString strtmp;
	pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	//串口号
	pCombox->GetLBText(pCombox->GetCurSel(), strtmp);
	AfxGetApp()->WriteProfileString("Config", "ComName", strtmp);

	strtmp.Empty();
	strtmp.Format("%d-%d-%d-%d",
		((CComboBox*)GetDlgItem(IDC_CbUartBaudrate))->GetCurSel()
		, ((CComboBox*)GetDlgItem(IDC_CbUartDatabit))->GetCurSel()
		, ((CComboBox*)GetDlgItem(IDC_CbUartStopbit))->GetCurSel()
		, ((CComboBox*)GetDlgItem(IDC_CbUartECC))->GetCurSel());
	//配置数据
	AfxGetApp()->WriteProfileString("Config", "BDSE", strtmp);

	CHARFORMAT cf;
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));
	pRich->GetSelectionCharFormat(cf);
	AfxGetApp()->WriteProfileInt("Config", "TextColor", cf.crTextColor);
	//没有获取背景色的方法只能用变量记录
	AfxGetApp()->WriteProfileInt("Config", "GroundColor", backgroudColor);

	TRACE("Write Reg Finish %d\n", backgroudColor);
}

void CUcomDlg::GetRegData(CString &comName, CString &dcbConfig)
{
	comName = AfxGetApp()->GetProfileString("Config", "ComName", "NULL");
	dcbConfig = AfxGetApp()->GetProfileString("Config", "BDSE", "NULL");
	TRACE(comName + dcbConfig);
}

void CUcomDlg::LoadRegConfig()
{
	CString comName, dcbConfig;
	CComboBox *pComBox = (CComboBox*)GetDlgItem(IDC_CbUartPort);
	CRichEditCtrl *pRich = (CRichEditCtrl*)GetDlgItem(IDC_RichRx);
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);;
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;

	if (InitRegData() == false)
	{
		//初次运行默认设置
		backgroudColor = RGB(242, 241, 215);
		cf.crTextColor = RGB(0, 0, 0);

		pRich->SetDefaultCharFormat(cf);
		pRich->SetBackgroundColor(false, backgroudColor);
	}
	else
	{
		GetRegData(comName, dcbConfig);
		int sel = pComBox->FindString(-1, comName);
		//没有该串口，或者不是串口号使用默认（初始化值）
		if (sel != CB_ERR && comName[0] == 'C')
			pComBox->SetCurSel(sel);

		int baudSel = 0, dataSel = 0, stopSel = 0, eccSel = 0;

		sscanf_s(dcbConfig, "%d-%d-%d-%d", &baudSel, &dataSel, &stopSel, &eccSel);
		((CComboBox*)GetDlgItem(IDC_CbUartBaudrate))->SetCurSel(baudSel);
		((CComboBox*)GetDlgItem(IDC_CbUartDatabit))->SetCurSel(dataSel);
		((CComboBox*)GetDlgItem(IDC_CbUartStopbit))->SetCurSel(stopSel);
		((CComboBox*)GetDlgItem(IDC_CbUartECC))->SetCurSel(eccSel);

		//颜色设置
		cf.crTextColor = AfxGetApp()->GetProfileInt("Config", "TextColor", RGB(0, 0, 0));
		pRich->SetDefaultCharFormat(cf);
		backgroudColor = AfxGetApp()->GetProfileInt("Config", "GroundColor", RGB(242, 241, 215));
		pRich->SetBackgroundColor(false, backgroudColor);
	}
}

//串口设置是否可操作
void CUcomDlg::DisableCbUart(bool choose)
{
	choose = !choose;
	GetDlgItem(IDC_CbUartBaudrate)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartDatabit)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartStopbit)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartECC)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartPort)->EnableWindow(choose);
}

void CUcomDlg::InitTabEx(void)
{
	CRect rect;
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABEx);
	pTab->InsertItem(0, "接收图表");
	pTab->InsertItem(1, "编码查询");
	pTab->InsertItem(2, "接收监视");
	pTab->InsertItem(3, "发送助手");

	//186*243
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


void CUcomDlg::SendEditBoxData(void)
{
	CString tmpStr("\r\n");
	//换行
	if (isNewLineSend == TRUE) {
		mUart.UnblockSend(DataTx.GetCStrData() + tmpStr);
		txCnt = txCnt+1+ DataTx.GetLength();
	}
	else {
		mUart.UnblockSend(DataTx.GetCStrData());
		txCnt += DataTx.GetLength();
	}
	
	//刷新计数
	tmpStr.Format(_T("发送数据：%d Bytes"), txCnt);
	SetDlgItemText(IDC_GrpSend, tmpStr);
}

//刷新接收框
void CUcomDlg::ReflashRecvEdit(void)
{
	CString tmpStr;
	CRichEditCtrl *pRich = (CRichEditCtrl *)GetDlgItem(IDC_RichRx);
	
	static int cnt=0,lastRxCnt=0;
	//计数就按rxFlashPeriod = 50ms来吧,队列长20

	lastRxCnt += DataRx.GetLength();
	if (++cnt == 10)
	{
		//刷新计数
		tmpStr.Format(_T("接收数据：%d Bytes ,接收速度：%d Bps"), rxCnt, lastRxCnt*2);
		SetDlgItemText(IDC_GrpRecv, tmpStr);
		cnt = 0;
		lastRxCnt = 0;
	}
	


	if (DataRx.GetLength() != 0)
	{
		//用Limt方法不行，其最大值会变
		if (isRxFullClc && (pRich->GetTextLength() >limitBytes))
		{
			pRich->SetWindowTextA("");
		}
		if (isDispRx)
		{
			//光标选择最后,更新编辑框内容
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
					pRich->ReplaceSel(DataRx.GetCStrData()); break;
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


void CUcomDlg::OnLaunch()
{
	//	AfxMessageBox(_T("Hello World!"));
	//TRACE("%d\n", DataRx.ValueOfString("12"));
	//::SendMessage(::AfxGetMainWnd()->m_hWnd, WM_COMM_RX_MSG, 1, 0);
	// TODO: Add your control notification handler code here
	//CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	//CRect rect;
	//GetClientRect(&rect);

	//CClientDC dc(this);
	//dc.MoveTo(rect.left, rect.top);
	//dc.LineTo(rect.right, rect.bottom);

	//dc.MoveTo(rect.left, rect.bottom);
	//dc.LineTo(rect.right, rect.top);
	//dc.MoveTo(rect.left, rect.top);

	//dc.Rectangle(0, 0, 100, 100);

	AfxMessageBox("电子学会欢迎您");


	//GetFocus()==GetDlgItem(  )
}

void CUcomDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
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

	//清零计数
	tmpStr.Format(_T("接收数据：%d Bytes ,接收速度：0 Bps"), rxCnt);
	SetDlgItemText(IDC_GrpRecv, tmpStr);

	tmpStr.Format(_T("发送数据：%d Bytes"), txCnt);
	SetDlgItemText(IDC_GrpSend, tmpStr);
}


void CUcomDlg::OnCkbSendHex()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbSendHex))
	{
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
		isSendHex = FALSE;
	
	DataTx.ClearData();
}


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
			//禁止输入其他字符
			if (!DataTx.isHexChar(*pCh))
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
			//16进制格式存入
			DataTx.ReString(strtmp, TRUE);
		}
		else
		{
			HWND hWnd = ::GetLastActivePopup(GetSafeHwnd()); //查找激活的MessageBox
			if (hWnd != GetSafeHwnd())
			{
				::EndDialog(hWnd, 0);
			}
			DataTx.ClearData();
			AfxMessageBox(_T("请输入有效的HEX格式!\n范围：('0-9','a-f','A-F')\n字节间可以用空格隔开"));
		}
	}
	else
	{
		DataTx.ReString(strtmp);

		//若是CMD模式回车发送(数据包含回车)，并将发送的内容添以特殊格式加到接收框的新行中
		if (isCmdMode && uartPortIsOpen && (strtmp.Find('\n') != -1)) {
			CRichEditCtrl *pRich = (CRichEditCtrl*)GetDlgItem(IDC_RichRx);
			//文字格式结构
			CHARFORMAT2 cf;
			//取得文本框当前文字的格式
			pRich->GetSelectionCharFormat(cf);
			//删除回车
			strtmp = DataTx.GetCStrData();
			strtmp.Remove('\r');
			strtmp.Remove('\n');
			//记录历史命令
			cmdHistory[cmdNextPointer].Empty();
			cmdHistory[cmdNextPointer] = strtmp;
			cmdDispPointer = cmdNextPointer;
			if (++cmdNextPointer == MAX_CMD_HISTORY)
				cmdNextPointer = 0;

			strtmp.Append("\r\n");
			mUart.UnblockSend(strtmp);
			txCnt += strtmp.GetLength();

			cf.dwEffects &= ~CFE_AUTOCOLOR;
			cf.crTextColor = RGB(240, 60, 60);
			cf.dwEffects |= CFM_ITALIC | CFM_BOLD;
			//将文本格式信息设置到文本框当前文本
			cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_COLOR;


			pRich->SetSel(-1, -1);
			pRich->ReplaceSel("\r\n" + strtmp);

			if (pRich->GetLineCount() == 2)
				pRich->SetSel(0, pRich->GetTextLength()-1);//头行特殊判断
			else
				pRich->SetSel(pRich->LineIndex(pRich->GetLineCount() - 2) - 1, 
								pRich->LineIndex(pRich->GetLineCount() - 1));
			
			pRich->SetSelectionCharFormat(cf);//设置颜色
			pRich->SetSel(-1, 0);
			
			OnBtnClearSend();
			//刷新计数
			strtmp.Format(_T("发送数据：%d Bytes"), txCnt);
			SetDlgItemText(IDC_GrpSend, strtmp);
		}
	}
}

//接收消息处理
afx_msg LRESULT CUcomDlg::OnRxMsgProc(WPARAM wParam, LPARAM lParam)
{
	CString strtmp;
	if (uartPortIsOpen == true)
	{
		mUart.UnblockRead(strtmp);
		rxCnt += strtmp.GetAllocLength();
		DataRx.AppendString(strtmp, isDispHex);

		DataWatchDlg.AddItem(strtmp);
		GraphDlg.AddDataString(strtmp);
	}

	return 0;
}


void CUcomDlg::OnClose()
{
	WriteRegData();
	CDialog::OnClose();
}


void CUcomDlg::OnBnClickedBtnwinsize()
{
	static bool isLarge = true;
	static CRect rectlarge;      //定义没有切割的（即全部的）矩形         
	static CRect rectsmall;      //定义剩余的小矩形

	if (rectlarge.IsRectEmpty())  //如果还没有给大矩形的大小赋值的话，在｛｝中赋值
	{
		GetWindowRect(&rectlarge);
		CRect rectseperator;
		GetDlgItem(IDC_BtnWinSize)->GetWindowRect(&rectseperator);
		rectsmall.left = rectlarge.left;
		rectsmall.top = rectlarge.top;
		rectsmall.right = rectseperator.right + 20;
		rectsmall.bottom = rectlarge.bottom;
	}

	if (isLarge)
	{
		SetWindowPos(NULL, 0, 0, rectsmall.Width(), rectsmall.Height(), SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "探索>>");
	}
	else
	{
		SetWindowPos(NULL, 0, 0, rectlarge.Width(), rectlarge.Height(), SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "极简<<");
	}
	isLarge = !isLarge;
}


void CUcomDlg::SetRichLineSpace(void)
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	PARAFORMAT2 pf;
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);

	memset(&pf, 0, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(PARAFORMAT2);
	pf.dwMask = PFM_LINESPACING;
	pf.bLineSpacingRule = 4;   //0到5，分别对应word中的单倍行距、1.5倍行距、2倍行距、大于1行设定、最小值、多倍行距
	
	pRich->GetDefaultCharFormat(cf);
	pf.dyLineSpacing = cf.yHeight * 14 / 10;	//等于word中磅的20倍，当bLineSpacingRule等于3、4、5时有效

	pRich->SetSel(0, -1);
	pRich->SetParaFormat(pf);
	pRich->SetSel(-1, 0);
}


void CUcomDlg::OnBnClickedBtnsaverx()
{
	CFileDialog fDlg(
		false, // FALSE为存储文件
		NULL, // 默认扩展名
		"UcomRecvData.txt", // 默认文件名
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // 隐藏只读复选和开启可覆写功能
		_T("文本文件(*.txt)|*.txt||"), // 后缀过滤
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


void CUcomDlg::OnSelchangeTabex(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABEx);
	int sel = pTab->GetCurSel();
	*pResult = 0;

	switch (sel)
	{
	case 0:
		GraphDlg.ShowWindow(true);
		EncoderDlg.ShowWindow(false);
		DataWatchDlg.ShowWindow(false);
		MultiSendDlg.ShowWindow(false);
		break;
	case 1:
		GraphDlg.ShowWindow(false);
		EncoderDlg.ShowWindow(true);
		DataWatchDlg.ShowWindow(false);
		MultiSendDlg.ShowWindow(false);
		break; 
	case 2:
		GraphDlg.ShowWindow(false);
		EncoderDlg.ShowWindow(false);
		DataWatchDlg.ShowWindow(true);
		MultiSendDlg.ShowWindow(false);
		break;
	case 3:
		GraphDlg.ShowWindow(false);
		EncoderDlg.ShowWindow(false);
		DataWatchDlg.ShowWindow(false);
		MultiSendDlg.ShowWindow(true);
		break;
	default:
		break;
	}
}


void CUcomDlg::OnBnClickedBtnencoder()
{
	if (++encoderMode == 3)
		encoderMode = 0;
	switch (encoderMode)
	{
	case 0:
		SetDlgItemText(IDC_BtnEncoder, "  接收编码：ASCII");
		break;
	case 1:
		SetDlgItemText(IDC_BtnEncoder, "  接收编码：UTF-8");
		break;
	case 2:
		SetDlgItemText(IDC_BtnEncoder, "  接收编码：Unicode");
		break;
	default:
		break;
	}

}

void CUcomDlg::OnOK()
{
	//回车不退出程序
	//CDialog::OnOK();
	return;
}


void CUcomDlg::OnBnClickedBtnsendfile()
{
	if (!uartPortIsOpen)
		return;
	CSendFile SendFileDlg(this, mUart.GetHandleAddr());
	SendFileDlg.DoModal();
}


BOOL CUcomDlg::PreTranslateMessage(MSG* pMsg)
{
	CEdit *pEdit= (CEdit*)GetDlgItem(IDC_EditTxData);
	//按键弹起并且选择按键触发而且焦点在最发送框
	if (pMsg->message == WM_KEYDOWN 
		&& GetDlgItem(IDC_EditTxData) == GetFocus())
	{
		//上下切换历史命令
		switch (pMsg->wParam)
		{
		case VK_UP:
			SetDlgItemText(IDC_EditTxData, cmdHistory[cmdDispPointer]);
			if (--cmdDispPointer == -1)
				cmdDispPointer = MAX_CMD_HISTORY - 1;
			pEdit->SetSel(-1, -1, FALSE);
			pEdit->SetFocus();
			break;
		case VK_DOWN:
			SetDlgItemText(IDC_EditTxData, cmdHistory[cmdDispPointer]);
			if (++cmdDispPointer == MAX_CMD_HISTORY)
				cmdDispPointer = 0;
			pEdit->SetSel(-1, -1, FALSE);
			pEdit->SetFocus();
			break;
		default:break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CUcomDlg::OnBnClickedCkbcmd()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbCMD))
	{
		if (isSendHex != true) {
			isCmdMode = TRUE;
		}
		else
		{
			//不支持HEX模式的cmd对话
			CButton *pCkb = (CButton *)GetDlgItem(IDC_CkbCMD);
			pCkb->SetCheck(BST_UNCHECKED);
		}
	}
	else
		isCmdMode = FALSE;
}
