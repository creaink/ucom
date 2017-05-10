// UcomDlgBase.cpp : 基础实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "UcomDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CUcomDlg::OnBnClickedBtnsend2()
{

}


void CUcomDlg::OnBtnSend()
{
	if (uartPortIsOpen == TRUE)
	{
		SendEditBoxData();
	}
}

//端口下拉长度控制
void CUcomDlg::OnDropdownCbUartPort()
{
	int DropSize;
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	DropSize = pComBox->GetItemHeight(0)*pComBox->GetCount();
	pComBox->SetDroppedWidth(DropSize);
}

//选取最后一项刷新动作
void CUcomDlg::OnSelendokCbUartPort()
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

void CUcomDlg::OnBtnClearRecv()
{
	SetDlgItemText(IDC_RichRx, _T(""));
	SetRichLineSpace();
	DataRx.ClearData();
}

void CUcomDlg::OnBtnClearSend()
{
	SetDlgItemText(IDC_EditTxData, _T(""));
	DataTx.ClearData();
}


void CUcomDlg::SetFullBytes(int nbytes)
{
	CRichEditCtrl *pRich = (CRichEditCtrl *)GetDlgItem(IDC_RichRx);

	if (nbytes == -1)
	{
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbRxFullClc))
		{
			isRxFullClc = true;
			limitBytes = GetDlgItemInt(IDC_EdbFullBytes);

		}
		else
			isRxFullClc = false;
	}
	else
		//接收的是字节
		limitBytes = nbytes;
}

void CUcomDlg::OnBnClickedCkbrxfullclc()
{
	SetFullBytes();
}
void CUcomDlg::OnChangeEdbfullbytes()
{
	SetFullBytes();
}



void CUcomDlg::OnBnClickedBtnclearall()
{
	OnBtnClearRecv();
	OnBtnClearSend();
	OnBtnClearnCnt();
}


void CUcomDlg::OnBnClickedBtnhelp()
{
	CString helpInfo;
	helpInfo = 
"版本1.03 & 帮助信息：\n\
0.本软件版权所有：长春理工大学电子学会，技术交流QQ群：560901616\n\
1.自动保存串口配置里的信息和用户设定的背景色和字体色配置，定制你的串口助手\n\
2.当有新设备接入时，点击串口号，在下拉菜单里选择刷新串口可以扫描端口变动\n\
3.完全支持接收区UTF-8和Unicode的编码显示\n\
4.快速启动，可以快速调用常用的工具和系统设置项\n\
5.提供数据统计和实时传输速率数据显示，快速清空等功能\n\
6.提供HEX接收显示和HEX模式发送，并对不规范输入进行提醒\n\
7.满收清空，当勾选此项时候，当接区达到毗邻的数值输入框的设定值(Byte)时会自动清空\n\
8.间隔发送，当勾选此项时候，当接区达到毗邻的数值输入框的设定值(  ms)时会自动发送\n\
9.发帧换行，当勾选此项时候，自动为发送框的数据末尾添加回车换行符\n\
10.暂停接收，暂停数据的显示，但是数据依然在后台接收，只是不显示\n\
11.保存接收，用于保存当前接收框里的文本信息为TXT文件\n\
12.背景色、字体色、字体样式，可以定制介绍框的样式，字体样式信息不会保存\n\
13.透明传输，不会阻碍0x00,0x11等字节的发送接收\n\
14.支持文件发送，建议不要发送太大的文件\n\
15.支持CMD模式，回车发送数据(数据包含回车)，发送的数据同时以特殊格式添到接收显示的新\n\
     行中上下调用历史命令(数据)\n\
16.支持横向窗口大小调整\n\n\
a.接收图表，支持指定格式的数值分拣、和以后的图形化显示\n\
b.编码查询，提供三种常用的字符编码的16进制编码查询、可变换显示风格\n\
c.监视注释，能够监视串口数据帧，时间到毫秒级，设置最大自动清空数，点击条目详细显示\n\
d.发送助手，最大十个条目发送，可时间轮发，自动换行，hex模式，单独按钮发送\n\
   在下拉框选择相应的条目，可以点击传送将下方编辑框的长文本传输至指定编辑框\n\
   选择按键触发，在最下方输入框按下按钮提示字符，即可触发相应数据的发送";

	SetDlgItemText(IDC_RichRx, helpInfo);
}


