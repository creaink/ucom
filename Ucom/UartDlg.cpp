// UartDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "UartDlg.h"
#include "afxdialogex.h"


// CUartDlg 对话框

IMPLEMENT_DYNAMIC(CUartDlg, CDialog)

bool CUartDlg::uartPortIsOpen = false;
iUart CUartDlg::mUart;

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
	ON_CBN_DROPDOWN(IDC_CbUartPort, OnDropdownCbUartPort)
	ON_CBN_SELENDOK(IDC_CbUartPort, OnSelendokCbUartPort)
	ON_BN_CLICKED(IDC_BtnOpen, OnBtnOpen)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_PicUartStatus, &CUartDlg::OnStnClickedPicuartstatus)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()



BOOL CUartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitPanel();
	//装载注册表数据
	LoadRegConfig();
	SwPanel(uartPortIsOpen);

	return TRUE; 
}


void CUartDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialog::OnPaint()

	//避免最小化后恢复出现图片不显示的情况
	SwPanel(uartPortIsOpen);
}


void CUartDlg::InitPanel(void)
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
}

//从空间得到配置信息字符串
DCB CUartDlg::GetUartConfigDCB(void)
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

void CUartDlg::ChangeBmpPic(int PicCtrlID, unsigned short nPicID)
{
	CBitmap bitmap;
	HBITMAP hBmp;
	CStatic *pStatic = (CStatic*)GetDlgItem(PicCtrlID);

	bitmap.LoadBitmap(nPicID);				// 将位图IDB_BITMAP1加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄

	pStatic->SetBitmap(hBmp);				// 设置图片控件
}

void CUartDlg::SwPanel(bool choose)
{
	if (choose) {
		ChangeBmpPic(IDC_PicUartStatus, IDB_SwOn);
		SetDlgItemText(IDC_BtnOpen, _T("关闭串口"));
	}
	else
	{
		ChangeBmpPic(IDC_PicUartStatus, IDB_SwOff);
		SetDlgItemText(IDC_BtnOpen, _T("打开串口"));
	}

	choose = !choose;
	GetDlgItem(IDC_CbUartBaudrate)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartDatabit)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartStopbit)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartECC)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartPort)->EnableWindow(choose);
}

bool CUartDlg::IsUartPortAvailable(void)
{
	CString comInfo;
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	pCombox->GetLBText(pCombox->GetCurSel(), comInfo);

	if (comInfo.Left(3) == _T("COM"))
		return true;
	else
		return false;
}

void CUartDlg::OpenUart()
{
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	CString strtmp;

	if (uartPortIsOpen == false)
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
		if (hRxThread == NULL) {
			TRACE("Rx Listenner Thread Created Failed");
			return;
		}

		uartPortIsOpen = true;
		//发送打开消息
		//::SendMessage(::AfxGetMainWnd()->m_hWnd, WM_COMM_RX_MSG, 1, 0);
		//GetParent()->SendMessage(WM_COMM_RX_MSG, 1, 0);
		::PostMessage(HWND_BROADCAST, WM_MYONRECVMSG, W_UART_RECV, 0);
	}
	else if (uartPortIsOpen == true)
	{
		//先置位标志位避免关闭时候接收错误
		uartPortIsOpen = false;

		//发送关闭消息
		::PostMessage(HWND_BROADCAST, WM_MYONRECVMSG, W_UART_RECV, 0);

		TerminateThread(hRxThread, 0);
		//关闭串口
		if (mUart.isConnected())
			mUart.ClosePort();
	}
	// 切换图标、禁止状态、按钮文本
	SwPanel(uartPortIsOpen);
}


//写入注册表数据
void CUartDlg::WriteRegData(void)
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
	CComboBox *pComBox = (CComboBox*)GetDlgItem(IDC_CbUartPort);

	GetRegData(comName, dcbConfig);
	if (comName != "NULL") {
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
	}
}

// CUartDlg 消息处理程序


//端口下拉长度控制
void CUartDlg::OnDropdownCbUartPort()
{
	int DropSize;
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	DropSize = pComBox->GetItemHeight(0)*pComBox->GetCount();
	pComBox->SetDroppedWidth(DropSize);
}

//选取最后一项刷新动作
void CUartDlg::OnSelendokCbUartPort()
{
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	int sel = pComBox->GetCurSel();
	int cnt = pComBox->GetCount();

	CString str;
	//末尾刷新串口
	if (sel == (cnt - 1))
	{
		//clear all
		pComBox->ResetContent();
		mUart.GetComList((CComboBox *)GetDlgItem(IDC_CbUartPort));

		//选取第一个
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
