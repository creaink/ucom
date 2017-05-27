/*
 *
 *	ChartCtrl.h
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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ChartSerie.h"
#include "ChartAxis.h"
#include "ChartGrid.h"
#include "ChartLegend.h"
#include "ChartTitle.h"
#include "ChartGradient.h"
#include "ChartCursor.h"
#include "ChartMouseListener.h"
#include "ChartStandardAxis.h"
#include "ChartLogarithmicAxis.h"
#include "ChartDateTimeAxis.h"
#include "ChartCrossHairCursor.h"
#include "ChartDragLineCursor.h"
	
#include <map>


class CChartStandardAxis;
class CChartLogarithmicAxis;
class CChartDateTimeAxis;
class CChartCrossHairCursor;
class CChartDragLineCursor;

class CChartPointsSerie;
class CChartLineSerie;
class CChartSurfaceSerie;
class CChartBarSerie;
class CChartCandlestickSerie;
class CChartGanttSerie;

/////////////////////////////////////////////////////////////////////////////
// CChartCtrl window

//! The main chart control class.
/**

**/
class CChartCtrl : public CWnd
{

public:
	//! Retrieves de device context.
	/**
		This function is used for internal purposes only.
	**/
	CDC* GetDC();
	//! Retrieves the plotting rectangle.
	CRect GetPlottingRect()  const { return m_PlottingRect; }

	//! Returns a pointer to the legend object.
	CChartLegend* GetLegend() const  { return m_pLegend; }
	//! Returns a pointer to the title object.
	CChartTitle*  GetTitle()  const  { return m_pTitles; }

	//! An enumeration of the different axis positions.
	enum EAxisPos
	{
		LeftAxis = 0,
		BottomAxis,
		RightAxis,
		TopAxis
	};

	//! Create and attach a standard axis to the control.
	/**
		@param axisPos
			The position of the axis. 
		@return The created standard axis.
	**/
	CChartStandardAxis* CreateStandardAxis(EAxisPos axisPos);
	//! Create and attach a logarithmic axis to the control.
	/**
		@param axisPos
			The position of the axis. 
		@return The created logarithmic axis.
	**/
	CChartLogarithmicAxis* CreateLogarithmicAxis(EAxisPos axisPos);
	//! Create and attach a date/time axis to the control.
	/**
		@param axisPos
			The position of the axis. 
		@return The created date/time axis.
	**/
	CChartDateTimeAxis* CreateDateTimeAxis(EAxisPos axisPos);
	//! Attach a custom axis to the control.
	/**
		This function takes ownership of the axis pointer, so you 
		must not destroy it. If the axis is alread attached to a
		chart control (even this one), the function will fail with 
		an assertion. This function should be used only when you want
		to provide a custom axis to the control, otherwise you should
		use the AttachStandardAxis, AttachLogarithmicAxis and 
		AttachDateTimeAxis instead.
		@param pAxis
			The axis to attach to the control.
		@param axisPos
			The position of the axis. This value can be:
			- LeftAxis
			- BottomAxis
			- RightAxis
			- TopAxis
	**/
	void AttachCustomAxis(CChartAxis* pAxis, EAxisPos axisPos);

