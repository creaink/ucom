/*
 *
 *	ChartCursor.cpp
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
#include "ChartCursor.h"
#include "ChartCtrl.h"

unsigned CChartCursor::m_uNextFreeId = 0;

CChartCursor::CChartCursor(CChartCtrl* pParent)
 : m_colCursor(RGB(0,0,0)), m_pParentCtrl(pParent), m_uCursorId(0),
   m_lstListeners()
{
	m_uCursorId = m_uNextFreeId;
	m_uNextFreeId++;
}

CChartCursor::~CChartCursor()
{
}

void CChartCursor::SetColor(COLORREF cursorColor)  
{ 
	m_colCursor = cursorColor; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartCursor::RegisterListener(CChartCursorListener* pListener)
{
	m_lstListeners.push_back(pListener);
}

void CChartCursor::CursorMoved(double newXValue, double newYValue)
{
	TListenerList::iterator iter = m_lstListeners.begin();
	for (iter; iter!=m_lstListeners.end(); iter++)
		(*iter)->OnCursorMoved(this, newXValue, newYValue);
}
