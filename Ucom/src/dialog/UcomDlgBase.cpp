// UcomDlgBase.cpp : 基础实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "UcomDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CUcomDlg::OnBtnSend()
{
	if (uBase->IsRWable() == TRUE)
	{
		SendEditBoxData();
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
		// 接收的是字节
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
	lastInfo.Empty();
}

// 帮助信息
void CUcomDlg::OnBnClickedBtnhelp()
{
	::ShellExecute(NULL, NULL, "https://github.com/creaink/ucom", NULL, NULL, SW_SHOWNORMAL);
}


void CUcomDlg::SetDelaySend(void)
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbSendOnTime))
	{
		int time = GetDlgItemInt(IDC_EdbSendDelay);
		// 不能选择
		if (time <= 0 || uBase->IsRWable() == FALSE)
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

// 改变发送间隔或者状态重新设定
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

// 暂停接收显示消息处理函数
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
	// 获取控件位置
	::GetWindowRect(GetDlgItem(IDC_BtnToolBox)->GetSafeHwnd(), &rect);
	CMenu *popmenu = menu.GetSubMenu(0);

	popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, rect.right, rect.top, this);
}


// 快捷菜单
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
	// 取得文本框当前文字的格式
	pRich->GetSelectionCharFormat(cf);
	// 创建颜色对话框,并用取得的文字格式初始化对话框
	CColorDialog dlg(cf.crTextColor, CC_FULLOPEN, this);
	// 显示对话框,选择颜色
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
	// 取得文本框当前文字的格式
	pRich->GetSelectionCharFormat(cf);
	// 创建颜色对话框,并用取得的文字格式初始化对话框
	CColorDialog dlg(cf.crTextColor, CC_FULLOPEN, this);
	// 显示对话框,选择颜色
	if (dlg.DoModal() == IDOK)
	{
		// 取得用户所选颜色
		cf.crTextColor = dlg.GetColor();
		// 将文本格式信息设置到文本框当前文本
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = 0;
		pRich->SetDefaultCharFormat(cf);
	}
}


void CUcomDlg::OnBnClickedBtnfont()
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	// 定义并初始化文字格式结构
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);
	// 取得文本框当前文字的格式
	pRich->GetSelectionCharFormat(cf);
	//创建字体对话框,并用取得的文字格式初始化对话框
	CFontDialog dlg(cf);
	// 显示对话框,选择字体信息
	if (dlg.DoModal() == IDOK)
	{
		// 取得用户所选字体信息
		dlg.GetCharFormat(cf);
		// 将文本格式信息设置到文本框当前文本
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		pRich->SetDefaultCharFormat(cf);

		SetRichLineSpace();
	}
}



// 控件ID为nID向右改变形状，isEnlarge=true是向右增加x(可为负数)像素，否则是以右边界移动
void CUcomDlg::ChangeItemSize(int nID, int x, int y, bool isEnlarge)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (x == 0 && y == 0)
		return;
	if (pWnd != NULL)
	{
		CRect rec;
		int width, height;
		pWnd->GetWindowRect(&rec);  // 获取控件变化前的大小
		ScreenToClient(&rec);   // 将控件大小转换为在对话框中的区域坐标
		width = rec.Width();
		height = rec.Height();
		if (isEnlarge)
		{
			rec.right = rec.right + x;
			rec.bottom = rec.bottom + y;
		}
		else {
			rec.left = rec.left + x;
			rec.top = rec.top + y;

			rec.right = rec.left + width;
			rec.bottom = rec.top + height;
		}
		pWnd->MoveWindow(rec); // 伸缩控件
	}
}

