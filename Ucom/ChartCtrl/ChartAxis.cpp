/*
 *
 *	ChartAxis.cpp
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
#include "ChartAxis.h"
#include "ChartAxisLabel.h"
#include "ChartGrid.h"
#include "ChartCtrl.h"

#include "Math.h"
#include <sstream>

using namespace std;

CChartAxis::CChartAxis() 
 : m_pParentCtrl(NULL), m_bIsHorizontal(true), m_bIsInverted(false), 
   m_AutoMode(NotAutomatic), m_bIsVisible(true), m_bIsSecondary(false), 
   m_MaxValue(0), m_MinValue(0), m_UnzoomMax(0), m_UnzoomMin(0), 
   m_bAutoTicks(true), m_bDiscrete(false),m_StartPos(0), m_EndPos(0), 
   m_nFontSize(80), m_strFontName(_T("Microsoft Sans Serif")), m_TextColor(0), 
   m_bAutoMargin(true), m_iMarginSize(0), m_bZoomEnabled(true), 
   m_dZoomLimit(0.001), m_pScrollBar(NULL), m_AxisColor(RGB(0,0,0))
{
	m_pAxisGrid = new CChartGrid();
	m_pAxisLabel = new CChartAxisLabel();
}

CChartAxis::~CChartAxis()
{
	if (m_pAxisGrid)
	{
		delete m_pAxisGrid;
		m_pAxisGrid = NULL;
	}
	if (m_pAxisLabel)
	{
		delete m_pAxisLabel;
		m_pAxisLabel = NULL;
	}

	if (m_pScrollBar)
	{
		delete m_pScrollBar;
		m_pScrollBar = NULL;
	}
}


int CChartAxis::GetPosition()
{
	if (m_bIsHorizontal)
	{
		if (m_bIsSecondary)
			return 0;
		else
			return 100;
	}
	else
	{
		if (m_bIsSecondary)
			return 100;
		else
			return 0;
	}
}

void CChartAxis::SetParent(CChartCtrl* pParent)
{
	m_pParentCtrl = pParent;
	m_pAxisGrid->m_pParentCtrl = pParent;
	m_pAxisLabel->m_pParentCtrl = pParent;
}

void CChartAxis::SetHorizontal(bool bHorizontal)  
{ 
	m_bIsHorizontal = bHorizontal; 
	m_pAxisGrid->m_bIsHorizontal = bHorizontal;
	m_pAxisLabel->SetHorizontal(bHorizontal);
}

void CChartAxis::Draw(CDC *pDC)
{
	if (!m_bIsVisible)
		return;
	if (pDC->GetSafeHdc() == NULL)
		return;

	// Initialize the different GDI objects
	CPen SolidPen(PS_SOLID,0,m_AxisColor);
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize,m_strFontName.c_str(),pDC) ;
	CPen* pOldPen = pDC->SelectObject(&SolidPen);
	CFont* pOldFont = pDC->SelectObject(&NewFont);
	COLORREF OldTextColor = pDC->SetTextColor(m_TextColor);
	int iPrevMode = pDC->SetBkMode(TRANSPARENT);

	// Draw the axis line 
	int Pos = 0;
	if (m_bIsHorizontal)
	{
		if (!m_bIsSecondary)
			Pos = m_AxisRect.top+1;
		else
			Pos = m_AxisRect.bottom-1;
		pDC->MoveTo(m_StartPos,Pos);
		pDC->LineTo(m_EndPos,Pos);
	}
	else
	{
		if (!m_bIsSecondary)
			Pos = m_AxisRect.right-1;
		else
			Pos = m_AxisRect.left+1;
		pDC->MoveTo(Pos,m_StartPos);
		pDC->LineTo(Pos,m_EndPos);
	}

	// Draw the label
	DrawLabel(pDC);
	// Clear the ticks on the grid
	m_pAxisGrid->ClearTicks();

	// Now draw all the ticks and their label.
	if (m_MaxValue == m_MinValue)
		DrawTick(pDC,m_MinValue);
	else
	{
		double TickValue = GetFirstTickValue();
		do 
		{
			DrawTick(pDC,TickValue);
		} while (GetNextTickValue(TickValue, TickValue));
	}

	// Draw the grid.
	m_pAxisGrid->Draw(pDC);

	// Reset the GDI objects
	pDC->SelectObject(pOldPen);
	SolidPen.DeleteObject();
	pDC->SelectObject(pOldFont);
	NewFont.DeleteObject();
	pDC->SetTextColor(OldTextColor);
	pDC->SetBkMode(iPrevMode);
}

void CChartAxis::DrawTick(CDC* pDC, 
						  double dTickVal)
{
	long TickPos = GetTickPos(dTickVal);
	long lLabelPos = ValueToScreen(dTickVal);
	TChartString strBuffer = GetTickLabel(dTickVal);
	CSize TextSize = pDC->GetTextExtent(strBuffer.c_str());
	CSize LabelSize = m_pAxisLabel->GetSize(pDC);

	bool bLabelOnAxis = IsLabelOnAxis(dTickVal);
	bool bTickOnAxis = true;
	if (m_bIsHorizontal)
	{
		if (TickPos<m_StartPos || TickPos>m_EndPos)
			bTickOnAxis = false;
		if (!m_bIsSecondary)
		{
			if (bTickOnAxis)
			{
				pDC->MoveTo(TickPos,m_AxisRect.top+1);
				pDC->LineTo(TickPos,m_AxisRect.top+4);
			}
			if (bLabelOnAxis)
			{
				pDC->ExtTextOut(lLabelPos-TextSize.cx/2,m_AxisRect.top+5,
					ETO_CLIPPED|ETO_OPAQUE,NULL,strBuffer.c_str(),NULL);
			}
		}
		else
		{
			if (bTickOnAxis)
			{
				pDC->MoveTo(TickPos,m_AxisRect.bottom-1);
				pDC->LineTo(TickPos,m_AxisRect.bottom-4);
			}
			if (bLabelOnAxis)
			{
				pDC->ExtTextOut(lLabelPos-TextSize.cx/2,m_AxisRect.bottom-5-TextSize.cy,
					ETO_CLIPPED|ETO_OPAQUE,NULL,strBuffer.c_str(),NULL);
			}
		}
	}
	else
	{
		if (TickPos>m_StartPos || TickPos<m_EndPos)
			bTickOnAxis = false;
		if (!m_bIsSecondary)
		{
			if (bTickOnAxis)
			{
				pDC->MoveTo(m_AxisRect.right-1,TickPos);
				pDC->LineTo(m_AxisRect.right-4,TickPos);
			}
			if (bLabelOnAxis)
			{
				pDC->ExtTextOut(m_AxisRect.left+LabelSize.cx+4,lLabelPos-TextSize.cy/2,
					ETO_CLIPPED|ETO_OPAQUE,NULL,strBuffer.c_str(),NULL);
			}
		}
		else
		{
			if (bTickOnAxis)
			{
				pDC->MoveTo(m_AxisRect.left+1,TickPos);
				pDC->LineTo(m_AxisRect.left+4,TickPos);
			}
			if (bLabelOnAxis)
			{
				pDC->ExtTextOut(m_AxisRect.left+6,lLabelPos-TextSize.cy/2,
					ETO_CLIPPED|ETO_OPAQUE,NULL,strBuffer.c_str(),NULL);
			}
		}
	}
	m_pAxisGrid->AddTick(TickPos);
}

bool CChartAxis::IsLabelOnAxis(double TickVal)
{
	long lLabelPos = ValueToScreen(TickVal);
	bool bLabelOnAxis = true;
	if (m_bIsHorizontal)
	{
		if (lLabelPos<m_StartPos || lLabelPos>m_EndPos)
			bLabelOnAxis = false;
	}
	else
	{
		if (lLabelPos>m_StartPos || lLabelPos<m_EndPos)
			bLabelOnAxis = false;
	}
	return bLabelOnAxis;
}

void CChartAxis::DrawLabel(CDC* pDC)
{
	// Draw the axis label.
	CSize LabelSize = m_pAxisLabel->GetSize(pDC);
	int HalfAxisPos = (int)fabs((m_EndPos + m_StartPos)/2.0);
	int XPos = 0;
	int YPos = 0;
	if (m_bIsHorizontal)
	{
		if (!m_bIsSecondary)
		{
			CSize TextSize = GetLargestTick(pDC);
			YPos = m_AxisRect.top  + TextSize.cy + 2;
			XPos = HalfAxisPos - LabelSize.cx/2;
		}
		else
		{
			YPos = m_AxisRect.top  + 0;
			XPos = HalfAxisPos - LabelSize.cx/2;
		}
	}
	else
	{
		if (!m_bIsSecondary)
		{
			YPos = HalfAxisPos + LabelSize.cy/2;
			XPos = m_AxisRect.left + 0;
		}
		else
		{
			YPos = HalfAxisPos + LabelSize.cy/2;
			XPos = m_AxisRect.right - LabelSize.cx - 2;
		}
	}
	m_pAxisLabel->SetPosition(XPos,YPos,pDC);
	m_pAxisLabel->Draw(pDC);
}

CSize CChartAxis::GetLargestTick(CDC* pDC)
{
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize,m_strFontName.c_str(),pDC);
	CFont* pOldFont = pDC->SelectObject(&NewFont);

	CSize MaxSize(0,0);
	if (m_MaxValue == m_MinValue)
	{
		TChartString strLabel = GetTickLabel(m_MinValue);
		MaxSize = pDC->GetTextExtent(strLabel.c_str(),strLabel.size());
	}
	else
	{
		double TickValue = GetFirstTickValue();
		do 
		{
			if (IsLabelOnAxis(TickValue))
			{
				TChartString strLabel = GetTickLabel(TickValue);
				CSize TextSize = pDC->GetTextExtent(strLabel.c_str(),strLabel.size());
				if (TextSize.cy > MaxSize.cy)
					MaxSize.cy = TextSize.cy;
				if (TextSize.cx > MaxSize.cx)
					MaxSize.cx = TextSize.cx;
			}
		} while (GetNextTickValue(TickValue, TickValue));
	}

	pDC->SelectObject(pOldFont);
	NewFont.DeleteObject();
	return MaxSize;
}

void CChartAxis::SetInverted(bool bInverted)
{
	m_bIsInverted = bInverted;
	RefreshScrollBar();
	m_pParentCtrl->RefreshCtrl();
}
	
void CChartAxis::SetAutomatic(bool bAutomatic)
{
//	m_bIsAutomatic = bAutomatic; 
	if (bAutomatic)
	{
		m_AutoMode = FullAutomatic;
		m_MinValue = m_MaxValue = 0;
	} 
	else 
		m_AutoMode = NotAutomatic;

	if (RefreshAutoAxis())
		m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::SetAutomaticMode(EAxisAutoModes AutoMode)
{
	m_AutoMode = AutoMode;
	if (m_AutoMode != NotAutomatic)
		m_MinValue = m_MaxValue = 0;

	if (RefreshAutoAxis())
		m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::SetDiscrete(bool bDiscrete)
{
	m_bDiscrete = bDiscrete;
	m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::SetMinMax(double Minimum, double Maximum)
{
	ASSERT(Maximum>=Minimum);

	m_MinValue = m_UnzoomMin = Minimum;
	m_MaxValue = m_UnzoomMax = Maximum;
	RefreshScrollBar();
	m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::RegisterSeries(CChartSerie* pSeries)
{
	// First check if the series is already present in the list
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if ( (*iter) == pSeries)
			return;
	}

	m_pRelatedSeries.push_back(pSeries);
}

void CChartAxis::UnregisterSeries(CChartSerie* pSeries)
{
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if ( (*iter) == pSeries)
		{
			m_pRelatedSeries.erase(iter);
			return;
		}
	}
}

bool CChartAxis::RefreshAutoAxis()
{
	RefreshScrollBar();
	bool bNeedRefresh = false;
	if (m_AutoMode == NotAutomatic)
		return bNeedRefresh;

	double SeriesMin = 0;
	double SeriesMax = 0;
	if (m_AutoMode == FullAutomatic)
		GetSeriesMinMax(SeriesMin, SeriesMax);
	if (m_AutoMode == ScreenAutomatic)
		GetSeriesScreenMinMax(SeriesMin, SeriesMax);

	if ( (SeriesMax!=m_MaxValue) || (SeriesMin!=m_MinValue) )
		SetMinMax(SeriesMin,SeriesMax);

	return bNeedRefresh;
}

bool CChartAxis::RefreshScreenAutoAxis()
{
	RefreshScrollBar();
	bool bNeedRefresh = false;
	if (m_AutoMode != ScreenAutomatic)
		return bNeedRefresh;
	return RefreshAutoAxis();
}

void CChartAxis::GetSeriesMinMax(double& Minimum, double& Maximum)
{
	Minimum = 0;
	Maximum = 0;
	double TempMin = 0;
	double TempMax = 0;
	
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	if (iter != m_pRelatedSeries.end())
	{
		if (m_bIsHorizontal)
			(*iter)->GetSerieXMinMax(Minimum,Maximum);
		else
			(*iter)->GetSerieYMinMax(Minimum,Maximum);
	}

	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if (m_bIsHorizontal)
			(*iter)->GetSerieXMinMax(TempMin,TempMax);
		else
			(*iter)->GetSerieYMinMax(TempMin,TempMax);

		if (TempMin < Minimum)
			Minimum = TempMin;
		if (TempMax > Maximum)
			Maximum = TempMax;
	}
}

void CChartAxis::GetSeriesScreenMinMax(double& Minimum, double& Maximum)
{
	Minimum = 0;
	Maximum = 0;
	double TempMin = 0;
	double TempMax = 0;
	
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	if (iter != m_pRelatedSeries.end())
	{
		if (m_bIsHorizontal)
			(*iter)->GetSerieXScreenMinMax(Minimum,Maximum);
		else
			(*iter)->GetSerieYScreenMinMax(Minimum,Maximum);
	}

	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if (m_bIsHorizontal)
			(*iter)->GetSerieXScreenMinMax(TempMin,TempMax);
		else
			(*iter)->GetSerieYScreenMinMax(TempMin,TempMax);

		if (TempMin < Minimum)
			Minimum = TempMin;
		if (TempMax > Maximum)
			Maximum = TempMax;
	}
}

long CChartAxis::ValueToScreen(double Value) const
{
    long Offset = 0;
	long retVal = 0;
	if (m_MaxValue==m_MinValue)
	{
		Offset = (int)fabs((m_EndPos-m_StartPos)/2.0);
		if (m_bIsHorizontal)
			retVal =  m_StartPos + Offset;
		else
			retVal =  m_StartPos - Offset;
	}
	else if (!m_bDiscrete)
        retVal = ValueToScreenStandard(Value);
	else
		retVal = ValueToScreenDiscrete(Value);

	return retVal;
}

long CChartAxis::ValueToScreenStandard(double Value) const
{
    long Offset = 0;
	long retVal = 0;

	Offset = (int)floor( (Value - m_MinValue) * GetAxisLenght()/(m_MaxValue-m_MinValue) );
	if (m_bIsHorizontal)
	{
		if (!m_bIsInverted)
			retVal = (m_StartPos + Offset);
		else
			retVal = (m_EndPos - Offset);
	}
	else
	{
		if (!m_bIsInverted)
			retVal = (m_StartPos - Offset);
		else
			retVal = (m_EndPos + Offset);
	}
	return retVal;
}

double CChartAxis::ScreenToValue(long ScreenVal) const
{
	if (m_MaxValue==m_MinValue)
		return m_MinValue;

    int AxisOffset = 0;
    if (!m_bIsHorizontal)
    {
        if (m_bIsInverted)
			AxisOffset = ScreenVal - m_EndPos;      
        else
			AxisOffset = m_StartPos - ScreenVal;     
    }
    else
    {
        if (!m_bIsInverted)
           AxisOffset = ScreenVal - m_StartPos;
        else
            AxisOffset = m_EndPos - ScreenVal;
    }

	return ( (AxisOffset * 1.0 / GetAxisLenght()*(m_MaxValue-m_MinValue)) + m_MinValue);
}

void CChartAxis::PanAxis(long PanStart, long PanEnd)
{
	double StartVal = ScreenToValue(PanStart);
	double EndVal = ScreenToValue(PanEnd);

	double Shift = StartVal - EndVal;
	SetZoomMinMax(m_MinValue+Shift,m_MaxValue+Shift);
}

void CChartAxis::SetZoomMinMax(double Minimum, double Maximum)
{
	if (!m_bZoomEnabled)
		return;
	if (m_MinValue == m_MaxValue)
		return;

	ASSERT(Maximum>=Minimum);

	m_MinValue = Minimum;
	if ( (Maximum - Minimum) < m_dZoomLimit)
		m_MaxValue = m_MinValue + m_dZoomLimit;
	else
		m_MaxValue = Maximum;
	RefreshScrollBar();
}

long CChartAxis::GetAxisLenght() const
{
    long Length = (long)fabs( (m_EndPos-m_StartPos) * 1.0);
    return Length;
}

void CChartAxis::SetVisible(bool bVisible)
{
	m_bIsVisible = bVisible;
	m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::CreateScrollBar()
{
	m_pScrollBar = new CChartScrollBar(this);
	m_pScrollBar->CreateScrollBar(m_pParentCtrl->GetPlottingRect());
}

void CChartAxis::UpdateScrollBarPos()
{
	CRect PlottingRect = m_pParentCtrl->GetPlottingRect();
	PlottingRect.top++; PlottingRect.left++;

	// TODO: check if other toolbars are already present
	// on other axes.
	CRect Temp;
	m_pScrollBar->GetWindowRect(&Temp);
	if (m_bIsHorizontal && !m_bIsSecondary)
		PlottingRect.top = PlottingRect.bottom - Temp.Height();
	if (!m_bIsHorizontal && !m_bIsSecondary)
		PlottingRect.right = PlottingRect.left + Temp.Width();
	if (m_bIsHorizontal && m_bIsSecondary)
		PlottingRect.bottom = PlottingRect.top + Temp.Height();
	if (!m_bIsHorizontal && m_bIsSecondary)
		PlottingRect.left = PlottingRect.right - Temp.Width();

	m_pScrollBar->MoveWindow(&PlottingRect);
}

void CChartAxis::RefreshScrollBar()
{
	if (m_pScrollBar)
		m_pScrollBar->Refresh();
}

void CChartAxis::SetTextColor(COLORREF NewColor)
{
	m_TextColor = NewColor; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::SetAxisColor(COLORREF NewColor)
{
	m_AxisColor = NewColor; 
	m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::SetFont(int nPointSize, 
						 const TChartString& strFaceName)
{
	m_nFontSize = nPointSize;
	m_strFontName = strFaceName;
	m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::SetMarginSize(bool bAuto, int iNewSize)
{
	m_bAutoMargin = bAuto;
	m_iMarginSize = iNewSize;
	m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::EnableScrollBar(bool bEnabled)
{
	if (m_pScrollBar)
	{
		m_pScrollBar->SetEnabled(bEnabled);
		if (bEnabled)
			m_pScrollBar->ShowWindow(SW_SHOW);
		else
			m_pScrollBar->ShowWindow(SW_HIDE);
	}
}

void CChartAxis::SetAutoHideScrollBar(bool bAutoHide)
{
	if (m_pScrollBar)
		m_pScrollBar->SetAutoHide(bAutoHide);
}

bool CChartAxis::GetAutoHideScrollBar() const
{
	if (m_pScrollBar)
		return (m_pScrollBar->GetAutoHide()); 
	else
		return false;
}

void CChartAxis::UndoZoom()
{
	SetMinMax(m_UnzoomMin,m_UnzoomMax);
}

void CChartAxis::SetAxisSize(const CRect& ControlRect, 
							 const CRect& MarginRect)
{
	if (m_bIsHorizontal)
	{
		m_StartPos = MarginRect.left;	
		m_EndPos = MarginRect.right;

		if (!m_bIsSecondary)
		{
			m_AxisRect = ControlRect;
			m_AxisRect.top = MarginRect.bottom;
		}
		else
		{
			m_AxisRect = ControlRect;
			m_AxisRect.bottom = MarginRect.top;
		}
	}
	else
	{
		m_StartPos = MarginRect.bottom;
		m_EndPos = MarginRect.top;

		if (!m_bIsSecondary)
		{
			m_AxisRect = ControlRect;
			m_AxisRect.right = MarginRect.left;
		}
		else
		{
			m_AxisRect = ControlRect;
			m_AxisRect.left = MarginRect.right;
		}
	}
}

int CChartAxis::ClipMargin(CRect ControlRect, CRect& MarginRect,CDC* pDC)
{
	if (!m_bIsVisible)
		return 0;

	int Size = 0;	
	CSize TickSize = GetLargestTick(pDC);
	CSize LabelSize = m_pAxisLabel->GetSize(pDC);

	if (m_bIsHorizontal)
	{
		if (!m_bAutoMargin)
			Size = m_iMarginSize;
		else
		{
			Size += 4 + 2;		//Space above and under the text

			Size += TickSize.cy;
			Size += LabelSize.cy;

			m_iMarginSize = Size;
		}

		if (!m_bIsSecondary)
		{
			ControlRect.bottom -= Size;
			ControlRect.right -= TickSize.cx/2+3;

			if (ControlRect.bottom < MarginRect.bottom)
				MarginRect.bottom = ControlRect.bottom;
			if (ControlRect.right < MarginRect.right)
				MarginRect.right = ControlRect.right;
		}
		else
		{
			ControlRect.top += Size;
			ControlRect.right -= TickSize.cx/2+3;

			if (ControlRect.top > MarginRect.top)
				MarginRect.top = ControlRect.top;
			if (ControlRect.right < MarginRect.right)
				MarginRect.right = ControlRect.right;
		}

	}
	else
	{
		if (!m_bAutoMargin)
			Size = m_iMarginSize;
		else
		{
			Size += 7 + 1;		//Space before and after the text + Tick

			Size += TickSize.cx;
			Size += LabelSize.cx + 2;
			m_iMarginSize = Size;
		}

		if (!m_bIsSecondary)
		{
			ControlRect.left += Size;
			ControlRect.top += TickSize.cy/2+3;

			if (ControlRect.top > MarginRect.top)
				MarginRect.top = ControlRect.top;
			if (ControlRect.left > MarginRect.left)
				MarginRect.left = ControlRect.left;
		}
		else
		{
			ControlRect.right -= Size;
			ControlRect.top += TickSize.cy/2+3;

			if (ControlRect.top > MarginRect.top)
				MarginRect.top = ControlRect.top;
			if (ControlRect.right < MarginRect.right)
				MarginRect.right = ControlRect.right;
		}
	}

	return Size;
}

void CChartAxis::Recalculate()
{
	if (m_bAutoTicks)
		RefreshTickIncrement();
	RefreshFirstTick();
}

void CChartAxis::GetScrollbarSteps(int& iTotalSteps, 
								   int& iCurrentStep)
{
	double SeriesMin=0, SeriesMax=0;
	GetSeriesMinMax(SeriesMin,SeriesMax);

	if ((m_MaxValue-m_MinValue) == 0 || (SeriesMax-SeriesMin)==0 )
	{
		iTotalSteps = 1;
		iCurrentStep = 1;
	}
	else
	{
		double dStep = (m_MaxValue - m_MinValue) / 10.0;
		iTotalSteps = (int)ceil((SeriesMax - SeriesMin)/dStep);
		iCurrentStep = (int)(iTotalSteps * ((m_MinValue - SeriesMin)/(SeriesMax-SeriesMin)));
	}
}

void CChartAxis::SetAxisToScrollStep(int iPreviousStep, 
									 int iCurrentStep, 
									 bool bScrollInverted)
{
	double dStep = (m_MaxValue - m_MinValue) / 10.0;
	double dOffset = (iCurrentStep - iPreviousStep) * dStep;
	if (bScrollInverted)
		SetZoomMinMax(m_MinValue-dOffset,m_MaxValue-dOffset);
	else
		SetZoomMinMax(m_MinValue+dOffset,m_MaxValue+dOffset);
	m_pParentCtrl->RefreshScreenAutoAxes();
}

BOOL CChartAxis::IsPointInside(const CPoint& screenPoint) const
{
	return m_AxisRect.PtInRect(screenPoint);
}
