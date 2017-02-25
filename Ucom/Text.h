#pragma once


//文字处理类
class TextBank
{
private:
	static const char HexTable[17];
	int ByteCnt;
	CString DataStr;
public:
	TextBank(void);
	virtual ~TextBank();

	void AppendString(CString &mStr, bool isHexFormat = false);
	void ReString(CString &mStr, bool isHexFormat = false);
	unsigned char ValueOfString(LPCTSTR buffer);
	
	const CString &GetCStrData(void){
		return DataStr;
	}
	unsigned int GetLength(void){
		return ByteCnt;
	}
	void ClearData(void){
		DataStr.Empty();
		ByteCnt = 0;
	}
	int isHexChar(char ch){
		if ((ch <= '9' && ch >= '0')
			|| (ch <= 'F' && ch >= 'A')
			|| (ch <= 'f' && ch >= 'a'))
			return 1;
		else if (ch == ' ')
			return 2;
		else
			return 0;
	}
};

