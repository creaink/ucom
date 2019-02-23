/*
 *
 *	ChartSerie.h
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

#if !defined(AFX_CHARTSERIE_H__FFCF0E32_10E7_4A4D_9FF3_3C6177EDE4B1__INCLUDED_)
#define AFX_CHARTSERIE_H__FFCF0E32_10E7_4A4D_9FF3_3C6177EDE4B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define INVALID_POINT	UINT_MAX

#include "ChartAxis.h"
#include "ChartSeriesMouseListener.h"

#include <map>
#include "ChartString.h"

class CChartLegend;
class CChartCtrl;

#define INVALID_POINT	UINT_MAX

//! Abstract class that provides a common "interface" for all series in the control
/**
	The class doesn't manipulate points (the type of point is unknown at this 
	level in the class hierarchy) but it provides all other functionalities
	which are independant of the point type.

	The drawing of the series is made through pure virtual functions which
	should be implemented by derived classes.

	Each series is identified by an Id.
**/
class CChartSerie
{
	friend CChartCtrl;
	friend CChartLegend;

public:
	//! Returns the number of points in the series.
	virtual unsigned GetPointsCount() const = 0;

	//! Returns the screen coordinate of a specific point
	virtual CPoint GetPointScreenCoord(unsigned uPointIndex) = 0;

	//! Retrieves the minimum and maxium Y values of the series.
	/**
		@param Min
			Minimum value will be stored in this parameter
		@param Max
			Maximum value will be stored in this parameter
		@return
			false if the series doesn't contain data or is invisible
	**/
	virtual bool GetSerieYMinMax(double& Min, double& Max)  const = 0;
	//! Retrieves the minimum and maxium X values of the series.
	/**
		@param Min
			Minimum value will be stored in this parameter
		@param Max
			Maximum value will be stored in this parameter
		@return
			false if the series doesn't contain data or is invisible
	**/
	virtual bool GetSerieXMinMax(double& Min, double& Max)  const = 0;
	//! Retrieves the minimum and maxium screen X values of the series.
	/**
		@param Min
			Minimum value will be stored in this parameter
		@param Max
			Maximum value will be stored in this parameter
		@return
			false if the series doesn't contain data or is invisible
	**/
	virtual bool GetSerieXScreenMinMax(double& Min, double& Max)  const = 0;
	//! Retrieves the minimum and maxium screen Y values of the series.
	/**
		@param Min
			Minimum value will be stored in this parameter
		@param Max
			Maximum value will be stored in this parameter
		@return
			false if the series doesn't contain data or is invisible
	**/
	virtual bool GetSerieYScreenMinMax(double& Min, double& Max)  const = 0;

	//! Sets the name of the series, which is displayed in the legend.
	void		 SetName(const TChartString& NewName);
	//! Returns the name of the series.
	TChartString GetName() const              { return m_strSerieName; }

	//! Converts any data point into its relative screen point.
	/**
		@param XValue
			The X value of the data point
		@param YValue
			The Y value of the data point
		@param ScreenPoint
			The screen point will be stored in the parameter
	**/
	void ValueToScreen(double XValue, double YValue, CPoint& ScreenPoint)  const;
	//! Converts an Y screen value into its relative Y data value.
	double YScreenToValue(long YScreenCoord)  const;
	//! Converts an Xscreen value into its relative X data value.
	double XScreenToValue(long XScreenCoord)  const;

	//! Constructor
	CChartSerie(CChartCtrl* pParent);
	//! Destructor
	virtual ~CChartSerie();

	//! Specifies if the series is visible or not.
	/**
		An invisible series won't affect automatic axis: it is considered
		as if it was not in the control.
	**/
	void SetVisible(bool bVisible);
	//! Returns true if the series is visible.
	bool IsVisible()  const         { return m_bIsVisible; }

	//! Returns the color of the series.
	COLORREF GetColor() const			   { return m_SerieColor; }
	//! Sets the color of the series.
	void SetColor(COLORREF NewColor);
	//! Returns the color of the shadow.
	COLORREF GetShadowColor() const		   { return m_ShadowColor; }
	//! Sets the color of the shadow.
	void SetShadowColor(COLORREF NewColor);
	//! Enables or disables the shadow for the series.
	void EnableShadow(bool bEnable);
	//! Sepcifies the depth (in pixels) of the shadow.
	void SetShadowDepth(int Depth);

