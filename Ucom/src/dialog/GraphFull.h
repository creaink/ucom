#pragma once

#define WM_COMM_CLOSE_ME_MSG  (WM_USER + 3)
// CGraphFull 对话框
#include "ChartCtrl.h"

class CGraphFull : public CDialog
{
private:
	CChartCtrl *pChartCtrl;
	
	DECLARE_DYNAMIC(CGraphFull)
public:
	CGraphFull::CGraphFull(CChartCtrl *pChart, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGraphFull();
	HWND pMianHwnd;
	void DoDDX_Control(bool isAttach);
	void Resize(LONG Bottom, LONG Right);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GraphFull };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnCancel();
};
