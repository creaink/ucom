/*
 *
 *	ChartTitle.h
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

#if !defined(AFX_CHARTTITLE_H__49972787_6D28_4F81_A12F_420947456913__INCLUDED_)
#define AFX_CHARTTITLE_H__49972787_6D28_4F81_A12F_420947456913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
#include "ChartString.h"
#include "ChartFont.h"

class CChartCtrl;

//! This class is responsible for the titles displayed on the control.
/**
	Several lines can be displayed in the title, each one possibly with
	its own font. It is retrieved by calling the GetTitle() function 
	from the CChartCtrl class. 
**/
class CChartTitle  
{
	friend CChartCtrl;

public:
	//! Returns the number of lines in the title.
	size_t GetStringCount() const;
	//! Returns a specific line, specified by its index.
	TChartString GetString(size_t Index) const;
	//! Adds a new line to the title.
	void AddString(const TChartString& NewString);
	//! Removes all the lines displayed in the title.
	void RemoveAll();

	//! Sets the default font.
	/**
		@param iPointSize
			The font point size.
		@param strFaceName
			The font face name ("Times New Roman", "Arial", ...)
	**/
	void SetFont(int iPointSize, const TChartString& strFaceName);
	//! Sets the default font.
	/**
		This function allows to set extended font style by passing
		a CChartFont object.
		@param newFont
			The new font.
	**/
	void SetFont(const CChartFont& newFont);
	//! Sets the font for a specific line.
	/**
		@param iLineIndex
			The index of the line to set the font.
		@param iPointSize
			The font point size.
		@param strFaceName
			The font face name ("Times New Roman", "Arial", ...)
	**/	
	void SetLineFont(int iLineIndex, int iPointSize, const TChartString& strFaceName);
	//! Sets the font for a specific line.
	/**
		This function allows to set extended font style by passing
		a CChartFont object.
		@param iLineIndex
			The index of the line to set the font.
		@param newFont
			The new font.
	**/
	void SetLineFont(int iLineIndex, const CChartFont& newFont);

	//! Shows/hides the title.
	void SetVisible(bool bVisible) { m_bIsVisible = bVisible; }
	//! Returns true if the title is visible.
	bool IsVisible()  const         { return m_bIsVisible; }

	//! Returns the text color.
	COLORREF GetColor() const			   { return m_TextColor; }
	//! Sets the text color.
	void SetColor(COLORREF NewColor);

	//! Returns true if a screen point is in the region of the title.
	BOOL IsPointInside(const CPoint& screenPoint) const;

private:
	//! Constructor
	CChartTitle(CChartCtrl* pParent);
	//! Destructor
	virtual ~CChartTitle();

	//! Sets the rectangle in which to display the title.
	void SetTitleRect(const CRect& newRect)  { m_TitleRect = newRect; }
	//! Returns the size of the title lines.
	CSize GetSize(CDC* pDC);
	//! Draw the title.
	void Draw(CDC *pDC);

	//! The parent charting control.
	CChartCtrl* m_pParentCtrl;
	//! Vector containing all strings to display.
	std::vector<TChartString> m_StringArray;
	//! Map containing all the fonts used for the title lines.
	std::map<int, CChartFont> m_mapLineFonts;

	//! Specifies if the title is visible.
	bool m_bIsVisible;
	//! The default font used to draw the titles.
	CChartFont m_DefaultFont;
	//! The text color.
	COLORREF m_TextColor;
	//! The rectangle in which to draw the titles.
	CRect m_TitleRect;
};

#endif // !defined(AFX_CHARTTITLE_H__49972787_6D28_4F81_A12F_420947456913__INCLUDED_)
