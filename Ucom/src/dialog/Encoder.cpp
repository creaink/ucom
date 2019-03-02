// Encoder.cpp : 实现文件
//

#include "stdafx.h"
#include "Ucom.h"
#include "Encoder.h"
#include "afxdialogex.h"


// CEncoder 对话框

IMPLEMENT_DYNAMIC(CEncoder, CDialog)

CEncoder::CEncoder(CWnd* pParent /*=NULL*/)
	: CDialog(CEncoder::IDD, pParent)
	, m_RadioEnc(0)
{

}

CEncoder::~CEncoder()
{
}

void CEncoder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_GrpEnc, mEncDisp);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioEnc);
}


BEGIN_MESSAGE_MAP(CEncoder, CDialog)
	ON_EN_CHANGE(IDC_EncInput, &CEncoder::OnEnChangeEncinput)
	ON_BN_CLICKED(IDC_EncClear, &CEncoder::OnBnClickedEncclear)
	ON_BN_CLICKED(IDC_RADIO1, &CEncoder::OnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CEncoder::OnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CEncoder::OnClickedRadio1)
	ON_CBN_SELCHANGE(IDC_CbEncDec, &CEncoder::OnSelchangeEncEec)
END_MESSAGE_MAP()


BOOL CEncoder::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox *pComBox;

	pComBox = (CComboBox *)GetDlgItem(IDC_CbEncDec);
	pComBox->InsertString(0, "HEX解码");
	pComBox->InsertString(0, "编码查询");
	// 选择转HEX为默认
	pComBox->SetCurSel(0);
	return TRUE;
}

// CEncoder 消息处理程序

const char CEncoder::HexTable[] = "0123456789ABCDEF";

// 将ascii编码格式的字符串转为小端Unicode编码的hex string
CString CEncoder::Ascii2Unicode(CString &strascii)
{
	CString res, dataStr;
	int nBytes = strascii.GetLength() * 2 + 2;
	res.GetBufferSetLength(nBytes);
	// 转为大端模式的Unicode
	MultiByteToWideChar(CP_ACP, 0, (LPCCH)strascii.GetBuffer(0), -1, (LPWSTR)res.GetBuffer(0), nBytes);

	const unsigned char *pCh = (const unsigned char *)(res.GetBuffer(0));
	//去掉停止符0x0000
	//nBytes = res.GetLength() - 2;

	char HexCh[] = "0xHLHL,";
	if (m_RadioEnc == 2)
		HexCh[6] = ' ';

	while (nBytes)
	{
		// 大端模式的Unicode转小端模式的hex string
		HexCh[2] = HexTable[*(pCh + 1) / 16];
		HexCh[3] = HexTable[*(pCh + 1) % 16];

		HexCh[4] = HexTable[(*pCh) / 16];
		HexCh[5] = HexTable[(*pCh) % 16];
		if (m_RadioEnc == 0)
			dataStr += (HexCh);
		else
			dataStr += (HexCh + 2);

		pCh += 2;
		nBytes -= 2;
		if (*(pCh + 1) == '\0'&&*pCh == '\0')
			break;
	}
	dataStr.Delete(dataStr.GetLength() - 1, 1);
	return dataStr;
}

// 将ascii编码格式的字符串转为utf8编码的hex string
CString CEncoder::Ascii2Utf8(CString &strascii)
{
	CString res, dataStr;
	int nBytes = strascii.GetLength() * 2 + 2;
	//先转为Unicode
	res.GetBufferSetLength(nBytes);
	MultiByteToWideChar(CP_ACP, 0, strascii.GetBuffer(0), -1, (LPWSTR)res.GetBuffer(0), nBytes);

	nBytes = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)res.GetBuffer(0), -1, NULL, 0, NULL, NULL);
	dataStr.GetBufferSetLength(nBytes);
	WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)res.GetBuffer(0), -1, dataStr.GetBuffer(0), nBytes, NULL, NULL);

	const unsigned char *pCh = (const unsigned char *)(dataStr.GetBuffer(0));
	//去掉停止符0x00
	nBytes--;
	res.Empty();

	char HexCh[] = "0xHL,";
	if (m_RadioEnc == 2)
		HexCh[4] = ' ';
	
	while (nBytes--)
	{
		HexCh[2] = HexTable[(*pCh) / 16];
		HexCh[3] = HexTable[(*pCh) % 16];
		if (m_RadioEnc == 0)
			res += HexCh;
		else
			res += (HexCh+2);

		pCh++;
	}
	res.Delete(res.GetLength() - 1, 1);
	return res;
}

