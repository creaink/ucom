#pragma once
#include "afxwin.h"
#include "Text.h"

// CEncoder 对话框

class CEncoder : public CDialog
{
	DECLARE_DYNAMIC(CEncoder)

public:
	CEncoder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEncoder();

// 对话框数据
	enum { IDD = IDD_ENCODER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
private:
	int encSel;
public:
	CString GetAsciiCode(CString &str);
	static const char HexTable[17];
	CString Ascii2Unicode(CString &strascii);
	CString Ascii2Utf8(CString &strascii);
	CString Unicode2Ascii(const CString &dataStr);
	CString Utf8toAscii(const CString &dataStr);

	CString GetHexString(CString &dataStr);
	CString GetDecString(const CString &dataStr);
	TextBank DataDec;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnEnChangeEncinput();
	afx_msg void OnBnClickedEncclear();
//	CStatic mEncDisp;
	int m_RadioEnc;
	afx_msg void OnClickedRadio1();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEncEec();
};
