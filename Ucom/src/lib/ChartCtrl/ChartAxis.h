/*
 *
 *	ChartAxis.h
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

#ifndef _CHARTAXIS_H_
#define _CHARTAXIS_H_

#include "ChartScrollBar.h"
#include "ChartString.h"
#include <afx.h>

#include <list>

class CChartCtrl;
class CChartGrid;
class CChartSerie;
class CChartAxisLabel;
   
//! Base class that takes care of the management of a chart axis.
/**
	This class cannot be instanciated but should be overriden in order
	to provide the required functionality (this is already done for 
	standard axis, date/time axis and logarithmic axis).<br>
	The class provides already a lot of functionalities but delegate the
	ticks positioning and labeling to the child classes.<br>
	By default, the class manages a continues range of double values (which 
	is the case for standard axis and date/time axis) but in some cases, this 
	is not valid (for instance, a logarithmic scale). In that case, you should 
	in addition override some specific functions (e.g. those handling the scrollbar).
	Take a look at the CChartLogarithmicAxis class for more details.
**/
class CChartAxis 
{
	friend CChartCtrl;
	friend CChartGrid;
	friend CChartSerie;
	friend CChartScrollBar;

public:
	//! Default constructor
	CChartAxis();
	//! Default destructor
	virtual ~CChartAxis();

	//! Retrieves the position (in %) of the axis.
	int GetPosition();

	//! Sets the axis in reverse.
	/**
		For an inverted axis, the values on the axis are 
		in decreasing order.
		@param bInverted
			true if the axis has to be inverted.
	**/
	void SetInverted(bool bInverted);
	//! Retrieves if the axis is inverted or not.
	bool IsInverted() const  { return m_bIsInverted; }
	
	//! Sets the axis in automatic or manual mode.
	/**
		In automatic mode, the axis min and max will be updated
		depending on the series related to this axis.
		@param bAutomatic
			true if the axis should be automatic.
		@deprecated You should use the SetAutomaticType instead.
	**/
	void SetAutomatic(bool bAutomatic); 
	//! Returns true if an automatic mode has been set on this axis.
	/**
		@deprecated You should use the GetAutomaticType instead.
	**/
	bool IsAutomatic()  const  { return m_AutoMode != NotAutomatic; }

	//! The different modes of automatic modes for an axis.
	enum EAxisAutoModes 
	{
		//! The axis min and max values are set manually
		NotAutomatic,
		//! The axis min and max values of the axis are the min and max values of all series associated with this axis. This corresponds to the "standard" automatic mode that was implemented before version 3.0.
		FullAutomatic,
		//! The axis min and max values of the axis are the <b>visible</b> min and max values of all series associated with this axis. The end result will then depends on how the other axes are configured.
		ScreenAutomatic
	};

	//! Sets the automatic mode of the axis
	void SetAutomaticMode(EAxisAutoModes AutoMode); 
	//! Gets the automatic type of the axis
	EAxisAutoModes GetAutomaticMode()  const  { return m_AutoMode; }

	//! Sets the axis visible/invisible.
	void SetVisible(bool bVisible); 
	//! Retrieves the axis automatic mode.
	bool IsVisible()  const  { return m_bIsVisible; }

	//! Sets the axis min and max values.
	/**
		This doesn't take into account the real type of the axis,
		so double values should be provided.
		@param Minimum
			The min value of the axis
		@param Maximum
			The max value of the axis.
	**/
	void SetMinMax(double Minimum, double Maximum);
	//! Gets the min anx max values of the axis.
	void GetMinMax(double& Minimum, double& Maximum) const
	{
		Minimum = m_MinValue;
		Maximum = m_MaxValue;
	}

	//! Sets the axis color.
	void	 SetAxisColor(COLORREF NewColor);
	//! Sets the tick labels color.
	void	 SetTextColor(COLORREF NewColor);
	//! Gets the tick labels color.
	COLORREF GetTextColor() const		{ return m_TextColor;		}
	//! Sets the tick labels font
	/**
		@param nPointSize
			The font point size
		@param strFaceName
			The font face name
	**/
	void SetFont(int nPointSize, const TChartString& strFaceName);

