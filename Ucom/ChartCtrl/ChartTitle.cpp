/*
 *
 *	ChartTitle.cpp
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

#include "stdafx.h"
#include "ChartTitle.h"
#include "ChartCtrl.h"
#include "Math.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartTitle::CChartTitle(CChartCtrl* pParent) 
{
	m_pParentCtrl = pParent;
	m_bIsVisible = true;
	m_TextColor = RGB(0,0,0);
}

CChartTitle::~CChartTitle()
{

}

void CChartTitle::SetFont(int iPointSize, const TChartString& strFaceName)
{
	m_DefaultFont.SetFont(strFaceName, iPointSize);
	m_pParentCtrl->RefreshCtrl();
}

void CChartTitle::SetFont(const CChartFont& newFont)
{
	m_DefaultFont = newFont;
	m_pParentCtrl->RefreshCtrl();
}

void CChartTitle::SetColor(COLORREF NewColor)
{
	m_TextColor = NewColor;
	m_pParentCtrl->RefreshCtrl();
}

void CChartTitle::SetLineFont(int iLineIndex, 
							  int iPointSize, 
							  const TChartString& strFaceName)
{
	CChartFont newFont(strFaceName,iPointSize);
	m_mapLineFonts[iLineIndex] = newFont;
}

void CChartTitle::SetLineFont(int iLineIndex, const CChartFont& newFont)
{
	m_mapLineFonts[iLineIndex] = newFont;
}

void CChartTitle::AddString(const TChartString& NewString)
{
	m_StringArray.push_back(NewString);
	m_pParentCtrl->RefreshCtrl();
}

size_t CChartTitle::GetStringCount() const
{
	return m_StringArray.size();
}

TChartString CChartTitle::GetString(size_t Index) const
{
	if ( (Index<0) || (Index>=m_StringArray.size()) )
		return _T("");
	return m_StringArray[Index];
}

void CChartTitle::RemoveAll()
{
	m_StringArray.clear();
	m_pParentCtrl->RefreshCtrl();
}

void CChartTitle::Draw(CDC *pDC)
{
	if (!pDC->GetSafeHdc())
		return;
	if (!m_bIsVisible)
		return;

	m_DefaultFont.SelectFont(pDC);
	COLORREF OldColor = pDC->SetTextColor(m_TextColor);
	int iPrevMode = pDC->SetBkMode(TRANSPARENT);

	//Draw all entries
	int YPos = 4;
	size_t TitleCount = m_StringArray.size();
	for (size_t i=0;i<TitleCount;i++)
	{
		map<int, CChartFont>::iterator iter = m_mapLineFonts.find(i);
		if (iter != m_mapLineFonts.end())
			iter->second.SelectFont(pDC);

		//Draw Text
		int TextWidth = pDC->GetTextExtent(m_StringArray[i].c_str()).cx;
		int TextHeigh = pDC->GetTextExtent(m_StringArray[i].c_str()).cy;

		int XPos = m_TitleRect.left + (int)fabs((m_TitleRect.left-m_TitleRect.right)/2.0) - TextWidth/2;

/*		if (m_bShadow)
		{
			pDC->SetTextColor(m_ShadowColor);
			pDC->ExtTextOut(XPos+m_iShadowDepth,m_TitleRect.top+YPos+m_iShadowDepth,
							ETO_CLIPPED,NULL,m_StringArray[i].c_str(),NULL);
			pDC->SetTextColor(m_TextColor);
		}*/
		pDC->ExtTextOut(XPos,m_TitleRect.top+YPos,ETO_CLIPPED,NULL,m_StringArray[i].c_str(),NULL);

		if (iter != m_mapLineFonts.end())
			iter->second.UnselectFont(pDC);

		YPos += TextHeigh + 2;
	}

	m_DefaultFont.UnselectFont(pDC);
	pDC->SetTextColor(OldColor);
	pDC->SetBkMode(iPrevMode);
}

CSize CChartTitle::GetSize(CDC *pDC)
{
	CSize TitleSize;

	if (!m_bIsVisible)
	{
		TitleSize.cx = TitleSize.cy = 0;
		return TitleSize;
	}

	int Height = 4;		//Upper space
	CSize TextSize = 0;
	int MaxTextWidth = 0;

	size_t TitleCount = m_StringArray.size();
	if (TitleCount==0)
	{
		TitleSize.cx = TitleSize.cy = 0;
		return TitleSize;
	}

	m_DefaultFont.SelectFont(pDC);
	for (size_t i=0;i<TitleCount;i++)
	{
		map<int, CChartFont>::iterator iter = m_mapLineFonts.find(i);
		if (iter != m_mapLineFonts.end())
			iter->second.SelectFont(pDC);

		TextSize = pDC->GetTextExtent(m_StringArray[i].c_str());
		Height += TextSize.cy + 2;
		if (TextSize.cx > MaxTextWidth)
			MaxTextWidth = TextSize.cx;

		if (iter != m_mapLineFonts.end())
			iter->second.UnselectFont(pDC);
	}

	TitleSize.cx = MaxTextWidth + 2;
	TitleSize.cy = Height;

	m_TitleRect.bottom = m_TitleRect.top + Height;

	m_DefaultFont.UnselectFont(pDC);
	return TitleSize;
}

BOOL CChartTitle::IsPointInside(const CPoint& screenPoint) const
{
	return m_TitleRect.PtInRect(screenPoint);
}
