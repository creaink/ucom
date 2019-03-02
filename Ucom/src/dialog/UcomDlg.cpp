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
, DataRx(), DataTx()
, MultiSendDlg(this, &uBase)
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

// CUcomDlg 消息处理程序

BOOL CUcomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect1, rect2;

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetWindowText(_T("Ucom v1.1.0 轻串口"));

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

	//窗口分隔按键初始化
	hzSplitter.SubclassDlgItem(IDC_BtnSplitter, this);
	hzSplitter.SetType(CS_HORZ);

	hzSplitter.AddToTopOrLeftCtrls(IDC_RichRx);
	hzSplitter.AddToBottomOrRightCtrls(IDC_EditTxData);
	hzSplitter.AddToBottomOrRightCtrls(IDC_TxtSendCnt, SPF_TOP);
	hzSplitter.AddToBottomOrRightCtrls(IDC_TxtRecvCnt, SPF_TOP);

	// 数据源子窗口初始化
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

	// 最大最小里面的值和这里获取的值有出入
	// 获取控件相对屏幕的区域坐标
	GetDlgItem(IDC_TABEx)->GetWindowRect(&rect1);

	// 数据收发框
	GetDlgItem(IDC_GrpRS)->GetWindowRect(&rect2);
	rect1.left = (rect2.right + rect1.left) / 2 + 2;

	GetWindowRect(&rect2);
	rect1.right = rect2.right;
	widthEx = rect1.Width();
	heightDlgMin = rect1.Height() + 55; // 补偿55

	GetDlgItem(IDC_BtnSplitter)->GetWindowRect(&rect2);
	widthDlgMin = rect2.left + 120; // 补偿120

	GetDlgItem(IDC_EditTxData)->GetWindowRect(&rect2);
	LargerMode = 0;
	//修改状态值人工将界面设置为极简模式
	isLarge = false;

	OnBnClickedBtnwinsize();

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

//当用户拖动最小化窗口时系统调用此函数取得光标显示
HCURSOR CUcomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//写入注册表数据
void CUcomDlg::WriteRegData(void)
{
	CHARFORMAT cf;
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));
	pRich->GetSelectionCharFormat(cf);
	AfxGetApp()->WriteProfileInt("Config", "TextColor", cf.crTextColor);
	//没有获取背景色的方法只能用变量记录
	AfxGetApp()->WriteProfileInt("Config", "GroundColor", backgroudColor);

	TRACE("Color Reg Finish %d\n", backgroudColor);
}

// 装载注册表里的软件设置信息
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
		//初次运行默认设置
		backgroudColor = RGB(242, 241, 215);
		cf.crTextColor = RGB(0, 0, 0);

		pRich->SetDefaultCharFormat(cf);
		pRich->SetBackgroundColor(false, backgroudColor);
		//初次运行输出帮助信息
		OnBnClickedBtnhelp();
	}
	else
	{
		//颜色设置
		cf.crTextColor = AfxGetApp()->GetProfileInt("Config", "TextColor", RGB(0, 0, 0));
		pRich->SetDefaultCharFormat(cf);
		backgroudColor = AfxGetApp()->GetProfileInt("Config", "GroundColor", RGB(242, 241, 215));
		pRich->SetBackgroundColor(false, backgroudColor);
	}
}

