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
#include "ChartStandardAxis.h"
#include "ChartCtrl.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

CChartStandardAxis::CChartStandardAxis()
 : CChartAxis(), m_dFirstTickValue(0), 
   m_dTickIncrement(1.0), m_uDecCount(0)	
{
}

CChartStandardAxis::~CChartStandardAxis()
{
}


void CChartStandardAxis::SetTickIncrement(bool bAuto, double newIncrement)		
{ 
	m_bAutoTicks = bAuto;
	if (!m_bAutoTicks)
	{
		m_dTickIncrement = newIncrement; 
	
		int Zeros = (int)floor(log10(m_dTickIncrement));

		int Digits = 0;
		if (Zeros<0)		
		{
			//We must set decimal places. In the other cases, Digits will be 0.
			Digits = (int)fabs(Zeros*1.0);
		}
		SetDecimals(Digits);
	}
}

double CChartStandardAxis::GetFirstTickValue() const
{ 
	double dRetVal = m_dFirstTickValue;
	if (m_bDiscrete)
	{
		dRetVal = m_dFirstTickValue - m_dTickIncrement;
	}
	return dRetVal; 
}

bool CChartStandardAxis::GetNextTickValue(double dCurrentTick, double& dNextTick) const
{
	if (m_dTickIncrement==0)
		return false;

	dNextTick = dCurrentTick + m_dTickIncrement;
	if (dNextTick <= m_MaxValue)
		return true;
	else
		return false;
}

long CChartStandardAxis::ValueToScreenDiscrete(double dValue) const
{
	// In discrete mode, all values between two ticks are "directed"
	// to the middle of the interval.
	double precision = 0.0000000001;
	if (dValue < 0)
		precision = -0.0000000001;
	int tickNr = (int)((dValue+precision)/m_dTickIncrement);
	dValue = tickNr * m_dTickIncrement;
	
	dValue += m_dTickIncrement/2.0;
	return ValueToScreenStandard(dValue);
}

long CChartStandardAxis::GetTickPos(double TickVal) const
{
	// The tick is always at the same position,
	// even if the axis is discrete.
	return ValueToScreenStandard(TickVal);
}

TChartString CChartStandardAxis::GetTickLabel(double TickValue) const
{
	TChartStringStream ssLabel;
	ssLabel << fixed << setprecision(m_uDecCount) << TickValue;
	return ssLabel.str();
}

void CChartStandardAxis::RefreshTickIncrement()
{
	if (!m_bAutoTicks)
		return;

	if (m_MaxValue == m_MinValue)
	{
		m_dTickIncrement = 0;
		return;
	}

	int PixelSpace;
	if (m_bIsHorizontal)
		PixelSpace = 30;
	else
		PixelSpace = 20;

	int MaxTickNumber = (int)fabs((m_EndPos-m_StartPos)/PixelSpace * 1.0);

	//Calculate the appropriate TickSpace (1 tick every 30 pixel +/-)
	//Temporary tick increment
	double TempTickIncrement = (m_MaxValue-m_MinValue)/MaxTickNumber;

	// Calculate appropriate tickSpace (not rounded on 'strange values' but 
	// on something like 1, 2 or 5*10^X  where X is optimalized for showing the most
	// significant digits)
	int Zeros = (int)floor(log10(TempTickIncrement));
	double MinTickIncrement = pow(10.0,Zeros);

	int Digits = 0;
	if (Zeros<0)		
	{
		//We must set decimal places. In the other cases, Digits will be 0.
		Digits = (int)fabs(Zeros*1.0);
	}

	if (MinTickIncrement>=TempTickIncrement)
	{
		m_dTickIncrement = MinTickIncrement;
		SetDecimals(Digits);
	}
	else if (MinTickIncrement*2>=TempTickIncrement)
	{
		m_dTickIncrement = MinTickIncrement*2;
		SetDecimals(Digits);
	}
	else if (MinTickIncrement*5>=TempTickIncrement)
	{
		m_dTickIncrement = MinTickIncrement*5;
		SetDecimals(Digits);
	}
	else if (MinTickIncrement*10>=TempTickIncrement)
	{
		m_dTickIncrement = MinTickIncrement*10;
		if (Digits)
			SetDecimals(Digits-1);
		else
			SetDecimals(Digits);
	}
}

void CChartStandardAxis::RefreshFirstTick()
{
	if (m_dTickIncrement!=0)
	{
		if (m_MinValue == 0)
			m_dFirstTickValue = 0;
		else if (m_MinValue>0)
		{
			m_dFirstTickValue = (int)(m_MinValue/m_dTickIncrement) * m_dTickIncrement;
			while (m_dFirstTickValue<m_MinValue)
				m_dFirstTickValue += m_dTickIncrement;
		}
		else
		{
			m_dFirstTickValue = (int)(m_MinValue/m_dTickIncrement) * m_dTickIncrement;
			while (m_dFirstTickValue>m_MinValue)
				m_dFirstTickValue -= m_dTickIncrement;
			if (!(m_dFirstTickValue == m_MinValue))
				m_dFirstTickValue += m_dTickIncrement;
		}
	}
	else	// m_TickIncrement!=0
	{
		m_dFirstTickValue = m_MinValue;
	}
}
