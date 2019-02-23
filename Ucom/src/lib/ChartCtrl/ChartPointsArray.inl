/*
 *
 *	ChartPointsArray.cpp
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
 */

#include "ChartPointsArray.h"

template <class T>
CChartPointsArray<T>::CChartPointsArray(unsigned iResize) 
  : m_pPoints(NULL), m_iMaxPoints(iResize), m_iCurrentPoints(0),
    m_iResize(iResize), m_Ordering(poXOrdering)
{
	m_pPoints = new T[iResize];
}

template <class T>
CChartPointsArray<T>::~CChartPointsArray()
{
	if (m_pPoints)
	{
		delete[] m_pPoints;
		m_pPoints = NULL;
	}
}

template <class T>
void CChartPointsArray<T>::AddPoint(const T& newPoint)
{
	if (m_iCurrentPoints == m_iMaxPoints)
	{
		m_iMaxPoints += m_iResize;
		T* pOldPoints = m_pPoints;
		m_pPoints =	new T[m_iMaxPoints];
		memcpy(m_pPoints,pOldPoints,m_iCurrentPoints*sizeof(T));
		delete[] pOldPoints;
	}

	if (m_iCurrentPoints == 0)
	{
		m_dXMinVal = newPoint.GetXMin();
		m_dXMaxVal = newPoint.GetXMax();
		m_dYMinVal = newPoint.GetYMin();
		m_dYMaxVal = newPoint.GetYMax();
	}
	else
	{
		if (newPoint.GetXMax() > m_dXMaxVal)  m_dXMaxVal = newPoint.GetXMax();
		if (newPoint.GetXMin() < m_dXMinVal)  m_dXMinVal = newPoint.GetXMin();
		if (newPoint.GetYMax() > m_dYMaxVal)  m_dYMaxVal = newPoint.GetYMax();
		if (newPoint.GetYMin() < m_dYMinVal)  m_dYMinVal = newPoint.GetYMin();
	}

	if (m_Ordering==poNoOrdering)
	{
		m_pPoints[m_iCurrentPoints] = newPoint;
		m_iCurrentPoints++;
	}
	else
	{
		InsertNewPoint(newPoint);
	}
}

template <class T>
void CChartPointsArray<T>::AddPoints(T* pPoints, 
										unsigned uCount)
{
	if (m_iCurrentPoints+uCount > m_iMaxPoints)
	{
		m_iMaxPoints = m_iCurrentPoints+uCount;
		T* pOldPoints = m_pPoints;
		m_pPoints =	new T[m_iMaxPoints];
		memcpy(m_pPoints,pOldPoints,m_iCurrentPoints*sizeof(T));
		delete[] pOldPoints;
	}
	for (unsigned i=0; i<uCount; i++)
		AddPoint(pPoints[i]);
}

template<class T>
void CChartPointsArray<T>::SetPoints(T* pPoints, 
										unsigned uCount)
{
	if (uCount > m_iMaxPoints)
	{
		if (m_pPoints)
			delete[] m_pPoints;
		m_pPoints = new T[uCount];
		m_iMaxPoints = uCount;
	}
	m_iCurrentPoints = uCount;

	for (unsigned i=0;i<uCount;i++)
	{
		m_pPoints[i] = pPoints[i];
	}

	ReorderPoints();
	RefreshMinMax();
}

template <class T>
void CChartPointsArray<T>::Clear()
{
	if (m_pPoints)
		delete[] m_pPoints;
	m_pPoints = new T[m_iResize];
	m_iMaxPoints = m_iResize;
	m_iCurrentPoints = 0;
}

template <class T>
void CChartPointsArray<T>::RemovePointsFromBegin(unsigned Count)
{
	ASSERT (Count < m_iCurrentPoints);
	T* pSource = m_pPoints + Count;
	memmove(m_pPoints, pSource, sizeof(T) * (m_iCurrentPoints-Count));
	m_iCurrentPoints -= Count;
	RefreshMinMax();
}

template <class T>
void CChartPointsArray<T>::RemovePointsFromEnd(unsigned Count)
{
	ASSERT (Count < m_iCurrentPoints);
	m_iCurrentPoints -= Count;
	RefreshMinMax();
}

template <class T>
T& CChartPointsArray<T>::operator[](unsigned Index)
{
	ASSERT(Index < m_iCurrentPoints);
	return m_pPoints[Index];
}

template <class T>
const T& CChartPointsArray<T>::operator[](unsigned Index) const
{
	ASSERT(Index < m_iCurrentPoints);
	return m_pPoints[Index];
}

template <class T>
bool CChartPointsArray<T>::GetSerieXMinMax(double& Min, double& Max)  const
{
	if (m_iCurrentPoints==0)
		return false;

	Min = m_dXMinVal;
	Max = m_dXMaxVal;
	return true;
}

template <class T>
bool CChartPointsArray<T>::GetSerieYMinMax(double& Min, double& Max)  const
{
	if (m_iCurrentPoints==0)
		return false;

	Min = m_dYMinVal;
	Max = m_dYMaxVal;
	return true;
}

template <class T>
void CChartPointsArray<T>::SetOrdering(PointsOrdering newOrdering)
{
	m_Ordering = newOrdering;
	ReorderPoints();
}

