#pragma once


// CMultiSend 对话框
// 发送助手
#include "Text.h"
#include "UBase.h"

#define LOOP_SEND_ID 1020
class CMultiSend : public CDialog
{
	DECLARE_DYNAMIC(CMultiSend)

public:
	CMultiSend::CMultiSend(CWnd* pParent, UcomBase** mbase);
	virtual ~CMultiSend();

// 对话框数据
	enum { IDD = IDD_XSEND };


private:
	bool isHotKey;
	void LoppSendSet(void);

	TextBank txData;
	void SendEdbox(int index);
	int UnblockSend(const CString &dataStr);
	void OnTimeSend(bool clearcnt=false);
	BOOL PreTranslateMessage(MSG* pMsg);

	UcomBase** uuBase;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持



	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCkbtimexsend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdbtimexsend();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnsend(UINT   uId);
	afx_msg void OnDropdownCbtransnum();
	afx_msg void OnBnClickedBtntrans();
	afx_msg void OnClickedCkbxhotkey();
};
