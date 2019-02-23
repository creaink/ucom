/*
 *
 *	ChartSurfaceSerie.h
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

#if !defined(AFX_CHARTSURFACESERIE_H__28A77823_43BD_4502_9AA7_A2B227454035__INCLUDED_)
#define AFX_CHARTSURFACESERIE_H__28A77823_43BD_4502_9AA7_A2B227454035__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartXYSerie.h"

//! Specialization of a CChartSerie to display a surface series.
/**
	A surface can be horizontal (default) or vertical: this defines how 
	the filling of the surface is done. For a horizontal surface, the 
	filling is done between the points and the associated horizontal axis 
	and for a vertical surface, the filling is done between the points 
	and the associated vertical axis. 
	The series can be associated with a secondary axis. For example, if 
	the surface series is horizontal and is associated with the top axis 
	(secondary axis), the filling is done between the top axis and the points.
**/
class CChartSurfaceSerie : public CChartXYSerie  
{
public:
	//! Constructor
	CChartSurfaceSerie(CChartCtrl* pParent);
	//! Destructor
	virtual ~CChartSurfaceSerie();

	//! The different fill styles
	enum FillStyle
	{
		fsSolid = 0,
		fsHatchDownDiag,
		fsHatchUpDiag,
		fsHatchCross,
		fsHatchDiagCross,
		fsHatchHorizontal,
		fsHatchVertical
	};

	//! Sets the fill style
	void SetFillStyle(FillStyle NewStyle);
	//! Returns the fill style
	FillStyle GetFillStyle() const		   { return m_FillStyle; }

	//! Sets the series in horizontal or vertical mode
	/**
		If the series is in horizontal mode, the filling will be done between
		the data points and the horizontal axis.
	**/
	void SetHorizontal(bool bHoriz);
	//! Returns true if the series is in horizontal mode
	bool GetHorizontal() const		{ return m_bHorizontal;   }

	//! Check whether a screen point is on the series.
	/**
		This function returns true if the screen point is on the surface.
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

	void CChartSurfaceSerie::SetSeriesOrdering(PointsOrdering );
private:
	//! Override in order to avoid changing series ordering.
//	void SetSeriesOrdering(CChartPointsArray::PointsOrdering);

	//! Draws the legend icon for the series.
	/**
		This pure virtual function should be overriden by child classes.
		@param pDC
			The device context used to draw
		@param rectBitmap
			The rectangle in which to draw the legend icon
	**/
    void DrawLegend(CDC* pDC, const CRect& rectBitmap) const;

	//! Draws the most recent points of the series.
	/**
		This pure virtual function should be overriden by child classes.
		This function should only draw the points that were not previously 
		drawn.
		@param pDC
			The device context used to draw
	**/
	void Draw(CDC* pDC);
	//! Redraws the full series.
	/**
		This pure virtual function should be overriden by child classes.
		@param pDC
			The device context used to draw
	**/
	void DrawAll(CDC *pDC);

	//! The brush style used to fill the surface
	FillStyle m_FillStyle;
	//! The horizontal/vertical mode
	bool m_bHorizontal;
};

#endif // !defined(AFX_CHARTSURFACESERIE_H__28A77823_43BD_4502_9AA7_A2B227454035__INCLUDED_)
