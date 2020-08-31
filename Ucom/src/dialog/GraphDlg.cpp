// GraphDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "GraphDlg.h"
#include "afxdialogex.h"


#define PI 3.14159

// CGraphDlg 对话框

IMPLEMENT_DYNAMIC(CGraphDlg, CDialog)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGraphDlg::IDD, pParent)
, isGraph(false)
, isFullGraph(false)
, GraphFullDlg(&mChartCtrl)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_Chart, mChartCtrl);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialog)
	ON_BN_CLICKED(IDC_BtnClearGraph, &CGraphDlg::OnBnClickedBtncleargraph)
	ON_WM_PAINT()
ON_MESSAGE(WM_COMM_CLOSE_ME_MSG, &CGraphDlg::CloseGraphFull)
ON_BN_CLICKED(IDC_BtnStartGraph, &CGraphDlg::OnBnClickedBtnstartgraph)
ON_BN_CLICKED(IDC_RadYAxis, &CGraphDlg::OnBnClickedRadAxis)
ON_BN_CLICKED(IDC_RadXAxis, &CGraphDlg::OnBnClickedRadAxis)
ON_BN_CLICKED(IDC_CkbAxisScroll, &CGraphDlg::OnBnClickedCkbaxisscroll)
ON_BN_CLICKED(IDC_CkbAxisAuto, &CGraphDlg::OnBnClickedCkbaxisauto)
ON_EN_KILLFOCUS(IDC_EdbAxisMin, &CGraphDlg::OnEdbaxismin)
ON_EN_KILLFOCUS(IDC_EdbAxisMax, &CGraphDlg::OnEdbaxismax)
ON_BN_CLICKED(IDC_CkbAxisGrid, &CGraphDlg::OnBnClickedCkbaxisgrid)
ON_BN_CLICKED(IDC_BtnGraphColor, &CGraphDlg::OnBnClickedBtngraphcolor)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_LstLineData, &CGraphDlg::OnCustomdrawLstlinedata)
ON_BN_CLICKED(IDC_CkbLineV, &CGraphDlg::OnBnClickedCkblinev)
ON_NOTIFY(NM_CLICK, IDC_LstLineData, &CGraphDlg::OnClickLstlinedata)
ON_BN_CLICKED(IDC_BtnLineColor, &CGraphDlg::OnBnClickedBtnlinecolor)
ON_CBN_SELENDOK(IDC_CbLineWidth, &CGraphDlg::OnSelendokCblinewidth)
ON_CBN_SELENDOK(IDC_CbLinePen, &CGraphDlg::OnSelendokCblinepen)
ON_BN_CLICKED(IDC_BtnSaveLine, &CGraphDlg::OnBnClickedBtnsaveline)
ON_BN_CLICKED(IDC_BtnGraphFull, &CGraphDlg::OnBnClickedBtngraphfull)
END_MESSAGE_MAP()



void CGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

}


