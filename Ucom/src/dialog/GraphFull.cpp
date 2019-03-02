// GraphFull.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "GraphFull.h"
#include "afxdialogex.h"


// CGraphFull 对话框

IMPLEMENT_DYNAMIC(CGraphFull, CDialog)

CGraphFull::CGraphFull(CChartCtrl *pChart, CWnd* pParent)
	: CDialog(IDD_GraphFull, pParent)
{
	pChartCtrl = pChart;
}

CGraphFull::~CGraphFull()
{
}

void CGraphFull::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ChartFull, *pChartCtrl);
}


BEGIN_MESSAGE_MAP(CGraphFull, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGraphFull 消息处理程序


void CGraphFull::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_ChartFull);
	if (pWnd != NULL)
	{
		CRect rec;
		GetClientRect(&rec);
		pWnd->MoveWindow(rec);
	}
}


void CGraphFull::DoDDX_Control(bool isAttach)
{
	if (isAttach) {
		//重绘
		pChartCtrl->SubclassDlgItem(IDC_ChartFull, this);
		OnSize(SIZE_RESTORED, 0, 0);
	}
	else {
		if (pChartCtrl->GetSafeHwnd())
			pChartCtrl->UnsubclassWindow();
	}
}

BOOL CGraphFull::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CMenu* pMenu = this->GetSystemMenu(FALSE);//系统菜单
	//pMenu->ModifyMenu(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);//禁用关闭按钮

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CGraphFull::OnClose()
{
	//CDialog::OnClose();

	//发送消息退出，调用父窗口关闭本窗口
	::SendMessage(pMianHwnd, WM_COMM_CLOSE_ME_MSG, 0, 0);
}
//将尺寸还原到内嵌的大小，避免显示不全
void CGraphFull::Resize(LONG Bottom, LONG Right)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_ChartFull);
	if (pWnd != NULL)
	{
		CRect rec;
		GetClientRect(&rec);

		rec.bottom = Bottom;
		rec.right = Right;

		pWnd->MoveWindow(rec);
		pChartCtrl->RefreshCtrl();
	}
}

void CGraphFull::OnCancel()
{
	CDialog::OnCancel();
}
