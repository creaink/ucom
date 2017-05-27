/*
 *
 *	ChartCrossHairCursor.cpp
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
#include "ChartCrossHairCursor.h"
#include "ChartCtrl.h"

CChartCrossHairCursor::CChartCrossHairCursor(CChartCtrl* pParent,
											 CChartAxis* pHorizAxis, 
											 CChartAxis* pVertAxis)
  : CChartCursor(pParent), m_pHorizontalAxis(pHorizAxis), m_pVerticalAxis(pVertAxis),
    m_lXPos(0), m_lYPos(0)
{
}

CChartCrossHairCursor::~CChartCrossHairCursor()
{
}

void CChartCrossHairCursor::Draw(CDC* pDC)
{
	CPen NewPen(PS_SOLID,1,m_colCursor);
	CPen* pOldPen = pDC->SelectObject(&NewPen);

	CRect plottingRect = m_pParentCtrl->GetPlottingRect();

	pDC->MoveTo(m_lXPos, plottingRect.top);
	pDC->LineTo(m_lXPos, plottingRect.bottom);
	pDC->MoveTo(plottingRect.left, m_lYPos);
	pDC->LineTo(plottingRect.right, m_lYPos);

	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

void CChartCrossHairCursor::OnMouseMove(CPoint mousePoint)
{
	m_lXPos = mousePoint.x;
	m_lYPos = mousePoint.y;

	double XVal = m_pHorizontalAxis->ScreenToValue(m_lXPos);
	double YVal = m_pVerticalAxis->ScreenToValue(m_lYPos);
	CursorMoved(XVal, YVal);
}
