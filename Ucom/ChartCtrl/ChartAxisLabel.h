/*
 *
 *	ChartAxisLabel.h
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

#if !defined(AFX_CHARTAXISLABEL_H__0E5519C8_A2F4_4CED_9681_32A56B25D0C5__INCLUDED_)
#define AFX_CHARTAXISLABEL_H__0E5519C8_A2F4_4CED_9681_32A56B25D0C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartString.h"
#include "ChartFont.h"

class CChartCtrl;
class CChartAxis;

//! Draws the label of an axis
/**
	The label axis is displayed under or next to the tick values.
	The label is retrieved by calling CChartAxis::GetAxisLabel.
**/
class CChartAxisLabel 
{
	friend CChartAxis;

public:
	//! Sets the text of the axis label.
	void SetText(const TChartString& NewText);
	//! Retrieves the text of the axis label.
	TChartString GetText() const        { return m_strLabelText;    }

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

	//! Shows/hides the title.
	void SetVisible(bool bVisible);
	//! Returns true if the title is visible.
	bool IsVisible()  const         { return m_bIsVisible; }

	//! Retrieves the text color.
	COLORREF GetColor() const			   { return m_TextColor; }
	//! Sets the text color.
	void SetColor(COLORREF NewColor);

private:
	//! Constructor
	CChartAxisLabel();
	//! Destructor
	virtual ~CChartAxisLabel();

	//! Sets in horizontal or vertical mode.
	void SetHorizontal(bool bHorizontal);
	//! Sets the position of the label.
	void SetPosition(int LeftBorder, int TopBorder, CDC *pDC);
	//! Draws the label.
	void Draw(CDC* pDC);
	//! Retrieves the size of the label.
	CSize GetSize(CDC* pDC) const;


	//! The parent charting control.
	CChartCtrl*	m_pParentCtrl;		
	//! Specifies if the label is visible or not.
	bool        m_bIsVisible;

	//! The rectangle in which the label is displayed.
	CRect		m_TextRect;		
	//! The text color.
	COLORREF	m_TextColor;		

	//! Specifies if the axis is horizontal or not.
	bool m_bIsHorizontal;     
	//! The font used for the text label.
	CChartFont m_Font;

	//! The string to display for the label.
	TChartString m_strLabelText;
};

#endif // !defined(AFX_CHARTAXISLABEL_H__0E5519C8_A2F4_4CED_9681_32A56B25D0C5__INCLUDED_)
