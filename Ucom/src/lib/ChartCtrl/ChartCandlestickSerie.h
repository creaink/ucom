/*
 *
 *	ChartCandlestickSerie.h
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

//! Point structure used as template parameter for candlestick series
struct SChartCandlestickPoint
{
	SChartCandlestickPoint() { }
	SChartCandlestickPoint(double XValue, double LowVal, 
		double HighVal, double OpenVal, double CloseVal):
			XVal(XValue), Low(LowVal), High(HighVal),
				Open(OpenVal), Close(CloseVal) { }

	//! The X value of the point (usually, a time)
	double XVal;
	//! The low market price
	double Low;
	//! The high market price
	double High;
	//! The open market price
	double Open;
	//! The close market price
	double Close;

	//! Returns the X value of the point
	double GetX() const { return XVal; }
	//! Returns the Y value of the point, which is the average between low and high
	double GetY() const { return (Low+High)/2; }
	//! Returns the minimum X value of the point
	double GetXMin() const { return XVal; }
	//! Returns the maximum X value of the point
	double GetXMax() const { return XVal; }
	//! Returns the minimum Y value of the point (the low value)
	double GetYMin() const { return Low; }
	//! Returns the maximum Y value of the point (the high value)
	double GetYMax() const { return High; }
};

//! Specialization of a CChartSerieBase to display a candlestick series.
/**
	Each point in the series has an X value (the time), a high value 
	(the highest market price), a low value (the lowest market price), 
	an open value (the market price at the opening) and a close value 
	(the market price at the closing).
**/
class CChartCandlestickSerie : public CChartSerieBase<SChartCandlestickPoint>
{
public:
	//! Constructor
	CChartCandlestickSerie(CChartCtrl* pParent);
	//! Destructor
	~CChartCandlestickSerie();

	//! Tests if a certain screen point is on the series.
	/**
		@param screenPoint
			The screen point to test
		@param uIndex
			If the point is close to a specific point of the series, its index is stored here.
		@return true if the point is on the series
	**/
	bool IsPointOnSerie(const CPoint& screenPoint, unsigned& uIndex) const;

	//! Adds a new point in the series
	/**
		@param XVal
			The X value of the point (the time)
		@param Low
			The lowest market price 
		@param High
			The highest market price
		@param Open
			The market price at the opening
		@param Close
			The market price at the closing
	**/
	void AddPoint(double XVal, double Low, double High, 
				  double Open, double Close);
	//! Sets the width (in pixels) of all candlestick points in the series
	void SetWidth(int Width);
	//! Returns the width (in pixels) of a point in the series
	int  GetWidth()		{ return m_iCandlestickWidth; }

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
	//! Draws a candle stick point
	void DrawCandleStick(CDC *pDC, SChartCandlestickPoint Point);

	//! The candlestick width
	int m_iCandlestickWidth;

	// Caches the pen and brushes to avoid creating them for each point
	mutable CBrush ShadowBrush;
	mutable CPen NewPen;
	mutable CPen ShadowPen;
	mutable CBrush BrushFill;
	mutable CBrush BrushEmpty;
};