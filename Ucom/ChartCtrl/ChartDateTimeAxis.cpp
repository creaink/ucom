/*
 *
 *	ChartDateTimeAxis.cpp
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
#include "ChartDateTimeAxis.h"
#include "ChartCtrl.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

CChartDateTimeAxis::CChartDateTimeAxis()
 : CChartAxis(), m_strDTTickFormat(), 
   m_bAutoTickFormat(true), m_BaseInterval(tiDay), 
   m_iDTTickIntervalMult(1), m_dFirstTickValue(0)
{
	m_ReferenceTick.SetDate(2000,1,1);
}

CChartDateTimeAxis::~CChartDateTimeAxis()
{
}


void CChartDateTimeAxis::SetTickIncrement(bool bAuto, 
										  TimeInterval Interval, 
										  int Multiplier)
{
	m_bAutoTicks = bAuto;
	if (!m_bAutoTicks)
	{
		m_BaseInterval = Interval;
		m_iDTTickIntervalMult = Multiplier;
	}
}

void CChartDateTimeAxis::SetTickLabelFormat(bool bAutomatic, 
											const TChartString& strFormat)
{
	m_bAutoTickFormat = bAutomatic;
	m_strDTTickFormat = strFormat;
	m_pParentCtrl->RefreshCtrl();
}

double CChartDateTimeAxis::GetFirstTickValue() const
{
	double dRetVal = m_dFirstTickValue;
	if (m_bDiscrete)
	{
		COleDateTime dtTick((DATE)m_dFirstTickValue);
		COleDateTimeSpan dtSpan;
		switch (m_BaseInterval)
		{
		case tiSecond:
			dtSpan.SetDateTimeSpan(0,0,0,m_iDTTickIntervalMult);
			dtTick -= dtSpan;
			break;
		case tiMinute:
			dtSpan.SetDateTimeSpan(0,0,m_iDTTickIntervalMult,0);
			dtTick -= dtSpan;
			break;
		case tiHour:
			dtSpan.SetDateTimeSpan(0,m_iDTTickIntervalMult,0,0);
			dtTick -= dtSpan;
			break;
		case tiDay:
			dtSpan.SetDateTimeSpan(m_iDTTickIntervalMult,0,0,0);
			dtTick -= dtSpan;
			break;
		case tiMonth:
			dtTick = AddMonthToDate(dtTick,-m_iDTTickIntervalMult);
			break;
		case tiYear:
			dtTick = AddMonthToDate(dtTick,-12*m_iDTTickIntervalMult);
			break;
		}
	}
	return dRetVal; 

}

bool CChartDateTimeAxis::GetNextTickValue(double dCurrentTick, double& dNextTick) const
{
	if (m_MinValue == m_MaxValue)
		return false;

	COleDateTime dtTick((DATE)dCurrentTick);
	COleDateTimeSpan dtSpan;
	switch (m_BaseInterval)
	{
	case tiSecond:
		dtSpan.SetDateTimeSpan(0,0,0,m_iDTTickIntervalMult);
		dtTick += dtSpan;
		break;
	case tiMinute:
		dtSpan.SetDateTimeSpan(0,0,m_iDTTickIntervalMult,0);
		dtTick += dtSpan;
		break;
	case tiHour:
		dtSpan.SetDateTimeSpan(0,m_iDTTickIntervalMult,0,0);
		dtTick += dtSpan;
		break;
	case tiDay:
		dtSpan.SetDateTimeSpan(m_iDTTickIntervalMult,0,0,0);
		dtTick += dtSpan;
		break;
	case tiMonth:
		dtTick = AddMonthToDate(dtTick,m_iDTTickIntervalMult);
		break;
	case tiYear:
		dtTick = AddMonthToDate(dtTick,12*m_iDTTickIntervalMult);
		break;
	}

	dNextTick = (DATE)dtTick;
	if (dNextTick <= m_MaxValue)
		return true;
	else
		return false;
}

TChartString CChartDateTimeAxis::GetTickLabel(double TickValue) const
{
	COleDateTime tickTime((DATE)TickValue);
	TChartString strLabel = tickTime.Format(m_strDTTickFormat.c_str());
	return strLabel;
}

long CChartDateTimeAxis::ValueToScreenDiscrete(double dValue) const
{
	// In discrete mode, all values between two ticks relates
	// to the middle of the interval (there's no other values than
	// the tick values).
	double tickAfter;
	double tickBefore = GetTickBeforeVal(dValue);
	GetNextTickValue(tickBefore, tickAfter);

	long tickPosBefore = ValueToScreenStandard(tickBefore);
	long tickPosAfter = ValueToScreenStandard(tickAfter);
	return tickPosBefore + (tickPosAfter-tickPosBefore)/2;
}

long CChartDateTimeAxis::GetTickPos(double TickVal) const
{
	// The tick is always at the same position,
	// even if the axis is discrete.
	return ValueToScreenStandard(TickVal);
}

COleDateTime CChartDateTimeAxis::AddMonthToDate(const COleDateTime& Date, 
												int iMonthsToAdd) const
{
	COleDateTime newDate;
	int nMonths = Date.GetMonth()-1 + iMonthsToAdd;
	int nYear = Date.GetYear() + nMonths/12;;
	// We can 'add' a negative number of months
	if (nMonths<0)
	{
		nYear = Date.GetYear() - (-nMonths)/12;
		nMonths += (-nMonths)/12 * 12;
	}

	newDate.SetDateTime(nYear,nMonths%12+1,Date.GetDay(),Date.GetHour(),
		Date.GetMinute(),Date.GetSecond());
	return newDate;
}

void CChartDateTimeAxis::RefreshTickIncrement()
{
	if (!m_bAutoTicks)
		return;

	if (m_MaxValue == m_MinValue)
	{
		m_iDTTickIntervalMult = 1;
		return;
	}

	int PixelSpace;
	if (m_bIsHorizontal)
		PixelSpace = 60;
	else
		PixelSpace = 20;

	int MaxTickNumber = (int)fabs((m_EndPos-m_StartPos)/PixelSpace * 1.0);
	if (MaxTickNumber == 0)
		MaxTickNumber = 1;

	COleDateTime StartDate(m_MinValue);
	COleDateTime EndDate(m_MaxValue);

	COleDateTimeSpan minTickInterval = (EndDate - StartDate)/MaxTickNumber;
	double Seconds = minTickInterval.GetTotalSeconds();
	double Minutes = minTickInterval.GetTotalMinutes();
	double Hours = minTickInterval.GetTotalHours();
	double Days = minTickInterval.GetTotalDays();
	if (Seconds < 60)
	{
		m_BaseInterval = tiSecond;
		if (Seconds > 30)
		{
			m_BaseInterval = tiMinute;
			m_iDTTickIntervalMult = 1;
		}
		else if (Seconds > 10)
			m_iDTTickIntervalMult = 30;
		else if (Seconds > 5)
			m_iDTTickIntervalMult = 10;
		else if (Seconds > 2)
			m_iDTTickIntervalMult = 5;
		else 
			m_iDTTickIntervalMult = 1;
	}
	else if (Minutes < 60)
	{
		m_BaseInterval = tiMinute;
		if (Minutes > 30)
		{
			m_BaseInterval = tiHour;
			m_iDTTickIntervalMult = 1;
		}
		else if (Minutes > 10)
			m_iDTTickIntervalMult = 30;
		else if (Minutes > 5)
			m_iDTTickIntervalMult = 10;
		else if (Minutes > 2)
			m_iDTTickIntervalMult = 5;
		else 
			m_iDTTickIntervalMult = 2;
	}
	else if (Hours < 24)
	{
		m_BaseInterval = tiHour;
		if (Hours > 12)
		{
			m_BaseInterval = tiDay;
			m_iDTTickIntervalMult = 1;
		}
		else if (Hours > 6)
			m_iDTTickIntervalMult = 12;
		else if (Hours > 2)
			m_iDTTickIntervalMult = 6;
		else 
			m_iDTTickIntervalMult = 2;
	}
	else if (Days < 31)
	{
		m_BaseInterval = tiDay;
		if (Days > 7)
		{
			m_BaseInterval = tiMonth;
			m_iDTTickIntervalMult = 1;
		}
		else if (Days > 2)
		{
			m_BaseInterval = tiDay;
			m_iDTTickIntervalMult = 7;
		}
		else 
			m_iDTTickIntervalMult = 2;
	}
	else if (Days < 365)
	{
		m_BaseInterval = tiMonth;
		if (Days > 186)	 // Approx 6 months
		{
			m_BaseInterval = tiYear;
			m_iDTTickIntervalMult = 1;
		}
		else if (Days > 124)
			m_iDTTickIntervalMult = 6;
		else if (Days > 62)
			m_iDTTickIntervalMult = 4;
		else
			m_iDTTickIntervalMult = 2;
	}
	else
	{
		m_BaseInterval = tiYear;
		m_iDTTickIntervalMult = (int)Days/365 + 1;
	}
}

void CChartDateTimeAxis::RefreshFirstTick()
{
	m_dFirstTickValue = GetTickBeforeVal(m_MinValue);
	if (m_bAutoTickFormat)
		RefreshDTTickFormat();
}

void CChartDateTimeAxis::SetReferenceTick(COleDateTime referenceTick)
{
	m_ReferenceTick = referenceTick;
	m_pParentCtrl->RefreshCtrl();
}

void CChartDateTimeAxis::RefreshDTTickFormat()
{
	switch (m_BaseInterval)
	{
	case tiSecond:
		m_strDTTickFormat = _T("%H:%M:%S");
		break;
	case tiMinute:
		m_strDTTickFormat = _T("%H:%M");
		break;
	case tiHour:
		m_strDTTickFormat = _T("%H:00");
		break;
	case tiDay:
		m_strDTTickFormat = _T("%d %b");
		break;
	case tiMonth:
		m_strDTTickFormat = _T("%b %Y");
		break;
	case tiYear:
		m_strDTTickFormat = _T("%Y");
		break;
	}
}

double CChartDateTimeAxis::GetTickBeforeVal(double dValue) const
{
	double precision = 0.0000000001;
	if (dValue < 0)
		precision = -0.0000000001;
	
	COleDateTime tickBefore;
	COleDateTime valueTime = COleDateTime(DATE(dValue+precision));
	COleDateTimeSpan dtSpan = valueTime - m_ReferenceTick;
	switch (m_BaseInterval)
	{
	case tiSecond:
		{
			int totalSecs = (int)dtSpan.GetTotalSeconds();
			totalSecs = (totalSecs/m_iDTTickIntervalMult) * m_iDTTickIntervalMult;
			int Days = totalSecs/86400;					// 86400 seconds in one day
			int Hours = (totalSecs%86400)/3600;			// 3600 seconds in one hour
			int Minutes = ((totalSecs%86400)%3600)/60;	// 60 seconds in one minute
			int Seconds = ((totalSecs%86400)%3600)%60;
			dtSpan.SetDateTimeSpan(Days, Hours, Minutes, Seconds);
			tickBefore = m_ReferenceTick + dtSpan;
		}
		break;
	case tiMinute:
		{
			int totalMinutes = (int)dtSpan.GetTotalMinutes();
			totalMinutes = (totalMinutes/m_iDTTickIntervalMult) * m_iDTTickIntervalMult;
			int Days = totalMinutes/1440;			// 1440 minutes in one day
			int Hours = (totalMinutes%1440)/60;		// 60 minutes in one hour
			int Minutes = (totalMinutes%1440)%60;	
			dtSpan.SetDateTimeSpan(Days, Hours, Minutes, 0);
			tickBefore = m_ReferenceTick + dtSpan;
		}
		break;
	case tiHour:
		{
			int totalHours = (int)dtSpan.GetTotalHours();
			totalHours = (totalHours/m_iDTTickIntervalMult) * m_iDTTickIntervalMult;
			int Days = totalHours/24;			// 24 hours in one day
			int Hours = totalHours%24;		
			dtSpan.SetDateTimeSpan(Days, Hours, 0, 0);
			tickBefore = m_ReferenceTick + dtSpan;
		}
		break;
	case tiDay:
		{
			int totalDays = (int)dtSpan.GetTotalDays();
			totalDays = (totalDays/m_iDTTickIntervalMult) * m_iDTTickIntervalMult;
			dtSpan.SetDateTimeSpan(totalDays, 0, 0, 0);
			tickBefore = m_ReferenceTick + dtSpan;
		}
		break;
	case tiMonth:
		{
			int yearDiff = valueTime.GetYear() - m_ReferenceTick.GetYear();
			int monthDiff = valueTime.GetMonth() - m_ReferenceTick.GetMonth();
			int totalMonths = ((yearDiff*12+monthDiff)/m_iDTTickIntervalMult) * m_iDTTickIntervalMult;
			tickBefore = AddMonthToDate(m_ReferenceTick,totalMonths);			
		}
		break;
	case tiYear:
		{
			int yearDiff = valueTime.GetYear() - m_ReferenceTick.GetYear();
			int year = ((yearDiff)/m_iDTTickIntervalMult) * m_iDTTickIntervalMult;
			tickBefore = AddMonthToDate(m_ReferenceTick,year*12);		
		}
		break;
	}

	return (DATE)tickBefore;
}
