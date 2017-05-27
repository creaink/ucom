/*
 *
 *	ChartDragLineCursor.h
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

#ifndef _CHARTDRAGLINECURSOR_H_
#define _CHARTDRAGLINECURSOR_H_

#include "ChartCursor.h"

class CChartAxis;

//! Specialization of a CChartCursor class for a dragline cursor.
/**
	A dragline cursor is a simple vertical or horizontal line associated
	with a specific axis. The line can be moved if the user clicks on the
	line (and keeps the button pressed) and moves the mouse. Once the mouse
	button is released, the line doesn't move anymore.<br>
	To create a dragline cursor, call the CreateDragLineCursor from the
	CChartCtrl class.
**/
class CChartDragLineCursor : public CChartCursor
{
	friend CChartCtrl;

public:
	//! Sets the position (by value) of the cursor.
	void SetPosition(double dPosition);

protected:
	//! Called when the mouse is moved over the plot area.
	void OnMouseMove(CPoint mousePoint);
	//! Called when the mouse button is pressed over the plot area.
	void OnMouseButtonDown(CPoint mousePoint);
	//! Called when the mouse button is released over the plot area.
	void OnMouseButtonUp(CPoint mousePoint);

	//! Draw the cursor.
	void Draw(CDC* pDC);

private:
	//! Constructor
	CChartDragLineCursor(CChartCtrl* pParent, CChartAxis* pRelatedAxis);
	//! Destructor
	~CChartDragLineCursor();

	//! The axis to which this cursor is attached.
	CChartAxis* m_pRelatedAxis;

	//! flag specifying if the cursor is currently being dragged.
	bool m_bDragged;
	//! The current screen position of the cursor.
	long m_lPosition;
};

#endif  // _CHARTDRAGLINECURSOR_H_