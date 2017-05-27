#pragma once


#define MAX_LOADSTRING 100
#include "ChartCtrl\ChartCtrl.h"
#include "ChartCtrl\ChartLineSerie.h" 
#include "ChartCtrl\PointsOrdering.h"

#include "GraphFull.h"
// CGraphDlg 对话框

#define MAX_GRAPH_LINES 6
#define MAX_GRAPH_POINT 512
struct
{
	double dataNew;
	double data[MAX_GRAPH_POINT];

	COLORREF color;
	bool IsVisible;
	CChartLineSerie *pLineSerie;
}typedef LineData;

class CGraphDlg : public CDialog
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGraphDlg();

// 对话框数据
	enum { IDD = IDD_GRAPH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	void AddDataString(CString str);
	void FlashGraph();
	bool ColorPicker(COLORREF &color);
	COLORREF HSI2RGB(double H, double S, double I);
	void DoDDX_Control(bool isAttach);
private:
	bool isGraph, isFullGraph;
	void InitChart(void);
	void InitLines(void);
	double lineXdata[MAX_GRAPH_POINT];

	CChartCtrl mChartCtrl;
	CChartStandardAxis* pBottomAxis;
	CChartStandardAxis* pLeftAxis;
	CChartStandardAxis* pAxisNow;

	CListCtrl *pList;
	LineData lines[MAX_GRAPH_LINES];
	int pLineNow;

	bool listColorAvailable;
	CGraphFull GraphFullDlg;

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtncleargraph();
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnstartgraph();
	afx_msg void OnBnClickedRadAxis();
	afx_msg void OnBnClickedCkbaxisscroll();
	afx_msg void OnBnClickedCkbaxisauto();
	afx_msg void OnEdbaxismin();
	afx_msg void OnEdbaxismax();
	afx_msg void OnBnClickedCkbaxisgrid();
	afx_msg void OnBnClickedBtngraphcolor();
	afx_msg void OnCustomdrawLstlinedata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCkblinev();
	afx_msg void OnClickLstlinedata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnlinecolor();
	afx_msg void OnSelendokCblinewidth();
	afx_msg void OnSelendokCblinepen();
	afx_msg void OnBnClickedBtnsaveline();
	afx_msg void OnBnClickedBtngraphfull();
	afx_msg LRESULT CloseGraphFull(WPARAM wParam, LPARAM lParam);
};
