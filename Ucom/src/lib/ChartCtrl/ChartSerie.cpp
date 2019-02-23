/*
 *
 *	ChartSerie.cpp
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
 *		- 11/08/2006: Management of the auto axis now done in the axis. Series Register
 *					  Unregister themselves to their respective axes	.
 *		- 29/02/2008: Taking into account that RefreshAutoAxis doesn't refresh the control.
 *		- 01/03/2008: RemovePointsFromBegin and RemovePointsFromEnd functions added.
 *		- 08/03/2008: AddPoints function added (thanks to Bruno Lavier).
 *		- 11/03/2008: Min and max values are now cached.
 *		- 14/03/2008: Series can be ordered. Speed improvement done in that case.
 *		- 13/08/2008: Bug fix: calling AddPoint was not drawing the new points.
 *		- 27/11/2008: Points are now stored into the CChartPointsArray class instead of
 *					  std::vector for efficiency.
 *
 */

#include "stdafx.h"
#include "ChartCtrl.h"
#include "ChartSerie.h"
#include "ChartAxis.h"

#include "Math.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

unsigned CChartSerie::m_uNextFreeId = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartSerie::CChartSerie(CChartCtrl* pParent)
{
	m_pParentCtrl = pParent;
//	m_uLastDrawnPoint = 0;
	m_strSerieName = _T("");

	m_pHorizontalAxis = m_pVerticalAxis = NULL;
	m_uSerieId = m_uNextFreeId;
	m_uNextFreeId++;

	m_bIsVisible = true;
	m_bShadow = false;		
	m_SerieColor = RGB(0, 0, 0);
	m_ShadowColor = RGB(150,150,150);
	m_iShadowDepth = 2;

	m_bMouseClickNotifications = true;
	m_bMouseMoveNotifications = false;
}

CChartSerie::~CChartSerie()
{
	m_pHorizontalAxis->UnregisterSeries(this);
	m_pVerticalAxis->UnregisterSeries(this);
}

//void CChartSerie::SetSeriesOrdering(CChartPointsArray::PointsOrdering newOrdering)
//{
//	m_vPoints.SetOrdering(newOrdering);
//}

void CChartSerie::SetName(const TChartString& NewName) 
{ 
	m_strSerieName = NewName; 
	m_pParentCtrl->RefreshCtrl();
}

double CChartSerie::XScreenToValue(long XScreenCoord) const
{
	return m_pHorizontalAxis->ScreenToValue(XScreenCoord);
}

double CChartSerie::YScreenToValue(long YScreenCoord) const
{
	return m_pVerticalAxis->ScreenToValue(YScreenCoord);
}

void CChartSerie::ValueToScreen(double XValue, double YValue, CPoint &ScreenPoint) const
{
	ScreenPoint.x = m_pHorizontalAxis->ValueToScreen(XValue);
	ScreenPoint.y = m_pVerticalAxis->ValueToScreen(YValue);
}

void CChartSerie::SetVisible(bool bVisible)
{ 
	m_bIsVisible = bVisible; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartSerie::SetColor(COLORREF NewColor)	   
{ 
	m_SerieColor = NewColor; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartSerie::SetShadowColor(COLORREF NewColor) 
{ 
	m_ShadowColor = NewColor; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartSerie::EnableShadow(bool bEnable)
{
	m_bShadow = bEnable;
	m_pParentCtrl->RefreshCtrl();
}

void CChartSerie::SetShadowDepth(int Depth)
{ 
	m_iShadowDepth = Depth; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartSerie::EnableMouseNotifications(bool bClickEnabled, bool bMoveEnabled)
{
	m_bMouseClickNotifications = bClickEnabled;
	m_bMouseMoveNotifications = bMoveEnabled;
}

void CChartSerie::RefreshAutoAxes(bool bForceRefresh)
{
	m_pParentCtrl->EnableRefresh(false);
	m_pHorizontalAxis->RefreshAutoAxis();
	m_pVerticalAxis->RefreshAutoAxis();
	if (bForceRefresh)
		m_pParentCtrl->RefreshCtrl();
	m_pParentCtrl->EnableRefresh(true);
}