	//! Retrieves the chart axis label object
	CChartAxisLabel* GetLabel() const	{ return m_pAxisLabel; }
	//! Retrieves the chart axis grid object
	CChartGrid*		 GetGrid()	const	{ return m_pAxisGrid;  }

	//! Sets the margin size
	/**
		@param bAuto
			Specifies if the margin size is automatic or not.
			In automatic mode, the iNewSize parameter is ignored
			and the margin size is calculated automatically.
		@param iNewSize
			The new size of the margin, in manual mode.
	**/
	void SetMarginSize(bool bAuto, int iNewSize);

	//! Enable the pan and zoom for this axis.
	void SetPanZoomEnabled(bool bEnabled)	{ m_bZoomEnabled = bEnabled; }
	//! Sets the zoom limit.
	/**
		The zoom limit is the minimum lenght (in values) of the axis.
	**/
	void SetZoomLimit(double dLimit)		{ m_dZoomLimit = dLimit; }

	//! Enables/disables the scroll bar.
	void EnableScrollBar(bool bEnabled);
	//! Retrieves if the scroll bar is enabled or not.
	bool ScrollBarEnabled() const  
	{ 
		if (m_pScrollBar)
			return (m_pScrollBar->GetEnabled()); 
		else
			return false;
	}
	//! Specifies if the scroll bar is in auto-hide mode.
	/**
		In auto-hide mode, the scroll bar will be hidden until
		you hover the mouse over it.
	**/
	void SetAutoHideScrollBar(bool bAutoHide);
	//! Retrieves if the scroll bar is in auto-hide mode.
	bool GetAutoHideScrollBar() const;

	//! Sets the axis in discrete mode
	/**
		@param bDiscrete
			true if the axis has to be discrete, false otherwise.
		In discrete mode, the axis doesn't have a continuous range of values
		but only steps which are defined by the tick interval. In this mode,
		you won't be able to plot points at a different location that in the 
		middle of two ticks. For instance, if you have a tick interval of 1.0,
		trying to plot a value of 0.9 will display the point at the same position
		as if the value was 0.3: it will be displayed in the middle of tick 0.0 and
		tick 1.0.
		<br>It is mainly used to display the tick label in the middle of two ticks.
		This is for instance nice with date/time axis.
	**/
	virtual void SetDiscrete(bool bDiscrete);

	//! Converts a value on the axis to a screen position
	/**
		The functions takes care of the discrete mode (internally,
		it calls ValueToScreenStandard or ValueToScreenDiscrete
		depending on the discrete mode).
		@param Value
			The value to convert
		@return the screen position of the value
	**/
    long ValueToScreen(double Value) const;
 	//! Converts a screen position to a value on the axis
	/**
		The function is implemented for an axis with a standard
		behavior (the axis shows a continuous range of doubles).
		It is the case for standard axis and date/time axis (date
		are converted to doubles internally). Axis that needs a different
		behavior should override this function (e.g. a logarithmic axis).
		The function does not take care of the discrete mode. 
		@param ScreenVal
			The screen value to convert
		@return the double value
	**/
   virtual double ScreenToValue(long ScreenVal) const;

   	//! Returns true if the axis is horizontal
	bool IsHorizontal() const { return m_bIsHorizontal; } 

	//! Returns true if a screen point is in the region of the axis.
	BOOL IsPointInside(const CPoint& screenPoint) const;

protected:
	//! Returns the first tick value.
	/**
		This pure virtual function must be implemented for specific
		axes type.
	**/
	virtual double GetFirstTickValue() const = 0;
	//! Retrieves the next tick value after a given tick.
	/**
		This pure virtual function must be implemented for specific
		axes type.
		@param dCurrentTick
			The value of the current tick
		@param dNextTick
			The value of the next tick will be stored in this parameter
		@return true if there is a next or false when the current tick is the last one.
	**/
	virtual bool GetNextTickValue(double dCurrentTick, double& dNextTick) const = 0;
	//! Retrieves the screen position of a certain tick.
	/**
		This pure virtual function must be implemented for specific
		axes type.
		@param Value
			The value of the tick for which we want to retrieve the position
		@return 
			the screen position of the tick
	**/
	virtual long GetTickPos(double Value) const = 0;
	//! Converts a value on the axis to a screen position.
	/**
		This function is called internally only when the axis is in 
		discrete mode. This pure virtual function must be implemented for specific
		axes type.
		@param Value
			The value to convert
		@return the screen position of the value
	**/
	virtual long ValueToScreenDiscrete(double Value) const = 0;
	//! Converts a value on the axis to a screen position.
	/**
		This function is called internally only when the axis is in 
		standard mode. This virtual function can be overriden when the 
		axis doesn't display a continuous range of values (e.g. log axis).
		@param Value
			The value to convert
		@return the screen position of the value
	**/	
	virtual long ValueToScreenStandard(double Value) const;

