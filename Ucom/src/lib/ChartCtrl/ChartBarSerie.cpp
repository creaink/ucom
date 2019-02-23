/*
 *
 *	ChartBarSerie.cpp
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
#include "ChartBarSerie.h"
#include "ChartCtrl.h"
#include <algorithm>

using namespace std;

int CChartBarSerie::m_iInterSpace = 0;
std::list<CChartBarSerie*> CChartBarSerie::m_lstBarSeries;

CChartBarSerie::CChartBarSerie(CChartCtrl* pParent) 
 : CChartXYSerie(pParent), m_iBarWidth(20), m_iBorderWidth(1),
   m_BorderColor(RGB(0,0,0)), m_uGroupId(0), m_bGradient(true), 
   m_GradientColor(RGB(255,255,255)), m_GradientType(gtHorizontalDouble),
   m_bHorizontal(false), m_dBaseLine(0), m_bAutoBaseLine(true), m_bStacked(false)
{
	m_lstBarSeries.push_back(this);
	m_iShadowDepth = 3;
	m_bShadow = false;
}

CChartBarSerie::~CChartBarSerie()
{
	TBarSeriesList::iterator iter = m_lstBarSeries.begin();
	while (iter!=m_lstBarSeries.end())
	{
		if ((*iter) == this)
			iter = m_lstBarSeries.erase(iter);
		else
			iter++;
	}
}

void CChartBarSerie::SetHorizontal(bool bHorizontal)  
{ 
	m_bHorizontal = bHorizontal; 
	if (m_bHorizontal)
		SetSeriesOrdering(poYOrdering);
	else
		SetSeriesOrdering(poXOrdering);
	m_pParentCtrl->RefreshCtrl();
}

void CChartBarSerie::SetBorderColor(COLORREF BorderColor)  
{ 
	m_BorderColor = BorderColor; 
	m_pParentCtrl->RefreshCtrl();
}
void CChartBarSerie::SetBorderWidth(int Width)  
{ 
	m_iBorderWidth = Width; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartBarSerie::SetBarWidth(int Width)		
{ 
	m_iBarWidth = Width;
	m_pParentCtrl->RefreshCtrl();
}

void CChartBarSerie::SetGroupId(unsigned GroupId)	
{ 
	m_uGroupId = GroupId; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartBarSerie::ShowGradient(bool bShow)		
{ 
	m_bGradient = bShow; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartBarSerie::SetGradient(COLORREF GradientColor, EGradientType GradientType)
{
	m_GradientColor = GradientColor;
	m_GradientType = GradientType;
	m_pParentCtrl->RefreshCtrl();
}

void CChartBarSerie::SetStacked(bool bStacked)
{
	m_bStacked = bStacked;
	m_pParentCtrl->RefreshCtrl();
}

bool CChartBarSerie::IsStacked()
{
	return m_bStacked;
}

void CChartBarSerie::DrawBar(CDC* pDC, CBrush* pFillBrush, CBrush* pBorderBrush, 
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

void CChartBarSerie::Draw(CDC* pDC)
{
	if (!m_bIsVisible)
		return;
	if (!pDC->GetSafeHdc())
		return;

	RefreshStackedCache();
	int iMinorOffset = GetMinorOffset();

	CRect TempClipRect(m_PlottingRect);
	TempClipRect.DeflateRect(1,1);
	pDC->SetBkMode(TRANSPARENT);
	pDC->IntersectClipRect(TempClipRect);

	CBrush BorderBrush(m_BorderColor);
	CBrush FillBrush(m_SerieColor);
	//Draw all points that haven't been drawn yet
	for (m_uLastDrawnPoint;m_uLastDrawnPoint<(int)GetPointsCount();m_uLastDrawnPoint++)
	{
		CRect BarRect = GetBarRectangle(m_uLastDrawnPoint,iMinorOffset);
		DrawBar(pDC, &FillBrush, &BorderBrush, BarRect);
	}

	pDC->SelectClipRgn(NULL);
	DeleteObject(BorderBrush);
	DeleteObject(FillBrush);
}

void CChartBarSerie::DrawAll(CDC *pDC)
{
	if (!m_bIsVisible)
		return;
	if (!pDC->GetSafeHdc())
		return;

	RefreshStackedCache();
	int iMinorOffset = GetMinorOffset();

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
		CRect BarRect = GetBarRectangle(m_uLastDrawnPoint,iMinorOffset);
		DrawBar(pDC, &FillBrush, &BorderBrush, BarRect);
	}

	pDC->SelectClipRgn(NULL);
	DeleteObject(BorderBrush);
	DeleteObject(FillBrush);
}

void CChartBarSerie::DrawLegend(CDC* pDC, const CRect& rectBitmap) const
{
	if (m_strSerieName== _T(""))
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

bool CChartBarSerie::IsPointOnSerie(const CPoint& screenPoint, unsigned& uIndex) const 
{ 
	uIndex = INVALID_POINT;
	if (!m_bIsVisible)
		return false;

	RefreshStackedCache();
	int iMinorOffset = GetMinorOffset();
	
	unsigned uFirst=0, uLast=0;
	if (!GetVisiblePoints(uFirst,uLast))
		return false;

	bool bResult = false;
	for (unsigned i=uFirst ; i < uLast ; i++)
	{
		CRect BarRect = GetBarRectangle(i,iMinorOffset);
		if (BarRect.PtInRect(screenPoint))
		{
			bResult = true;
			uIndex = i;
			break;
		}
	}
	return bResult; 
}

int CChartBarSerie::GetMinorOffset() const
{
	int iOffset = 0;
	int iTotalWidth = 0;
	int iLargestStacked = 0;
	bool bFound = false;

	TBarSeriesList::const_iterator iter = m_lstBarSeries.begin();
	for (iter; iter!=m_lstBarSeries.end(); iter++)
	{
		// Skip the series with a different group Id
		if ((*iter)->GetGroupId() != m_uGroupId)
			continue;

		if ((*iter)->IsStacked() )
		{
			if ((*iter)->GetBarWidth() > iLargestStacked)
				iLargestStacked = (*iter)->GetBarWidth();
		} 
		else
		{
			if (!bFound)
			{
				if ((*iter)==this)
					bFound = true;
				else 
					iOffset += (*iter)->GetBarWidth() + m_iInterSpace;
			}
			iTotalWidth += (*iter)->GetBarWidth() + m_iInterSpace;
		}
	}
	// Remove the interspace because it has been counted once too much.
	iTotalWidth -= m_iInterSpace;

	if (iLargestStacked > 0)
		iTotalWidth += iLargestStacked + m_iInterSpace;

	// If this series is a stacked series, it is put at the start
	if (m_bStacked)
		iOffset = 0;
	else if (iLargestStacked > 0)
	{
		iOffset += iLargestStacked + m_iInterSpace;
	}
	return (iOffset - iTotalWidth/2);  
}

CRect CChartBarSerie::GetBarRectangle(unsigned uPointIndex, int minorOffset) const
{
	SChartXYPoint point = GetPoint(uPointIndex);
	TBarSeriesList::const_iterator iter = m_lstStackedSeries.begin();

	bool bCalcOffset = false;
	if (m_bStacked && (*iter)!=this)
		bCalcOffset = true;

	double dXOffset = 0;
	double dYOffset = 0;
	if (bCalcOffset)
	{
		for (iter; iter!=m_lstStackedSeries.end(); iter++)
		{
			if ( (*iter)->GetPointsCount() <= uPointIndex)
				continue;

			if ( (*iter) == this)
				break;

			dXOffset += (*iter)->GetPoint(uPointIndex).GetX();
			dYOffset += (*iter)->GetPoint(uPointIndex).GetY();
		}
	}

	int barXStart, barXEnd, barYStart, barYEnd;
	if (m_bHorizontal)
	{
		if (bCalcOffset)
		{
			barXStart = m_pHorizontalAxis->ValueToScreen(dXOffset);
			barXEnd = m_pHorizontalAxis->ValueToScreen(dXOffset+point.X);
		}
		else 
		{
			barXEnd = m_pHorizontalAxis->ValueToScreen(point.X);
			if (!m_bAutoBaseLine)
				barXStart = m_pHorizontalAxis->ValueToScreen(m_dBaseLine);
			else
			{
				double Position = m_pVerticalAxis->GetPosition()/100.00;
				barXStart = m_PlottingRect.left + (int)(Position * (m_PlottingRect.right-m_PlottingRect.left));
			}
		}
		barYStart = m_pVerticalAxis->ValueToScreen(point.Y) + minorOffset;
		barYEnd = m_pVerticalAxis->ValueToScreen(point.Y) + minorOffset + m_iBarWidth;
	}
	else
	{
		if (bCalcOffset)
		{
			barYStart = m_pVerticalAxis->ValueToScreen(dYOffset);
			barYEnd = m_pVerticalAxis->ValueToScreen(dYOffset+point.Y);
		}
		else 
		{
			barYEnd = m_pVerticalAxis->ValueToScreen(point.Y);
			if (!m_bAutoBaseLine)
				barYStart = m_pVerticalAxis->ValueToScreen(m_dBaseLine);
			else
			{
				double Position = m_pHorizontalAxis->GetPosition()/100.00;
				barYStart = m_PlottingRect.left + (int)(Position * (m_PlottingRect.bottom-m_PlottingRect.top));
			}
		}
		barXStart = m_pHorizontalAxis->ValueToScreen(point.X) + minorOffset;
		barXEnd = m_pHorizontalAxis->ValueToScreen(point.X) + minorOffset + m_iBarWidth;
	}
	
	CRect barRect(min(barXStart, barXEnd), min(barYStart, barYEnd),
				  max(barXStart, barXEnd), max(barYStart, barYEnd) );
	return barRect;
}

void CChartBarSerie::RefreshStackedCache() const
{
	m_lstStackedSeries.clear();
	if (!m_bStacked)
		return;

	TBarSeriesList::const_iterator iter = m_lstBarSeries.begin();
	for (iter; iter!=m_lstBarSeries.end(); iter++)
	{
		// Skip the series with a different group Id
		if ((*iter)->GetGroupId() != m_uGroupId)
			continue;

		if ((*iter)->IsStacked() )
		{
			m_lstStackedSeries.push_back(*iter);
		}
	}
}
