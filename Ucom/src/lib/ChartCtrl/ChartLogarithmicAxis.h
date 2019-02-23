/*
 *
 *	ChartLogarithmicAxis.h
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

#ifndef _CHARTLOGARITHMICAXIS_H_
#define _CHARTLOGARITHMICAXIS_H_

#include "ChartAxis.h"

//! Specialization of a CChartAxis to display a logarithmic scale.
/**
	Currently this class only allows to have a logarithmic axis with a
	base of 10. 
**/
class CChartLogarithmicAxis : public CChartAxis
{
	friend CChartCtrl;

private:
	//! Constructor
	CChartLogarithmicAxis();
	//! Destructor
	~CChartLogarithmicAxis();

	double ScreenToValue(long ScreenVal) const;
	void PanAxis(long PanStart, long PanEnd);

	double GetFirstTickValue() const;
	bool GetNextTickValue(double dCurrentTick, double& dNextTick) const;
	TChartString GetTickLabel(double TickValue) const;
	long ValueToScreenStandard(double Value) const;
	long ValueToScreenDiscrete(double Value) const;
	long GetTickPos(double TickVal) const;

	void RefreshTickIncrement();
	void RefreshFirstTick();

	void GetScrollbarSteps(int& iTotalSteps, int& iCurrentStep);
	void SetAxisToScrollStep(int iPreviousStep, int iCurrentStep, bool bScrollInverted);

	//! Caches the value of the first tick.
	double m_dFirstTickValue;
};

#endif  // _CHARTLOGARITHMICAXIS_H_