	//! Create and attach a point series to the control
	/**
		@param bSecondaryHorizAxis
			Specifies if the horizontal axis is the secondary axis or not. 
		@param bSecondaryVertAxis
			Specifies if the vertical axis is the secondary axis or not. 
		@return The created chart point series.
		@remark The function will assert if the associated axes are not attached 
		to the control.
		@see AttachCustomSerie for more info about the parameters of the function.
	**/
	CChartPointsSerie* CreatePointsSerie(bool bSecondaryHorizAxis=false, bool bSecondaryVertAxis=false);
	//! Create and attach a line series to the control
	/**
		The function will assert if the associated axes are not attached 
		to the control.
		@param bSecondaryHorizAxis
			Specifies if the horizontal axis is the secondary axis or not. 
		@param bSecondaryVertAxis
			Specifies if the vertical axis is the secondary axis or not. 
		@return The created chart line series.
		@remark The function will assert if the associated axes are not attached 
		to the control.
		@see AttachCustomSerie for more info about the parameters of the function.
	**/
	CChartLineSerie* CreateLineSerie(bool bSecondaryHorizAxis=false, bool bSecondaryVertAxis=false);
	//! Create and attach a surface series to the control
	/**
		The function will assert if the associated axes are not attached 
		to the control.
		@param bSecondaryHorizAxis
			Specifies if the horizontal axis is the secondary axis or not. 
		@param bSecondaryVertAxis
			Specifies if the vertical axis is the secondary axis or not. 
		@return The created chart surface series.
		@remark The function will assert if the associated axes are not attached 
		to the control.
		@see AttachCustomSerie for more info about the parameters of the function.
	**/
	CChartSurfaceSerie* CreateSurfaceSerie(bool bSecondaryHorizAxis=false, bool bSecondaryVertAxis=false);
	//! Create and attach a bar series to the control
	/**
		@param bSecondaryHorizAxis
			Specifies if the horizontal axis is the secondary axis or not. 
		@param bSecondaryVertAxis
			Specifies if the vertical axis is the secondary axis or not. 
		@return The created chart bar series.
		@remark The function will assert if the associated axes are not attached 
		to the control.
		@see AttachCustomSerie for more info about the parameters of the function.
	**/
	CChartBarSerie* CreateBarSerie(bool bSecondaryHorizAxis=false, bool bSecondaryVertAxis=false);
	//! Create and attach a candlestick series to the control
	/**
		@param bSecondaryHorizAxis
			Specifies if the horizontal axis is the secondary axis or not. 
		@param bSecondaryVertAxis
			Specifies if the vertical axis is the secondary axis or not. 
		@return The created chart candlestick series.
		@remark The function will assert if the associated axes are not attached 
		to the control.
		@see AttachCustomSerie for more info about the parameters of the function.
	**/
	CChartCandlestickSerie* CreateCandlestickSerie(bool bSecondaryHorizAxis=false, bool bSecondaryVertAxis=false);
	//! Create and attach a gantt series to the control
	/**
		@param bSecondaryHorizAxis
			Specifies if the horizontal axis is the secondary axis or not. 
		@param bSecondaryVertAxis
			Specifies if the vertical axis is the secondary axis or not. 
		@return The created chart gantt series.
		@remark The function will assert if the associated axes are not attached 
		to the control.
		@see AttachCustomSerie for more info about the parameters of the function.
	**/
	CChartGanttSerie* CreateGanttSerie(bool bSecondaryHorizAxis=false, bool bSecondaryVertAxis=false);
	//! Attaches a custom series to the chart
	/**
		You should only use this function if you want to attach a custom series
		to the control. Otherwise, you should use the CreateXXXSerie helper functions.
		The function will assert if the associated axes are not attached 
		to the control.
		@param pNewSeries
			The new series to be added. The control will take ownership of 
			the pointer, so dont't delete it yourself.
		@param bSecondaryHorizAxis
			Specifies if the associated horizontal axis is secondary or not. 
			If this value is false, the associated horizontal axis is the 
			bottom axis, otherwise it is the top axis.
		@param bSecondaryVertAxis
			Specifies if the associated vertical axis is secondary or not. 
			If this value is false, the associated vertical axis is the 
			left axis, otherwise it is the right axis.
	**/
	void AttachCustomSerie(CChartSerie* pNewSeries, bool bSecondaryHorizAxis=false, bool bSecondaryVertAxis=false);
	//! Retrieves a specific series from the chart
	/**
		@param uSerieId
			The Id of the series to retrieve
		@return The series or NULL if uSerieId is not attributed.
	**/
	CChartSerie* GetSerie(unsigned uSerieId) const;
	//! Removes a specific series from the chart
	/**
		@param uSerieId
			The Id of the series to be removed.
	**/
	void RemoveSerie(unsigned uSerieId);
	//! Removes all the series from the chart
	void RemoveAllSeries();
	//! Returns the number of series in the chart
	size_t GetSeriesCount() const;

