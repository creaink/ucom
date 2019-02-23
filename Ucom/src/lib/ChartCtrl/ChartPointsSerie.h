/*
 *
 *	ChartPointsSerie.h
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

#if !defined(AFX_CHARTPOINTSSERIE_H__F66C180F_F04C_4E2D_878C_08BDBCE91863__INCLUDED_)
#define AFX_CHARTPOINTSSERIE_H__F66C180F_F04C_4E2D_878C_08BDBCE91863__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartXYSerie.h"

//! Specialization of a CChartSerie to display a points series.
/**
	The data points are simply displayed as independant points.
**/
class CChartPointsSerie : public CChartXYSerie  
{
public:
	//! The different point shapes
	enum PointType
	{
		ptEllipse=0,
		ptRectangle=1,
		ptTriangle=2
	};

	//! Sets the width and height of each points
	void SetPointSize(int XSize, int YSize);
	//! Retrieves the width and height of each points
	void GetPointSize(int& XSize, int& YSize) const
	{
        XSize = m_iXPointSize;
        YSize = m_iYPointSize;
    }
	//! Sets the points shape
	void SetPointType(PointType Type);
	//! Returns the points shape
	PointType GetPointType() const     { return m_iPointType; }

	//! Sets the border color of the points
	void SetBorderColor(COLORREF Color);
	//! Returns the border color of the points
	COLORREF GetBorderColor()	{ return m_colBorder; }

	//! Constructor
	CChartPointsSerie(CChartCtrl* pParent);
	//! Destructor
	virtual ~CChartPointsSerie();
	
	//! Check whether a screen point is on the series.
	/**
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

	//! Width of the points
	int m_iXPointSize;		
	//! Height of the points
	int m_iYPointSize;	
	//! Shape of the points
	PointType m_iPointType;
	//! The border color
	COLORREF m_colBorder;
};

#endif // !defined(AFX_CHARTPOINTSSERIE_H__F66C180F_F04C_4E2D_878C_08BDBCE91863__INCLUDED_)
