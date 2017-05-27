/*
 *
 *	ChartGanttSerie.h
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
#include "ChartSerieBase.h"

//! Point structure used as template parameter for gantt series
struct SChartGanttPoint
{
	//! Default constructor
	SChartGanttPoint() : StartTime(0.0), EndTime(0.0), YValue(0.0)  { }
	//! Construct a new gantt point with the specifed values
	SChartGanttPoint(double Start, double End, double YVal) 
		: StartTime(Start), EndTime(End), YValue(YVal)  { }

	//! The start time of the gantt point
	double StartTime;
	//! The end time of the gantt point
	double EndTime;
	//! The Y value of the gantt point
	double YValue;

	//! Returns the X value of the point, which is the average between start time and end time
	double GetX() const { return (EndTime-StartTime)/2; }
	//! Returns the Y value
	double GetY() const { return YValue; }
	//! Returns the start time
	double GetXMin() const { return StartTime; }
	//! Returns the end time
	double GetXMax() const { return EndTime; }
	//! Returns the Y value
	double GetYMin() const { return YValue; }
	//! Returns the Y value
	double GetYMax() const { return YValue; }
};

//! Specialization of a CChartSerieBase to display a gantt series.
/**
	Each point in a gantt series is amde of three values: a start and 
	end time and an Y value. The points are displayed as horizontal bars 
	that are positionned on the Y axis depending on their Y value and 
	which starts at the start time and end at the end time along the X 
	axis.
**/
class CChartGanttSerie : public CChartSerieBase<SChartGanttPoint>
{
public:
	//! Constructor
	CChartGanttSerie(CChartCtrl* pParent);
	//! Destructor
	~CChartGanttSerie();

	//! Adds a new point to the series.
	/**
		@param StartTime
			The start time of the Gantt bar
		@param EndTime
			The end time of the Gantt bar
		@param YValue
			The YValue of the Gantt bar
	**/
	void AddPoint(double StartTime, double EndTime, double YValue);

	//! Tests if a certain screen point is on the series.
	/**
		@param screenPoint
			The screen point to test
		@param uIndex
			If the point is close to a specific point of the series, its index is stored here.
		@return true if the point is on the series
	**/
	bool IsPointOnSerie(const CPoint& screenPoint, unsigned& uIndex) const;

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

protected:
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

private:
	//! Returns the rectangle of a specific point of the series.
	CRect GetBarRectangle(unsigned uPointIndex) const;

	void DrawBar(CDC* pDC, CBrush* pFillBrush, CBrush* pBorderBrush, 
				 CRect BarRect);

	//! The bar width
	int m_iBarWidth;
	//! The bar border width
	int m_iBorderWidth;
	//! The bar border color
	COLORREF m_BorderColor;

	//! True if a gradient is applied to fill the bar
	bool m_bGradient;
	//! The second color of the gradient
	COLORREF m_GradientColor;
	//! The type of gradient to apply
	EGradientType m_GradientType;
};
