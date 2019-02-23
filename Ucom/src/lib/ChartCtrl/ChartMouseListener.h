/*
 *
 *	ChartMouseListener.h
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

#ifndef _CHARTMOUSELISTENER_H_
#define _CHARTMOUSELISTENER_H_

#pragma warning( disable : 4100 )

//! Listener for mouse events occuring on the chart control.
/**
	This is an interface which must be implemented in order to receive
	mouse notifications. You can then register your class with the chart
	control by calling RegisterMouseListener.
**/
class CChartMouseListener
{
public:
	//! Constructor
	CChartMouseListener()  { }
	//! Destructor
	virtual ~CChartMouseListener()  { }

	//! Enumeration listing the type of mouse events
	enum MouseEvent
	{
		MouseMove,
		LButtonUp,
		LButtonDown,
		LButtonDoubleClick,
		RButtonUp,
		RButtonDown,
		RButtonDoubleClick,
	};

	//! Virtual function to implement in order to be notified when the title is clicked.
	/**
		@param mouseEvent
			The mouse event which occured
		@param point
			The screen point on which the event occured
	**/
	virtual void OnMouseEventTitle(MouseEvent mouseEvent, CPoint point) { }
	//! Virtual function to implement in order to be notified when an axis is clicked.
	/**
		@param mouseEvent
			The mouse event which occured
		@param point
			The screen point on which the event occured
		@param pAxisClicked
			The axis on which the event occured
	**/
	virtual void OnMouseEventAxis(MouseEvent mouseEvent, CPoint point, 
									CChartAxis* pAxisClicked) { }
	//! Virtual function to implement in order to be notified when the legend is clicked.
	/**
		@param mouseEvent
			The mouse event which occured
		@param point
			The screen point on which the event occured
	**/
	virtual void OnMouseEventLegend(MouseEvent mouseEvent, CPoint point) { }
	//! Virtual function to implement in order to be notified when the plotting area is clicked.
	/**
		@param mouseEvent
			The mouse event which occured
		@param point
			The screen point on which the event occured
	**/
	virtual void OnMouseEventPlotArea(MouseEvent mouseEvent, CPoint point) { }
};

#endif  // _CHARTMOUSELISTENER_H_