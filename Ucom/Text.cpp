#include "stdafx.h"
#include "Ucom.h"
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
		char HexCh[4] = " HL";
		while (nBytes--)
		{
			HexCh[1] = HexTable[(*pCh) / 16];
			HexCh[2] = HexTable[(*pCh) % 16];
			DataStr += HexCh;
			pCh++;
		}
	}
	else
	{
		//拷贝'0'
		for (int i = 0; i < nBytes; i++)
		{
			DataStr += mStr[i];
		}

	}
}


unsigned char TextBank::ValueOfString(LPCTSTR buffer)
{
	unsigned char dat = 0;
	for (int i = 16; i > 0; i -= 15)
	{
		if (*buffer <= '9')
			dat += i*(*buffer - '0');
		else if (*buffer <= 'f')
			dat += i*(*buffer - 'a' + 10);
		else if (*buffer <= 'F')
			dat += i*(*buffer - 'A' + 10);
		buffer++;
	}
	return dat;
}


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



/**
* @brief 队列的数组方式实现
* @param QD_TYPE为队列的数据类型，NewDat新数据，pQueue备操作的队列
* @retval
**/
QD_TYPE Quene(QD_TYPE NewDat, Queue_Type *pQueue)
{
	QD_TYPE tmp = pQueue->dat[pQueue->pNow];
	pQueue->dat[pQueue->pNow] = NewDat;
	if (++pQueue->pNow == pQueue->QUEUE_NUM)
	{
		pQueue->pNow = 0;
	}
	return tmp;
}

QD_TYPE GetQueneAver(Queue_Type *pQueue)
{
	int cnt;
	int Aver = 0;
	for (cnt = 0; cnt<pQueue->QUEUE_NUM; cnt++)
	{
		Aver += pQueue->dat[cnt];
	}
	Aver = Aver / pQueue->QUEUE_NUM;
	return Aver;
}

QD_TYPE GetQueneSum(Queue_Type *pQueue)
{
	int cnt;
	int sum = 0;
	for (cnt = 0; cnt<pQueue->QUEUE_NUM; cnt++)
	{
		sum += pQueue->dat[cnt];
	}
	return sum;
}