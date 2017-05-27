/*
 *
 *	ChartFont.h
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

#ifndef _CHARTFONT_H_
#define _CHARTFONT_H_

#include "ChartString.h"
#include <afx.h>

//! Wrapper class for fonts with advanced properties (italic, bold or underlined).
class CChartFont
{
public:
	//! Copy constructor.
	CChartFont(const CChartFont& copy);
	//! Constructor
	/**
		@param strFaceName
			The font face name
		@param iPointSize
			The font point size
	**/
	CChartFont(const TChartString& strFaceName, int iPointSize);
	//! Default constructor
	/**
		Construct a font with the "Microsoft Sans Serif" face name and
		with a point size of 100.
	**/
	CChartFont();
	//! Destructor
	~CChartFont();

	//! Sets the font with extended properties.
	/**
		@param strFaceName
			The font face name
		@param iPointSize
			The font point size
		@param bItalic
			Specifies if the text is in italic
		@param bBold
			Specifies if the text is in bold
		@param bUnderline
			Specifies if the text is underlined
	**/
	void SetFont(const TChartString& strFaceName, int iPointSize, 
				 bool bItalic=false, bool bBold=false, bool bUnderline=false);

	//! Select this font in the device context passed in argument.
	/**
		This function stores the current font selected in the DC to 
		set it back when calling UnselectFont. This function is mainly
		used internally.
	**/
	void SelectFont(CDC* pDC) const;
	//! Reset the font to its original in the device context.
	void UnselectFont(CDC* pDC) const;

	//! Sets the text in vertical mode.
	/**
		This function is mainly used internally.
	**/
	void SetVertical(bool bVertical);

	//! Assignement operator.
	void operator=(const CChartFont& objectSrc);       

private:
	//! The font face name
	TChartString m_strFaceName;
	//! The font point size
	int m_iPointSize;

	//! Specifies if the font is italic
	bool m_bItalic;
	//! Specifies if the font is bold
	bool m_bBold;
	//! Specifies if the font is underlined
	bool m_bUnderline;

	//! Specifies if the font is vertical
	bool m_bVertical;

	//! Caches the current font
	mutable CFont m_Font;
	//! Specifies if the font information has been modified
	mutable bool m_bDirty;

	//! The old font which is stored when calling SelectFont
	mutable CFont* m_pOldFont;
};

#endif  // _CHARTFONT_H_