	//! Create and attach a cross-hair cursor to the control
	/**
		A cross-hair cursor display a cross on the control and move accordingly
		to the mouse. It is attached to an horizontal and a vertical axis.
		@param bSecondaryHorizAxis
			Specifies if the horizontal axis is the secondary axis or not. 
		@param bSecondaryVertAxis
			Specifies if the vertical axis is the secondary axis or not. 
		@return The created cross-hair cursor.
		@remark The function will assert if the associated axes are not attached 
		to the control.
	**/
	CChartCrossHairCursor* CreateCrossHairCursor(bool bSecondaryHorizAxis=false, bool bSecondaryVertAxis=false);
	//! Create and attach a drag-line cursor to the control
	/**
		A drag-line cursor is a simple line (horizontal or vertical) that can be
		dragged with the mouse by clicking on it. It is attached to a specific axis.
		@param relatedAxis
			The axis position to which the cursor is attached to.
		@return The created drag-line cursor.
		@remark The function will assert if the associated axis is not attached 
		to the control.
	**/
	CChartDragLineCursor* CreateDragLineCursor(EAxisPos relatedAxis);
	//! Attach a custom cursor to the control
	/**
		You should only use this function if you want to attach a custom cursor
		to the control. Otherwise, you should use the CreateXXXCursor helper functions.
		@param pCursor
			The custom cursor to be attached to the control.
	**/
	void AttachCustomCursor(CChartCursor* pCursor);
	//! Removes a cursor with a specific Id from the control.
	/**
		The cursor Id can be retrieved on through the CChartCursor::GetCursorId function.
		@param cursorId
			The Id of the cursor to remove from the control.
	**/
	void RemoveCursor(unsigned cursorId);

	//! Shows/hides the mouse cursor when it is over the plotting area.
	void ShowMouseCursor(bool bShow);

	CChartAxis* GetBottomAxis() const;
	CChartAxis* GetLeftAxis() const;
	CChartAxis* GetTopAxis() const;
	CChartAxis* GetRightAxis() const;
	//! Returns a specific axis attached to the control
	/**
		If the specified axis does not exist, NULL is returned.
		@param axisPos
			The axis position (left, bottom, right or top).
	**/
	CChartAxis* GetAxis(EAxisPos axisPos) const
	{
		return m_pAxes[axisPos];
	}

	//! Returns the type of the edge used as border.
	UINT GetEdgeType() const        { return EdgeType;    }
	//! Sets the edge type.
	/**
		@param NewEdge 
			The type of the edge. See the DrawEdge function in MSDN for 
			a list of the different types.
	**/
	void SetEdgeType(UINT NewEdge)  
	{ 
		EdgeType = NewEdge; 
		RefreshCtrl();
	}

	//! Returns the background color
	COLORREF GetBackColor() const			{ return m_BackColor;   }
	//! Sets the background color.
	void SetBackColor(COLORREF NewCol)		
	{ 
		m_BackColor = NewCol;  
		m_bBackGradient = false; 
		RefreshCtrl();
	}
	//! Returns the color of the plotting area's border 
	COLORREF GetBorderColor() const			{ return m_BorderColor;   }
	//! Sets the color of the plotting area's border 
	void SetBorderColor(COLORREF NewCol)	{ m_BorderColor = NewCol;	RefreshCtrl(); }
	//! Returns the color of the zoom rectangle
	COLORREF GetZoomRectColor() const		{ return m_ZoomRectColor;   }
	//! Sets the color of the zoom rectangle
	void SetZoomRectColor(COLORREF NewCol)	{ m_ZoomRectColor = NewCol; RefreshCtrl(); }
	//! Sets a gradient background
	/**
		@param Col1
			The first gradient color
		@param Col2
			The second gradient color
		@param GradientType
			The type of gradient used from Col1 to Col2. It can take the following values:
				- gtHorizontal: a simple left-to-right gradient, from Col1 to Col2.
				- gtVertical: a simple top-to-bottom gradient, from Col1 to Col2. 
				- gtHorizontalDouble: a left-to-middle-to-right gradient, with Col2 in the middle. 
				- gtVerticalDouble: a top-to-middle-to-bottom gradient, with Col2 in the middle. 
	**/
	void SetBackGradient(COLORREF Col1, COLORREF Col2, EGradientType GradientType);