BOOL CGraphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;
	CComboBox *pComBox;
	CString str;

	// 句柄分配有点慢不能放在构造里面，将本窗口句柄传递给子窗口
	GraphFullDlg.pMianHwnd = this->GetSafeHwnd();

	// 初始化线的几个参数
	pComBox = (CComboBox*)GetDlgItem(IDC_CbLineWidth);
	pComBox->InsertString(0, "4");
	pComBox->InsertString(0, "3");
	pComBox->InsertString(0, "2");
	pComBox->InsertString(0, "1");
	pComBox->SetCurSel(0);
	/*	#define PS_SOLID            0
		#define PS_DASH             1
		#define PS_DOT              2
		#define PS_DASHDOT          3
		#define PS_DASHDOTDOT       4*/
	pComBox = (CComboBox*)GetDlgItem(IDC_CbLinePen);
	pComBox->InsertString(0, "点划点");
	pComBox->InsertString(0, "点划");
	pComBox->InsertString(0, "点线");
	pComBox->InsertString(0, "下划线");
	pComBox->InsertString(0, "实线");
	pComBox->SetCurSel(0);

	// 数据 list 初始化
	GetDlgItem(IDC_LstLineData)->GetWindowRect(rect);
	listColorAvailable = true;
	pList = (CListCtrl *)GetDlgItem(IDC_LstLineData);
	pList->ModifyStyle(0, LVS_REPORT);// 报表模式
	pList->SetExtendedStyle(pList->GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	pList->InsertColumn(0, "头", LVCFMT_RIGHT);
	pList->InsertColumn(1, "值", LVCFMT_RIGHT);

	pList->GetClientRect(rect); // 获得当前客户区信息
	pList->SetColumnWidth(0, rect.Width() / 4 * 1); // 设置列的宽度。
	pList->SetColumnWidth(1, rect.Width() / 4 * 3);

	// 初始化表格
	InitChart();

	// 在初始化线色后插入 list 数据条目
	for (int i = 0; i < MAX_GRAPH_LINES; i++)
	{
		str.Format("%d", i);
		pList->InsertItem(i, str);
	}

	((CButton*)GetDlgItem(IDC_RadYAxis))->SetCheck(1);
	// 载入默认设置
	OnBnClickedRadAxis();
	OnSelendokCblinewidth();
	OnSelendokCblinewidth();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

// HSI 颜色模型到 RGB 转换，H 范围 0 到 2PI; S, I 归一化到 0 到 1
COLORREF CGraphDlg::HSI2RGB(double H, double S, double I)
{
	unsigned char rB, rG, rR;
	double R, G, B;
	// 算法参考冈萨勒斯数字图像处理
	if (H >= 0 && H<(PI * 2 / 3))
	{
		B = I*(1 - S);
		R = I*(1 + S*cos(H) / cos(PI / 3 - H));
		G = 3 * I - (R + B);
	}
	else if (H >= (PI * 2 / 3) && H<(PI * 4 / 3))
	{
		R = I*(1 - S);
		G = I*(1 + S*cos(H - 2 * PI / 3) / cos(PI - H));
		B = 3 * I - (R + G);
	}
	else
	{
		G = I*(1 - S);
		B = I*(1 + S*cos(H - 4 * PI / 3) / cos(5 * PI / 3 - H));
		R = 3 * I - (G + B);
	}
	// 还原归一化值
	rR = (int)(R * 255);
	rG = (int)(G * 255);
	rB = (int)(B * 255);
	// 大小限定
	rR = max(min(rR, 255), 0);
	rG = max(min(rG, 255), 0);
	rB = max(min(rB, 255), 0);
	return RGB(rR, rG, rB);
}

// 初始化曲线
void CGraphDlg::InitLines(void)
{
	mChartCtrl.RemoveAllSeries(); // 先清空
	double hueStep = 2*PI / (MAX_GRAPH_LINES);
	for (int i = 0; i < MAX_GRAPH_LINES; i++)
	{
		lines[i].pLineSerie = mChartCtrl.CreateLineSerie();
		lines[i].pLineSerie->SetSeriesOrdering(poNoOrdering);
		lines[i].pLineSerie->SetWidth(1);
		// 以色度步进
		lines[i].color = HSI2RGB(hueStep*i, 0.45, 0.50);
		lines[i].IsVisible = false;
		for (int j = 0; j < MAX_GRAPH_POINT; j++)
			lines[i].data[j] = 0;
	}
	// 自定义颜色，是否显示

	for (int i = 0; i < MAX_GRAPH_LINES; i++)
	{
		lines[i].pLineSerie->SetVisible(lines[i].IsVisible);
		lines[i].pLineSerie->SetColor(lines[i].color);
	}
	for (int i = 0; i < MAX_GRAPH_POINT; i++)
		lineXdata[i] = i;;
	pLineNow = 0;

}

// 初始化图表
void CGraphDlg::InitChart(void)
{
	CChartAxis *pAxis = NULL;
	// 关闭刷新
	mChartCtrl.EnableRefresh(false);
	// 设置轴和曲线
	pBottomAxis = mChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pLeftAxis = mChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);

	InitLines();

	pBottomAxis->SetAutomatic(false);
	pBottomAxis->EnableScrollBar(false);
	pBottomAxis->SetAutoHideScrollBar(true);
	pBottomAxis->SetMinMax(0, MAX_GRAPH_POINT);

	pLeftAxis->SetAutomatic(false);
	pLeftAxis->EnableScrollBar(false);
	pLeftAxis->SetAutoHideScrollBar(true);
	pLeftAxis->SetMinMax(-50, 50);

	COLORREF BackColor = RGB(225, 235, 225);
	mChartCtrl.SetBackColor(BackColor);
	// 开启刷新
	mChartCtrl.EnableRefresh(true);
}

