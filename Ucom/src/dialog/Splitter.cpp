// Splitter.cpp : 实现文件

#include "stdafx.h"
#include "Ucom.h"
#include "Splitter.h"


CSplitter::CSplitter()
{
	m_pOldDragCapture = NULL;
    m_bDragging = false;
	m_nType = CS_NONE;	
	m_hCursor = NULL;
	m_rectMax.SetRectEmpty();
	
}

CSplitter::~CSplitter()
{
	DestroyCursor(m_hCursor);
}


BEGIN_MESSAGE_MAP(CSplitter, CButton)
	//{{AFX_MSG_MAP(CSplitter)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	//if(DragDetect(m_hWnd,point))
	{        
        m_bDragging = true;
        if(m_pOldDragCapture != NULL)
		{
			ReleaseCapture();
			m_pOldDragCapture = NULL;
		}
		//TRACE0("Drag");
		m_pOldDragCapture = SetCapture();
		GetCursorPos(&m_ptStartDrag);
		CRect rect;
		GetWindowRect(&rect);
		m_ptStartPos = rect.TopLeft();
	}
	SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);
	CButton::OnLButtonDown(nFlags, point);
}

void CSplitter::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//TRACE0("OnLButtonUp");
	if(m_pOldDragCapture != NULL)
    {		
		ReleaseCapture();
        m_pOldDragCapture = NULL;
    }
	m_bDragging = false;
	GetParent()->Invalidate();
	GetParent()->SendMessage(UWM_SPLIT_MOVED,(WPARAM)m_hWnd,(LPARAM)MAKELONG(point.x,point.y));
	CButton::OnLButtonUp(nFlags, point);
}

void CSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{		
	if(m_bDragging)
	{
		CRect rect;
		::GetWindowRect(m_hWnd,&rect);		
		CPoint ptMouse;
		GetCursorPos(&ptMouse);
		CSize sizeDiff = ptMouse - m_ptStartDrag;
		CSize sizeMove = m_ptStartPos-rect.TopLeft();
		rect.OffsetRect(sizeMove);
		if(m_nType == CS_HORZ)
		{							
			rect.OffsetRect(0, sizeDiff.cy);
		}else{
			rect.OffsetRect(sizeDiff.cx,0);
		}
		std::vector<DWORD>::iterator it;
		GetParent()->ScreenToClient(&rect);			
		if((m_nType == CS_HORZ) && ((m_rectMax.top >= rect.top) || (m_rectMax.bottom <= rect.bottom)))
		{
			return;
		}
		else if((m_nType == CS_VERT) && ((m_rectMax.left >= rect.left) || (m_rectMax.right <= rect.right)))
		{
			return;
		}
		/*
		TRACE2("M(%d,%d,",m_rectMax.top,m_rectMax.left);
		TRACE2("%d,%d)\n",m_rectMax.bottom,m_rectMax.right);
		TRACE2("(%d,%d,",rect.top,rect.left);
		TRACE2("%d,%d)\n",rect.bottom,rect.right);
		*/
		for ( it=m_vtTopLeftControls.begin() ; it < m_vtTopLeftControls.end(); it++ )
		{
			CWnd* pCtrl = GetParent()->GetDlgItem(LOWORD(*it));
			UINT nFlag = HIWORD(*it);
			if(pCtrl == NULL) continue;
			CRect rectCtrl;
			pCtrl->GetWindowRect(&rectCtrl);
			if(m_nType == CS_HORZ)
			{
				if(nFlag&SPF_BOTTOM)
				{
					rectCtrl.bottom +=  sizeMove.cy;
					rectCtrl.bottom += sizeDiff.cy;
				}
				if(!(nFlag&SPF_TOP))
				{
					rectCtrl.top +=  sizeMove.cy;
					rectCtrl.top += sizeDiff.cy;
				}

			}else{
				if((nFlag&SPF_RIGHT))
				{
					rectCtrl.right +=  sizeMove.cx;
					rectCtrl.right += sizeDiff.cx;
				}
				if(0 ==(nFlag&SPF_LEFT))
				{
					rectCtrl.left +=  sizeMove.cx;
					rectCtrl.left += sizeDiff.cx;
				}
			}			
			GetParent()->ScreenToClient(&rectCtrl);		
			pCtrl->MoveWindow(rectCtrl);
		}
		for ( it=m_vtBottomRightControls.begin() ; it < m_vtBottomRightControls.end(); it++ )
		{
			CWnd* pCtrl = GetParent()->GetDlgItem(LOWORD(*it));
			UINT nFlag = HIWORD(*it);
			if(pCtrl == NULL) continue;
			CRect rectCtrl;
			pCtrl->GetWindowRect(&rectCtrl);
			if(m_nType == CS_HORZ)
			{
				if(nFlag&SPF_TOP)
				{
					rectCtrl.top +=  sizeMove.cy;
					rectCtrl.top += sizeDiff.cy;
				}
				if(!(nFlag&SPF_BOTTOM))
				{
					rectCtrl.bottom +=  sizeMove.cy;
					rectCtrl.bottom += sizeDiff.cy;
				}
			}else{
				if((nFlag&SPF_LEFT))
				{
					rectCtrl.left +=  sizeMove.cx;
					rectCtrl.left += sizeDiff.cx;
				}
				if(!(nFlag&SPF_RIGHT))
				{
					rectCtrl.right +=  sizeMove.cx;
					rectCtrl.right += sizeDiff.cx;
				}
			}
			GetParent()->ScreenToClient(&rectCtrl);		
			pCtrl->MoveWindow(rectCtrl);
			
		}
		
		MoveWindow(rect);		
	}
	CButton::OnMouseMove(nFlags, point);
	
}