	//! Tests if a certain screen point is on the series.
	/**
		This function should be overidden by all child classes.
		@param screenPoint
			The screen point to test
		@param uIndex
			If the point is close to a specific point of the series, its index is stored here.
		@return true if the point is on the series
	**/
	virtual bool IsPointOnSerie(const CPoint& screenPoint, unsigned& uIndex) const = 0;

	//! Returns the series Id.
	unsigned GetSerieId() const  { return m_uSerieId; }
	//! Enables or disables certain mouse notifications on the series.
	/**
		Checking if a point is on the series could degrade performances if 
		it has to be done for each mouse event. This function allows to disable
		certain notifications, in which case the test won't be done. By default
		the series reacts on mouse clicks but not on mouse moves.
		@param bClickEnabled
			Specifies if the series reacts on mouse clicks.
		@param bMoveEnabled
			Specifies if the series reacts on mouse moves.
	**/
	void EnableMouseNotifications(bool bClickEnabled, bool bMoveEnabled);

protected:
	//! Refreshes the automatic axes associated with this series
	void RefreshAutoAxes(bool bForceRefresh);
	//! Returns the first and last visible points of the series.
	/**
		This function only works if ordering is enabled.
		@param uFirst
			The index of the first visible point is stored in this argument
		@param uLast
			The index of the last visible point is stored in this argument
		@return false if the series has no ordering or no data points.
	**/
	virtual bool GetVisiblePoints(unsigned& uFirst, unsigned& uLast) const = 0;

	//! Draws the legend icon for the series.
	/**
		This pure virtual function should be overriden by child classes.
		@param pDC
			The device context used to draw
		@param rectBitmap
			The rectangle in which to draw the legend icon
	**/
    virtual void DrawLegend(CDC* pDC, const CRect& rectBitmap) const =0;

	//! Draws the most recent points of the series.
	/**
		This pure virtual function should be overriden by child classes.
		This function should only draw the points that were not previously 
		drawn.
		@param pDC
			The device context used to draw
	**/
	virtual void Draw(CDC* pDC) =0;
	//! Redraws the full series.
	/**
		This pure virtual function should be overriden by child classes.
		@param pDC
			The device context used to draw
	**/
	virtual void DrawAll(CDC *pDC) =0;
	//! Draws the labels of the series.
	/**
		This pure virtual function should be overriden by child classes.
		@param pDC
			The device context used to draw
	**/
	virtual void DrawLabels(CDC* pDC) =0;

	//! Called when a mouse event is detected on the chart
	/**
		This pure virtual function should be overriden by child classes.
		@param mouseEvent
			The event that occured on the control
		@param screenPoint
			The screen point on which the event occured
		@return true if the event occured on the series.
	**/
	virtual bool OnMouseEvent(CChartMouseListener::MouseEvent mouseEvent, 
								const CPoint& screenPoint) = 0;

	//! Returns true if the series reacts on mouse moves.
	bool NotifyMouseMoveEnabled()  { return m_bMouseMoveNotifications;  }
	//! Returns true if the series reacts on mouse clicks.
	bool NotifyMouseClickEnabled() { return m_bMouseClickNotifications; }

	//! The parent charting control.
	CChartCtrl* m_pParentCtrl;
	//! The related vertical axis.
	CChartAxis* m_pVerticalAxis;    
	//! The related horizontal axis.
	CChartAxis* m_pHorizontalAxis;  

	//! The series name displayed in the legend.
	TChartString m_strSerieName;	

	//! Specifies if the series is visible.
	bool        m_bIsVisible;
	//! Specifies if the series has shadow enabled.
	bool		m_bShadow;	
	//! Color of the series
	COLORREF	m_SerieColor;
	//! Color of the shadow
	COLORREF	m_ShadowColor;
	//! Depth (in pixels) of the shadow
	int			m_iShadowDepth;
	//! The rectangle in which the series should be drawn.
	CRect		m_PlottingRect;

private:
	//! Sets the plotting rectangle.
	void SetPlottingRect(const CRect& plottingRect)  { m_PlottingRect = plottingRect; }

	//! The next available series Id
	static unsigned m_uNextFreeId;
	//! The series Id
	unsigned m_uSerieId;

	//! Specifies if the series reacts on mouse clicks.
	bool m_bMouseClickNotifications;
	//! Specifies if the series reacts on mouse moves.
	bool m_bMouseMoveNotifications;
};

#endif // !defined(AFX_CHARTSERIE_H__FFCF0E32_10E7_4A4D_9FF3_3C6177EDE4B1__INCLUDED_)