// 刷新波形图
// 增加一个点，刷新全图，时间在绘图操作上刷新较多，后面可以改进
void CGraphDlg::FlashGraph(void)
{
	int i, j;
	mChartCtrl.EnableRefresh(false);
	for (i = 0; i < MAX_GRAPH_LINES; i++)
	{
		if (lines[i].IsVisible)
		{
			for (j = 0; j < MAX_GRAPH_POINT - 1; j++)
			{
				lines[i].data[j] = lines[i].data[j + 1];
			}
			lines[i].data[j] = lines[i].dataNew;
			lines[i].pLineSerie->SetPoints(lineXdata, lines[i].data, MAX_GRAPH_POINT);
		}
	}

	mChartCtrl.EnableRefresh(true);
}

// 添加原始数据，并将其转换为分类，添加到曲线数据
// 可以用正则表达式重构
void CGraphDlg::AddDataString(CString str)
{
	if (!isGraph)
		return;
	int lastSpacePos, nowSpacePos;
	int index;
	double data;
	CString tmp;
	static CString lastStr;
	static bool isLast = false;
	static int cnt;

	// 数据是否是完整的一帧
	if (isLast) {
		if (str.FindOneOf("\r\n") == -1)
		{
			str = lastStr + str;
			return;
		}
		else
		{
			str = lastStr + str;
			lastStr.Empty();
			isLast = false;
		}
	}
	else
	{
		if (str.FindOneOf("\r\n") == -1)
		{
			isLast = true;
			lastStr.Append(str);
			return;
		}
	}
	nowSpacePos = 0;
	for (int i = 0; i < MAX_GRAPH_LINES; i++)
	{
		lastSpacePos = str.Find(' ', nowSpacePos) + 1;

		if (lastSpacePos == 0)
		{
			break;	// 没有空格帧，或到帧末尾
		}

		nowSpacePos = str.Find(' ', lastSpacePos);
		if (nowSpacePos == -1)
		{	// 末尾帧
			nowSpacePos = str.GetLength();
		}

		// 字符串长度得大于3，否则帧错误
		if (nowSpacePos - lastSpacePos < 3)
		{
			return;
		}

		tmp = str.Mid(lastSpacePos, nowSpacePos - lastSpacePos) + '\0';
		// 当读取两个有效数据时正确
		if (sscanf_s(tmp.GetBuffer(0), "%d:%lf", &index, &data) == 2)
		{
			TRACE("index:%d data:%lf\r\n", index, data);
			//添加数据
			if (index < MAX_GRAPH_LINES) {
				lines[index].dataNew = data;
				tmp.Format("%g", data);
				pList->SetItemText(index, 1, tmp);
			}
		}
		else
		{
			return;	// 帧错误
		}
		// 继续一帧
		nowSpacePos=lastSpacePos;
	}
	cnt++;
	FlashGraph();
}

// 清除图表数据
void CGraphDlg::OnBnClickedBtncleargraph()
{
	for (int i = 0; i < MAX_GRAPH_LINES; i++)
	{
		for (int j = 0; j < MAX_GRAPH_POINT; j++)
			lines[i].data[j] = 0;
	}
}

void CGraphDlg::OnOK()
{
	// CDialog::OnOK();
	return;
}


void CGraphDlg::OnBnClickedBtnstartgraph()
{
	isGraph = !isGraph;
	SetDlgItemText(IDC_BtnStartGraph, isGraph ? "停止" : "开始");
}

// 切换设置的轴
void CGraphDlg::OnBnClickedRadAxis()
{
	double max, min;
	CString strTmp;

	CButton* btn = (CButton*)GetDlgItem(IDC_RadYAxis);
	if (btn->GetCheck() == 1)
		pAxisNow = pLeftAxis;
	else
		pAxisNow = pBottomAxis;

	((CButton*)GetDlgItem(IDC_CkbAxisAuto))->SetCheck(pAxisNow->IsAutomatic());
	((CButton*)GetDlgItem(IDC_CkbAxisScroll))->SetCheck(pAxisNow->ScrollBarEnabled());
	((CButton*)GetDlgItem(IDC_CkbAxisGrid))->SetCheck(pAxisNow->GetGrid()->IsVisible());

	pAxisNow->GetMinMax(min, max);
	strTmp.Format("%g", min);
	SetDlgItemText(IDC_EdbAxisMin, strTmp);
	strTmp.Format("%g", max);
	SetDlgItemText(IDC_EdbAxisMax, strTmp);
}

