/*
 *
 *	ChartScrollBar.h
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

class CChartAxis;

//! Class which manages the interaction with the axis scroll bar.
/**
	This class is used internally by the CChartAxis class. 
**/
class CChartScrollBar : public CScrollBar
{
friend CChartAxis;

public:
	//! Creates the scroll bar within a specified rectangle.
	void CreateScrollBar(const CRect& PlottingRect);

	//! Called on horizontal scrolling.
	void OnHScroll(UINT nSBCode, UINT nPos);
	//! Called on vertical scrolling.
	void OnVScroll(UINT nSBCode, UINT nPos);
	//! Refreshes the scroll bar position.
	void Refresh();

	//! Enables/disables the scroll bar.
	void SetEnabled(bool bEnabled)  { m_bEnabled = bEnabled; }
	//! Returns true if the scroll bar is enabled
	bool GetEnabled() const			{ return m_bEnabled; }
	//! Enables/disables the auto-hide mode.
	/**
		In auto-hide mode, the scroll bar is not visible unless the mouse
		is over the region of the scroll bar.
	**/
	void SetAutoHide(bool bAutoHide)  { m_bAutoHide = bAutoHide; }
	//! Returns true if the auto-hide mode is activated.
	bool GetAutoHide() const		  { return m_bAutoHide; }

	//! Called when the mouse enters the scroll bar area.
	void OnMouseEnter();
	//! Called when the mouse leaves the scroll bar area.
	void OnMouseLeave();

private:
	//! Constructor
	CChartScrollBar(CChartAxis* pParentAxis);
	//! Destructor
	~CChartScrollBar();

	bool IsScrollInverted() const;
	void MoveAxisToPos(int PreviousPos, int CurPos);

	CChartAxis* m_pParentAxis;
	bool m_bEnabled;
	bool m_bAutoHide;
};
