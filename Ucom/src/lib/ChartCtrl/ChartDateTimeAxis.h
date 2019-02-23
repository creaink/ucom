/*
 *
 *	ChartDateTimeAxis.h
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

#ifndef _CHARTDATETIMEAXIS_H_
#define _CHARTDATETIMEAXIS_H_

#include "ChartAxis.h"
#include "ChartString.h"

//! A specialization of the CChartAxis class for displaying date and time data.
class CChartDateTimeAxis : public CChartAxis
{
	friend CChartCtrl;

public:
	//! Enum listing the different base intervals.
	enum TimeInterval
	{
		tiSecond,
		tiMinute,
		tiHour,
		tiDay,
		tiMonth,
		tiYear
	};

	//! Sets the tick increment.
	/**
		The tick increment is the value between two adjacents
		ticks on the axis. In case of a date time axis, the interval
		is specified by a time period because this interval might not
		be constant (for instance, if a tick interval of one month is 
		specified, the distance between two adjacents ticks is not 
		constant: it depends on the number of days in the month).
		The full tick interval is made of a base interval (day, month,
		hour, ...) and a multiplier, that is applied to this base interval.
		So, for an interval of three months between two ticks, you have to
		specify tiMonth for the interval and 3 for the multiplier.
		@param bAuto
			Specifies if the tick increment is automatically calculated.
		@param Interval
			The base interval.
		@param Multiplier
			The multiplier applied to the base interval.
	**/
	void SetTickIncrement(bool bAuto, TimeInterval Interval, int Multiplier);
	//! Sets the format of the tick labels.
	/**
		@param bAutomatic
			Specifies if the format is calculated automatically.
		@param strFormat
			The format to apply to the tick label if bAutomatic is false.
		<br>Check the documentation of the COleDateTime::Format function on 
		MSDN for more information about the format string. 
	**/
	void SetTickLabelFormat(bool bAutomatic, const TChartString& strFormat);
	//! Sets the reference tick.
	/**
		The reference tick is a date/time which specifies a tick which should
		always be displayed on the axis. This is needed when the tick
		interval multiplier is not 1 (e.g. the interval between two ticks is 3 
		months). In that specific case, there is no way for the control to know 
		which ticks should be displayed (in our example, the chart doesn't know 
		if the first tick will be january, february or march). This is particularly
		annoying when the axis is panned (in that case, if we always take the first
		month on the axis as first tick, the ticks will always switch from one month
		to another). By having a refence tick, this forces the control to calculate
		all tick intervals based on this reference. It is set to January 1st 2000 by
		default.
	**/
	void SetReferenceTick(COleDateTime referenceTick);

private:
	//! Default constructor
	CChartDateTimeAxis();
	//! Default destructor
	~CChartDateTimeAxis();

	double GetFirstTickValue() const;
	bool GetNextTickValue(double dCurrentTick, double& dNextTick) const;
	TChartString GetTickLabel(double TickValue) const;
	long ValueToScreenDiscrete(double Value) const;
	long GetTickPos(double TickVal) const;

	void RefreshTickIncrement();
	void RefreshFirstTick();
	//! Forces a refresh of the date/time tick label format
	void RefreshDTTickFormat();

	//! Add a number of months to a date.
	/**
		The function takes care of 'overflow' (total number
		of months higher than 12) error when adding the months.
		@param Date
			The date to which months will be added.
		@param iMonthsToAdd
			The number of months to add to the date.
		@return the resulting date.
	**/
	COleDateTime AddMonthToDate(const COleDateTime& Date, 
								int iMonthsToAdd) const;

	double GetTickBeforeVal(double dValue) const;

	//! Format of the date/time tick labels
	TChartString m_strDTTickFormat;		
	//! Specifies if the tick labels format is automatic
	bool m_bAutoTickFormat;
	//! Specifies the base time interval for ticks
	/**
		This specifies an base interval in sec, min, hour,
		day, month or year. The total tick increment is 
		a mutliple of this base interval (specified by 
		m_iDTTickIntervalMult). E.g: 2 days
	**/
	TimeInterval m_BaseInterval;
	//! Specifies the multiplicator for the base interval
	/**
		This multiplies the base interval for the ticks, resulting
		in something like 3 minutes (a multiplicator of 1 can also
		be specified).
	**/
	int m_iDTTickIntervalMult;

	//! Caches the value of the first tick.
	double m_dFirstTickValue;

	//! The reference tick. See the SetReferenceTick function for details.
	COleDateTime m_ReferenceTick;
};

#endif  // _CHARTDATETIMEAXIS_H_