// 设置滚动
void CGraphDlg::OnBnClickedCkbaxisscroll()
{
	pAxisNow->EnableScrollBar(IsDlgButtonChecked(IDC_CkbAxisScroll) != 0);
}

// 设置自动
void CGraphDlg::OnBnClickedCkbaxisauto()
{
	pAxisNow->SetAutomatic(IsDlgButtonChecked(IDC_CkbAxisAuto) != 0);
}

// 设置网格
void CGraphDlg::OnBnClickedCkbaxisgrid()
{
	pAxisNow->GetGrid()->SetVisible(IsDlgButtonChecked(IDC_CkbAxisGrid) != 0);
}

// 失去焦点时候，设置最大最小值
void CGraphDlg::OnEdbaxismin()
{
	CString strTmp;
	double min, max, num;
	int ret;
	GetDlgItemText(IDC_EdbAxisMin, strTmp);
	pAxisNow->GetMinMax(min,max);
	ret = sscanf_s(strTmp, "%lf", &num);
	// 不违反范围，和格式
	if (num < max || (ret != 1))
	{
		min = num;
	}
	else
	{
		strTmp.Format("%g", min);
		SetDlgItemText(IDC_EdbAxisMin, strTmp);
	}
	pAxisNow->SetMinMax(min,max);
}

void CGraphDlg::OnEdbaxismax()
{
	CString strTmp;
	double min, max, num;
	int ret;
	GetDlgItemText(IDC_EdbAxisMax, strTmp);
	pAxisNow->GetMinMax(min, max);

	ret = sscanf_s(strTmp, "%lf", &num);
	// 不违反范围，和格式
	if (num > min || (ret != 1))
	{
		max = num;
	}
	else
	{
		strTmp.Format("%g", max);
		SetDlgItemText(IDC_EdbAxisMax, strTmp);
	}
	pAxisNow->SetMinMax(min, max);

}

bool CGraphDlg::ColorPicker(COLORREF &color)
{
	COLORREF dcolor=RGB(200,200,200);
	//创建颜色对话框,并用取得的文字格式初始化对话框
	CColorDialog dlg(dcolor, CC_FULLOPEN, this);
	if (dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		return true;
	}
	return false;
}

void CGraphDlg::OnBnClickedBtngraphcolor()
{
	COLORREF color;
	if (ColorPicker(color))
		mChartCtrl.SetBackColor(color);
}

// 设置曲线可见
void CGraphDlg::OnBnClickedCkblinev()
{
	if (IsDlgButtonChecked(IDC_CkbLineV) == 0)
	{
		lines[pLineNow].pLineSerie->SetVisible(false);
		lines[pLineNow].IsVisible = false;
	}
	else {
		lines[pLineNow].pLineSerie->SetVisible(true);
		lines[pLineNow].IsVisible = true;
	}

}

// list 数据重绘，不同颜色
void CGraphDlg::OnCustomdrawLstlinedata(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//*pResult = 0;

	NMLVCUSTOMDRAW* pNMCD = (NMLVCUSTOMDRAW*)(pNMHDR);

	if (CDDS_PREPAINT == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pNMCD->nmcd.dwDrawStage)
	{
		COLORREF  clrNewTextColor, clrNewBkColor;
		int nItem = static_cast<int>(pNMCD->nmcd.dwItemSpec);

		clrNewTextColor = RGB(0, 0, 0);
		if (nItem < MAX_GRAPH_LINES)
		{
			clrNewBkColor = lines[nItem].color;
		}
		else
			clrNewBkColor = RGB(255,0,0);
		pNMCD->clrText = clrNewTextColor;
		pNMCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_DODEFAULT;

	}
}
// 切换选择曲线
void CGraphDlg::OnClickLstlinedata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	CString str;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	// 如果iItem不是-1，就说明有列表项被选择
	if (-1 != pNMListView->iItem)
	{
		pLineNow = pNMListView->iItem;
		str.Format("曲线:%d", pLineNow);
		SetDlgItemText(IDC_TxtLineP, str);
		((CButton*)GetDlgItem(IDC_CkbLineV))->SetCheck(lines[pLineNow].pLineSerie->IsVisible());

		((CComboBox*)GetDlgItem(IDC_CbLineWidth))->SetCurSel(lines[pLineNow].pLineSerie->GetWidth()-1);
		((CComboBox*)GetDlgItem(IDC_CbLinePen))->SetCurSel(lines[pLineNow].pLineSerie->GetPenStyle());

	}
}

