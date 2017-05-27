/*
 *
 *	ChartBarSerie.h
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

#pragma once
#include "ChartXYSerie.h"
#include "ChartGradient.h"
#include <list>

//! Specialization of a CChartSerie to display a bars series.
/**
	This class is a specialized series class used to display vertical (default) 
	or horizontal bars. Each bar in the series is centered around its X value 
	(for vertical bars) or Y value (for horizontal bars). Bars can be grouped 
	together, so that they do not overlap but are stacked next to each other 
	(or on top of each other). This is done by specifying a group Id: bar series 
	with the same group Id will be grouped together (stacked). Series with different 
	group Id will be independant (they will be drawn as if they were the only one, 
	meaning that the different series will probably overlap).
**/
class CChartBarSerie : public CChartXYSerie
{
public:
	//! Constructor
	CChartBarSerie(CChartCtrl* pParent);
	//! Destructor
	~CChartBarSerie();

	//! Specifies if the bars are vertical or horizontal.
	void SetHorizontal(bool bHorizontal);
	//! Returns true if bars are horizontal, false otherwise.
	bool GetHorizontal() const		  { return m_bHorizontal; }

	//! Sets the bars border color
	void SetBorderColor(COLORREF BorderColor);
	//! Returns the bars border color
	COLORREF GetBorderColor() const			   { return m_BorderColor; }
	//! Sets the bars border width
	void SetBorderWidth(int Width);
	//! Returns the bars border width
	int  GetBorderWidth() const		{ return m_iBorderWidth;  }
	//! Sets the bars width (in pixels)
	void SetBarWidth(int Width);
	//! Returns the bars width (in pixels)
	int  GetBarWidth() const		{ return m_iBarWidth;  }

	//! Set the group Id of the series
	/**
		The group Id allows to stack series next to each other (or on top of
		each other).
	**/
	void SetGroupId(unsigned GroupId);
	//! Returns the group Id of the series
	unsigned GetGroupId() const			{ return m_uGroupId;    }

	//! Specifies if the series is stacked with other bar series
	/**
		All bar series with the same group Id and with the stacked 
		flag to true will be drawn on top of each other (for vertical
		bars).
	**/
	void SetStacked(bool bStacked);
	//! Returns true if the series is stacked
	bool IsStacked();

	//! Specifies if a gradient is applied to the bars
	void ShowGradient(bool bShow);
	//! Sets the gradient style
	/**
		@param GradientColor
			The second color used for the gradient (the first one being
			the original series color).
		@param GradientType
			The type of gradient used between the two colors (vertical, horizontal, ...)
	**/
	void SetGradient(COLORREF GradientColor, EGradientType GradientType);

	//! Static function used to specify the space (in pixels) between series of the same group
	static void SetInterSpace(int Space)	{ m_iInterSpace = Space; }
	//! Static function returning the space between series of the same group.
	static int  GetInterSpace()				{ return m_iInterSpace;  }

	//! Specifies a base line for the bars.
	/**
		If a baseline is specified, the bars will be drawn between that value
		and the point value, instead of being drawn between the axis ans the 
		point value.
		@param bAutomatic
			If true, the bars are drawn between the axis and the point value.
		@param dBaseLine
			The value of the baseline. This parameter is ignored if bAutomatic is true.
	**/
	void SetBaseLine(bool bAutomatic, double dBaseLine)
	{
		m_bAutoBaseLine = bAutomatic;
		m_dBaseLine = dBaseLine;
	}

	//! Check whether a screen point is on the series.
	/**
		This function returns true if the screen point is on one of the bars of
		the series. In that case, the index of the point is stored in the uIndex 
		parameter.
		@param screenPoint
			The screen point to test
		@param uIndex
			If the point is close to a specific point of the series, its index is stored here.
		@return true if the point is on the series
	**/
	bool IsPointOnSerie(const CPoint& screenPoint, unsigned& uIndex) const;

private:
	typedef std::list<CChartBarSerie*> TBarSeriesList;

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

	void DrawBar(CDC* pDC, CBrush* pFillBrush, CBrush* pBorderBrush, 
				 CRect BarRect);

	// Retrieves the offset of the serie along the minor axis (horizontal
	// axis for vertical bars). This offset is based on series with the
	// same group Id.
	int GetMinorOffset() const;
	CRect GetBarRectangle(unsigned uPointIndex, int minorOffset) const;

	void RefreshStackedCache() const;


	//! Space between two series of the same group.
	static int m_iInterSpace;	
	//! List of all bar series added to the control
	static TBarSeriesList m_lstBarSeries;
	
	//! Specifies if the bars are horizontal or vertical.
	bool m_bHorizontal;

	// The base line specifies the position (in terms of the value
	// axis) where the bars start from. If m_bAutoBaseLine is true
	// they start from the axis position.
	double m_dBaseLine;
	bool m_bAutoBaseLine;

	// Specifies to which group this series belongs to. Series in the same group are
	// 'stacked' next to each other.
	unsigned m_uGroupId;		

	int m_iBarWidth;
	int m_iBorderWidth;
	COLORREF m_BorderColor;

	bool m_bGradient;
	COLORREF m_GradientColor;
	EGradientType m_GradientType;

	// Specifies if the bar series has to be stacked with other bar 
	// series with the same group Id
	bool m_bStacked;

	// Cache of the stacked series with the same group Id as this series.
	mutable TBarSeriesList m_lstStackedSeries;
};