template <class T>
bool CChartPointsArray<T>::GetVisiblePoints(double dAxisMin, 
											double dAxisMax,
											unsigned& uFirstPt, 
											unsigned& uLastPt) const
{
	if (m_iCurrentPoints == 0)
		return false;

	if (m_Ordering == poNoOrdering)
	{
		uFirstPt = 0;
		uLastPt = m_iCurrentPoints - 1;
		return true;
	}

	uFirstPt = BinarySearch(0,m_iCurrentPoints-1,dAxisMin);		     
	uLastPt = BinarySearch(uFirstPt,m_iCurrentPoints-1,dAxisMax);                
	return true;
}

template <class T>
void CChartPointsArray<T>::ReorderPoints()
{
	switch (m_Ordering)
	{
	case poNoOrdering:
		break;
	case poXOrdering:
	    qsort(m_pPoints, m_iCurrentPoints, sizeof(T), 
			  CChartPointsArray::ComparePointsOnX);
		break;
	case poYOrdering:
	    qsort(m_pPoints, m_iCurrentPoints, sizeof(T), 
			  CChartPointsArray::ComparePointsOnY);
		break;
	}
}

template <class T>
void CChartPointsArray<T>::InsertNewPoint(const T& newPoint)
{
	if (m_iCurrentPoints == 0)
	{
		m_pPoints[0] = newPoint;
		m_iCurrentPoints++;
		return;
	}

	if (m_Ordering == poXOrdering)
	{
		if (newPoint.GetX() >= m_pPoints[m_iCurrentPoints-1].GetX())
			m_pPoints[m_iCurrentPoints] = newPoint;
		else
		{
			for (unsigned i=0; i<m_iCurrentPoints; i++)
			{
				if (newPoint.GetX() < m_pPoints[i].GetX())
				{
					InsertPointAtPos(newPoint,i);
					break;
				}
			}
		}
	}
	else if (m_Ordering == poYOrdering)
	{
		if (newPoint.GetY() >= m_pPoints[m_iCurrentPoints-1].GetY())
			m_pPoints[m_iCurrentPoints] = newPoint;
		else
		{
			for (unsigned i=0; i<m_iCurrentPoints; i++)
			{
				if (newPoint.GetY() < m_pPoints[i].GetY())
				{
					InsertPointAtPos(newPoint,i);
					break;
				}
			}
		}
	}

	m_iCurrentPoints++;
}

template <class T>
void CChartPointsArray<T>::InsertPointAtPos(const T& newPoint, int iPos)
{
	// Find the address of the insert point
	T* pPointPos = m_pPoints + iPos;
	// Move all remaining points one place to the right.
	memmove(pPointPos+1,pPointPos,(m_iCurrentPoints-iPos)*sizeof(T));
	// Store the new point
	*pPointPos = newPoint;
}

template <class T>
int CChartPointsArray<T>::ComparePointsOnX(void const* pA, void const* pB)
{
    T* pPointA = (T *) pA;
    T* pPointB = (T *) pB;

    if (pPointA->GetX() < pPointB->GetX()) return -1;
    if (pPointA->GetX() > pPointB->GetX()) return 1;
    return 0;
}

template <class T>
int CChartPointsArray<T>::ComparePointsOnY(void const* pA, void const* pB)
{
    T* pPointA = (T *) pA;
    T* pPointB = (T *) pB;

    if (pPointA->GetY() < pPointB->GetY()) return -1;
    if (pPointA->GetY() > pPointB->GetY()) return 1;
    return 0;
}

template <class T>
void CChartPointsArray<T>::RefreshMinMax()
{
	m_dXMinVal = m_pPoints[0].GetXMin();
	m_dXMaxVal = m_pPoints[0].GetXMax();
	m_dYMinVal = m_pPoints[0].GetYMin();
	m_dYMaxVal = m_pPoints[0].GetYMax();
	for (unsigned uIndex=0; uIndex<m_iCurrentPoints; uIndex++)
	{
		if (m_pPoints[uIndex].GetXMax() > m_dXMaxVal)  
			m_dXMaxVal = m_pPoints[uIndex].GetXMax();
		if (m_pPoints[uIndex].GetXMin() < m_dXMinVal)  
			m_dXMinVal = m_pPoints[uIndex].GetXMin();
		if (m_pPoints[uIndex].GetYMax() > m_dYMaxVal)  
			m_dYMaxVal = m_pPoints[uIndex].GetYMax();
		if (m_pPoints[uIndex].GetYMin() < m_dYMinVal)  
			m_dYMinVal = m_pPoints[uIndex].GetYMin();
	}
}

template <class T>
int CChartPointsArray<T>::BinarySearch(unsigned uLeft, 
									   unsigned uRight, 
									   double dFind) const
{
	unsigned middle = uLeft + ((uRight - uLeft) /2);	
	double midVal = 0;
	if (m_Ordering == poXOrdering)
		midVal = m_pPoints[middle].GetX();
	if (m_Ordering == poYOrdering)
		midVal = m_pPoints[middle].GetY();

	if(midVal > dFind)
	{
		if(uLeft < middle)				
			return BinarySearch(uLeft,middle-1,dFind);			
		else				
			return uLeft;	
	}
	else if(middle < uRight)					
		return BinarySearch(middle+1,uRight,dFind);				
	else					
		return uRight;
}


