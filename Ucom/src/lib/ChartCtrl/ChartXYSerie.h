/*
 *
 *	ChartXYSerie.h
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

#pragma once
#include "ChartSerieBase.h"

//! Structure containing a point data with X and Y values
struct SChartXYPoint
{
	SChartXYPoint() : X(0.0), Y(0.0)  
	{ 
		#ifndef NO_USER_DATA
		pUserData = NULL;
		#endif
	}
	SChartXYPoint(double XVal, double YVal) : X(XVal), Y(YVal)  
	{ 
		#ifndef NO_USER_DATA
		pUserData = NULL;
		#endif
	}

	double GetX() const { return X; }
	double GetY() const { return Y; }
	double GetXMin() const { return X; }
	double GetXMax() const { return X; }
	double GetYMin() const { return Y; }
	double GetYMax() const { return Y; }

	//! The point X value.
	double X;
	//! The point Y value.
	double Y;
	#ifndef NO_USER_DATA
	//! Optional user data.
	void *pUserData;
	#endif
};

//! Specialization of a CChartSerieBase for series having data with an X and an Y value.
/**
	This class is abstract and has to be implemented for specific series. It 
	already provides features which are common to all series with points having
	an X and an Y value. Examples of such series are: point series, line series,
	surface series and bar series.
**/
class CChartXYSerie : public CChartSerieBase<SChartXYPoint>
{
public:
	//! Constructor
	CChartXYSerie(CChartCtrl* pParent);
	//! Destructor
	virtual ~CChartXYSerie();

	//! Adds a single data point to the series.
	void AddPoint(double X, double Y);
	//! Adds an array of points to the series.
	/**
		Points which were already present in the series are kept.
		@param pX
			Array of X values for the points
		@param pY
			Array of Y values for the points
		@param Count
			Size of each of both arrays (number of points to add)
	**/
	void AddPoints(double* pX, double* pY, unsigned Count);
	//! Sets an array of points to the series.
	/**
		Points which were already present in the series are discarded.
		@param pX
			Array of X values for the points
		@param pY
			Array of Y values for the points
		@param Count
			Size of each of both arrays (number of points to add)
	**/
	void SetPoints(double* pX, double* pY, unsigned Count);

	//! Returns the Y value of a specific point in the series.
	double GetYPointValue(unsigned PointIndex) const;
	//! Returns the X value of a specific point in the series.
	double GetXPointValue(unsigned PointIndex) const;
	//! Sets the Y value of a specific point in the series.
	/**
		The control is refreshed to display the change.
		@param PointIndex
			The index of the points to change the Y value
		@param NewVal 
			The new Y value of the point
	**/
	void   SetYPointValue(unsigned PointIndex, double NewVal);
	//! Sets the X value of a specific point in the series.
	/**
		The control is refreshed to display the change.
		@param PointIndex
			The index of the points to change the Y value
		@param NewVal 
			The new X value of the point
	**/
	void   SetXPointValue(unsigned PointIndex, double NewVal);	
	
#ifndef NO_USER_DATA
	//! Sets user data for a specific point. 
	/**
		User data can be disabled by adding the flag NO_USER_DATA in the preprocessor
		definitions. This is usefull when you don't want to have an additional pointer
		stored for each points in your series.
	**/
	void  SetUserData(unsigned uPointIndex, void* pData);
	//! Retrieves user data for a specific point. 
	/**
		User data can be disabled by adding the flag NO_USER_DATA in the preprocessor
		definitions. This is usefull when you don't want to have an additional pointer
		stored for each points in your series.
	**/
	void* GetUserData(unsigned uPointIndex);
#endif

protected:
	//! Retrieves the control points of a bezier curve fitting the points stored in the array.
	/**
		@param uFirst
			The index of the first point of the bezier curve
		@param uLast
			The index of the last point of the bezier curve
		@param pKnots
			This parameter will store the points data
		@param pFirstControlPoints
			This parameter will store the primary control points of the bezier curve
		@param pSecondControlPoints
			This parameter will store the secondary control points of the bezier curve
	**/
	void GetBezierControlPoints(unsigned uFirst, unsigned uLast, SChartXYPoint* &pKnots,
				SChartXYPoint* &pFirstControlPoints, SChartXYPoint* &pSecondControlPoints) const;

private:
	double* GetFirstControlPoints(double* rhs, int Count) const;
};