	//! Retrieves the label for a specific tick.
	/**
		This pure virtual function must be implemented for specific
		axes type.		
		@param TickValue
			The tick value for which we need to get the label.
		@return A TChartString containing the label for the tick.
	**/
	virtual TChartString GetTickLabel(double TickValue) const = 0;

	//! Forces a recalculation of the tick increment.
	virtual void RefreshTickIncrement() = 0;
	//! Forces a recalculation of the first tick value.
	virtual void RefreshFirstTick() = 0;

	//! Retrieves the step information related to the scrollbar.
	/**
		This function can be implemented for specific axis types which
		should provide a behavior different than the standard behavior
		(for instance log axis). 
		@param iTotalSteps
			Stores the total number of steps for the scrollbar
		@param iCurrentStep
			Stores the current step index for the scrollbar
	**/
	virtual void GetScrollbarSteps(int& iTotalSteps, int& iCurrentStep);
	//! Sets the axis to the specified scrollbar step.
	/**
		This function can be implemented for specific axis types which
		should provide a behavior different than the standard behavior
		(for instance log axis). 
		@param iPreviousStep
			The previous scroll step.
		@param iCurrentStep
			The current scroll step to which the axis should be moved.
		@param bScrollInverted
			Specifies if the scroll is inverted or not.
	**/
	virtual void SetAxisToScrollStep(int iPreviousStep, int iCurrentStep, bool bScrollInverted);

	//! Pan the axis
	/**
		This function can be overriden in case the axis doesn't display
		a continuous range of values (e.g. log axis).
		@param PanStart
			The position of the start of the pan
		@param PanEnd
			The position of the end of the pan
	**/
	virtual void PanAxis(long PanStart, long PanEnd);
	//! Sets the min and max values of the axis due to a zoom operation.
	virtual void SetZoomMinMax(double Minimum, double Maximum);
	//! Reverts the zoom and pan settings.
	void UndoZoom();

	//! Retrieves the lenght (in pixels) of the axis
    long  GetAxisLenght() const;
	//! Retrieves the min and max values for all the series related to this axis
	void GetSeriesMinMax(double& Minimum, double& Maximum);
	//! Retrieves the screen min and max values for all the series related to this axis
	void GetSeriesScreenMinMax(double& Minimum, double& Maximum);

private:
	//! Refresh the axis if it is automatic
	/**
		@return true if the axis has changed
	**/
	bool RefreshAutoAxis();
	//! Refresh the axis if it is automatic for the screen only
	/**
		@return true if the axis has changed
	**/
	bool RefreshScreenAutoAxis();

	//! Returns the largest tick width and the largest tick heigh.
	/**
		The function returns a CSize object for which the x value
		is the largest width and the y value is the largest height.
		They do not necessarily belong to the same tick.
	**/
	CSize GetLargestTick(CDC* pDC);

	//! Sets the axis as a secondary axis.
	/**
		A secondary axis is on top for horizontal axis and on the
		right for vertical axis.
	**/
	void SetSecondary(bool bSecondary)	{ m_bIsSecondary = bSecondary; }
	//! Returns true if the axis is secondary.
	bool IsSecondary() const			{ return m_bIsSecondary; }
	//! Sets the axis to horizontal/vertical
	void SetHorizontal(bool bHorizontal);

