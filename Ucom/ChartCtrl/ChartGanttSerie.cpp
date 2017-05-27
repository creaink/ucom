/*
 *
 *	ChartGanttSerie.cpp
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
#include "ChartGanttSerie.h"
#include "ChartCtrl.h"

CChartGanttSerie::CChartGanttSerie(CChartCtrl* pParent) 
  : CChartSerieBase<SChartGanttPoint>(pParent), m_iBarWidth(10), m_iBorderWidth(1),
    m_BorderColor(RGB(0,0,0)), m_bGradient(true), 
    m_GradientColor(RGB(255,255,255)), m_GradientType(gtHorizontalDouble)
{
	m_bShadow = true;
	this->SetSeriesOrdering(poYOrdering);
}

CChartGanttSerie::~CChartGanttSerie()
{
}

void CChartGanttSerie::AddPoint(double StartTime, double EndTime, double YValue)
{
	SChartGanttPoint newPoint(StartTime, EndTime, YValue);
	CChartSerieBase<SChartGanttPoint>::AddPoint(newPoint);
}

bool CChartGanttSerie::IsPointOnSerie(const CPoint& screenPoint, 
											unsigned& uIndex) const
{
	uIndex = INVALID_POINT;
	if (!m_bIsVisible)
		return false;

	unsigned uFirst=0, uLast=0;
	if (!GetVisiblePoints(uFirst,uLast))
		return false;
	if (uFirst>0)
		uFirst--;
	if (uLast<GetPointsCount()-1)
		uLast++;

	bool bResult = false;
	for (unsigned i=uFirst ; i <= uLast ; i++)
	{
		CRect BarRect = GetBarRectangle(i);
		if (BarRect.PtInRect(screenPoint))
		{
			bResult = true;
			uIndex = i;
			break;
		}
	}
	return bResult; 
}

void CChartGanttSerie::DrawLegend(CDC* pDC, const CRect& rectBitmap) const
{
	if (m_strSerieName == _T(""))
		return;
	if (!pDC->GetSafeHdc())
		return;

	//Draw bar:
	CBrush BorderBrush(m_BorderColor);
	pDC->FillRect(rectBitmap,&BorderBrush);

	CRect FillRect(rectBitmap);
	CBrush FillBrush(m_SerieColor);
	FillRect.DeflateRect(m_iBorderWidth,m_iBorderWidth);
	if (m_bGradient)
	{
		CChartGradient::DrawGradient(pDC,FillRect,m_SerieColor,m_GradientColor,m_GradientType);
	}
	else
	{
		pDC->FillRect(FillRect,&FillBrush);
	}
}

void CChartGanttSerie::Draw(CDC* pDC)
{
	if (!m_bIsVisible)
		return;
	if (!pDC->GetSafeHdc())
		return;

	unsigned uFirst=0, uLast=0;
	if (!GetVisiblePoints(uFirst,uLast))
		return;

	CRect TempClipRect(m_PlottingRect);
	TempClipRect.DeflateRect(1,1);
	pDC->SetBkMode(TRANSPARENT);
	pDC->IntersectClipRect(TempClipRect);

	CBrush BorderBrush(m_BorderColor);
	CBrush FillBrush(m_SerieColor);
	//Draw all points that haven't been drawn yet
	for (m_uLastDrawnPoint;m_uLastDrawnPoint<(int)GetPointsCount();m_uLastDrawnPoint++)
	{
		CRect BarRect = GetBarRectangle(m_uLastDrawnPoint);
		DrawBar(pDC, &FillBrush, &BorderBrush, BarRect);
	}

	pDC->SelectClipRgn(NULL);
	DeleteObject(BorderBrush);
	DeleteObject(FillBrush);
}

void CChartGanttSerie::DrawAll(CDC *pDC)
{
	if (!m_bIsVisible)
		return;
	if (!pDC->GetSafeHdc())
		return;

	unsigned uFirst=0, uLast=0;
	if (!GetVisiblePoints(uFirst,uLast))
		return;

	CRect TempClipRect(m_PlottingRect);
	TempClipRect.DeflateRect(1,1);
	if (uFirst>0)
		uFirst--;
	if (uLast<GetPointsCount()-1)
		uLast++;

	pDC->SetBkMode(TRANSPARENT);
	pDC->IntersectClipRect(TempClipRect);

	CBrush BorderBrush(m_BorderColor);
	CBrush FillBrush(m_SerieColor);
	for (m_uLastDrawnPoint=uFirst;m_uLastDrawnPoint<=uLast;m_uLastDrawnPoint++)
	{
		CRect BarRect = GetBarRectangle(m_uLastDrawnPoint);
		DrawBar(pDC, &FillBrush, &BorderBrush, BarRect);
	}

	pDC->SelectClipRgn(NULL);
	DeleteObject(BorderBrush);
	DeleteObject(FillBrush);
}

void CChartGanttSerie::SetBorderColor(COLORREF BorderColor)  
{ 
	m_BorderColor = BorderColor; 
	m_pParentCtrl->RefreshCtrl();
}
void CChartGanttSerie::SetBorderWidth(int Width)  
{ 
	m_iBorderWidth = Width; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartGanttSerie::SetBarWidth(int Width)		
{ 
	m_iBarWidth = Width;
	m_pParentCtrl->RefreshCtrl();
}

void CChartGanttSerie::ShowGradient(bool bShow)		
{ 
	m_bGradient = bShow; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartGanttSerie::SetGradient(COLORREF GradientColor, EGradientType GradientType)
{
	m_GradientColor = GradientColor;
	m_GradientType = GradientType;
	m_pParentCtrl->RefreshCtrl();
}

CRect CChartGanttSerie::GetBarRectangle(unsigned uPointIndex) const
{
	SChartGanttPoint point = GetPoint(uPointIndex);
	int PointXStart = m_pHorizontalAxis->ValueToScreen(point.StartTime);
	int PointXEnd = m_pHorizontalAxis->ValueToScreen(point.EndTime);
	int YVal = m_pVerticalAxis->ValueToScreen(point.YValue);
	int PointYStart = YVal - m_iBarWidth;
	int PointYEnd = YVal + m_iBarWidth;
	CRect PointRect(min(PointXStart, PointXEnd), min(PointYStart, PointYEnd),
					max(PointXStart, PointXEnd), max(PointYStart, PointYEnd) );
	return PointRect;
}

void CChartGanttSerie::DrawBar(CDC* pDC, CBrush* pFillBrush, CBrush* pBorderBrush, 
							   CRect BarRect)
{
	if (m_bShadow)
	{
		CBrush ShadowBrush(m_ShadowColor);
		CRect ShadowRect(BarRect);
		ShadowRect.OffsetRect(m_iShadowDepth,m_iShadowDepth);
		pDC->FillRect(ShadowRect,&ShadowBrush);
	}
	pDC->FillRect(BarRect,pBorderBrush);

	CRect FillRect(BarRect);
	FillRect.DeflateRect(m_iBorderWidth,m_iBorderWidth);
	if (m_bGradient)
	{
		CChartGradient::DrawGradient(pDC,FillRect,m_SerieColor,m_GradientColor,
									 m_GradientType);
	}
	else
	{
		pDC->FillRect(FillRect,pFillBrush);
	}
}