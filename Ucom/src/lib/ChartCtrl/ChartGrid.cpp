/*
 *
 *	ChartGrid.cpp
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
#include "ChartGrid.h"
#include "ChartAxis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartGrid::CChartGrid() 
  : m_GridColor(RGB(128,128,128)), m_pParentCtrl(NULL), m_bIsVisible(true), 
    m_bIsHorizontal(true), m_lstTickPos()
{
}

CChartGrid::~CChartGrid()
{

}

void CChartGrid::AddTick(int Position)
{
	m_lstTickPos.push_back(Position);
}

void CChartGrid::ClearTicks()
{
	m_lstTickPos.clear();
}

void CChartGrid::Draw(CDC *pDC)
{
	if (!m_bIsVisible)
		return;
	if (!pDC->GetSafeHdc() )
		return;
	
	CRect plottingRect = m_pParentCtrl->GetPlottingRect();
	pDC->IntersectClipRect(plottingRect);

	CPen* pOldPen;
	CPen NewPen(PS_SOLID,0,m_GridColor);
	pOldPen = pDC->SelectObject(&NewPen);

	list<int>::iterator iter = m_lstTickPos.begin();
	int ActuPosition = 0;

	for (iter; iter!=m_lstTickPos.end(); iter++)
	{
		ActuPosition = *iter;
		if (!m_bIsHorizontal)
		{
			int ActuX = plottingRect.left;

			while (ActuX<plottingRect.right)
			{
				pDC->MoveTo(ActuX,ActuPosition);
				ActuX += 3;
				pDC->LineTo(ActuX,ActuPosition);
				ActuX += 3;
			}
		}
		else
		{
			int ActuY = plottingRect.bottom;

			while (ActuY>plottingRect.top)
			{
				pDC->MoveTo(ActuPosition,ActuY);
				ActuY -= 3;
				pDC->LineTo(ActuPosition,ActuY);
				ActuY -= 3;
			}
		}
	}

	pDC->SelectClipRgn(NULL);
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

void CChartGrid::SetVisible(bool bVisible)
{
	m_bIsVisible = bVisible;
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

void CChartGrid::SetColor(COLORREF NewColor)
{
	m_GridColor = NewColor; 
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}
