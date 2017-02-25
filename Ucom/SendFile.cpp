// SnedFile.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "SendFile.h"
#include "afxdialogex.h"


// CSnedFile 对话框

IMPLEMENT_DYNAMIC(CSendFile, CDialog)

CSendFile::CSendFile(CWnd* pParent, HANDLE *hUART)
: CDialog(CSendFile::IDD, pParent)
{
	hUartCom = hUART;
}

CSendFile::~CSendFile()
{
}

void CSendFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CSendFile::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(_T("文件发送"));
	strFilePath = "";
	memset(&m_osWrite, 0, sizeof(OVERLAPPED));
	m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PrgSend);
	pProgress->SetStep(1);
	pProgress->SetRange(0, 10);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

BEGIN_MESSAGE_MAP(CSendFile, CDialog)
	ON_BN_CLICKED(IDC_BtnLoad, &CSendFile::OnBnClickedBtnload)
	ON_BN_CLICKED(IDC_BtnSendFile, &CSendFile::OnBnClickedBtnsendfile)
END_MESSAGE_MAP()


// CSendFile 消息处理程序
void CSendFile::AppendLog(CString str)
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EdbFileLog);
	pEdit->SetSel(-1, -1);
	pEdit->ReplaceSel(str);
}

void CSendFile::OnBnClickedBtnload()
{
	CFileDialog fDlg(
		true, // FALSE为存储文件
		NULL, // 默认扩展名
		NULL, // 默认文件名
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // 隐藏只读复选和开启可覆写功能
		_T("文本文件(*.txt)|*.txt|All Files (*.*)|*.*||"), // 后缀过滤
		NULL);

	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PrgSend);
	pProgress->SetPos(0);

	if (IDOK == fDlg.DoModal())
	{
		strFilePath = fDlg.GetPathName();
		CString strtmp;
		strtmp += ">> "+fDlg.GetFileName()+" 载入成功\r\n";
		AppendLog(strtmp);
	}
}

#define MAX_PIC_Bytes 1024
void CSendFile::OnBnClickedBtnsendfile()
{
	CString strtmp;
	if (strFilePath.IsEmpty())
	{
		strtmp += ">> Error：文件不存在\r\n";
		AppendLog(strtmp);
	}
	else
	{
		CFile mFile;
		if (mFile.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
		{
			ULONGLONG lBytes=mFile.GetLength();
			UINT nBytes = (UINT)lBytes;
			UINT nCnt = nBytes / MAX_PIC_Bytes;
			UINT rBytes = nBytes % MAX_PIC_Bytes;
			CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PrgSend);

			char buf[MAX_PIC_Bytes + 1];
			if (nBytes > MAXUINT)
			{
				strtmp+=">> Error：文件过大\r\n";
				AppendLog(strtmp);
				return;
			}
			strtmp.Format(">> 文件大小：%d B\r\n<< 正在发送...\r\n",nBytes);
			AppendLog(strtmp);

			if (nBytes < MAX_PIC_Bytes)
			{
				pProgress->SetRange(0, 1);
				pProgress->SetPos(0);

				mFile.Read(buf, nBytes);
				UnblockSend(buf, nBytes);
				
				pProgress->SetPos(1);
			}
			else
			{
				pProgress->SetRange(0, nCnt);
				pProgress->SetPos(0);
				TRACE("loop:%d\n", nCnt);

				for (UINT i = 0; i < nCnt; i++)
				{
					mFile.Read(buf, MAX_PIC_Bytes);
					pProgress->SetPos(i);
					UnblockSend(buf, MAX_PIC_Bytes);
				}
				if (rBytes != 0)
				{
					mFile.Read(buf, rBytes);
					UnblockSend(buf, rBytes);
					pProgress->SetPos(nCnt);
				}
			}
			strtmp=">> 发送成功！\r\n--------------------我是分隔线--------------------\r\n";
			AppendLog(strtmp);
			mFile.Close();
		}
	}
}

int CSendFile::UnblockSend(const char*pBuff, UINT len)
{
	BOOL bWriteStatus;
	COMSTAT ComStat;
	DWORD dwErrorFlags, dwLength;

	ClearCommError(*hUartCom, &dwErrorFlags, &ComStat);
	if (dwErrorFlags>0)
	{
		TRACE("Unblock Write Failed\n");
		PurgeComm(*hUartCom, PURGE_TXABORT | PURGE_TXCLEAR);
		return 0;
	}
	m_osWrite.Offset = 0;

	bWriteStatus = WriteFile(*hUartCom, pBuff, len, &dwLength, &m_osWrite);

	if (!bWriteStatus)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			//如果重叠操作未完成,等待直到操作完成
			GetOverlappedResult(*hUartCom, &m_osWrite, &dwLength, TRUE);
		}
		else
			dwLength = 0;
	}
	return dwLength;
}


void CSendFile::OnOK()
{
}
