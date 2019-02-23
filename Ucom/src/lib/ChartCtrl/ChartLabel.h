/*
 *
 *	ChartLabel.h
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

#ifndef _CHARTLABEL_H_ 
#define _CHARTLABEL_H_

template <class PointType>
class CChartSerieBase;

//! Interface which should be implemented in order to provide text to a label.
/**
	This class is a template class with the template parameter being the point
	type of the series to which the label is attached.

	Using a CChartLabelProvider provides more flexibility in the way to 
	supply text to the label. You can for instance embedd in the string some 
	information about the point (XValue, YValue, index, ...). In that case, a 
	single CChartLabelProvider object can be provided for all labels. Changing 
	the displayed text of all labels becomes also easier: you only have to adapt
	the string returned by this object and refresh the control and all labels will
	be updated.
**/
template <class PointType>
class CChartLabelProvider
{
public:
	//! Constructor
	CChartLabelProvider()  { }
	//! Destructor
	virtual ~CChartLabelProvider()  { }

	//! Method to override in order to provide the text of the label.
	/**
		@param pSerie
			The series to which the label is attached
		@param uPtIndex
			The index of the point in the series to which the label is attached
		@return a string which will be the text displayed in the label.
	**/
	virtual TChartString GetText(CChartSerieBase<PointType>* pSerie,
			unsigned PointIndex) = 0;
};

//! Draws a label containing some text which is attached to a point of a series.
/**
	This is a base class which should be overriden for specific label types. 
**/
template <class PointType>
class CChartLabel
{
	friend CChartSerieBase<PointType>;

public:
	//! Sets a static text to be displayed in the label.
	void SetLabelText(const TChartString& strText);
	//! Sets the font of the text label.
	/**
		@param nPointSize
			The font point size
		@param strFaceName
			The font face name
	**/
	void SetFont(int nPointSize, const TChartString& strFaceName);
	//! Shows/hides the label.
	void SetVisisble(bool bVisible);
	//! Sets a label provider for more flexibility in how the text is supplied.
	void SetLabelProvider(CChartLabelProvider<PointType>* pProvider)
	{
		m_pLabelProvider = pProvider;
	}

protected:
	//! Constructor
	CChartLabel(CChartCtrl* pParentCtrl, CChartSerieBase<PointType>* pParentSeries);
	//! Destructor
	virtual ~CChartLabel();

	//! Draws the label.
	/**
		This pure virtual function must be overriden by all child classes.
	**/
	virtual void Draw(CDC* pDC, unsigned uPointIndex) = 0;

	//! Specifies if the label is visible or not.
	bool m_bIsVisible;
	//! The text font size.
	int m_iFontSize;
	//! The text font face name.
	TChartString m_strFontName;

	//! The static text of the label.
	TChartString m_strLabelText;
	//! The text provider.
	CChartLabelProvider<PointType>* m_pLabelProvider;

	//! The parent charting control.
	CChartCtrl*  m_pParentCtrl;
	//! The parent series.
	CChartSerieBase<PointType>* m_pParentSeries;
};

#include "ChartLabel.inl"

#endif  // _CHARTLABEL_H_