// 发送数据发送框里的数据
void CUcomDlg::SendEditBoxData(void)
{
	CString tmpStr("\r\n");
	int tmpCnt;
	//换行
	if (isNewLineSend == TRUE) {
		tmpCnt = 1 + uBase->AsyncSend(DataTx.GetCStrData() + tmpStr);
	}
	else {
		tmpCnt = uBase->AsyncSend(DataTx.GetCStrData());
	}
	if (tmpCnt > 0) {
		txCnt += tmpCnt;
		//刷新计数
		tmpStr.Format(_T("发送：%d Bytes"), txCnt);
		SetDlgItemText(IDC_TxtSendCnt, tmpStr);
	}
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
		tmpStr.Format(_T("接收：%d Bytes ,速率：%d Bps"), rxCnt, lastRxCnt*2);
		SetDlgItemText(IDC_TxtRecvCnt, tmpStr);
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

// 定时器回调，用于刷新显示数据和自动发送
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

	//清零计数
	tmpStr.Format(_T("接收：%d Bytes ,速率：0 Bps"), rxCnt);
	SetDlgItemText(IDC_TxtRecvCnt, tmpStr);

	tmpStr.Format(_T("发送数据：%d Bytes"), txCnt);
	SetDlgItemText(IDC_TxtSendCnt, tmpStr);
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

//输入框输入数据格式处理
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
		if (isCmdMode && (strtmp.Find('\n') != -1)) {
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
			
			if (++cmdNextPointer == MAX_CMD_HISTORY) {
				cmdNextPointer = 0;
				cmdDispPointer = MAX_CMD_HISTORY - 1;
			}
			else
			{
				cmdDispPointer = cmdNextPointer;
			}
			// 自动在AT指令后加上换行符
			strtmp.Append("\r\n");
			uBase->AsyncSend(strtmp);
			txCnt += strtmp.GetLength();

			cf.dwEffects &= ~CFE_AUTOCOLOR;
			cf.crTextColor = RGB(240, 60, 60);
			cf.dwEffects |= CFM_ITALIC | CFM_BOLD;
			//将文本格式信息设置到文本框当前文本
			cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_COLOR;

			int preLen = pRich->GetWindowTextLengthA();
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
			strtmp.Format(_T("发送：%d Bytes"), txCnt);
			SetDlgItemText(IDC_TxtSendCnt, strtmp);
		}
	}
}

// 处理发来主界面对话框的消息
afx_msg LRESULT CUcomDlg::OnMyReceiveMsg(WPARAM wParam, LPARAM lParam)
{
	CString dataStr,infoStr;

	TRACE("Main MSG:%x,%x\n", wParam, lParam);
	if ((wParam&WL_MASK) == WL_UCOM_RECV && (wParam&WKIND_MASK) == curDataSrc)
	{
		// 消息来到要尽快读取数据
		uBase->AsyncRead(dataStr, infoStr, wParam, lParam);
		// 是否更新额外信息
		if (infoStr.GetLength() != 0 && lastInfo != infoStr) {
			DataRx.AppendString(infoStr);
			lastInfo = infoStr;
		}
		TRACE("RecSize:%d\n", dataStr.GetAllocLength());
		DataWatchDlg.AddItem(dataStr);
		// 加入显示窗口刷新区域
		rxCnt += dataStr.GetAllocLength();
		DataRx.AppendString(dataStr, isDispHex);
		GraphDlg.AddDataString(dataStr);
	}
	else if ((wParam&WL_MASK) == WL_UCOM_OPEN)
	{
		if (!isWorking) {
			isWorking = true;
			SetDelaySend();
			//设定接收框文本刷新时间 人眼24Hz
			SetTimer(FLASH_RX_EDITBOX_TIMER_ID, rxFlashPeriod, NULL);
		}
	}
	else if ((wParam&WL_MASK) == WL_UCOM_CLOSE)
	{
		if (isWorking) {
			isWorking = false;
			KillTimer(AUTO_SEND_TIMER_ID); //自动发送关闭定时器
			KillTimer(FLASH_RX_EDITBOX_TIMER_ID); //关闭接收显示定时器
			// 如果关闭时候还有未刷新的数据手动刷新
			if (DataRx.GetLength()) {
				ReflashRecvEdit();
			}
		}
	}
	return 0;
}

//窗口关闭处理，写入一些用户数据到注册表
void CUcomDlg::OnClose()
{
	WriteRegData();
	UartDlg.WriteRegData();
	NetDlg.WriteRegData();
	CDialog::OnClose();
}

