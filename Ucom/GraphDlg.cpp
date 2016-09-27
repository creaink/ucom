// GraphDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "GraphDlg.h"
#include "afxdialogex.h"


// CGraphDlg 对话框

IMPLEMENT_DYNAMIC(CGraphDlg, CDialog)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGraphDlg::IDD, pParent), isGraph(false)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialog)
	ON_BN_CLICKED(IDC_BtnClearGraph, &CGraphDlg::OnBnClickedBtncleargraph)
	ON_WM_PAINT()
//	ON_STN_CLICKED(IDC_PIC_DZXH, &CGraphDlg::OnStnClickedPicDzxh)
ON_BN_CLICKED(IDC_BtnStartGraph, &CGraphDlg::OnBnClickedBtnstartgraph)
END_MESSAGE_MAP()


//void CGraphDlg::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO:  在此处添加消息处理程序代码
//	// 不为绘图消息调用 CDialog::OnPaint()
//}


void CGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	hCanvas = GetDlgItem(IDC_CANVAS)->m_hWnd;
	GetDlgItem(IDC_CANVAS)->GetClientRect(&rectCanvas);
	pCanvas = (GetDlgItem(IDC_CANVAS)->GetDC());
	//hdc = ::BeginPaint(hCanvas, &ps);
	//::EndPaint(hCanvas, &ps);
}


BOOL CGraphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCChart();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CGraphDlg::InitCChart(void)
{

}

void CGraphDlg::AddDataString(CString str)
{
	if (!isGraph)
		return;
	int lastSpacePos, nowSpacePos;
	int index;
	double data;
	CString tmp;
	static int cnt;

	nowSpacePos = 0;
	for (int i = 0; i < 4; i++)
	{
		lastSpacePos = str.Find(' ', nowSpacePos) + 1;
		
		if (lastSpacePos == 0)
			return;	//没有空格帧错误

		nowSpacePos = str.Find(' ', lastSpacePos);
		if (nowSpacePos == -1)
		{	//末尾帧
			nowSpacePos = str.GetLength();
		}

		//字符串长度得大于3，否则帧错误
		if (nowSpacePos - lastSpacePos < 3)
			return;

		tmp = str.Mid(lastSpacePos, nowSpacePos - lastSpacePos) + '\0';
		//当读取两个有效数据时正确
		if (sscanf_s(tmp.GetBuffer(0), "%d:%lf", &index, &data) == 2)
		{
			SetDlgItemText(IDC_EdbData0+i, tmp);
			TRACE("index:%d data:%lf\r\n", index, data);
			
			
		}
		else
			return;	//帧错误
		//继续一帧
		nowSpacePos=lastSpacePos;
	}
	cnt++;

	InvalidateRect(&rectCanvas, true);
}


void CGraphDlg::OnBnClickedBtncleargraph()
{

	InvalidateRect(&rectCanvas, true);
}


void CGraphDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
	return;

}


void CGraphDlg::OnBnClickedBtnstartgraph()
{
	isGraph = !isGraph;
	if (isGraph)
		SetDlgItemText(IDC_BtnStartGraph, "停止");
	else
		SetDlgItemText(IDC_BtnStartGraph, "开始");
}