	//! Draw the axis on the chart
	/**
	  @param pDC
		The DC used to draw the axis.
	**/
	void Draw(CDC* pDC);
	//! Draw the axis label
	/**
	  @param pDC
		The DC used to draw the axis.
	**/
	void DrawLabel(CDC* pDC);
	//! Draw a specific tick on the axis.
	/**
		@param pDC
			The DC used to draw the axis.
		@param dTickVal
			The tick value.
		The tick label will be drawn on the tick or between two ticks
		depending if the axis is set as standard or discrete.
	**/
	void DrawTick(CDC* pDC, double dTickVal);
	//! Check whether a specific label is still on the axis.
	/**
		@param dTickVal
			The tick value.
		The function returns false if the label is outside
		the axis range (and thus, should not be drawn).
	**/
	bool IsLabelOnAxis(double TickVal);

	//! Register a series that is associated with this axis.
	/**
		This is used when the axis is in automatic mode.
	**/
	void RegisterSeries(CChartSerie* pSeries);
	//! Unregister a series with this axis.
	void UnregisterSeries(CChartSerie* pSeries);

	//! Creates and attach a new scroll bar to the axis
	void CreateScrollBar();
	//! Update the scroll bar position depending on the plotting rect.
	void UpdateScrollBarPos();
	//! Refreshes the scroll bar.
	void RefreshScrollBar();

	//! Sets the parent charting control.
	void SetParent(CChartCtrl* pParent);

	//! Sets the axis size
	/**
		@param ControlRect
			The rectangle of the control
		@param MarginRect
			The rectangle in which the axis should be contained
	**/
	void SetAxisSize(const CRect& ControlRect, const CRect& MarginRect);
	//! Removes the margin needed for the axis from the full control rect.
	/**
		@param ControlRect
			The rectangle of the control
		@param MarginRect
			The rectangle in which the axis should be contained
		@param pDC
			The CDC used to draw the axis.
	**/
	int ClipMargin(CRect ControlRect,CRect& MarginRect,CDC* pDC);	
	//! Recalculate the axis properties. 
	/**
		This function simply calls RefreshTickIncrement and 
		RefreshFirstTick.
	**/
	void Recalculate();

protected:
	//! The parent chart control.
	CChartCtrl* m_pParentCtrl;

	//! Indicates if this is an horizontal or vertical axis
	bool m_bIsHorizontal;	  
	//! Indicates if the axis is inverted
	bool m_bIsInverted;	
	//! Indicates if the axis is automatic
//	bool m_bIsAutomatic;  
	//! Indicates the automatic mode of the axis
	EAxisAutoModes m_AutoMode;

	//! Indicates if the axis is visible or not
	bool m_bIsVisible;

	//! Specifies if the axis is secondary
	/**
		The secondary axis is either on the top (for horizontal
		axis) or on the right (for vertical axis) of the chart.
	**/
	bool m_bIsSecondary;	

	//! The axis max value
	double m_MaxValue;		
	//! The axis min value
	double m_MinValue;	
	//! Min value of the axis before it has been zoomed
	double m_UnzoomMin;		
	//! Max value of the axis before it has been zoomed
	double m_UnzoomMax;		

	//! Specify if the tick increment is manual or automatic
	bool m_bAutoTicks;	
	//! Specify if the axis has to be in discrete mode or not
	bool m_bDiscrete;

	//! Start position of the axis (in pixels)
	int m_StartPos;
	//! End position of the axis (in pixels)
	int m_EndPos;
	//! The rectangle in which the axis is contained.
	CRect m_AxisRect;

private:
	//! The font size used for the axis labels
	int  m_nFontSize;		
	//! The font face name used for the axis labels
	TChartString m_strFontName;
	//! The color used for the axis labels
	COLORREF m_TextColor;
	//! The color used for the axis line
	COLORREF m_AxisColor;

	//! The grid related to this axis
	CChartGrid*			m_pAxisGrid;
	//! The axis label associated with this axis
	CChartAxisLabel*	m_pAxisLabel;

	typedef std::list<CChartSerie*> SeriesList;
	//! List containing pointers to series related to this axis
	SeriesList m_pRelatedSeries;		

	//! Specify if the margin size is calculated automatically
	bool m_bAutoMargin;
	//! The margin size, used in manual mode
	int m_iMarginSize;

	//! Specifies if the zoom is enabled for this axis
	bool m_bZoomEnabled;
	//! The zoom limit (axis can't be zoomed under this limit)
	double m_dZoomLimit;

	//! The axis scrollbar
	CChartScrollBar* m_pScrollBar;
};

#endif  // _CHARTAXIS_H_