// 得到ascii编码的hex string
CString CEncoder::GetAsciiCode(CString &str)
{
	CString DataStr;
	const unsigned char *pCh = (const unsigned char *)(str.GetBuffer(0));
	int nBytes = str.GetAllocLength();

	char HexCh[] = "0xHL,";
	if (m_RadioEnc == 2)
		HexCh[4] = ' ';
	while (nBytes--)
	{
		HexCh[2] = HexTable[(*pCh) / 16];
		HexCh[3] = HexTable[(*pCh) % 16];
		if (m_RadioEnc == 0)
			DataStr += HexCh;
		else
			DataStr += (HexCh + 2);
		pCh++;
	}
	DataStr.Delete(DataStr.GetLength() - 1, 1);
	return DataStr;
}

// Unicode编码转ascii编码
CString CEncoder::Unicode2Ascii(const CString &dataStr)
{
	int nBytes;
	CString res, uniStr = dataStr;
	uniStr = uniStr + '\0'+'\0';

	nBytes = -1 + WideCharToMultiByte(CP_ACP, 0, (LPWSTR)uniStr.GetBuffer(0), -1, NULL, 0, NULL, 0);
	res.GetBufferSetLength(nBytes);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)uniStr.GetBuffer(0), -1, res.GetBuffer(0), nBytes, NULL, 0);
	nBytes = res.GetAllocLength();
	for (int i=0; i < nBytes;i++)
	{
		if (res[i] == '\0')
			res.Delete(i, 1);//删除UNICODE兼容ascii生成的'\0'
	}

	return res;
}


CString CEncoder::Utf8toAscii(const CString &dataStr)
{
	int nBytes;
	CString res, uniStr = dataStr;
	uniStr = uniStr + '\0';
	//先转为Unicode
	nBytes = MultiByteToWideChar(CP_UTF8, 0, uniStr.GetBuffer(0), -1, NULL, 0);
	res.GetBufferSetLength(2*nBytes);

	MultiByteToWideChar(CP_UTF8, 0, uniStr.GetBuffer(0), -1, (LPWSTR)res.GetBuffer(0), nBytes);
	uniStr = Unicode2Ascii(res);
	return uniStr;
}

CString CEncoder::GetHexString(CString &dataStr)
{
	return  "ASCII编码:\r\n" + GetAsciiCode(dataStr) + "\r\n\r\nUtf8编码:\r\n" + 
				Ascii2Utf8(dataStr) + "\r\n\r\nUnicode编码(16位小端):\r\n" + Ascii2Unicode(dataStr);
}

CString CEncoder::GetDecString(const CString &dataStr)
{
	CString strtmp;
	strtmp.Format("ASCII解码:\r\n%s\r\n\r\nUtf8解码:\r\n%s\r\n\r\nUnicode解码(16位大端):\r\n%s", dataStr, Utf8toAscii(dataStr), Unicode2Ascii(dataStr));
	return strtmp;
}


void CEncoder::OnEnChangeEncinput()
{
	CString strtmp;
	GetDlgItemText(IDC_EncInput, strtmp);
	if (encSel == 0)
	{
		SetDlgItemText(IDC_EncOuput, GetHexString(strtmp));
	}
	// 代码同发送框的hex格式检查
	else if (encSel == 1)
	{
		char *pCh;
		bool isPure = true;
		CEdit *pEb = (CEdit *)GetDlgItem(IDC_EncInput);
		pCh = strtmp.GetBuffer(0);
		while (*pCh != '\0')
		{
			//禁止输入其他字符
			if (!TextBank::isHexChar(*pCh))
			{
				int pos = strtmp.Find(*pCh);
				strtmp = strtmp.Left(pos);
				SetDlgItemText(IDC_EncInput, strtmp);
				pEb->SetSel(pos, pos, TRUE);
				isPure = FALSE;
				break;
			}
			pCh++;
		}
		if (isPure)
		{
			//16进制格式存入
			DataDec.ReString(strtmp, TRUE);
			SetDlgItemText(IDC_EncOuput, GetDecString(DataDec.GetCStrData()));
		}
		else
		{
			HWND hWnd = ::GetLastActivePopup(GetSafeHwnd()); //查找激活的MessageBox
			if (hWnd != GetSafeHwnd())
			{
				::EndDialog(hWnd, 0);
			}
			AfxMessageBox(_T("请输入有效的HEX格式!\n范围：('0-9','a-f','A-F')\n字节间可以用空格隔开"));
		}
	}
}


void CEncoder::OnBnClickedEncclear()
{
	SetDlgItemText(IDC_EncInput, "");
	SetDlgItemText(IDC_EncOuput, "");
}


void CEncoder::OnClickedRadio1()
{
	UpdateData(true);
	OnEnChangeEncinput();
}


void CEncoder::OnOK()
{
	//CDialog::OnOK();
	return;

}

void CEncoder::OnSelchangeEncEec()
{
	encSel = ((CComboBox *)GetDlgItem(IDC_CbEncDec))->GetCurSel();
	OnBnClickedEncclear();
}
