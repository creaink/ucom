#pragma once

#define WM_COMM_CLOSE_ME_MSG  (WM_USER + 3)
// CGraphFull Dialog
#include "ChartCtrl.h"

class CGraphFull : public CDialog
{
private:
	CChartCtrl *pChartCtrl;

	DECLARE_DYNAMIC(CGraphFull)
public:
	CGraphFull::CGraphFull(CChartCtrl *pChart, CWnd* pParent = NULL);   // Standard constructor
	virtual ~CGraphFull();
	HWND pMianHwnd;
	void DoDDX_Control(bool isAttach);
	void Resize(LONG Bottom, LONG Right);
// Dialog data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GraphFull };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnCancel();
};
