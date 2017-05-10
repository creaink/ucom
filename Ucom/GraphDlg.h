#pragma once


#define MAX_LOADSTRING 100

// CGraphDlg 对话框

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
	void InitCChart(void);

private:
	HWND hCanvas;
	RECT rectCanvas;
	CDC *pCanvas;
	bool isGraph;
	

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtncleargraph();
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnstartgraph();
};
