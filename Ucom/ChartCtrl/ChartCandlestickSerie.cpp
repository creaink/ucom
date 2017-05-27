/*
 *
 *	ChartCandlestickSerie.cpp
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
#include "ChartCandlestickSerie.h"
#include "ChartCtrl.h"

CChartCandlestickSerie::CChartCandlestickSerie(CChartCtrl* pParent) 
  : CChartSerieBase<SChartCandlestickPoint>(pParent), m_iCandlestickWidth(7)
{
	m_bShadow = true;
}

CChartCandlestickSerie::~CChartCandlestickSerie()
{
}

void CChartCandlestickSerie::AddPoint(double XVal, 
									  double Low, 
									  double High, 
									  double Open, 
									  double Close)
{
	SChartCandlestickPoint NewPoint(XVal, Low, High, Open, Close);
	CChartSerieBase<SChartCandlestickPoint>::AddPoint(NewPoint);
}

void CChartCandlestickSerie::SetWidth(int Width)
{
	m_iCandlestickWidth = Width; 
	m_pParentCtrl->RefreshCtrl();
}

bool CChartCandlestickSerie::IsPointOnSerie(const CPoint& screenPoint, 
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
	if (uLast<GetPointsCount())
		uLast++;

	bool bResult = false;
	for (unsigned i=uFirst ; i < uLast ; i++)
	{
		SChartCandlestickPoint Point = GetPoint(i);
		int ScreenXVal = m_pHorizontalAxis->ValueToScreen(Point.XVal);
		int ScreenLow = m_pVerticalAxis->ValueToScreen(Point.Low);
		int ScreenHigh = m_pVerticalAxis->ValueToScreen(Point.High);

		int halfWidth = m_iCandlestickWidth/2;
		CRect Rectangle;
		if (ScreenLow > ScreenHigh) 
			Rectangle.SetRect(ScreenXVal-halfWidth, ScreenHigh,
			ScreenXVal+halfWidth+1, ScreenLow+1);
		else 
			Rectangle.SetRect(ScreenXVal-halfWidth, ScreenLow,
			ScreenXVal+halfWidth+1, ScreenHigh+1);
		if (Rectangle.PtInRect(screenPoint))
		{
			bResult = true;
			uIndex = i;
			break;
		}
	}
	return bResult; 
}

void CChartCandlestickSerie::DrawLegend(CDC* pDC, const CRect& rectBitmap) const
{
	if (m_strSerieName == _T(""))
		return;
	if (!pDC->GetSafeHdc())
		return;

	NewPen.CreatePen(PS_SOLID,1,m_SerieColor);
	BrushFill.CreateSolidBrush(m_SerieColor);
	CBrush* pOldBrush = pDC->SelectObject(&BrushFill);
	CPen* pOldPen = pDC->SelectObject(&NewPen);
	int OldBkMode = pDC->SetBkMode(TRANSPARENT);

	int margin = (rectBitmap.Width() - m_iCandlestickWidth)/2;
	CRect CandleRect(rectBitmap.left+margin, rectBitmap.top+4,
					 rectBitmap.right-margin+1, rectBitmap.bottom-3);
	pDC->Rectangle(CandleRect);
	int MiddleX = rectBitmap.left + rectBitmap.Width()/2;
	pDC->MoveTo(MiddleX, CandleRect.top);
	pDC->LineTo(MiddleX, rectBitmap.top);
	pDC->MoveTo(MiddleX, CandleRect.bottom);
	pDC->LineTo(MiddleX, rectBitmap.bottom);

	pDC->SetBkMode(OldBkMode);
	pDC->SelectClipRgn(NULL);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	BrushFill.DeleteObject();
	NewPen.DeleteObject();
}

void CChartCandlestickSerie::Draw(CDC* pDC)
{
	if (!m_bIsVisible)
		return;

	unsigned uFirst=0, uLast=0;
	if (!GetVisiblePoints(uFirst,uLast))
		return;

	if (pDC->GetSafeHdc())
	{
		ShadowBrush.CreateSolidBrush(m_ShadowColor);
		NewPen.CreatePen(PS_SOLID,1,m_SerieColor);
		ShadowPen.CreatePen(PS_SOLID,1,m_ShadowColor);
		BrushFill.CreateSolidBrush(m_SerieColor);
		BrushEmpty.CreateSolidBrush(RGB(255,255,255));

		int OldBkMode = pDC->SetBkMode(TRANSPARENT);
		//To have lines limited in the drawing rectangle :
		pDC->IntersectClipRect(m_PlottingRect);

		for (m_uLastDrawnPoint;m_uLastDrawnPoint<GetPointsCount();m_uLastDrawnPoint++)
		{
			SChartCandlestickPoint Point = GetPoint(m_uLastDrawnPoint);
			DrawCandleStick(pDC, Point);
		}

		pDC->SetBkMode(OldBkMode);
		pDC->SelectClipRgn(NULL);
		BrushFill.DeleteObject();
		BrushEmpty.DeleteObject();
		NewPen.DeleteObject();
		ShadowBrush.DeleteObject();
		ShadowPen.DeleteObject();
	}
}

void CChartCandlestickSerie::DrawAll(CDC *pDC)
{
	if (!m_bIsVisible)
		return;

	unsigned uFirst=0, uLast=0;
	if (!GetVisiblePoints(uFirst,uLast))
		return;

	if (pDC->GetSafeHdc())
	{
		ShadowBrush.CreateSolidBrush(m_ShadowColor);
		NewPen.CreatePen(PS_SOLID,1,m_SerieColor);
		ShadowPen.CreatePen(PS_SOLID,1,m_ShadowColor);
		BrushFill.CreateSolidBrush(m_SerieColor);
		BrushEmpty.CreateSolidBrush(RGB(255,255,255));

		pDC->SetBkMode(TRANSPARENT);
		//To have lines limited in the drawing rectangle :
		pDC->IntersectClipRect(m_PlottingRect);

		for (m_uLastDrawnPoint=uFirst;m_uLastDrawnPoint<=uLast;m_uLastDrawnPoint++)
		{
			SChartCandlestickPoint Point = GetPoint(m_uLastDrawnPoint);
			DrawCandleStick(pDC, Point);
		}

		pDC->SelectClipRgn(NULL);
		BrushFill.DeleteObject();
		BrushEmpty.DeleteObject();
		NewPen.DeleteObject();
		ShadowBrush.DeleteObject();
		ShadowPen.DeleteObject();
	}
}

void CChartCandlestickSerie::DrawCandleStick(CDC* pDC, SChartCandlestickPoint Point)
{
	int ScreenXVal = m_pHorizontalAxis->ValueToScreen(Point.XVal);
	int ScreenOpen = m_pVerticalAxis->ValueToScreen(Point.Open);
	int ScreenClose = m_pVerticalAxis->ValueToScreen(Point.Close);
	int ScreenLow = m_pVerticalAxis->ValueToScreen(Point.Low);
	int ScreenHigh = m_pVerticalAxis->ValueToScreen(Point.High);

	int halfWidth = m_iCandlestickWidth/2;
	CPoint TopLeft, BottomRight;
	CRect Body;
	CRect ShadowBody;
	if (ScreenOpen > ScreenClose) 
		Body.SetRect(ScreenXVal-halfWidth, ScreenClose,
					 ScreenXVal+halfWidth+1, ScreenOpen+1);
	else 
		Body.SetRect(ScreenXVal-halfWidth, ScreenOpen,
					 ScreenXVal+halfWidth+1, ScreenClose+1);

	ShadowBody = Body;
	ShadowBody.OffsetRect(m_iShadowDepth, m_iShadowDepth);

	CBrush* pOldBrush = pDC->SelectObject(&ShadowBrush);
	CPen* pOldPen = pDC->SelectObject(&ShadowPen);

	if (m_bShadow) 
		pDC->Rectangle(ShadowBody);
	if (Point.Open > Point.Close)
		pDC->SelectObject(&BrushFill);
	else
		pDC->SelectObject(&BrushEmpty);

	if (m_bShadow) 
	{
		pDC->MoveTo(ScreenXVal+m_iShadowDepth, ScreenOpen+m_iShadowDepth);
		pDC->LineTo(ScreenXVal+m_iShadowDepth, ScreenHigh+m_iShadowDepth);		
		pDC->MoveTo(ScreenXVal+m_iShadowDepth, ScreenClose+m_iShadowDepth);
		pDC->LineTo(ScreenXVal+m_iShadowDepth, ScreenLow+m_iShadowDepth);	
	}
	pDC->SelectObject(&NewPen);
	pDC->MoveTo(ScreenXVal, ScreenOpen);	pDC->LineTo(ScreenXVal, ScreenHigh);
	pDC->MoveTo(ScreenXVal, ScreenClose);	pDC->LineTo(ScreenXVal, ScreenLow);

	pDC->Rectangle(Body);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}