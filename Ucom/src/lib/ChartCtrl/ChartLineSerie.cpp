/*
 *
 *	ChartLineSerie.cpp
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
 *	History:
 *		- 25/03/2008: Line series with a width > 1 can now have a style other than solid
 *					  (thanks to Bruno Lavier).
 *		- 12/08/2008: Performance fix: pen use the PS_GEOMETRIC style only when necessary
 *					  (thanks to Nick Holgate).
 *
 *
 */

#include "stdafx.h"
#include "ChartLineSerie.h"
#include "ChartCtrl.h"

#include "Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartLineSerie::CChartLineSerie(CChartCtrl* pParent) : CChartXYSerie(pParent)
{
	m_iLineWidth = 1;
	m_iPenStyle = PS_SOLID;
	m_bSmooth = false;
	m_bShadow = false;
}

CChartLineSerie::~CChartLineSerie()
{

}

void CChartLineSerie::SetPenStyle(int NewStyle)  
{ 
	m_iPenStyle = NewStyle; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartLineSerie::SetWidth(int PenWidth)  
{ 
	m_iLineWidth = PenWidth; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartLineSerie::SetSmooth(bool bSmooth)
{
	m_bSmooth = bSmooth; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartLineSerie::DrawAll(CDC *pDC)
{
	if (!m_bIsVisible)
		return;
	if (!pDC->GetSafeHdc())
		return;

	unsigned uFirst=0, uLast=0;
	if (!GetVisiblePoints(uFirst,uLast))
		return;

	if (uFirst>0)
		uFirst--;
	if (uLast<GetPointsCount()-1)
		uLast++;
	if (uLast-uFirst < 1)
		return;

	CPen NewPen;
	CPen ShadowPen;
	if (m_iPenStyle != PS_SOLID)
	{
		LOGBRUSH lb;
		lb.lbStyle = BS_SOLID;
		lb.lbColor = m_SerieColor;
		NewPen.CreatePen(PS_GEOMETRIC | m_iPenStyle, m_iLineWidth, &lb);
		lb.lbColor = m_ShadowColor;
		ShadowPen.CreatePen(PS_GEOMETRIC | m_iPenStyle, m_iLineWidth, &lb);
	}
	else
	{
		NewPen.CreatePen(m_iPenStyle, m_iLineWidth, m_SerieColor);
		ShadowPen.CreatePen(m_iPenStyle, m_iLineWidth, m_ShadowColor);
	}
	CPen* pOldPen;

	pDC->SetBkMode(TRANSPARENT);
	//To have lines limited in the drawing rectangle :
	pDC->IntersectClipRect(m_PlottingRect);
	pOldPen = pDC->SelectObject(&NewPen);

	if (m_bSmooth)
	{
		// For a Bezier curve, all points must be drawn.
		uFirst = 0;
		uLast = GetPointsCount() - 1;
		SChartXYPoint* pKnots = NULL;
		SChartXYPoint* pFirstControlPts = NULL;
		SChartXYPoint* pSecondControlPts = NULL;
		GetBezierControlPoints(uFirst,uLast,pKnots,pFirstControlPts,pSecondControlPts);

		unsigned Count = uLast - uFirst;
		CPoint* pBezierPts = new CPoint[3*(Count-1)+1];
		CPoint* pShadowPts = NULL;
		if (m_bShadow)
			pShadowPts = new CPoint[3*(Count-1)+1];
		
		unsigned index = 0;
		for (unsigned n=0; n<Count-1; n++)
		{
			ValueToScreen(pKnots[n].X, pKnots[n].Y, pBezierPts[index]);
			ValueToScreen(pFirstControlPts[n].X, pFirstControlPts[n].Y, pBezierPts[index+1]);
			ValueToScreen(pSecondControlPts[n].X, pSecondControlPts[n].Y, pBezierPts[index+2]);
			
			if (m_bShadow)
			{
				pShadowPts[index] = pBezierPts[index];
				pShadowPts[index].Offset(m_iShadowDepth,m_iShadowDepth);
				pShadowPts[index+1] = pBezierPts[index+1];
				pShadowPts[index+1].Offset(m_iShadowDepth,m_iShadowDepth);
				pShadowPts[index+2] = pBezierPts[index+2];
				pShadowPts[index+2].Offset(m_iShadowDepth,m_iShadowDepth);
			}
			index += 3;
		}
		ValueToScreen(pKnots[Count-1].X, pKnots[Count-1].Y, pBezierPts[index]);
		if (m_bShadow)
		{
			pShadowPts[index] = pBezierPts[index];
			pShadowPts[index].Offset(m_iShadowDepth,m_iShadowDepth);
			pDC->SelectObject(&ShadowPen);
			pDC->PolyBezier(pShadowPts,3*(Count-1)+1);
			pDC->SelectObject(&NewPen);
			delete[] pShadowPts;
		}
		pDC->PolyBezier(pBezierPts,3*(Count-1)+1);

		delete[] pKnots;
		delete[] pFirstControlPts;
		delete[] pSecondControlPts;
		delete[] pBezierPts;
	}
	else	// Non-smoothed curve
	{	
		if (uLast-uFirst >= 1)
		{
			CPoint* pPoints = new CPoint[uLast-uFirst+1];
			CPoint* pShadow = NULL;
			if (m_bShadow)
				pShadow = new CPoint[uLast-uFirst+1];

			unsigned long pointsCount = 0;
			CPoint LastScreenPoint;
			for (m_uLastDrawnPoint=uFirst;m_uLastDrawnPoint<=uLast;m_uLastDrawnPoint++)
			{
				//We don't draw a line between the origin and the first point -> we must have
				// a least 2 points before begining drawing
				SChartXYPoint Point = GetPoint(m_uLastDrawnPoint);
				CPoint ScreenPoint;
				ValueToScreen(Point.X, Point.Y, ScreenPoint);

				if(LastScreenPoint != ScreenPoint)
				{
					//Only collate the unique points 
					pPoints[pointsCount] = ScreenPoint;
					LastScreenPoint = ScreenPoint;
					
					if (m_bShadow) 
					{
						ScreenPoint.Offset(m_iShadowDepth,m_iShadowDepth);
						pShadow[pointsCount] = ScreenPoint;
					}
					pointsCount++;
				}
			}

			// We have to do that in order for the Draw function to work properly.
			m_uLastDrawnPoint--;
			if (m_bShadow)
			{
				pDC->SelectObject(&ShadowPen);
				pDC->Polyline(pShadow, pointsCount);
			}
			pDC->SelectObject(&NewPen);
			pDC->Polyline(pPoints, pointsCount);

			delete[] pPoints;
			delete[] pShadow;
		}
	}

	pDC->SelectClipRgn(NULL);
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
	ShadowPen.DeleteObject();
}

void CChartLineSerie::Draw(CDC* pDC)
{
	if (!m_bIsVisible)
		return;

	// If shadow or smooth is enabled, then the complete series 
	// must be redrawn.
	if (m_bShadow || m_bSmooth)
	{
		DrawAll(pDC);
		return;
	}

	if (pDC->GetSafeHdc())
	{
		CPen NewPen;
		if (m_iPenStyle != PS_SOLID)
		{
			LOGBRUSH lb;
			lb.lbStyle = BS_SOLID;
			lb.lbColor = m_SerieColor;
			NewPen.CreatePen(PS_GEOMETRIC | m_iPenStyle, m_iLineWidth, &lb);
		}
		else
		{
			NewPen.CreatePen(m_iPenStyle, m_iLineWidth, m_SerieColor);
		}
		CPen* pOldPen;

		pDC->SetBkMode(TRANSPARENT);
		//To have lines limited in the drawing rectangle :
		pDC->IntersectClipRect(m_pParentCtrl->GetPlottingRect());
		pOldPen = pDC->SelectObject(&NewPen);

		//Draw all points that haven't been drawn yet
		for (m_uLastDrawnPoint;m_uLastDrawnPoint<GetPointsCount()-1;m_uLastDrawnPoint++)
		{
			SChartXYPoint Point = GetPoint(m_uLastDrawnPoint);
			CPoint ScreenPoint;
			ValueToScreen(Point.X, Point.Y, ScreenPoint);
			pDC->MoveTo(ScreenPoint.x,ScreenPoint.y);

			Point = GetPoint(m_uLastDrawnPoint+1);
			ValueToScreen(Point.X, Point.Y, ScreenPoint);
			pDC->LineTo(ScreenPoint.x,ScreenPoint.y);
		}

		pDC->SelectClipRgn(NULL);
		pDC->SelectObject(pOldPen);
		DeleteObject(NewPen);
	}
}

void CChartLineSerie::DrawLegend(CDC *pDC, const CRect& rectBitmap) const
{
	if (m_strSerieName== _T(""))
		return;

	//Draw line:
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = m_SerieColor;
	CPen NewPen(PS_GEOMETRIC | m_iPenStyle,m_iLineWidth,&lb);
	CPen* pOldPen = pDC->SelectObject(&NewPen);
	pDC->MoveTo(rectBitmap.left,rectBitmap.CenterPoint().y);
	pDC->LineTo(rectBitmap.right,rectBitmap.CenterPoint().y);
	pDC->SelectObject(pOldPen);
	DeleteObject(NewPen);
}

bool CChartLineSerie::IsPointOnSerie(const CPoint& screenPoint, unsigned& uIndex) const
{
	uIndex = INVALID_POINT;
	if (!m_bIsVisible)
        return false;

	unsigned uFirst=0, uLast=0;
	if (!GetVisiblePoints(uFirst, uLast))
		return false;
	if (uFirst>0)
		uFirst--;
	if (uLast<GetPointsCount()-1)
		uLast++;

	bool bResult = false;
	for (unsigned i=uFirst ; i < uLast ; i++)
	{
		SChartXYPoint PointOrig = GetPoint(i);
		SChartXYPoint PointDest = GetPoint(i+1);
		CPoint ScreenPointOrig, ScreenPointDest;
		ValueToScreen(PointOrig.X, PointOrig.Y, ScreenPointOrig);
		ValueToScreen(PointDest.X, PointDest.Y, ScreenPointDest);

		if (IsNearLine(ScreenPointOrig.x, ScreenPointOrig.y, ScreenPointDest.x, ScreenPointDest.y, screenPoint.x, screenPoint.y))
		{
			// Check if the click is close to one of the two points.
			int xDist = abs(screenPoint.x - ScreenPointOrig.x);
			int yDist = abs(screenPoint.y - ScreenPointOrig.y);
			if (xDist<=5 && yDist<=5)
				uIndex = i;
			xDist = abs(screenPoint.x - ScreenPointDest.x);
			yDist = abs(screenPoint.y - ScreenPointDest.y);
			if (xDist<=5 && yDist<=5)
				uIndex = i+1;

			bResult = true;
			break;
		}
    }
    return bResult;
}

bool CChartLineSerie::IsNearLine(long Axl, long Ayl, long Bxl, 
								 long Byl, long Cxl, long Cyl) const
{
    double Ax = Axl;
    double Ay = Ayl;
    double Bx = Bxl;
    double By = Byl;
    double Cx = Cxl;
    double Cy = Cyl;

    // Make a perpendicular projection of point C on line AB
    // algorithm from http://www.exaflop.org/docs/cgafaq/cga1.html#Subject%201.02:%20How%20do%20I%20find%20the%20distance%20from%20a%20point%20to%20a%20line?
    double L = sqrt((Bx-Ax)*(Bx-Ax) + (By-Ay)*(By-Ay));
    double r = (Ay-Cy)*(Ay-By)-(Ax-Cx)*(Bx-Ax);
    r = r /(L*L);
    if ((0 <= r) && (r <= 1))
    {
        double Px = Ax + r*(Bx-Ax);
        double Py = Ay + r*(By-Ay);
        if ((abs(Cx - Px) <= 3.0) &&
            (abs(Cy - Py) <= 3.0))
        {
            return true;
        }
    }
    return false;
}