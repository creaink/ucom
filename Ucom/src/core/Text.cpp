#include "stdafx.h"
#include "dialog/Ucom.h"
#include "Text.h"


TextBank::TextBank(void)
{
	DataStr.Empty();
	ByteCnt = 0;
}

TextBank::~TextBank()
{
}

const char TextBank::HexTable[] = "0123456789ABCDEF";

void TextBank::AppendString(CString &mStr, bool isHexFormat)
{
	int nBytes = mStr.GetAllocLength();

	ByteCnt += nBytes;
	if (isHexFormat)
	{
		const unsigned char *pCh = (const unsigned char *)(mStr.GetBuffer(0));
		char * buffer = (char *)malloc(nBytes * 3 + 1);

		buffer[nBytes * 3] = '\0';
		for (int i = 0; i < nBytes; i++)
		{
			buffer[i * 3] = ' ';
			buffer[i * 3 + 1] = HexTable[(*pCh) / 16];
			buffer[i * 3 + 2] = HexTable[(*pCh) % 16];
			pCh++;
		}
		DataStr += buffer;
		free(buffer);
	}
	else
	{
		// 拷贝 '\0'
		DataStr.Append(mStr, nBytes);
	}
}


unsigned char TextBank::ValueOfString(LPCTSTR buffer)
{
	unsigned char dat = 0;
	for (int i = 16; i > 0; i -= 15)
	{
		if (*buffer <= '9')
			dat += i * (*buffer - '0');
		else if (*buffer <= 'F')
			dat += i * (*buffer - 'A' + 10);
		else if (*buffer <= 'f')
			dat += i * (*buffer - 'a' + 10);
		buffer++;
	}
	return dat;
}

// isHexFormat=true 将16进制字符串转为二进制数据存入
void TextBank::ReString(CString &mStr, bool isHexFormat)
{
	ByteCnt = 0;
	if (isHexFormat)
	{
		DataStr.Empty();
		char *pStr = mStr.GetBuffer(0);
		for (int i = 0; *pStr != '\0'; pStr++)
		{
			if (isHexChar(*pStr) == 1
				&& isHexChar(*(pStr + 1)) == 1)
			{
				DataStr.AppendChar(ValueOfString(pStr));
				ByteCnt++;
				pStr++;
			}
			else
				continue;
		}
	}
	else
	{
		DataStr = mStr;
		ByteCnt = mStr.GetLength();
	}
}
