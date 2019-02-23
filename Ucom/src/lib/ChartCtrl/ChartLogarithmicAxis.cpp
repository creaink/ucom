/*
 *
 *	ChartLogarithmicAxis.cpp
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
#include "ChartLogarithmicAxis.h"
#include "ChartCtrl.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

CChartLogarithmicAxis::CChartLogarithmicAxis()
 : CChartAxis(), m_dFirstTickValue(1)
{
}

CChartLogarithmicAxis::~CChartLogarithmicAxis()
{
}

double CChartLogarithmicAxis::GetFirstTickValue() const
{
	double dRetVal = m_dFirstTickValue;
	if (m_bDiscrete)
		dRetVal = m_dFirstTickValue/10;
	return dRetVal; 
}

bool CChartLogarithmicAxis::GetNextTickValue(double dCurrentTick, 
											 double& dNextTick) const
{
	dNextTick = dCurrentTick * 10;
	if (dNextTick <= m_MaxValue)
		return true;
	else
		return false;
}

TChartString CChartLogarithmicAxis::GetTickLabel(double TickValue) const
{
	double fLogDecCount; 
	int    nLogDecCount; 

	fLogDecCount = log10(TickValue); 

	if (fLogDecCount < 0.0) 
		nLogDecCount = (int)(fabs(fLogDecCount) + 0.1); 
	else 
		nLogDecCount = 0; 

	TChartStringStream ssLabel;
	ssLabel << fixed << setprecision(nLogDecCount) << TickValue;
	return ssLabel.str();
}

long CChartLogarithmicAxis::ValueToScreenStandard(double Value) const
{
    long Offset = 0;
	long retVal = 0;

	Offset = (int)floor((log10(Value)-log10(m_MinValue)) * GetAxisLenght()/(log10(m_MaxValue)-log10(m_MinValue)) );
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

long CChartLogarithmicAxis::ValueToScreenDiscrete(double Value) const
{
	// In discrete mode, al values between two ticks are "directed"
	// to the middle of the interval.
	double lowTick = pow(10,floor(log10(Value)));
	double highTick = pow(10,floor(log10(Value*10)));

	long lowTickPos = ValueToScreenStandard(lowTick);
	long highTickPos = ValueToScreenStandard(highTick);
	return (lowTickPos + (highTickPos-lowTickPos)/2);
}

double CChartLogarithmicAxis::ScreenToValue(long ScreenVal) const
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

	return (pow(10.0,(AxisOffset *1.0 / GetAxisLenght()*(log10(m_MaxValue)-log10(m_MinValue)) ) + log10(m_MinValue)) );
}

void CChartLogarithmicAxis::PanAxis(long PanStart, long PanEnd)
{
	double StartVal = ScreenToValue(PanStart);
	double EndVal = ScreenToValue(PanEnd);

	double Factor = StartVal/EndVal;
	SetZoomMinMax(m_MinValue*Factor,m_MaxValue*Factor);
}

long CChartLogarithmicAxis::GetTickPos(double TickVal) const
{
	// The tick is always at the same position,
	// even if the axis is discrete.
	return ValueToScreenStandard(TickVal);
}

void CChartLogarithmicAxis::RefreshTickIncrement()
{
	// Do nothing. This is done by the user.
}

void CChartLogarithmicAxis::RefreshFirstTick()
{
	int LogBase = (int)log10(m_MinValue);
	m_dFirstTickValue = pow(10.0,LogBase);
}

void CChartLogarithmicAxis::GetScrollbarSteps(int& iTotalSteps, int& iCurrentStep)
{
	double SeriesMin=0, SeriesMax=0;
	GetSeriesMinMax(SeriesMin,SeriesMax);

	if ((m_MaxValue-m_MinValue) == 0 || (SeriesMax-SeriesMin)==0 ||
		(SeriesMin<=0) )
	{
		iTotalSteps = 1;
		iCurrentStep = 1;
	}
	else
	{
		double dStep = pow(m_MaxValue/m_MinValue,0.1);
		iTotalSteps = (int)ceil(log(SeriesMax/SeriesMin)/log10(dStep));
		iCurrentStep = (int)(log(m_MinValue/SeriesMin)/log10(dStep)); 
	}
}

void CChartLogarithmicAxis::SetAxisToScrollStep(int iPreviousStep, 
												int iCurrentStep, 
												bool bScrollInverted)
{
	double dStep = pow(m_MaxValue/m_MinValue,0.1);
	double dFactor = pow(dStep,(iCurrentStep - iPreviousStep));
	if (bScrollInverted)
		SetZoomMinMax(m_MinValue/dFactor,m_MaxValue/dFactor);
	else
		SetZoomMinMax(m_MinValue*dFactor,m_MaxValue*dFactor);
}
