/*
 *
 *	ChartSerieBase.cpp
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

#include "ChartSerieBase.h"
#include "ChartAxis.h"
#include "ChartCtrl.h"
#include "ChartLabel.h"

#include "Math.h"
#include <algorithm>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template<class T>
CChartSerieBase<T>::CChartSerieBase(CChartCtrl* pParent) : CChartSerie(pParent)
{
	m_uLastDrawnPoint = 0;
	m_pMouseListener = NULL;
}

template<class T>
CChartSerieBase<T>::~CChartSerieBase()
{
	TLabelMap::iterator iter = m_mapLabels.begin();
	for (iter; iter!=m_mapLabels.end(); iter++)
	{
		delete iter->second;
	}
}

template<class T>
void CChartSerieBase<T>::SetSeriesOrdering(PointsOrdering newOrdering)
{
	m_vPoints.SetOrdering(newOrdering);
}


template<class T>
void CChartSerieBase<T>::AddPoint(const T& newPoint)
{
	m_vPoints.AddPoint(newPoint);
	RefreshAutoAxes(false);

	CDC* pDC = m_pParentCtrl->GetDC();
	Draw(pDC);
	m_pParentCtrl->Invalidate();
}

template<class T>
void CChartSerieBase<T>::AddPoints(T* pPoints, unsigned Count)
{
	m_vPoints.AddPoints(pPoints, Count);
	RefreshAutoAxes(false);

	CDC* pDC = m_pParentCtrl->GetDC();
	Draw(pDC);
	m_pParentCtrl->Invalidate();
}

template<class T>
const T& CChartSerieBase<T>::GetPoint(unsigned Index) const
{
	return m_vPoints[Index];
}

template<class T>
void CChartSerieBase<T>::SetPoints(T* pPoints, unsigned Count)
{
	m_vPoints.SetPoints(pPoints, Count);
	RefreshAutoAxes(true);
}

template<class T>
void CChartSerieBase<T>::RemovePointsFromBegin(unsigned Count)
{
	m_vPoints.RemovePointsFromBegin(Count);
	// Remove all the labels associated with those points
	for (unsigned i=0; i<=Count; i++)
	{
		TLabelMap::iterator iter = m_mapLabels.find(i);
		if (iter != m_mapLabels.end()) 
		{
			delete iter->second;
			m_mapLabels.erase(iter);
		}
	}

	RefreshAutoAxes(true);
}

template<class T>
void CChartSerieBase<T>::RemovePointsFromEnd(unsigned Count)
{
	unsigned uPtsCount = m_vPoints.GetPointsCount();

	m_vPoints.RemovePointsFromEnd(Count);
	// Remove all the labels associated with those points
	unsigned uStart = uPtsCount - Count;
	for (unsigned i=0; i<=Count; i++)
	{
		TLabelMap::iterator iter = m_mapLabels.find(uStart + i);
		if (iter != m_mapLabels.end()) 
		{
			delete iter->second;
			m_mapLabels.erase(iter);
		}
	}

	RefreshAutoAxes(true);
}

template<class T>
void CChartSerieBase<T>::ClearSerie()
{
	m_vPoints.Clear();
	TLabelMap::iterator iter = m_mapLabels.begin();
	for (iter; iter!=m_mapLabels.end(); iter++)
	{
		delete iter->second;
	}
	m_mapLabels.clear();
	m_uLastDrawnPoint = 0;

	RefreshAutoAxes(true);
}

template<class T>
bool CChartSerieBase<T>::GetSerieXMinMax(double &Min, double &Max) const
{
	if (!IsVisible()) 
		return false;
	return m_vPoints.GetSerieXMinMax(Min, Max); 
}

template<class T>
bool CChartSerieBase<T>::GetSerieYMinMax(double &Min, double &Max) const
{
	if (!IsVisible())
		return false;
	return m_vPoints.GetSerieYMinMax(Min, Max); 
}

template<class T>
bool CChartSerieBase<T>::GetSerieXScreenMinMax(double& Min, double& Max)  const
{
	if (!IsVisible())
		return false;
	if (m_vPoints.GetOrdering() != poYOrdering)
		return false;

	unsigned first=0, last=0;
	bool bRes = GetVisiblePoints(first, last);
	if (!bRes)
		return false;

	Min = m_vPoints[first].GetXMin();
	Max = m_vPoints[first].GetXMax();
	for (unsigned i=first; i<last; i++)
	{
		if (m_vPoints[i].GetXMin() < Min)
			Min = m_vPoints[i].GetXMin();
		if (m_vPoints[i].GetXMax() > Max)
			Max = m_vPoints[i].GetXMax();
	}
	return true; 
}

template<class T>
bool CChartSerieBase<T>::GetSerieYScreenMinMax(double& Min, double& Max)  const
{
	if (!IsVisible())
		return false;
	if (m_vPoints.GetOrdering() != poXOrdering)
		return false;

	unsigned first=0, last=0;
	bool bRes = GetVisiblePoints(first, last);
	if (!bRes)
		return false;

	Min = m_vPoints[first].GetYMin();
	Max = m_vPoints[first].GetYMax();
	for (unsigned i=first; i<last; i++)
	{
		if (m_vPoints[i].GetYMin() < Min)
			Min = m_vPoints[i].GetYMin();
		if (m_vPoints[i].GetYMax() > Max)
			Max = m_vPoints[i].GetYMax();
	}
	return true; 
}

template<class T>
bool CChartSerieBase<T>::GetVisiblePoints(unsigned& uFirst, unsigned& uLast) const
{
	if (m_vPoints.GetPointsCount() == 0)
		return false;

	double Min=0, Max=0;
	bool bResult = false;
	switch (m_vPoints.GetOrdering())
	{
	case poNoOrdering:
		uFirst = 0;
		uLast = GetPointsCount() - 1;
		bResult = true;
		break;
	case poXOrdering:
		m_pHorizontalAxis->GetMinMax(Min, Max);
		bResult = m_vPoints.GetVisiblePoints(Min, Max, uFirst, uLast);
		break;
	case poYOrdering:
		m_pVerticalAxis->GetMinMax(Min, Max);
		bResult = m_vPoints.GetVisiblePoints(Min, Max, uFirst, uLast);
		break;
	}

	return bResult;
}

template<class T>
CPoint CChartSerieBase<T>::GetPointScreenCoord(unsigned uPointIndex)
{
	unsigned uCount = m_vPoints.GetPointsCount();
	ASSERT(uPointIndex<uCount);

	T Point = m_vPoints[uPointIndex];
	CPoint ScreenPoint;
	ValueToScreen(Point.GetX(), Point.GetY(), ScreenPoint);
	return ScreenPoint;
}

template<class T>
CChartBalloonLabel<T>* CChartSerieBase<T>::CreateBalloonLabel(unsigned uPointIndex, 
														   const TChartString& strLabelText)
{
	ASSERT(uPointIndex<GetPointsCount());

	CChartBalloonLabel<T>* pToReturn = new CChartBalloonLabel<T>(m_pParentCtrl, this);
	pToReturn->SetLabelText(strLabelText);
	AttachCustomLabel(uPointIndex, pToReturn);
	return pToReturn;
}

template<class T>
void CChartSerieBase<T>::RegisterMouseListener(CChartSeriesMouseListener<T>* pListener)
{
	m_pMouseListener = pListener;
}

template<class T>
void CChartSerieBase<T>::UnregisterMouseListener()
{
	m_pMouseListener = NULL;
}

template<class T>
CChartBalloonLabel<T>* CChartSerieBase<T>::CreateBalloonLabel(unsigned uPointIndex,
											 CChartLabelProvider<T>* pLabelProvider)
{
	ASSERT(uPointIndex<GetPointsCount());

	CChartBalloonLabel<T>* pToReturn = new CChartBalloonLabel<T>(m_pParentCtrl, this);
	pToReturn->SetLabelProvider(pLabelProvider);
	AttachCustomLabel(uPointIndex, pToReturn);
	return pToReturn;
}

template<class T>
void CChartSerieBase<T>::AttachCustomLabel(unsigned uPointIndex, CChartLabel<T>* pLabel)
{
	ASSERT(uPointIndex<GetPointsCount());

	TLabelMap::iterator iter = m_mapLabels.find(uPointIndex);
	if (iter != m_mapLabels.end())
	{
		delete iter->second;
	}
	m_mapLabels[uPointIndex] = pLabel;
}

template<class T>
void CChartSerieBase<T>::DrawLabels(CDC* pDC)
{
	TLabelMap::iterator iter = m_mapLabels.begin();
	for (iter; iter!=m_mapLabels.end(); iter++)
	{
		iter->second->Draw(pDC,iter->first);
	}
}

template<class T>
bool CChartSerieBase<T>::OnMouseEvent(CChartMouseListener::MouseEvent mouseEvent, 
					const CPoint& screenPoint)
{
	bool bHandled = false;
	if (m_pMouseListener == NULL)
		return bHandled;

	if ( (mouseEvent==CChartMouseListener::MouseMove) &&
		  !NotifyMouseMoveEnabled())
	{
		return bHandled;
	}
	if ( (mouseEvent!=CChartMouseListener::MouseMove) &&
		  !NotifyMouseClickEnabled())
	{
		return bHandled;
	}

	unsigned uPtIndex = 0;
	if (IsPointOnSerie(screenPoint,uPtIndex))
	{
		m_pMouseListener->OnMouseEventSeries(mouseEvent,screenPoint,this,uPtIndex);
		bHandled = true;
	}

	return bHandled;
}
