/*
 *
 *	ChartCursor.h
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

#ifndef _CHARTCURSOR_H_
#define _CHARTCURSOR_H_

#include <list>

class CChartCtrl;
class CChartCursor;

//! Interface to implement in order to be notified about a cursor movement.
/**
	This class must be overriden and registered with a CChartCursor by
	calling RegisterListener.
**/
class CChartCursorListener
{
public:
	//! Default constructor
	CChartCursorListener()  { }
	//! Destructor
	virtual ~CChartCursorListener()  { }

	//! Pure virtual function to implement in order to be notified about a cursor movement.
	/**
		Note that not all cursor types have an X and a Y value, in which case, only
		the relevant information is passed, the other value will be 0.
		@param pCursor
			The cursor which was moved
		@param xValue
			The cursor xValue
		@param yValue
			The cursor yValue
	**/
	virtual void OnCursorMoved(CChartCursor* pCursor, double xValue, double yValue) = 0;
};

//! Base class for cursors which can be added to the chart control.
/**
	This class must be overriden for specific cursor types. This is already done
	for a cross-hair cursor and a dragline cursor. Each cursor is assigned an Id
	when it is added to the control.
**/
class CChartCursor
{
	friend CChartCtrl;

public:
	//! Sets the cursor color.
	void SetColor(COLORREF cursorColor);
	//! Retrieves the cursor Id.
	unsigned GetCursorId() const  { return m_uCursorId; }

	//! Registers a cursor listener with this cursor.
	void RegisterListener(CChartCursorListener* pListener);

protected:
	//! Default constructor
	CChartCursor(CChartCtrl* pParent); 
	//! Default destructor
	virtual ~CChartCursor();

	//! Pure virtual function that is called when the mouse moved on the plot area.
	/**
		This function must be overriden by child classes to take appropriate
		actions on the mouse move event.
	**/
	virtual void OnMouseMove(CPoint mousePoint) = 0;
	//! Virtual function that is called when the left mouse button is pressed.
	/**
		This function can be overriden by child classes to take appropriate
		actions on the mouse click event.
	**/
	virtual void OnMouseButtonDown(CPoint /*mousePoint*/)  { }
	//! Virtual function that is called when the left mouse button is released.
	/**
		This function can be overriden by child classes to take appropriate
		actions on the mouse click event.
	**/
	virtual void OnMouseButtonUp(CPoint /*mousePoint*/)    { }

	//! Pure virtual function that draws the cursor.
	virtual void Draw(CDC* pDC) = 0;
	//! Function that is called by the child classes when the cursor has been moved.
	/**
		This will notify all the listeners registered with the cursor.
	**/
	void CursorMoved(double newXValue, double newYValue);


	//! The color of the cursor.
	COLORREF m_colCursor;
	//! The parent charting control.
	CChartCtrl* m_pParentCtrl;

	//! Static variable holding the next free cursor Id.
	static unsigned m_uNextFreeId;
	//! The Id of this curosr.
	unsigned m_uCursorId;

	typedef std::list<CChartCursorListener*> TListenerList;
	//! List of all listeners registered with this cursor.
	TListenerList m_lstListeners;
};

#endif  // _CHARTCURSOR_H_
