/*
 *
 *	ChartBalloonLabel.h
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

#ifndef _CHARTBALLOONLABEL_H_ 
#define _CHARTBALLOONLABEL_H_

#include "ChartLabel.h"
#include "ChartFont.h"

//! Specialization of the CChartLabel to display a balloon label.
/**
	A balloon label is a label with a rounded rectangle area in which the 
	text is displayed and which is connected with a line to the point to
	which it is attached.
**/
template <class PointType>
class CChartBalloonLabel : public CChartLabel<PointType>
{
	friend CChartSerieBase<PointType>;

public:
	//! Sets the background color of the text area.
	void SetBackgroundColor(COLORREF colBackground);
	//! Retrieves the background color of the text area.
	COLORREF GetBackgroundColor() const		{ return m_colBackground; }
	//! Sets the color of the line connecting the point to the text area.
	void SetLineColor(COLORREF colArrow);	
	//! Retrieves the color of the line connecting the point to the text area.
	COLORREF GetLineColor() const			{ return m_colLine; }
	//! Sets the color of border's text area.
	void SetBorderColor(COLORREF colBorder);
	//! Retrieves the color of border's text area.
	COLORREF GetBorderColor() const			{ return m_colBorder; }

	//! Specifies if the text area is rounded or not.
	void SetRoundedRect(bool bRounded);
	//! Returns true if the text area is rounded.
	bool GetRoundedRect() const		{ return m_bRoundedRect; }

	//! Sets the font of the text.
	/**
		@param nPointSize
			The font point size.
		@param strFaceName
			The font face name ("Times New Roman", "Arial", ...)
	**/
	void SetFont(int nPointSize, const TChartString& strFaceName);
	//! Sets the font of the text.
	/**
		This function allows to set extended font style by passing
		a CChartFont object.
		@param newFont
			The new font.
	**/
	void SetFont(const CChartFont& newFont);

	//! Constructor
	CChartBalloonLabel(CChartCtrl* pParentCtrl, CChartSerieBase<PointType>* pParentSeries);
	//! Destructor
	~CChartBalloonLabel();

protected:
	//! Draw the label.
	void Draw(CDC* pDC, unsigned uPointIndex);

private:
	//! Color of the liune connecting the point to the text area.
	COLORREF m_colLine;
	//! Color of the text area's background.
	COLORREF m_colBackground;
	//! Color of border's text area.
	COLORREF m_colBorder;

	//! The font used for the text label.
	CChartFont m_Font;

	//! Specifies if the rectangle is rounded or not.
	bool m_bRoundedRect;
};

#include "ChartBalloonLabel.inl"

#endif  // _CHARTBALLOONLABEL_H_