	//! Enables/disables the pan feature
	void SetPanEnabled(bool bEnabled)  { m_bPanEnabled = bEnabled;  }
	//! Returns true if the pan feature is enabled
	bool GetPanEnabled() const		   { return m_bPanEnabled;	    }
	//! Enables/disables the zoom feature
	void SetZoomEnabled(bool bEnabled) { m_bZoomEnabled = bEnabled; }
	//! Returns true if the zoom feature is enabled
	bool GetZoomEnabled() const		   { return m_bZoomEnabled;	    }
	//! Undo all pan and zoom operations that were done on the chart
	void UndoPanZoom();

	//! Forces a refresh of the control.
	/**
		This function is used for internal purposes.
	**/
	void RefreshCtrl();
	//! Enables/disables the refresh of the control
	/**
		This function is used when several settings have to be changed at the same 
		time on the control. This way we can avoid refreshing the control when it 
		is not needed.
		@param bEnable
			false to disable the refresh and true to re-enable the refresh.
	**/
	void EnableRefresh(bool bEnable);
	//! Creates the control dynamically.
	/**
		@param pParentWnd 
			Parent window of the control
		@param rect 
			Position of the control
		@param nID
			ID of the control
		@param dwStyle
			Style of the control
	**/
	int Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle=WS_VISIBLE);
	
	//! Helper function to convert a date to a double value
	static double DateToValue(const COleDateTime& Date);
	//! Helper function to convert a double value to a date
	static COleDateTime ValueToDate(double Value);

	//! Print the chart
	/**
		@param strTitle
			The title of the print document.
		@param pPrntDialog
			A pointer to a CPrintDialog. 
			If NULL is passed, the default print dialog will be displayed.
	**/
    virtual void Print(const TChartString& strTitle, CPrintDialog* pPrntDialog = NULL);

#if _MFC_VER > 0x0600
	//! Saves the chart to an image file
	/**
		This function is not available for VC6 and earlier.
		@param strFilename
			The name of the file in which to save the image.
		@param rect
			The size of the image. If an empty rectangle is provided, the
			size of the chart on screen will be used (this results in an identical
			image as what is seen on the screen).
		@param nBPP
			The numbers of bits per pixel in the bitmap. Usually 4, 8, 16, 24, or 32. 
		@param guidFileType
			The file type to save the image as. See the CImage::Save in MSDN
			for more information.
	**/
	void SaveAsImage(const TChartString& strFilename, const CRect& rect, 
			int nBPP, REFGUID guidFileType= GUID_NULL);
#endif

	//! Default constructor
    CChartCtrl();	
	//! Default destructor
    virtual ~CChartCtrl();

	//! Register a mouse listener with the control.
	/**
		This listener will be notified each time a mouse event occurs on the control.
		@param pMouseListener
			The mouse listener to register with this control.
	**/
	void RegisterMouseListener(CChartMouseListener* pMouseListener) { m_pMouseListener = pMouseListener;}

	//! Tell the control to set the current series to the first series.
	/**
		This function is used with the GetNextSerie to iterate over all 
		series in the control.
	**/
	void GoToFirstSerie();
	//! Returns the next series in the control.
	/**
		This function is used with the GoToFirstSerie to iterate over all 
		series in the control. First call GoToFirstSerie and then call this 
		function until it returns NULL to iterate over all series.
		Warning: calling this function without calling GoToFirstSerie before
		might lead to unpredicted results. The same if you add or remove
		series between the call to GetFirstSerie and the call to GetNextSerie.
		@return the next series or NULL if we already are at the last series.
	**/
	CChartSerie* GetNextSerie();

	//! Refreshes all the axes which are automatic for the screen.
	void RefreshScreenAutoAxes();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