//BOOL CSplitter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
//{	
//	if(m_hCursor != NULL)
//	{
//		DestroyCursor(m_hCursor);
//	}
//	if(m_nType == CS_HORZ)
//	{
//		m_hCursor = ::LoadCursor(NULL,IDC_SIZENS);
//	}else
//	{
//		m_hCursor = ::LoadCursor(NULL,IDC_SIZEWE);
//	}
//	SetClassLong(m_hWnd,GCL_HCURSOR,(LONG)m_hCursor);		
//		
//	return CButton::OnSetCursor(pWnd, nHitTest, message);
//}


void CSplitter::SetType(UINT nType)
{
	m_nType = nType;
}

void CSplitter::AddToTopOrLeftCtrls(UINT nCtrlId, WORD nFlags)
{
	ASSERT(m_nType);
	m_vtTopLeftControls.push_back(MAKELONG(nCtrlId,nFlags));
	if(m_rectMax.IsRectEmpty())
	{
		GetParent()->GetWindowRect(m_rectMax);
		/*
		TRACE3("I-%d(%d,%d,",nCtrlId,m_rectMax.top,m_rectMax.left);
		TRACE2("%d,%d)\n",m_rectMax.bottom,m_rectMax.right);
		*/
	}
	CWnd * pCtrl = GetParent()->GetDlgItem(nCtrlId);
	
	if(pCtrl != NULL)
	{
		CRect rect;
		pCtrl->GetWindowRect(&rect);
		//GetParent()->ClientToScreen(rect);
		if((m_nType == CS_HORZ) &&(m_rectMax.top < rect.top))
		{
			m_rectMax.top = rect.top;
		}
		if((m_nType == CS_VERT) && (m_rectMax.left < rect.left))
		{
			m_rectMax.left = rect.left;
		}
	}
	/*
	TRACE3("M-%d(%d,%d,",nCtrlId,m_rectMax.top,m_rectMax.left);
	TRACE2("%d,%d)\n",m_rectMax.bottom,m_rectMax.right);
	*/
}

void CSplitter::AddToBottomOrRightCtrls(UINT nCtrlId, WORD nFlags)
{
	ASSERT(m_nType);
	m_vtBottomRightControls.push_back(MAKELONG(nCtrlId,nFlags));
	if(m_rectMax.IsRectEmpty())
	{
		GetParent()->GetWindowRect(m_rectMax);
		GetParent()->ClientToScreen(m_rectMax);
		/*
		TRACE3("I-%d(%d,%d,",nCtrlId,m_rectMax.top,m_rectMax.left);
		TRACE2("%d,%d)\n",m_rectMax.bottom,m_rectMax.right);
		*/
	}
	CWnd * pCtrl = GetParent()->GetDlgItem(nCtrlId);
	if(pCtrl != NULL)
	{
		CRect rect;
		pCtrl->GetWindowRect(&rect);
		GetParent()->ClientToScreen(rect);
		if((m_nType == CS_HORZ) &&(m_rectMax.bottom > rect.bottom))
		{
			// 手动修正最下面边界
			m_rectMax.bottom = rect.bottom - 70;
		}
		if((m_nType == CS_VERT) &&(m_rectMax.right > rect.right))
		{
			m_rectMax.right = rect.right;
		}
	}
	/*
	TRACE3("M-%d(%d,%d,",nCtrlId,m_rectMax.top,m_rectMax.left);
	TRACE2("%d,%d)\n",m_rectMax.bottom,m_rectMax.right);
	*/
}

// 设置最下边界
void CSplitter::SetBottomLimit(UINT nCtrlId)
{
	if (m_nType == CS_HORZ){
			m_rectMax.bottom += nCtrlId;
	}
}