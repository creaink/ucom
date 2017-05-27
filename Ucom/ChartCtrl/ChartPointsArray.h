/*
 *
 *	ChartPointsArray.h
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

#include "PointsOrdering.h"

//! Manages an array of points which supports fast resizing.
/**
	This class is used internally to store the data for all the points. The data
	is stored in a C-style array. The internal buffer can grow dynamically depending
	on the needs.
	
	The class is a template class with the template parameter being the type of 
	the points to be stored. The points have to provide the following methods:
	<ul><li>double GetXMin()</li>
	<li>double GetX()</li>
	<li>double GetXMax()</li>
	<li>double GetYMin()</li>
	<li>double GetY()</li>
	<li>double GetYMax()</li></ul>
**/
template <class T>
class CChartPointsArray
{
public:
	//! Constructor
	/**
		@param iResize
			The size by which the internal buffer is increased when reallocation occurs
	**/
	CChartPointsArray(unsigned iResize = 1000);
	//! Destructor
	~CChartPointsArray();

	//! Returns the number of points currently stored.
	unsigned GetPointsCount() const   { return m_iCurrentPoints; }
	//! Sets the size by which the internal buffer is increased when reallocation occurs
	void SetResize(int iResize)  { m_iResize = iResize; }

	//! Adds a new point in the array.
	/**
		@param newPoint
			The new point to add
	**/
	void AddPoint(const T& newPoint);
	//! Adds multiple points in the array.
	/**
		The points are added to the ones currently stored in the array.
		@param pPoints
			Array containing the points
		@param uCount
			The number of points to add
	**/
	void AddPoints(T* pPoints, unsigned uCount);
	//! Sets multiple points in the array.
	/**
		The points currently stored in the array are first removed
		before adding the new points.
		@param pPoints
			Array containing the new points
		@param uCount
			The number of points to add
	**/
	void SetPoints(T* pPoints, unsigned uCount);
	//! Removes all the points from the array.
	void Clear();
	//! Removes a certain amount of points from the begining of the series.
	void RemovePointsFromBegin(unsigned Count);
	//! Removes a certain amount of points from the end of the series.
	void RemovePointsFromEnd(unsigned Count);
	//! Retrieves the points at the specified index
	T& operator[](unsigned Index);
	//! Retrieves the points at the specified index
	const T& operator[](unsigned Index) const;

	//! Retrieves the minimum and maximum X values of the points stored in the array.
	bool GetSerieXMinMax(double& Min, double& Max)  const;
	//! Retrieves the minimum and maximum Y values of the points stored in the array.
	bool GetSerieYMinMax(double& Min, double& Max)  const;

	//! Specifies how the points should be ordered in the array.
	/**
		This specifies if the points should be ordered on their X values,
		on their Y values or not ordered (kept in order they are added to 
		the control). Ordering can improve performances a lot but makes it
		impossible to draw some specific curves (for instance, drawing an 
		ellipse is only possible if no ordering is set).
	**/
	void SetOrdering(PointsOrdering newOrdering);
	//! Retrieves the ordering of the points in the array.
	PointsOrdering GetOrdering() const  { return m_Ordering; } 
	//! Refreshes the point ordering.
	void ReorderPoints();

	//! Retrieves the index of the points which are between two given values.
	/**
		If the points are not ordered, uFirstPt will contain 0 and uLastPt
		will contain the index of the last point in the array.
		@param dAxisMin
			The minimum value to retrieve the first visible point
		@param dAxisMax
			The maximum value to retrieve the last visible point
		@param uFirstPt
			This parameter will store the index of the first visible point
		@param uLastPt
			This parameter will store the index of the last visible point
		@return false if no points are in the array. 
	**/
	bool GetVisiblePoints(double dAxisMin, double dAxisMax, 
						  unsigned& uFirstPt, unsigned& uLastPt) const;
	

	//! Returns the internal buffer of the array
	T* GetInternalBuffer() const	{ return m_pPoints; }

private:
	//! Caches the minimum X value.
	double m_dXMinVal;
	//! Caches the maximum X value.
	double m_dXMaxVal;
	//! Caches the minimum Y value.
	double m_dYMinVal;
	//! Caches the maximum Y value.
	double m_dYMaxVal;

	//! Recalculates the min and max values.
	void RefreshMinMax();
	//! Inserts a new point in the array.
	void InsertNewPoint(const T& newPoint);
	//! Inserts a new point at a specific position in the array.
	void InsertPointAtPos(const T& newPoint, int iPos);
	//! Comparison function which compares two points based on their X values.
	static int ComparePointsOnX(void const* pA, void const* pB);
	//! Comparison function which compares two points based on their Y values.
	static int ComparePointsOnY(void const* pA, void const* pB);
	//! Implements a binary search used to find the index of a points give the X or Y value.
	int BinarySearch(unsigned uLeft, unsigned uRight, double dFind) const;

	//! The array of points
	T* m_pPoints;
	//! The number of allocated points 
	unsigned m_iMaxPoints;
	//! The number of points currently used 
	unsigned m_iCurrentPoints;
	//! The size by which the array is incremented once it is full
	unsigned m_iResize;
	//! The ordering of the points
	PointsOrdering m_Ordering;
};

#include "ChartPointsArray.inl"