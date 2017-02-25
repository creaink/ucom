#pragma once


// CMultiSend 对话框
#include "Text.h"
#define LOOP_SEND_ID 1020
class CMultiSend : public CDialog
{
	DECLARE_DYNAMIC(CMultiSend)

public:
	CMultiSend::CMultiSend(CWnd* pParent, bool *pUartOpenStatus, HANDLE *hUART);
	virtual ~CMultiSend();

// 对话框数据
	enum { IDD = IDD_XSEND };


private:
	bool isHotKey;
	void LoppSendSet(void);
	bool *pIsUartOpen;
	HANDLE *hUartCom;

	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;

	TextBank txData;
	void SendEdbox(int index);
	int UnblockSend(const CString &dataStr);
	void OnTimeSend(bool clearcnt=false);
	BOOL CMultiSend::PreTranslateMessage(MSG* pMsg);

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
