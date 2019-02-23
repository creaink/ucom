/*
 *
 *	ChartSurfaceSerie.cpp
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
#include "ChartSurfaceSerie.h"
#include "ChartCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartSurfaceSerie::CChartSurfaceSerie(CChartCtrl* pParent) 
 : CChartXYSerie(pParent), m_FillStyle(fsHatchDownDiag), m_bHorizontal(true)
{

}

CChartSurfaceSerie::~CChartSurfaceSerie()
{

}

void CChartSurfaceSerie::Draw(CDC* pDC)
{
	DrawAll(pDC);
}

void CChartSurfaceSerie::DrawAll(CDC* pDC)
{
	unsigned uFirst=0, uLast=0;
	if (!GetVisiblePoints(uFirst,uLast))
		return;

	if (uFirst>0)
		uFirst--;
	if (uLast<GetPointsCount()-1)
		uLast++;
	unsigned uCount = uLast - uFirst + 1;
	CPoint* pPoints = new CPoint[uCount+2];

	CBrush NewBrush;
	if (m_FillStyle == fsSolid)
		NewBrush.CreateSolidBrush(m_SerieColor);
	else
	{
		int nIndex = 0;
		switch (m_FillStyle)
		{
		case fsHatchDownDiag:
			nIndex = HS_FDIAGONAL;
			break;
		case fsHatchUpDiag:
			nIndex = HS_BDIAGONAL;
			break;
		case fsHatchCross:
			nIndex = HS_CROSS;
			break;
		case fsHatchDiagCross:
			nIndex = HS_DIAGCROSS;
			break;
		case fsHatchHorizontal:
			nIndex = HS_HORIZONTAL;
			break;
		case fsHatchVertical:
			nIndex = HS_VERTICAL;
			break;
		}
		NewBrush.CreateHatchBrush(nIndex,m_SerieColor);
	}

	CBrush* pOldBrush = pDC->SelectObject(&NewBrush);

	for (unsigned index=uFirst; index<=uLast; index++)
	{
		SChartXYPoint Point = GetPoint(index);
		ValueToScreen(Point.X, Point.Y, pPoints[index-uFirst+1]);
	}

	if (m_bHorizontal)
	{
		pPoints[0].x = pPoints[1].x;
		pPoints[uCount+1].x = pPoints[uCount].x;

		double Position = m_pHorizontalAxis->GetPosition()/100.00;
		int AxisPos = m_PlottingRect.top + (int)(Position * (m_PlottingRect.bottom-m_PlottingRect.top));

		pPoints[0].y = AxisPos;
		pPoints[uCount+1].y = AxisPos;
	}
	else
	{
		pPoints[0].y = pPoints[1].y;
		pPoints[uCount+1].y = pPoints[uCount].y;

		double Position = m_pVerticalAxis->GetPosition()/100.00;
		int AxisPos = m_PlottingRect.left + (int)(Position * (m_PlottingRect.right-m_PlottingRect.left));

		pPoints[0].x = AxisPos;
		pPoints[uCount+1].x = AxisPos;
	}

	pDC->SetBkMode(TRANSPARENT);
	//To have lines limited in the drawing rectangle :
	CRect TempClipRect(m_PlottingRect);
	TempClipRect.DeflateRect(1,1);
	pDC->SetBkMode(TRANSPARENT);
	pDC->IntersectClipRect(TempClipRect);

	pDC->Polygon(pPoints,uCount+2);
	pDC->SelectClipRgn(NULL);
	pDC->SelectObject(pOldBrush);
	DeleteObject(NewBrush);

	delete[] pPoints;
}

void CChartSurfaceSerie::DrawLegend(CDC* pDC, const CRect& rectBitmap) const
{
	if (m_strSerieName== _T(""))
		return;

	// Draw the bitmap
	CBrush NewBrush;
	if (m_FillStyle == fsSolid)
		NewBrush.CreateSolidBrush(m_SerieColor);
	else
	{
		int nIndex = 0;
		switch (m_FillStyle)
		{
		case fsHatchDownDiag:
			nIndex = HS_FDIAGONAL;
			break;
		case fsHatchUpDiag:
			nIndex = HS_BDIAGONAL;
			break;
		case fsHatchCross:
			nIndex = HS_CROSS;
			break;
		case fsHatchDiagCross:
			nIndex = HS_DIAGCROSS;
			break;
		case fsHatchHorizontal:
			nIndex = HS_HORIZONTAL;
			break;
		case fsHatchVertical:
			nIndex = HS_VERTICAL;
			break;
		}
		NewBrush.CreateHatchBrush(nIndex,m_SerieColor);
	}

	CBrush* pOldBrush = pDC->SelectObject(&NewBrush);

	pDC->Rectangle(rectBitmap);

	pDC->SelectObject(pOldBrush);
	DeleteObject(NewBrush);
}

void CChartSurfaceSerie::SetFillStyle(FillStyle NewStyle)  
{ 
	m_FillStyle = NewStyle; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartSurfaceSerie::SetHorizontal(bool bHoriz) 
{ 
	m_bHorizontal = bHoriz; 
	if (m_bHorizontal)
		m_vPoints.SetOrdering(poXOrdering);
	else
		m_vPoints.SetOrdering(poYOrdering);
	m_pParentCtrl->RefreshCtrl();
}

void CChartSurfaceSerie::SetSeriesOrdering(PointsOrdering )
{
	TRACE("Can't change the series ordering of a surface series.");
}

bool CChartSurfaceSerie::IsPointOnSerie(const CPoint& screenPoint, 
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
	for (unsigned ptIndex=uFirst; ptIndex<uLast-1; ptIndex++)
	{
		SChartXYPoint SeriePoint1 = GetPoint(ptIndex);
		SChartXYPoint SeriePoint2 = GetPoint(ptIndex+1);
		CPoint point1;
		CPoint point2;
		ValueToScreen(SeriePoint1.X, SeriePoint1.Y, point1);
		ValueToScreen(SeriePoint2.X, SeriePoint2.Y, point2);

		double lineSlope = (1.0*(point2.y-point1.y))/(point2.x-point1.x);
		double lineOffset = (1.0*(point2.x*point1.y-point1.x*point2.y))/(point2.x-point1.x);

		if (m_bHorizontal)
		{
			if ( (screenPoint.x < point1.x) || (screenPoint.x > point2.x))
				continue;

			int Position = m_pHorizontalAxis->GetPosition();
			if ( (Position==100) && (screenPoint.y > (screenPoint.x *lineSlope + lineOffset)) )
				bResult = true;
			if ( (Position==0) && (screenPoint.y < (screenPoint.x *lineSlope + lineOffset)) )
				bResult = true;

			if (bResult)
			{
				// Check if the click is close to one of the two points.
				int xDist = abs(screenPoint.x - point1.x);
				int yDist = abs(screenPoint.y - point1.y);
				if (xDist<=5 && yDist<=5)
					uIndex = ptIndex;
				xDist = abs(screenPoint.x - point2.x);
				yDist = abs(screenPoint.y - point2.y);
				if (xDist<=5 && yDist<=5)
					uIndex = ptIndex+1;
				break;
			}
		}
		else
		{
			if ( (screenPoint.y > point1.y) || (screenPoint.y < point2.y))
				continue;

			int Position = m_pVerticalAxis->GetPosition();
			if ( (Position==0) && (screenPoint.x < (screenPoint.y-lineOffset)/lineSlope) ) 
				bResult = true;
			if ( (Position==100) && (screenPoint.x > (screenPoint.y-lineOffset)/lineSlope) )
				bResult = true;

			if (bResult)
			{
				// Check if the click is close to one of the two points.
				int xDist = abs(screenPoint.x - point1.x);
				int yDist = abs(screenPoint.y - point1.y);
				if (xDist<=5 && yDist<=5)
					uIndex = ptIndex;
				xDist = abs(screenPoint.x - point2.x);
				yDist = abs(screenPoint.y - point2.y);
				if (xDist<=5 && yDist<=5)
					uIndex = ptIndex+1;
				break;
			}
		}
	}

	return bResult; 
}
