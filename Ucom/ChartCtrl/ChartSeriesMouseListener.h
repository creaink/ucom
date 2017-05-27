/*
 *
 *	ChartSeriesMouseListener.h
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

#ifndef _CHARTSERIESMOUSELISTENER_H_
#define _CHARTSERIESMOUSELISTENER_H_

#include "ChartMouseListener.h"

//#pragma warning( disable : 4100 )

template <class PointType>
class CChartSerieBase;

//! Listener for mouse events occuring on a series.
/**
	This is an interface which must be implemented in order to receive
	mouse notifications. You can then register your class with the chart
	control by calling RegisterMouseListener.
**/
template <class PointType>
class CChartSeriesMouseListener
{
public:
	//! Constructor
	CChartSeriesMouseListener()  { }
	//! Destructor
	virtual ~CChartSeriesMouseListener()  { }

	//! Virtual function to implement in order to be notified when a mouse event occurs on a series.
	/**
		@param mouseEvent
			The mouse event which occured
		@param point
			The screen point on which the event occured
		@param pSerie
			The series on which the event occured
		@param uPointIndex
			The index of the point on which the event occured. In case the event
			did not occur on a specific point but on the series itself (e.g. clicking
			between two points on a line series), INVALID_POINT is passed for this
			parameter.
	**/
	virtual void OnMouseEventSeries(CChartMouseListener::MouseEvent mouseEvent, CPoint point,
					CChartSerieBase<PointType>* pSerie, unsigned uPointIndex) { }
};

#endif  // _CHARTSERIESMOUSELISTENER_H_