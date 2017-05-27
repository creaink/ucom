/*
 *
 *	ChartFont.cpp
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
 */

#include "stdafx.h"
#include "ChartFont.h"

CChartFont::CChartFont(const TChartString& strFaceName, int iPointSize)
  : m_strFaceName(strFaceName), m_iPointSize(iPointSize), m_bItalic(false),
    m_bBold(false), m_bUnderline(false), m_bVertical(false), m_Font(), m_bDirty(true), 
	m_pOldFont(NULL)
{
}

CChartFont::CChartFont(const CChartFont& copy)
{
	*this = copy;
}

CChartFont::CChartFont() 
  : m_strFaceName(_T("Microsoft Sans Serif")), m_iPointSize(100), m_bItalic(false),
    m_bBold(false), m_bUnderline(false), m_bVertical(false), m_Font(), m_bDirty(true), 
	m_pOldFont(NULL)
{
}

CChartFont::~CChartFont()
{
	m_Font.DeleteObject();
}

/*CFont* CChartFont::GetFont(CDC* pDC)
{
	if (!m_pFont)
		m_pFont = new CFont();

	if (m_bDirty)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));       
		lf.lfHeight = m_iPointSize;     
		_tcscpy_s(lf.lfFaceName,LF_FACESIZE-1 , m_strFaceName.c_str());
		lf.lfItalic = m_bItalic;
		lf.lfUnderline = m_bUnderline;
		if (m_bBold)
			lf.lfWeight = FW_BOLD;
		else
			lf.lfWeight = FW_NORMAL;

		m_pFont->CreatePointFontIndirect(&lf, pDC);
	}

	return m_pFont;
}*/

void CChartFont::operator=(const CChartFont& objectSrc)
{
	m_strFaceName = objectSrc.m_strFaceName;
	m_iPointSize = objectSrc.m_iPointSize;

	m_bItalic = objectSrc.m_bItalic;
	m_bBold = objectSrc.m_bBold;
	m_bUnderline = objectSrc.m_bUnderline;
	m_bVertical = objectSrc.m_bVertical;

	m_bDirty = true;
}

void CChartFont::SelectFont(CDC* pDC) const
{
	if (m_bDirty)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));       
		lf.lfHeight = m_iPointSize;     
#ifdef _CRT_INSECURE_DEPRECATE
		_tcscpy_s(lf.lfFaceName,LF_FACESIZE-1 , m_strFaceName.c_str());
#else
		_tcscpy(lf.lfFaceName, m_strFaceName.c_str());
#endif
		lf.lfItalic = m_bItalic;
		lf.lfUnderline = m_bUnderline;
		if (m_bBold)
			lf.lfWeight = FW_BOLD;
		else
			lf.lfWeight = FW_NORMAL;

		if (m_bVertical)
		{
			lf.lfOrientation = 900;
			lf.lfEscapement = 900;
		}

		m_Font.DeleteObject();
		m_Font.CreatePointFontIndirect(&lf, pDC);
		m_bDirty = false;
	}

	m_pOldFont = pDC->SelectObject(&m_Font);
}

void CChartFont::UnselectFont(CDC* pDC) const
{
	ASSERT(m_pOldFont);
	pDC->SelectObject(m_pOldFont);
	m_pOldFont = NULL;
}

void CChartFont::SetFont(const TChartString& strFaceName, 
						 int iPointSize,
						 bool bItalic, 
						 bool bBold, 
						 bool bUnderline)
{
	m_strFaceName = strFaceName;
	m_iPointSize = iPointSize;

	m_bItalic = bItalic;
	m_bBold = bBold;
	m_bUnderline = bUnderline;

	m_bDirty = true;
}

void CChartFont::SetVertical(bool bVertical)
{
	m_bVertical = bVertical;
	m_bDirty = true;
}
