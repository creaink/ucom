// DataWatch.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "DataWatch.h"
#include "afxdialogex.h"


// CDataWatch �Ի���

IMPLEMENT_DYNAMIC(CDataWatch, CDialog)

CDataWatch::CDataWatch(CWnd* pParent /*=NULL*/)
	: CDialog(CDataWatch::IDD, pParent)
	, ItemCnt(0)
{

}

CDataWatch::~CDataWatch()
{
}

void CDataWatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDataWatch, CDialog)
	ON_BN_CLICKED(IDC_BtnWatch, &CDataWatch::OnBnClickedBtnwatch)
	ON_BN_CLICKED(IDC_CkbDispHex, &CDataWatch::OnBnClickedCkbdisphex)
	ON_BN_CLICKED(IDC_BtnClearList, &CDataWatch::OnBnClickedBtnclearlist)
	ON_EN_CHANGE(IDC_EditMaxListNum, &CDataWatch::OnEnChangeEdbmaxlist)
	ON_NOTIFY(NM_CLICK, IDC_ListWatch, &CDataWatch::OnClickListwatch)
END_MESSAGE_MAP()


// CDataWatch ��Ϣ�������


BOOL CDataWatch::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;
	isRecData = false;
	isFullClear = false;
	SetDlgItemInt(IDC_EditMaxListNum, 50);

	pList = (CListCtrl *)GetDlgItem(IDC_ListWatch);
	pList->ModifyStyle(0, LVS_REPORT); // ����ģʽ
	pList->SetExtendedStyle(pList->GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	pList->InsertColumn(0, "���");
	pList->InsertColumn(2, "����");
	pList->InsertColumn(1, "ʱ��");

	pList->GetClientRect(rect); // ��õ�ǰ�ͻ�����Ϣ
	pList->SetColumnWidth(0, rect.Width() / 16*2); // �����еĿ�ȡ�
	pList->SetColumnWidth(1, rect.Width() / 16*5);
	pList->SetColumnWidth(2, rect.Width() / 16*9);
	pList->DeleteColumn(3);
	return TRUE;
	// �쳣:  OCX ����ҳӦ���� FALSE
}
const char CDataWatch::HexTable[] = "0123456789ABCDEF";

void CDataWatch::AddItem(CString str)
{
	if (!isRecData)
		return;
	if (isFullClear&&ItemCnt > MaxItem)
		ClearList();

	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format("%d",ItemCnt++);
	pList->InsertItem(0, strTime);

	strTime.Format("%2d:%2d:%2d:%3d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	pList->SetItemText(0, 1, strTime);


	int nBytes = str.GetAllocLength();
	if (BST_CHECKED == IsDlgButtonChecked(IDC_ChkHexList))
	{
		const unsigned char *pCh = (const unsigned char *)(str.GetBuffer(0));
		char HexCh[4] = " HL";
		strTime.Empty();
		while (nBytes--)
		{
			HexCh[1] = HexTable[(*pCh) / 16];
			HexCh[2] = HexTable[(*pCh) % 16];
			strTime += HexCh;
			pCh++;
		}
		pList->SetItemText(0, 2, strTime);
	}
	else
	{
		pList->SetItemText(0, 2, str);
	}

}



void CDataWatch::OnBnClickedBtnwatch()
{
	isRecData = !isRecData;
	if (isRecData)
	{
		SetDlgItemText(IDC_BtnWatch, "���ڼ���");
	}
	else
	{
		SetDlgItemText(IDC_BtnWatch, "��ʼ����");
	}
}

void CDataWatch::ClearList(void)
{
	ItemCnt = 0;
	pList->DeleteAllItems();
}


void CDataWatch::ChangeMaxItem(void)
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbDispHex))
	{
		MaxItem = GetDlgItemInt(IDC_EditMaxListNum);
		isFullClear = true;
	}
	else
	{
		isFullClear = false;
	}
}

void CDataWatch::OnBnClickedCkbdisphex()
{
	ChangeMaxItem();
}


void CDataWatch::OnEnChangeEdbmaxlist()
{
	ChangeMaxItem();
}


void CDataWatch::OnBnClickedBtnclearlist()
{
	ClearList();
}


void CDataWatch::OnClickListwatch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	SetDlgItemText(IDC_EdbListItem, pList->GetItemText((int)pList->GetFirstSelectedItemPosition()-1, 2));
}


void CDataWatch::OnOK()
{
	//CDialog::OnOK();
	return;
}


BOOL CDataWatch::PreTranslateMessage(MSG* pMsg)
{
	// ����ESC������ֹ�Ի����˳�
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
