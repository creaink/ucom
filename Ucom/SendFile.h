#pragma once


// CSnedFile 对话框

class CSendFile : public CDialog
{
	DECLARE_DYNAMIC(CSendFile)

public:
	CSendFile(CWnd* pParent, HANDLE *hUART);   // 标准构造函数
	virtual ~CSendFile();

// 对话框数据
	enum { IDD = IDD_FILE_LOAD };

private:
	CString strFilePath;

	HANDLE *hUartCom;
	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;
	int UnblockSend(const char*pBuff, UINT len);
	void CSendFile::AppendLog(CString str);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnload();
	afx_msg void OnBnClickedBtnsendfile();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
