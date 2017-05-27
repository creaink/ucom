
// UcomDlg.h : 头文件
//

#pragma once

#include "Text.h"
#include "iUart.h"
#include "GraphDlg.h"
#include "Encoder.h"
#include "DataWatch.h"
#include "MultiSend.h"
#include "SendFile.h"


#define AUTO_SEND_TIMER_ID 100
#define FLASH_RX_EDITBOX_TIMER_ID 101
// CUcomDlg 对话框
//class CUcomDlg : public CDialogEx
class CUcomDlg : public CDialog
{
// 构造
public:
	CUcomDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UCOM_DIALOG };
	
public:
	const int rxFlashPeriod = 50;

	//是否有可用串口
	bool uartPortIsOpen;
	bool isNewLineSend;
	bool isDispHex, isSendHex, isDispRx, isRxFullClc,isCmdMode;
	int rxCnt, txCnt, limitBytes;

	void InitCbBuart(void);
	void DisableCbUart(bool choose);
	DCB GetUartConfigDCB(void);

	void ChangeBmpPic(int PicCtrlID, unsigned short  nPicID);

	bool IsUartPortAvailable(void);
	void ReflashRecvEdit(void);

	void OpenUart(void);
	void SendEditBoxData();
	void SetDelaySend(void);

	HANDLE hRxThread;

	iUart mUart;
	TextBank DataRx, DataTx;

	void SetRichLineSpace(void);
	void SetFullBytes(int nbytes = -1);

	DWORD backgroudColor;
	void LoadRegConfig();
	bool InitRegData(void);
	void WriteRegData(void);
	void GetRegData(CString &comName, CString &dcbConfig);

	CGraphDlg GraphDlg;
	CEncoder EncoderDlg;
	CDataWatch DataWatchDlg;
	CMultiSend MultiSendDlg;
	void InitTabEx(void);

	int encoderMode;

private:
	#define MAX_CMD_HISTORY 10
	BOOL PreTranslateMessage(MSG* pMsg);
	int cmdNextPointer, cmdDispPointer;
	CString cmdHistory[MAX_CMD_HISTORY];

	int LargerMode;
	bool isLarge;
	void ChangeItemSize(int nID, int x, int y, bool isEnlarge);
	int MaxWndHeight;

	CRect rectEx;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnSend();
	afx_msg void OnDropdownCbUartPort();
	afx_msg void OnSelendokCbUartPort();
	afx_msg void OnLaunch();
	afx_msg void OnBtnClearRecv();
	afx_msg void OnBtnClearSend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCkbSendOnTime();
	afx_msg void OnChangeEdbSendDelay();
	afx_msg void OnCkbFrameNewLine();
	afx_msg void OnBtnClearnCnt();
	afx_msg void OnCkbDispHex();
	afx_msg void OnChangeEditTxData();
	afx_msg void OnCkbSendHex();
	afx_msg void OnBnClickedBtnisdisprx();
	afx_msg LRESULT OnRxMsgProc(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtntoolbox();
	afx_msg void OnMeudevmanger();
	afx_msg void OnMeucalc();
	afx_msg void OnMeunotepad();
	afx_msg void OnMenureg();
	afx_msg void OnMenucmd();
	afx_msg void OnMenuserv();
	afx_msg void OnBnClickedBtnwinsize();
	afx_msg void OnBnClickedBtnbackcolor();
	afx_msg void OnBnClickedBtnfontcolor();
	afx_msg void OnBnClickedBtnfont();
	afx_msg void OnBnClickedCkbrxfullclc();
	afx_msg void OnChangeEdbfullbytes();
	afx_msg void OnBnClickedBtnsaverx();
	afx_msg void OnBnClickedBtnclearall();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnhelp();
	afx_msg void OnSelchangeTabex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnencoder();
	afx_msg void OnBnClickedBtnsend2();
	virtual void OnOK();
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedBtnsendfile();
	afx_msg void OnBnClickedCkbcmd();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnClickedPicuartstatus();
};
