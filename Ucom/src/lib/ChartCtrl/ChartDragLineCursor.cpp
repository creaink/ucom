/*
 *
 *	ChartDragLineCursor.cpp
 *
 *	Written by Cédric Moonen (cedric_moonen@hotmail.com)
 *
 *
 *
 *	This code may be used for any non-commercial and commercial purposes in a compiled form.
 *	The code may be redistributed as long as it remains unmodified and providing that the 
 *	author name and this disclaimer remain intact. The sources can be modified WITH the author 
 *	consent only.
 *	
 *	This code is provided without any garanties. I cannot be held responsible for the damage or
 *	the loss of time it causes. Use it at your own risks
 *
 *	An e-mail to notify me that you are using this code is appreciated also.
 *
 *
 */

#include "stdafx.h"
#include "ChartDragLineCursor.h"
#include "ChartCtrl.h"

CChartDragLineCursor::CChartDragLineCursor(CChartCtrl* pParent,
										   CChartAxis* pRelatedAxis)
  : CChartCursor(pParent), m_pRelatedAxis(pRelatedAxis), m_lPosition(0),
	m_bDragged(false)
{
}

CChartDragLineCursor::~CChartDragLineCursor()
{
}

void CChartDragLineCursor::Draw(CDC* pDC)
{
	CPen NewPen(PS_SOLID,1,m_colCursor);
	CPen* pOldPen = pDC->SelectObject(&NewPen);

	CRect plottingRect = m_pParentCtrl->GetPlottingRect();

	if (m_pRelatedAxis->IsHorizontal())
	{
		pDC->MoveTo(m_lPosition, plottingRect.top);
		pDC->LineTo(m_lPosition, plottingRect.bottom);
	}
	else 
	{
		pDC->MoveTo(plottingRect.left, m_lPosition);
		pDC->LineTo(plottingRect.right, m_lPosition);
	}

	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

void CChartDragLineCursor::OnMouseButtonDown(CPoint mousePoint)  
{ 
	long position = 0;
	if (m_pRelatedAxis->IsHorizontal())
		position = mousePoint.x;
	else
		position = mousePoint.y;

	if ( (position >= m_lPosition-3) && 
		 (position <= m_lPosition+3) )
	{
		m_bDragged = true;
	}
}

void CChartDragLineCursor::OnMouseButtonUp(CPoint /*mousePoint*/)    
{ 
	m_bDragged = false;
}

void CChartDragLineCursor::OnMouseMove(CPoint mousePoint)
{
	if (!m_bDragged)
		return;

	double XVal = 0;
	double YVal = 0;
	if (m_pRelatedAxis->IsHorizontal())
	{
		m_lPosition = mousePoint.x;
		XVal = m_pRelatedAxis->ScreenToValue(m_lPosition);
	}
	else
	{
		m_lPosition = mousePoint.y;
		YVal = m_pRelatedAxis->ScreenToValue(m_lPosition);
	}

	CursorMoved(XVal, YVal);
}

void CChartDragLineCursor::SetPosition(double dPosition)
{
	m_lPosition = m_pRelatedAxis->ValueToScreen(dPosition);
}
