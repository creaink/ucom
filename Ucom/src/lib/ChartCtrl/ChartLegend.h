/*
 *
 *	ChartLegend.h
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

#if !defined(AFX_CHARTLEGEND_H__CD72E5A0_8F52_472A_A611_C588F642080B__INCLUDED_)
#define AFX_CHARTLEGEND_H__CD72E5A0_8F52_472A_A611_C588F642080B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartCtrl.h"

#include "ChartString.h"

class CChartSerie;

//! This class is responsible for the legend displayed on the control.
/**
	Series which are named will be displayed in the legend. The legend
	object is retrieved	by calling the GetLegend() function on the 
	CChartCtrl class. 
**/
class CChartLegend  
{
	friend CChartCtrl;

public:
	//! Sets the font used to display the series names.
	void SetFont(int iPointSize, const TChartString& strFaceName);

	//! Enumeration specifying on which side of the control the legend is docked.
	enum DockSide
	{
		dsDockRight,
		dsDockLeft,
		dsDockTop,
		dsDockBottom
	};

	//! Dock the legend on a specific side of the control. Default is right.
	void DockLegend(DockSide dsSide);
	//! Undock the legend.
	/**
		When the legend is undocked (floating), it doesn't take any margin size
		but is drawn on top of the control at the specified location (it can be
		above the plotting area for instance).
		@param iLeftPos
			The left position of the legend, in pixels (from the left of the control)
		@param iTopPos
			The top position of the legend, in pixels (from the top of the control)
	**/
	void UndockLegend(int iLeftPos, int iTopPos);

	//! Sets the background of the legend transparent.
	void SetTransparent(bool bTransparent);  
	//! Sets the legend in horizontal/vertical mode.
	/**
		In horizontal mode, the names are drawn next to each other
		instead of on top of each other.
	**/
	void SetHorizontalMode(bool bHorizontal);

	//! Sets the legend visible/invisible.
	void SetVisible(bool bVisible);
	//! Returns true if the legend is visible.
	bool IsVisible()  const         { return m_bIsVisible; }

	//! Returns the back color of the legend.
	COLORREF GetBackColor() const			   { return m_BackColor; }
	//! Sets the back color of the legend.
	void SetBackColor(COLORREF NewColor);
	//! Returns the shadow color.
	COLORREF GetShadowColor() const		   { return m_ShadowColor; }
	//! Sets the shadow color.
	void SetShadowColor(COLORREF NewColor);
	//! Enables/disables the shadow.
	void EnableShadow(bool bEnable);
	//! Sets the shadow depth (in pixels).
	void SetShadowDepth(int Depth);

	//! Returns true if the screen point is on the legend region.
	BOOL IsPointInside(const CPoint& screenPoint) const;

private:
	//! Constructor
	CChartLegend(CChartCtrl* pParent);
	//! Destructor
	virtual ~CChartLegend();

	//! Draw the legend.
	void Draw(CDC* pDC);
	//! Remove the area needed for the legend from the chart rectangle.
	void ClipArea(CRect& rcControl, CDC* pDC);
	//! Recalculate the legend size and position.
	void UpdatePosition(CDC* pDC, const CRect& rcControl);

	//! The parent charting control.
	CChartCtrl* m_pParentCtrl;
	//! The rectangle used to draw the legend.
	CRect m_LegendRect;

	//! The font face name used to display the series names.
	TChartString m_strFontName;
	//! The font point size.
	int          m_iFontSize;

	//! True if the legend is docked
	bool m_bDocked;	
	//! The side of the control on which the legend is docked.
	DockSide m_DockSide;

	//! The left position of the legend if in floating mode.
	int m_iLeftPos;
	//! The top position of the legend if in floating mode.
	int m_iTopPos;

	//! Specifies if the legend is visible.
	bool m_bIsVisible;
	//! Specifies if the background of the legend is transparent.
	bool m_bIsTransparent;
	//! Specifies if the legend is in horizontal mode.
	bool m_bIsHorizontal;
	//! Specifies if the legend shadow should be displayed.
	bool m_bShadow;
	//! Specifies the shadow depth (in pixels).
	int m_iShadowDepth;

	//! Specifies the legend back color.
	COLORREF m_BackColor;
	//! Specifies the shadow color.
	COLORREF m_ShadowColor;

	//! Specifies the size of the bitmap used by each series.
	CSize m_BitmapSize;
};

#endif // !defined(AFX_CHARTLEGEND_H__CD72E5A0_8F52_472A_A611_C588F642080B__INCLUDED_)
