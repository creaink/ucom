/*
 *
 *	ChartGrid.h
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

#if !defined(AFX_CHARTGRID_H__ECCBEFF4_2365_49CD_A865_F1B4DD8CA138__INCLUDED_)
#define AFX_CHARTGRID_H__ECCBEFF4_2365_49CD_A865_F1B4DD8CA138__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "ChartCtrl.h"


class CChartAxis;

//! Class which draws the grid associated with a specific axis.
/**
	This object is retrieved through the CChartAxis::GetGrid function.
**/
class CChartGrid  
{
	friend CChartAxis;

public:
	//! Shows/hides the grid.
	void SetVisible(bool bVisible);
	//! Returns true if the grid is visible.
	bool IsVisible()  const         { return m_bIsVisible; }

	//! Sets the color of the grid.
	void SetColor(COLORREF NewColor);
	//! Returns the grid color.
	COLORREF GetColor() const		{ return m_GridColor; }

private:
	//! Constructor
	CChartGrid();
	//! Destructor
	virtual ~CChartGrid();

	//! Draws the grid
	void Draw(CDC* pDC);

	//! Add a tick at a certain position
	void AddTick(int Position);
	//! Removes all the ticks.
	void ClearTicks();


	//! The grid color.
	COLORREF	m_GridColor;		

	//! The parent charting control.
	CChartCtrl*	m_pParentCtrl;		
	//! Specifies if the grid is visible or not.
	bool        m_bIsVisible;

	//! List containing all the tick positions.
	std::list<int> m_lstTickPos;
	//! Specifies if the grid is associated with a vertical or horizontal axis.
	bool           m_bIsHorizontal;
};

#endif // !defined(AFX_CHARTGRID_H__ECCBEFF4_2365_49CD_A865_F1B4DD8CA138__INCLUDED_)