//扩展按钮回调函数
void CUcomDlg::OnBnClickedBtnwinsize()
{
	CRect rectOr;
	GetWindowRect(&rectOr);
	isLarge = !isLarge;
	if (isLarge)
	{
		LargerMode = 2;
		SetWindowPos(NULL, 0, 0, rectOr.Width()- widthEx, rectOr.Height(), SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "探索>>");
	}
	else
	{
		LargerMode = 1;
		SetWindowPos(NULL, 0, 0, rectOr.Width() + widthEx, rectOr.Height(),SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "极简<<");
	}
}

//设置接收区富文本行间距
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

//将接收区保存为文件
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

//初始化扩展选项卡
void CUcomDlg::InitTabEx(void)
{
	CRect rect;
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABEx);
	pTab->InsertItem(0, "接收图表");
	pTab->InsertItem(1, "编码解码");
	pTab->InsertItem(2, "接收监视");
	pTab->InsertItem(3, "发送助手");

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

//初始化数据源选项卡
void CUcomDlg::InitTabSrc(void)
{
	CRect rect;
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABSrc);
	pTab->InsertItem(0, "串口");
	pTab->InsertItem(1, "网络");

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
	// 切换数据源
	SwitchCurDataSrc(WH_UCOM_UART);
}

//切换数据收发源
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
		// socket如果有缓存数据不会生成新的msg，需要读取清空一下
		uBase->AsyncRead(tmp, tmp, 0, 0);
		break;
	default:
		break;
	}
}


//切换选项卡
void CUcomDlg::OnSelchangeTabex(NMHDR *pNMHDR, LRESULT *pResult)
{
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

// 数据源tab切换
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

//切换接收显示编码
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

// 回车不退出程序
void CUcomDlg::OnOK()
{
	//CDialog::OnOK();
	return;
}


void CUcomDlg::OnBnClickedBtnsendfile()
{
	if (!uBase->IsRWable())
		return;
	CSendFile SendFileDlg(this, &uBase);
	SendFileDlg.DoModal();
}

// 按键预处理
BOOL CUcomDlg::PreTranslateMessage(MSG* pMsg)
{
	CEdit *pEdit= (CEdit*)GetDlgItem(IDC_EditTxData);
	int len;
	//按键弹起并且选择按键触发而且焦点在发送框
	if (pMsg->message == WM_KEYDOWN && GetDlgItem(IDC_EditTxData) == GetFocus())
	{
		if ((GetKeyState(VK_CONTROL) & 0x80)) 
		{
			switch (pMsg->wParam)
			{
			// 支持输入框内的ctrl+a全选操作
			case 'a':
			case 'A':
				pEdit->SetSel(0, -1); return true;;
			}
		}
		// AT模式的高级功能
		if (isCmdMode)
		{
			// 上下切换历史命令
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
				// tab补全AT+
				case VK_TAB:
					pEdit->SetSel(0, 0);
					pEdit->ReplaceSel("AT+");
					len = pEdit->GetWindowTextLengthA();
					pEdit->SetSel(len, len);
					pEdit->SetFocus();
					return true;
				default:break;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// AT模式切换
void CUcomDlg::OnBnClickedCkbcmd()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbCMD))
	{
		if (isSendHex != true) {
			isCmdMode = TRUE;
		}
		else
		{
			// 不支持HEX模式的cmd对话
			CButton *pCkb = (CButton *)GetDlgItem(IDC_CkbCMD);
			pCkb->SetCheck(BST_UNCHECKED);
		}
	}
	else
		isCmdMode = FALSE;
}
// 窗口动态变化
void CUcomDlg::OnSize(UINT nType, int cx, int cy)
{
	static int lastCx = 0, lastCy = 0;

	CDialog::OnSize(nType, cx, cy);
	if (LargerMode) {
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
	// 窗口最小化避免
	if (cx != 0)
		lastCx = cx;
	if (cy != 0)
		lastCy = cy;
}

// 拖动窗口变动极限值回调
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
