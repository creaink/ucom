// MultiSend.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "MultiSend.h"
#include "afxdialogex.h"


// CMultiSend �Ի���

IMPLEMENT_DYNAMIC(CMultiSend, CDialog)

CMultiSend::CMultiSend(CWnd* pParent, UcomBase** mbase)
: CDialog(CMultiSend::IDD, pParent), isHotKey(false)
{
	uuBase = mbase;
}

CMultiSend::~CMultiSend()
{
}

void CMultiSend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMultiSend, CDialog)
	ON_BN_CLICKED(IDC_CkbTimeXSend, &CMultiSend::OnBnClickedCkbtimexsend)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EdbTimeXSend, &CMultiSend::OnEnChangeEdbtimexsend)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BtnSend1, IDC_BtnSend10, OnBnClickedBtnsend)
	ON_CBN_DROPDOWN(IDC_CbTransNum, &CMultiSend::OnDropdownCbtransnum)
	ON_BN_CLICKED(IDC_BtnTrans, &CMultiSend::OnBnClickedBtntrans)
//	ON_WM_KEYDOWN()
//	ON_WM_CHAR()
ON_BN_CLICKED(IDC_CkbXHotKey, &CMultiSend::OnClickedCkbxhotkey)
END_MESSAGE_MAP()


BOOL CMultiSend::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox *pCombox;

	SetDlgItemInt(IDC_EdbTimeXSend, 1000);

	// �趨���ڲ���
	pCombox = (CComboBox*)GetDlgItem(IDC_CbTransNum);
	char str[2] = "0";
	for (int i = 0; i < 10; i++)
	{
		str[0] = '0' + i;
		pCombox->AddString(str);
	}
	pCombox->SetCurSel(0);
	return true;
	// �쳣:  OCX ����ҳӦ���� FALSE
}


// CMultiSend ��Ϣ�������
void CMultiSend::LoopSendSet(void)
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbTimeXSend))
	{
		int time = GetDlgItemInt(IDC_EdbTimeXSend);
		// ����ѡ��
		if (time <= 0 || (*uuBase)->IsRWable() == false)
		{
			((CButton *)GetDlgItem(IDC_CkbTimeXSend))->SetCheck(BST_UNCHECKED);
				return;
		}
		TRACE("Set Time\n");
		SetTimer(LOOP_SEND_ID, time, NULL);
	}
	else
	{
		KillTimer(LOOP_SEND_ID);
		OnTimeSend(true);
	}

}


void CMultiSend::OnEnChangeEdbtimexsend()
{
	LoopSendSet();
}
void CMultiSend::OnBnClickedCkbtimexsend()
{
	LoopSendSet();
}

int CMultiSend::UnblockSend(const CString &dataStr)
{
	if ((*uuBase)->IsRWable() == false)
		return -1;

	return (*uuBase)->AsyncSend(dataStr);
}

void CMultiSend::OnTimeSend(bool clearcnt)
{
	static int cnt=0,lastCnt=9;

	if (clearcnt)
	{
		GetDlgItem(IDC_EdbData1 + lastCnt)->EnableWindow(true);
		cnt = 0, lastCnt = 9;
		return;
	}
	else
	{
		GetDlgItem(IDC_EdbData1 + lastCnt)->EnableWindow(true);
		GetDlgItem(IDC_EdbData1 + cnt)->EnableWindow(false);

		//��������
		SendEdbox(cnt);

		lastCnt = cnt;
		if (++cnt == 10)
			cnt = 0;
	}
}

void CMultiSend::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case LOOP_SEND_ID:
		OnTimeSend();
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CMultiSend::OnOK()
{
	//CDialog::OnOK();
	return;
}

//��0��ʼ
void CMultiSend::SendEdbox(int index)
{
	bool isHex=false;
	CString strTmp;
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbXHex1+index))
		isHex = true;

	GetDlgItemText(IDC_EdbData1 + index, strTmp);
	txData.ReString(strTmp, isHex);
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbXNewLine))
	{
		strTmp = "\r\n";
		//txData.AppendString(strTmp);
		UnblockSend(txData.GetCStrData()+strTmp);
	}
	else
	{
		UnblockSend(txData.GetCStrData());
	}
}

void CMultiSend::OnBnClickedBtnsend(UINT   uId)
{
	int btnNum = uId - IDC_BtnSend1;
	SendEdbox(btnNum);
}


void CMultiSend::OnDropdownCbtransnum()
{
	int DropSize;
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbTransNum);
	DropSize = pComBox->GetItemHeight(0)*pComBox->GetCount();
	pComBox->SetDroppedWidth(DropSize);
}


void CMultiSend::OnBnClickedBtntrans()
{
	CString strTmp;

	GetDlgItemText(IDC_EdbXDetail,strTmp);
	SetDlgItemText(IDC_EdbData1 + ((CComboBox *)GetDlgItem(IDC_CbTransNum))->GetCurSel()
		, strTmp);
}


BOOL CMultiSend::PreTranslateMessage(MSG* pMsg)
{
	// ����ESC������ֹ�Ի����˳�
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	// ����������ѡ�񰴼��������ҽ��������·�ϸ�ڿ���
	if (pMsg->message == WM_KEYDOWN
		&& isHotKey
		&& GetDlgItem(IDC_EdbXDetail) == GetFocus())
	{
		TRACE("KeyVal:%d\n", pMsg->wParam);
		// ��ԭ��ascii
		switch (pMsg->wParam + 32)
		{
		case VK_ESCAPE:
			return TRUE;
		case 'w':
			OnBnClickedBtnsend(IDC_BtnSend1);break;
		case 's':
			OnBnClickedBtnsend(IDC_BtnSend2);break;
		case 'a':
			OnBnClickedBtnsend(IDC_BtnSend3);break;
		case 'd':
			OnBnClickedBtnsend(IDC_BtnSend4);break;
		case 'i':
			OnBnClickedBtnsend(IDC_BtnSend5);break;
		case 'k':
			OnBnClickedBtnsend(IDC_BtnSend6);break;
		case 'j':
			OnBnClickedBtnsend(IDC_BtnSend7);break;
		case 'l':
			OnBnClickedBtnsend(IDC_BtnSend8);break;
		case 'q':
			OnBnClickedBtnsend(IDC_BtnSend9);break;
		case 'e':
			OnBnClickedBtnsend(IDC_BtnSend10);break;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMultiSend::OnClickedCkbxhotkey()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbXHotKey))
		isHotKey = true;
	else
		isHotKey = false;
}
