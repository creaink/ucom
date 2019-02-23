/*
 *
 *	ChartLineSerie.h
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

#if !defined(AFX_CHARTLINESERIE_H__792C2F20_9650_42FA_B13D_E63911C98CE5__INCLUDED_)
#define AFX_CHARTLINESERIE_H__792C2F20_9650_42FA_B13D_E63911C98CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartXYSerie.h"

//! Specialization of a CChartSerie to display a line series.
/**
	The data points are connected by line segments. The curve can also
	be smoothed.
**/
class CChartLineSerie : public CChartXYSerie  
{
public:
	//! Returns the pen style (plain, dashed, dotted, ...)
	/**
		For a list of pen styles available, see the CreatePen function in MSDN.
	**/
	int  GetPenStyle() const        { return m_iPenStyle; }
	//! Sets the pen style (plain, dashed, dotted, ...)
	/**
		For a list of pen styles available, see the CreatePen function in MSDN.
	**/
	void SetPenStyle(int NewStyle);

	//! Returns the pen width
	int	 GetWidth() const       { return m_iLineWidth; }
	//! Sets the pen width
	void SetWidth(int PenWidth);
	//! Enables the smoothing of the curve (slower).
	void SetSmooth(bool bSmooth);

	//! Constructor
	CChartLineSerie(CChartCtrl* pParent);
	//! Destructor
	virtual ~CChartLineSerie();

	//! Check whether a screen point is on the series.
	/**
		This function returns true if the screen point is close to a line segment.
		If the screen point is also close to a specific point of the series, the
		index of the point is stored in the uIndex parameter. Otherwise, this 
		parameter contains INVALID_POINT.
		@param screenPoint
			The screen point to test
		@param uIndex
			If the point is close to a specific point of the series, its index is stored here.
		@return true if the point is on the series
	**/
	bool IsPointOnSerie(const CPoint& screenPoint, unsigned& uIndex) const;

private:
	//! Draws the legend icon for the series.
	/**
		@param pDC
			The device context used to draw
		@param rectBitmap
			The rectangle in which to draw the legend icon
	**/
    void DrawLegend(CDC* pDC, const CRect& rectBitmap) const;

	//! Draws the most recent points of the series.
	/**
		This function should only draw the points that were not previously 
		drawn.
		@param pDC
			The device context used to draw
	**/
	void Draw(CDC* pDC);
	//! Redraws the full series.
	/**
		@param pDC
			The device context used to draw
	**/
	void DrawAll(CDC *pDC);

	//! Checks whether a point is close to a line segment
	bool IsNearLine(long Axl, long Ayl,long Bxl, 
					long Byl, long Cxl, long Cyl) const;


	//! The pen width
	int m_iLineWidth;
	//! The pen style
	int m_iPenStyle;
	//! Specifies if the curve is smoothed
	bool m_bSmooth;
};

#endif // !defined(AFX_CHARTLINESERIE_H__792C2F20_9650_42FA_B13D_E63911C98CE5__INCLUDED_)
