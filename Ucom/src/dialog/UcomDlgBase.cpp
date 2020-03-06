// UcomDlgBase.cpp : ����ʵ���ļ�
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
		// ���յ����ֽ�
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

// ������Ϣ
void CUcomDlg::OnBnClickedBtnhelp()
{
	::ShellExecute(NULL, NULL, "https://github.com/creaink/ucom", NULL, NULL, SW_SHOWNORMAL);
}


void CUcomDlg::SetDelaySend(void)
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbSendOnTime))
	{
		int time = GetDlgItemInt(IDC_EdbSendDelay);
		// ����ѡ��
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

// �ı䷢�ͼ������״̬�����趨
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
	isNewLineSend = (BST_CHECKED == IsDlgButtonChecked(IDC_CkbFrameNewLine));
}

void CUcomDlg::OnCkbDispHex()
{
	isDispHex = (BST_CHECKED == IsDlgButtonChecked(IDC_CkbDispHex));
}

// ��ͣ������ʾ��Ϣ������
void CUcomDlg::OnBnClickedBtnisdisprx()
{
	isDispRx = !isDispRx;
	SetDlgItemText(IDC_BtnIsDispRx, isDispRx ? "��ͣ����" : "��������");
}


void CUcomDlg::OnBnClickedBtntoolbox()
{
	CMenu menu;
	RECT rect;
	menu.LoadMenu(IDR_MENU_ToolsBox);
	// ��ȡ�ؼ�λ��
	::GetWindowRect(GetDlgItem(IDC_BtnToolBox)->GetSafeHwnd(), &rect);
	CMenu *popmenu = menu.GetSubMenu(0);

	popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, rect.right, rect.top, this);
}


// ��ݲ˵�
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
	// ȡ���ı���ǰ���ֵĸ�ʽ
	pRich->GetSelectionCharFormat(cf);
	// ������ɫ�Ի���,����ȡ�õ����ָ�ʽ��ʼ���Ի���
	CColorDialog dlg(cf.crTextColor, CC_FULLOPEN, this);
	// ��ʾ�Ի���,ѡ����ɫ
	if (dlg.DoModal() == IDOK)
	{
		//ȡ���û���ѡ��ɫ
		cf.crTextColor = dlg.GetColor();
		//���ı���ʽ��Ϣ���õ��ı���ǰ�ı�
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
	// ȡ���ı���ǰ���ֵĸ�ʽ
	pRich->GetSelectionCharFormat(cf);
	// ������ɫ�Ի���,����ȡ�õ����ָ�ʽ��ʼ���Ի���
	CColorDialog dlg(cf.crTextColor, CC_FULLOPEN, this);
	// ��ʾ�Ի���,ѡ����ɫ
	if (dlg.DoModal() == IDOK)
	{
		// ȡ���û���ѡ��ɫ
		cf.crTextColor = dlg.GetColor();
		// ���ı���ʽ��Ϣ���õ��ı���ǰ�ı�
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = 0;
		pRich->SetDefaultCharFormat(cf);
	}
}


void CUcomDlg::OnBnClickedBtnfont()
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	// ���岢��ʼ�����ָ�ʽ�ṹ
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);
	// ȡ���ı���ǰ���ֵĸ�ʽ
	pRich->GetSelectionCharFormat(cf);
	//��������Ի���,����ȡ�õ����ָ�ʽ��ʼ���Ի���
	CFontDialog dlg(cf);
	// ��ʾ�Ի���,ѡ��������Ϣ
	if (dlg.DoModal() == IDOK)
	{
		// ȡ���û���ѡ������Ϣ
		dlg.GetCharFormat(cf);
		// ���ı���ʽ��Ϣ���õ��ı���ǰ�ı�
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		pRich->SetDefaultCharFormat(cf);

		SetRichLineSpace();
	}
}



// �ؼ�IDΪnID���Ҹı���״��isEnlarge=true����������x(��Ϊ����)���أ����������ұ߽��ƶ�
void CUcomDlg::ChangeItemSize(int nID, int x, int y, bool isEnlarge)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (x == 0 && y == 0) return;

	if (pWnd != NULL)
	{
		CRect rec;
		int width, height;
		pWnd->GetWindowRect(&rec);  // ��ȡ�ؼ��仯ǰ�Ĵ�С
		ScreenToClient(&rec);   // ���ؼ���Сת��Ϊ�ڶԻ����е���������
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
		pWnd->MoveWindow(rec); // �����ؼ�
	}
}

