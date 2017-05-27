/*
 *
 *	ChartBalloonLabel.cpp
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

#include "ChartCtrl.h"
#include "ChartSerie.h"
#include "ChartBalloonLabel.h"

template<class PointType>
CChartBalloonLabel<PointType>::CChartBalloonLabel(CChartCtrl* pParentCtrl, 
												  CChartSerieBase<PointType>* pParentSeries)
  : CChartLabel(pParentCtrl, pParentSeries), m_bRoundedRect(true)
{
	m_colBackground = RGB(255,255,225);
	m_colLine = RGB(255,255,255);
	m_colBorder = RGB(0,0,0);
}

template<class PointType>
CChartBalloonLabel<PointType>::~CChartBalloonLabel()
{
}

template<class PointType>
void CChartBalloonLabel<PointType>::SetBackgroundColor(COLORREF colBackground)  
{ 
	m_colBackground = colBackground; 
	m_pParentCtrl->RefreshCtrl();
}

template<class PointType>
void CChartBalloonLabel<PointType>::SetLineColor(COLORREF colArrow)			 
{ 
	m_colLine = colArrow; 
	m_pParentCtrl->RefreshCtrl();
}

template<class PointType>
void CChartBalloonLabel<PointType>::SetBorderColor(COLORREF colBorder)			 
{ 
	m_colBorder = colBorder; 
	m_pParentCtrl->RefreshCtrl();
}

template<class PointType>
void CChartBalloonLabel<PointType>::SetRoundedRect(bool bRounded)  
{ 
	m_bRoundedRect = bRounded; 
	m_pParentCtrl->RefreshCtrl();
}

template<class PointType>
void CChartBalloonLabel<PointType>::SetFont(int nPointSize, const TChartString& strFaceName)
{
	m_Font.SetFont(strFaceName, nPointSize);
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

template<class PointType>
void CChartBalloonLabel<PointType>::SetFont(const CChartFont& newFont)
{
	m_Font = newFont;
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

template<class PointType>
void CChartBalloonLabel<PointType>::Draw(CDC* pDC, unsigned uPointIndex)
{
	if (m_pLabelProvider)
	{
		PointType Point = m_pParentSeries->GetPoint(uPointIndex);
		m_strLabelText = m_pLabelProvider->GetText(m_pParentSeries, uPointIndex);
	}
	if (m_strLabelText == _T(""))
		return;

	CPoint screenPt = m_pParentSeries->GetPointScreenCoord(uPointIndex);

	// Create the pen for the arrow
	CPen newPen(PS_SOLID, 1, m_colLine);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	// Draw first the arrow
	pDC->MoveTo(screenPt);
	pDC->LineTo(screenPt.x,screenPt.y-20);

	// Create and select a new pen for the border
	newPen.DeleteObject();
	newPen.CreatePen(PS_SOLID, 1, m_colBorder);
	pDC->SelectObject(&newPen);
	m_Font.SelectFont(pDC);

	// Create the brush to fill the rectangle
	CBrush newBrush(m_colBackground);
	CBrush* pOldBrush = pDC->SelectObject(&newBrush);

	// Calculate the size of the 
	CSize labelSize = pDC->GetTextExtent(m_strLabelText.c_str());
	labelSize += CSize(10,10);
	int x = screenPt.x;
	int y = screenPt.y;
	CRect labelRect(CPoint(x-labelSize.cx/2,y-19-labelSize.cy),labelSize);

	// Draw the rectangle
	if (m_bRoundedRect)
		pDC->RoundRect(labelRect,CPoint(10,10));
	else
		pDC->Rectangle(labelRect);

	// Draw the text
	pDC->TextOut(labelRect.left+5,labelRect.top+5,m_strLabelText.c_str());

	// Clean the objects
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	newPen.DeleteObject();
	newBrush.DeleteObject();
	m_Font.UnselectFont(pDC);
}