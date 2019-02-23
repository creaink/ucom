/*
 *
 *	ChartCrossHairCursor.h
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

#ifndef _CHARTCROSSHAIRCURSOR_H_
#define _CHARTCROSSHAIRCURSOR_H_

#include "ChartCursor.h"

class CChartAxis;

//! Specialization of a CChartCursor class for a cross-hair cursor.
/**
	A cross-hair cursor is a simple cross displayed in the plotting area.
	The cursor moves along with the mouse (but stay within the bounds of
	the plot area).<br>
	To create a cross-hair cursor, call the CreateCrossHairCursor from the
	CChartCtrl class.
**/
class CChartCrossHairCursor : public CChartCursor
{
	friend CChartCtrl;

protected:
	//! Called when the mouse is moved over the plot area.
	void OnMouseMove(CPoint mousePoint);
	//! Draws the cursor.
	void Draw(CDC* pDC);

private:
	//! Constructor
	/**
		@param pParent
			The parent charting control
		@param pHorizAxis
			The associated horizontal axis
		@param pVertAxis
			The associated vertical axis
	**/
	CChartCrossHairCursor(CChartCtrl* pParent, CChartAxis* pHorizAxis, CChartAxis* pVertAxis);
	//! Destructor
	~CChartCrossHairCursor();

	//! The associated horizontal axis
	CChartAxis* m_pHorizontalAxis;
	//! The associated vertical axis
	CChartAxis* m_pVerticalAxis;

	//! The current X screen position
	long m_lXPos;
	//! The current Y screen position
	long m_lYPos;
};

#endif  // _CHARTCROSSHAIRCURSOR_H_