void CUcomDlg::SetDelaySend(void)
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbSendOnTime))
	{
		int time = GetDlgItemInt(IDC_EdbSendDelay);
		//不能选择
		if (time <= 0 || uartPortIsOpen == FALSE)
		{
			CButton *pCkb = (CButton *)GetDlgItem(IDC_CkbSendOnTime);
			pCkb->SetCheck(BST_UNCHECKED);
			return;
		}

		SetTimer(AUTO_SEND_TIMER_ID, time, NULL);
	}
	else
	{
		KillTimer(AUTO_SEND_TIMER_ID);
	}
}

//改变发送间隔或者状态重新设定
void CUcomDlg::OnCkbSendOnTime()
{
	SetDelaySend();
}
void CUcomDlg::OnChangeEdbSendDelay()
{
	SetDelaySend();
}

void CUcomDlg::OnCkbFrameNewLine()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbFrameNewLine))
		isNewLineSend = TRUE;
	else
		isNewLineSend = FALSE;
}

void CUcomDlg::OnCkbDispHex()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbDispHex))
		isDispHex = TRUE;
	else
		isDispHex = FALSE;
}

//暂停接收显示消息处理函数
void CUcomDlg::OnBnClickedBtnisdisprx()
{
	isDispRx = !isDispRx;
	if (isDispRx)
		SetDlgItemText(IDC_BtnIsDispRx, "暂停接收");
	else
		SetDlgItemText(IDC_BtnIsDispRx, "继续接收");
}


void CUcomDlg::OnBnClickedBtntoolbox()
{
	CMenu menu;
	RECT rect;
	menu.LoadMenu(IDR_MENU_ToolsBox);
	//获取控件位置
	::GetWindowRect(GetDlgItem(IDC_BtnToolBox)->GetSafeHwnd(), &rect);
	CMenu *popmenu = menu.GetSubMenu(0);

	popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, rect.right, rect.top, this);
}


void CUcomDlg::OnMeudevmanger()
{
	::ShellExecute(m_hWnd, "open", "devmgmt.msc", NULL, NULL, SW_SHOWNORMAL);
}
void CUcomDlg::OnMeucalc()
{
	::ShellExecute(m_hWnd, "open", "calc", NULL, NULL, SW_SHOWNORMAL);
}
void CUcomDlg::OnMeunotepad()
{
	::ShellExecute(m_hWnd, "open", "notepad", NULL, NULL, SW_SHOWNORMAL);
}

void CUcomDlg::OnMenureg()
{
	::ShellExecute(m_hWnd, "open", "regedit", NULL, NULL, SW_SHOWNORMAL);
}
void CUcomDlg::OnMenucmd()
{
	::ShellExecute(m_hWnd, "open", "cmd", NULL, NULL, SW_SHOWNORMAL);
}
void CUcomDlg::OnMenuserv()
{
	::ShellExecute(m_hWnd, "open", "services.msc", NULL, NULL, SW_SHOWNORMAL);
}



void CUcomDlg::OnBnClickedBtnbackcolor()
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);
	//取得文本框当前文字的格式 
	pRich->GetSelectionCharFormat(cf);
	//创建颜色对话框,并用取得的文字格式初始化对话框 
	CColorDialog dlg(cf.crTextColor, CC_FULLOPEN, this);
	//显示对话框,选择颜色 
	if (dlg.DoModal() == IDOK)
	{
		//取得用户所选颜色 
		cf.crTextColor = dlg.GetColor();
		//将文本格式信息设置到文本框当前文本 
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = 0;
		backgroudColor = cf.crTextColor;
		pRich->SetBackgroundColor(FALSE, cf.crTextColor);
	}
}


void CUcomDlg::OnBnClickedBtnfontcolor()
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);
	//取得文本框当前文字的格式 
	pRich->GetSelectionCharFormat(cf);
	//创建颜色对话框,并用取得的文字格式初始化对话框 
	CColorDialog dlg(cf.crTextColor, CC_FULLOPEN, this);
	//显示对话框,选择颜色 
	if (dlg.DoModal() == IDOK)
	{
		//取得用户所选颜色 
		cf.crTextColor = dlg.GetColor();
		//将文本格式信息设置到文本框当前文本 
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = 0;
		pRich->SetDefaultCharFormat(cf);
	}
}


void CUcomDlg::OnBnClickedBtnfont()
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	//定义并初始化文字格式结构
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);
	//取得文本框当前文字的格式
	pRich->GetSelectionCharFormat(cf);
	//创建字体对话框,并用取得的文字格式初始化对话框
	CFontDialog dlg(cf);
	//显示对话框,选择字体信息
	if (dlg.DoModal() == IDOK)
	{
		//取得用户所选字体信息
		dlg.GetCharFormat(cf);
		//将文本格式信息设置到文本框当前文本
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		pRich->SetDefaultCharFormat(cf);

		SetRichLineSpace();
	}
}
