#pragma once

#include <vector>

// CSplitter
#define SPF_TOP             0x0010                  /* distance of the control to the top of the window will be constant */
#define SPF_BOTTOM          0x0020                  /* distance of the control to the bottom of the window will be constant */
#define SPF_LEFT            0x0040                  /* distance of the control to the left of the window will be constant */
#define SPF_RIGHT           0x0080                  /* distance of the control to the right of the window will be constant */

#define UWM_SPLIT_MOVED_MSG		_T("UWM_SPLIT_MOVED_MSG	")
static UINT UWM_SPLIT_MOVED = ::RegisterWindowMessage(UWM_SPLIT_MOVED_MSG);

enum {
	CS_VERT = 1,
	CS_HORZ = 2,
	CS_NONE = 0
};

class CSplitter : public CButton
{
public:
	CSplitter();

public:
	void AddToBottomOrRightCtrls(UINT nCtrlId, WORD nFlags = SPF_TOP | SPF_LEFT | SPF_RIGHT | SPF_BOTTOM);
	void AddToTopOrLeftCtrls(UINT nCtrlId, WORD nFlags = SPF_TOP | SPF_LEFT | SPF_BOTTOM | SPF_RIGHT);
	void SetType(UINT nType);
	void SetBottomLimit(UINT nCtrlId);
	virtual ~CSplitter();


protected:
	//{{AFX_MSG(CSplitter)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	unsigned int m_nType;
	std::vector<DWORD> m_vtTopLeftControls;
	std::vector<DWORD> m_vtBottomRightControls;
	CWnd * m_pOldDragCapture;
	CPoint m_ptStartDrag, m_ptStartPos;
	bool m_bDragging;
	HCURSOR m_hCursor;
	CRect m_rectMax;
//	DECLARE_DYNAMIC(CSplitter)
//
//public:
//	CSplitter();
//	virtual ~CSplitter();
//
//protected:
//	DECLARE_MESSAGE_MAP()
};


