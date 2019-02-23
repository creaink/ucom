/*
 *
 *	ChartLabel.cpp
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

#include "ChartCtrl.h"

template<class PointType>
CChartLabel<PointType>::CChartLabel(CChartCtrl* pParentCtrl, 
									CChartSerieBase<PointType>* pParentSeries) 
  : m_iFontSize(100),m_strFontName(_T("Microsoft Sans Serif")), 
    m_strLabelText(_T("")), m_pLabelProvider(NULL), m_pParentCtrl(pParentCtrl),	
	m_pParentSeries(pParentSeries)
{
}

template<class PointType>
CChartLabel<PointType>::~CChartLabel()
{
}

template<class PointType>
void CChartLabel<PointType>::SetLabelText(const TChartString& strText)  
{ 
	m_strLabelText = strText; 
	m_pParentCtrl->RefreshCtrl();
}

template<class PointType>
void CChartLabel<PointType>::SetFont(int nPointSize, const TChartString& strFaceName)
{
	m_iFontSize = nPointSize;
	m_strFontName = strFaceName;
	m_pParentCtrl->RefreshCtrl();
}

template<class PointType>
void CChartLabel<PointType>::SetVisisble(bool bVisible)
{
	m_bIsVisible = bVisible;
	m_pParentCtrl->RefreshCtrl();
}

