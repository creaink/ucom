/*
 *
 *	ChartXYSerie.cpp
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
#include "ChartXYSerie.h"

CChartXYSerie::CChartXYSerie(CChartCtrl* pParent) 
  : CChartSerieBase<SChartXYPoint>(pParent)
{
}

CChartXYSerie::~CChartXYSerie()
{
}

void CChartXYSerie::AddPoint(double X, double Y)
{
	SChartXYPoint newPoint(X, Y);
	CChartSerieBase<SChartXYPoint>::AddPoint(newPoint);
}

void CChartXYSerie::AddPoints(double* pX, double* pY, unsigned Count)
{
	SChartXYPoint* pPoints = new SChartXYPoint[Count];
	for (unsigned i=0; i<Count; i++)
	{
		pPoints[i].X = pX[i];
		pPoints[i].Y = pY[i];
	}
	CChartSerieBase<SChartXYPoint>::AddPoints(pPoints, Count);
	delete pPoints;
}

void CChartXYSerie::SetPoints(double* pX, double* pY, unsigned Count)
{
	SChartXYPoint* pPoints = new SChartXYPoint[Count];
	for (unsigned i=0; i<Count; i++)
	{
		pPoints[i].X = pX[i];
		pPoints[i].Y = pY[i];
	}
	CChartSerieBase<SChartXYPoint>::SetPoints(pPoints, Count);
	delete pPoints;
}

double CChartXYSerie::GetXPointValue(unsigned PointIndex) const
{
	return m_vPoints[PointIndex].GetX();
}

double CChartXYSerie::GetYPointValue(unsigned PointIndex) const
{
	return m_vPoints[PointIndex].GetY();
}

void CChartXYSerie::SetXPointValue(unsigned PointIndex, double NewVal)
{
	m_vPoints[PointIndex].X = NewVal;
	m_vPoints.ReorderPoints();

	RefreshAutoAxes(true);
}

void CChartXYSerie::SetYPointValue(unsigned PointIndex, double NewVal)
{
	m_vPoints[PointIndex].Y = NewVal;
	m_vPoints.ReorderPoints();

	RefreshAutoAxes(true);
}

#ifndef NO_USER_DATA
void  CChartXYSerie::SetUserData(unsigned uPointIndex, void* pData)
{
	m_vPoints[uPointIndex].pUserData = pData;
}

void* CChartXYSerie::GetUserData(unsigned uPointIndex)
{
	return m_vPoints[uPointIndex].pUserData;
}
#endif

void CChartXYSerie::GetBezierControlPoints(unsigned uFirst, 
											   unsigned uLast,
											   SChartXYPoint* &pKnots, 
											   SChartXYPoint* &pFirstControlPoints, 
											   SChartXYPoint* &pSecondControlPoints) const
{
	int Count = uLast - uFirst - 1;
	if (Count < 1)
	{
		pFirstControlPoints = new SChartXYPoint[0];
		pSecondControlPoints = new SChartXYPoint[0];
		pKnots = new SChartXYPoint[0];
		return;
	}

	pKnots = new SChartXYPoint[uLast-uFirst];
	SChartXYPoint* pPoints = m_vPoints.GetInternalBuffer();
	memcpy(pKnots, pPoints+uFirst, (uLast-uFirst)*sizeof(SChartXYPoint));

	// Calculate first Bezier control points
	// Right hand side vector
	double* rhs = new double[Count];

	// Set right hand side X values
	int i=0;
	for (i = 1; i < Count - 1; ++i)
		rhs[i] = 4 * pKnots[i].X + 2 * pKnots[i + 1].X;
	rhs[0] = pKnots[0].X + 2 * pKnots[1].X;
	rhs[Count - 1] = 3 * pKnots[Count - 1].X;
	// Get first control points X-values
	double* pFirstX = GetFirstControlPoints(rhs, Count);

	// Set right hand side Y values
	for (i = 1; i < Count - 1; ++i)
		rhs[i] = 4 * pKnots[i].Y + 2 * pKnots[i + 1].Y;
	rhs[0] = pKnots[0].Y + 2 * pKnots[1].Y;
	rhs[Count - 1] = 3 * pKnots[Count - 1].Y;
	// Get first control points Y-values
	double* pFirstY = GetFirstControlPoints(rhs, Count);

	// Fill output arrays.
	pFirstControlPoints = new SChartXYPoint[Count];
	pSecondControlPoints = new SChartXYPoint[Count];
	for (i = 0; i < Count; ++i)
	{
		// First control point
		pFirstControlPoints[i].X = pFirstX[i];
		pFirstControlPoints[i].Y = pFirstY[i];
		// Second control point
		if (i < Count - 1)
		{
			pSecondControlPoints[i].X =  2 * pKnots[i + 1].X - pFirstX[i + 1];
			pSecondControlPoints[i].Y =  2 * pKnots[i + 1].Y - pFirstY[i + 1];
		}
		else
		{
			pSecondControlPoints[i].X =  (pKnots[Count].X + pFirstX[Count - 1]) / 2;
			pSecondControlPoints[i].Y =  (pKnots[Count].Y + pFirstY[Count - 1]) / 2;
		}
	}

	delete[] rhs;
	delete[] pFirstX;
	delete[] pFirstY;
}

double* CChartXYSerie::GetFirstControlPoints(double* rhs, int Count) const
{
	double* pPoints = new double[Count]; // Solution vector.
	double* pTemp = new double[Count]; // Temp workspace.

	double b = 2.0;
	pPoints[0] = rhs[0] / b;
	int i =0;
	for (i = 1; i < Count; i++) // Decomposition and forward substitution.
	{
		pTemp[i] = 1 / b;
		b = (i < Count - 1 ? 4.0 : 2.0) - pTemp[i];
		pPoints[i] = (rhs[i] - pPoints[i - 1]) / b;
	}
	for (i = 1; i < Count; i++)
		pPoints[Count - i - 1] -= pTemp[Count - i] * pPoints[Count - i]; // Backsubstitution.
	delete[] pTemp;
	return pPoints;
}