// 设置曲线颜色
void CGraphDlg::OnBnClickedBtnlinecolor()
{
	COLORREF color;
	if (ColorPicker(color))
	{
		//设置列表颜色
		lines[pLineNow].color = color;
		//设置曲线颜色
		lines[pLineNow].pLineSerie->SetColor(color);
	}
}

// 设置线宽
void CGraphDlg::OnSelendokCblinewidth()
{
	CString tmpStr;
	CComboBox *pComBox;

	pComBox = (CComboBox*)GetDlgItem(IDC_CbLineWidth);
	lines[pLineNow].pLineSerie->SetWidth(pComBox->GetCurSel()+1);
}

// 设置线样式
void CGraphDlg::OnSelendokCblinepen()
{
	CString tmpStr;
	CComboBox *pComBox;

	pComBox = (CComboBox*)GetDlgItem(IDC_CbLinePen);
	lines[pLineNow].pLineSerie->SetPenStyle(pComBox->GetCurSel());
}

// 设置滚动
void CGraphDlg::OnBnClickedBtnsaveline()
{
	CFileDialog fDlg(
		FALSE, // FALSE为存储文件
		"txt", // 默认扩展名
		"LinesData", // 默认文件名
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // 隐藏只读复选和开启可覆写功能
		NULL, // 后缀过滤
		NULL);

	if (IDOK == fDlg.DoModal())
	{
		CString strFilePath;
		CString strTmp,str;
		CFile mFile;
		strFilePath = fDlg.GetPathName();
		if (mFile.Open(strFilePath, CFile::modeCreate | CFile::modeWrite))
		{
			for (int i = 0; i < MAX_GRAPH_LINES; i++)
			{
				str.Format("Line:%d",i);
				for (int j = 0; j < MAX_GRAPH_POINT; j++)
				{
					strTmp.Format("\t%g", lines[i].data[j]);
					str += strTmp;
				}
				str += "\r\n";
				mFile.Write(str, str.GetLength());
			}
			mFile.Close();
		}
	}
}
//true为绑定
void CGraphDlg::DoDDX_Control(bool isAttach)
{
	if (isAttach) {
		if (mChartCtrl.SubclassDlgItem(IDC_Chart, this) == false)
			MessageBox("Fail");
	}
	else
	{
		if (mChartCtrl.GetSafeHwnd())
			mChartCtrl.UnsubclassWindow();
	}
}

afx_msg LRESULT CGraphDlg::CloseGraphFull(WPARAM wParam, LPARAM lParam)
{
	// 接收关闭消息
	OnBnClickedBtngraphfull();
	return 0;
}

// 外挂模式
void CGraphDlg::OnBnClickedBtngraphfull()
{
	bool isChange = false;
	if (isGraph) {
		OnBnClickedBtnstartgraph();
		isChange = true;
	}
	isFullGraph = !isFullGraph;
	if (isFullGraph)
	{
		SetDlgItemText(IDC_BtnGraphFull, "内嵌");
		GraphFullDlg.Create(IDD_GraphFull);
		GraphFullDlg.ShowWindow(true);
		// 解绑，绑定
		DoDDX_Control(false);
		GraphFullDlg.DoDDX_Control(true);

	}
	else
	{
		SetDlgItemText(IDC_BtnGraphFull, "外挂");
		// 将尺寸还原到内嵌的大小
		CRect rec;
		GetDlgItem(IDC_Chart)->GetClientRect(&rec);
		GraphFullDlg.Resize(rec.bottom, rec.right);

		// 解绑，绑定
		GraphFullDlg.DoDDX_Control(false);
		DoDDX_Control(true);
		GraphFullDlg.OnCancel();
	}

	if (isChange)
		OnBnClickedBtnstartgraph();
}


BOOL CGraphDlg::PreTranslateMessage(MSG* pMsg)
{
	// 屏蔽ESC键，防止对话框退出
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
