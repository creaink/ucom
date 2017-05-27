/*
 *
 *	ChartAxisLabel.cpp
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
 *	History:
 *		- 24/08/2007: Bug fix in color of the label (text was always black)
 *
 *
 */

#include "stdafx.h"
#include "ChartAxisLabel.h"
#include "ChartCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartAxisLabel::CChartAxisLabel()
  : m_pParentCtrl(NULL), m_bIsVisible(true), m_TextColor(RGB(0,0,0)),
	m_bIsHorizontal(true), m_Font(), m_strLabelText(_T(""))
{
	m_Font.SetVertical(!m_bIsHorizontal);
}

CChartAxisLabel::~CChartAxisLabel()
{
}

void CChartAxisLabel::SetVisible(bool bVisible)
{
	m_bIsVisible = bVisible;
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

void CChartAxisLabel::SetColor(COLORREF NewColor)
{
	m_TextColor = NewColor; 
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

void CChartAxisLabel::SetText(const TChartString& NewText)  
{ 
	m_strLabelText = NewText; 
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

void CChartAxisLabel::SetFont(int nPointSize, const TChartString& strFaceName)
{
	m_Font.SetFont(strFaceName, nPointSize);
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

void CChartAxisLabel::SetHorizontal(bool bHorizontal)
{
	m_bIsHorizontal = bHorizontal;
	m_Font.SetVertical(!m_bIsHorizontal);
}

void CChartAxisLabel::SetFont(const CChartFont& newFont)
{
	m_Font = newFont;
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

CSize CChartAxisLabel::GetSize(CDC *pDC) const
{
	CSize LabelSize;
	LabelSize.cx = 0;
	LabelSize.cy = 0;

	if (!m_bIsVisible)
		return LabelSize;
	if (!pDC->GetSafeHdc())
		return LabelSize;
	if (m_strLabelText == _T(""))
		return LabelSize;

	m_Font.SelectFont(pDC);

	LabelSize = pDC->GetTextExtent(m_strLabelText.c_str());
	LabelSize.cx += 4;
	LabelSize.cy += 4;
	if (!m_bIsHorizontal)
	{
		int Width = LabelSize.cy;
		int Height = LabelSize.cx;
		LabelSize.cx = Width;
		LabelSize.cy = Height;
	}
	m_Font.UnselectFont(pDC);

	return LabelSize;
}

void CChartAxisLabel::Draw(CDC *pDC)
{
	if (!m_bIsVisible)
		return;
	if (!pDC->GetSafeHdc())
		return;
	if (m_strLabelText == _T(""))
		return;

	int iPrevMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF OldColor = pDC->SetTextColor(m_TextColor);

	m_Font.SelectFont(pDC);
	if (!m_bIsHorizontal)
	{
		pDC->ExtTextOut(m_TextRect.left + 2,m_TextRect.top,
					ETO_CLIPPED,NULL,m_strLabelText.c_str(),NULL);
	}
	else
	{		
		pDC->ExtTextOut(m_TextRect.left,m_TextRect.top + 2,
					ETO_CLIPPED,NULL,m_strLabelText.c_str(),NULL);
	}
	m_Font.UnselectFont(pDC);

	pDC->SetBkMode(iPrevMode);
	pDC->SetTextColor(OldColor);
}

void CChartAxisLabel::SetPosition(int LeftBorder, int TopBorder, CDC *pDC)
{
	CSize NewSize = GetSize(pDC);
	m_TextRect.top = TopBorder;
	m_TextRect.bottom = TopBorder + NewSize.cy;
	m_TextRect.left = LeftBorder;
	m_TextRect.right = LeftBorder + NewSize.cx;
}
