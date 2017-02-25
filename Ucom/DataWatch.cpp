// DataWatch.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "DataWatch.h"
#include "afxdialogex.h"


// CDataWatch 对话框

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


// CDataWatch 消息处理程序


BOOL CDataWatch::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;
	isRecData = false;
	isFullClear = false;
	SetDlgItemInt(IDC_EditMaxListNum, 50);

	pList = (CListCtrl *)GetDlgItem(IDC_ListWatch);
	pList->ModifyStyle(0, LVS_REPORT);// 报表模式  
	pList->SetExtendedStyle(pList->GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	pList->InsertColumn(0, "序号");
	pList->InsertColumn(2, "数据");
	pList->InsertColumn(1, "时间");

	pList->GetClientRect(rect); //获得当前客户区信息  
	pList->SetColumnWidth(0, rect.Width() / 16*2); //设置列的宽度。  
	pList->SetColumnWidth(1, rect.Width() / 16*5);
	pList->SetColumnWidth(2, rect.Width() / 16*9);
	pList->DeleteColumn(3);
	return TRUE;
	// 异常:  OCX 属性页应返回 FALSE
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
		pList->SetItemText(0, 2, str);

}



void CDataWatch::OnBnClickedBtnwatch()
{
	isRecData = !isRecData;
	if (isRecData)
	{
		SetDlgItemText(IDC_BtnWatch,"正在监视");
	}
	else
	{
		SetDlgItemText(IDC_BtnWatch, "开始监视");
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
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	SetDlgItemText(IDC_EdbListItem, pList->GetItemText((int)pList->GetFirstSelectedItemPosition()-1, 2));
}


void CDataWatch::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
	return;

}
