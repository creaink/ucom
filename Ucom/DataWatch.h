#pragma once

// CDataWatch 对话框

class CDataWatch : public CDialog
{
	DECLARE_DYNAMIC(CDataWatch)

public:
	CDataWatch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataWatch();

// 对话框数据
	enum { IDD = IDD_WATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	bool isRecData,isFullClear;
	int ItemCnt,MaxItem;

	static const char HexTable[];

	CListCtrl *pList;
	void ClearList(void);
	void ChangeMaxItem(void);

public:
	virtual BOOL OnInitDialog();
	void CDataWatch::AddItem(CString str);
	afx_msg void OnBnClickedBtnwatch();
	afx_msg void OnBnClickedCkbdisphex();
	afx_msg void OnEnChangeEdbmaxlist();
	afx_msg void OnBnClickedBtnclearlist();
	afx_msg void OnClickListwatch(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
};