protected:
    virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

	// This function can be called to draw the chart
	// on the screen or for printing.
	virtual void DrawChart(CDC* pDC, CRect ChartRect);
	virtual void DrawBackground(CDC* pDC, CRect ChartRect);

private:
	//! Register the window class used for this control
	bool RegisterWindowClass();

	//! Notifies the mouse listeners about a mouse event on the control.
	/**
		The function detects on which part of the control the event happened.
		@param mouseEvent
			The type of the mouse event
		@param screenPoint
			The screen point on which the event occured.
	**/
	void SendMouseEvent(CChartMouseListener::MouseEvent mouseEvent, const CPoint& screenPoint) const;

	//! Specifies if the refresh is currently enabled or not.
	int m_iEnableRefresh ;
	//! Specifies if there is a pending refresh. 
	/**
		If true, once EnableRefresh(true) is called, a refresh of the control
		will be	done.
	**/
	bool m_bPendingRefresh;
	//! Memory bitmap on which the chart background is drawn (axis, grid, title, ...)
	CDC m_BackgroundDC;	
	//! Specifies if the memory bitmap has already been created.
	bool m_bMemDCCreated;

	//! Specifies if the background is gradient or solid
	bool m_bBackGradient;	
	//! First gradient color for the background
	COLORREF m_BackGradientCol1;
	//! Second gradient color for the background
	COLORREF m_BackGradientCol2;
	//! The gradient type used for the background
	EGradientType m_BackGradientType;
	//! The background color (if no gradient used)
	COLORREF m_BackColor;	
	//! The border color
	COLORREF m_BorderColor;
	//! The type of edge
	UINT EdgeType;		

	//! Zone in wich the series will be plotted
	CRect m_PlottingRect;	

	typedef std::map<unsigned, CChartSerie*> TSeriesMap;
	//! Map containing all the series added to the chart.
	TSeriesMap m_mapSeries;	
	//! The four axis of the control.
	CChartAxis* m_pAxes[4];

	//! The chart legend
	CChartLegend* m_pLegend;	
	//! The chart titles
	CChartTitle*  m_pTitles;	

	//! Specifies if the mouse panning is enabled
	bool m_bPanEnabled;
	//! Specifies if the right mouse button is currently pressed
	bool m_bRMouseDown;		
	//! The point on which the panning started
	CPoint m_PanAnchor;

	//! Specifies if the zoom is enabled
	bool  m_bZoomEnabled;
	//! Specifies if the left mouse button is currently pressed
	bool  m_bLMouseDown;
	//! The rectangle of the zoom area
	CRect m_rectZoomArea;
	//! The color of the zoom rectangle
	COLORREF m_ZoomRectColor;

	//! Specifies if the toolbars have already been created
	bool m_bToolBarCreated;

	//! The font used for printing
    CFont  m_PrinterFont;  
	//! Page size in chartctrl units.
    CSize m_LogicalPageSize;     
	//! Page size in device units.
    CSize m_PaperSize;    

	typedef std::map<unsigned, CChartCursor*> TCursorMap;
	//! The map containing all the cursors
	TCursorMap m_mapCursors;

	//! The mouse listener registered with this control
	CChartMouseListener* m_pMouseListener;

	//! Specifies if the mouse is visible when over the plotting area.
	bool m_bMouseVisible;

	typedef TSeriesMap::const_iterator TSeriesMapIter;
	//! The iterator of the current series
	TSeriesMapIter m_currentSeries;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

