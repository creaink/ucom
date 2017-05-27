/*
 *
 *	ChartGradient.cpp
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

#include "stdafx.h"
#include "ChartGradient.h"

CChartGradient::CChartGradient()
{
}

CChartGradient::~CChartGradient()
{
}

void CChartGradient::DrawGradient(CDC* pDC, const CRect& GradientRect, COLORREF Color1, 
								  COLORREF Color2, EGradientType GradientType)
{
#if _MFC_VER > 0x0600
	if ( (GradientType == gtHorizontal) || (GradientType == gtVertical) )
	{
		TRIVERTEX vertex[2] ;
		vertex[0].x     = GradientRect.left;
		vertex[0].y     = GradientRect.top;
		vertex[0].Red   = ((COLOR16)GetRValue(Color1))<<8;
		vertex[0].Green = ((COLOR16)GetGValue(Color1))<<8;
		vertex[0].Blue  = ((COLOR16)GetBValue(Color1))<<8;
		vertex[0].Alpha = 0x0000;
		vertex[1].x     = GradientRect.right;
		vertex[1].y     = GradientRect.bottom; 
		vertex[1].Red   = ((COLOR16)GetRValue(Color2))<<8;
		vertex[1].Green = ((COLOR16)GetGValue(Color2))<<8;
		vertex[1].Blue  = ((COLOR16)GetBValue(Color2))<<8;
		vertex[1].Alpha = 0x0000;
		GRADIENT_RECT gRect;
		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;
		if (GradientType == gtHorizontal)
			pDC->GradientFill(vertex,2,&gRect,1,GRADIENT_FILL_RECT_H);
		else
			pDC->GradientFill(vertex,2,&gRect,1,GRADIENT_FILL_RECT_V);
	}
	else
	{
		for (int i=0;i<2; i++)
		{
			TRIVERTEX vertex[2] ;
			if (GradientType == gtHorizontalDouble)
			{
				vertex[0].x     = GradientRect.left + (GradientRect.Width()/2) * i;
				vertex[0].y     = GradientRect.top;
			}
			else
			{
				vertex[0].x     = GradientRect.left;
				vertex[0].y     = GradientRect.top + (GradientRect.Height()/2) * i; 
			}
			if (i==0)
			{
				vertex[0].Red   = ((COLOR16)GetRValue(Color1))<<8;
				vertex[0].Green = ((COLOR16)GetGValue(Color1))<<8;
				vertex[0].Blue  = ((COLOR16)GetBValue(Color1))<<8;
			}
			else
			{
				vertex[0].Red   = ((COLOR16)GetRValue(Color2))<<8;
				vertex[0].Green = ((COLOR16)GetGValue(Color2))<<8;
				vertex[0].Blue  = ((COLOR16)GetBValue(Color2))<<8;
			}
			vertex[0].Alpha = 0x0000;
			if (GradientType == gtHorizontalDouble)
			{
				vertex[1].x     = GradientRect.left + (GradientRect.Width()/2) * (i+1);
				vertex[1].y     = GradientRect.bottom; 
			}
			else
			{
				vertex[1].x     = GradientRect.right;
				vertex[1].y     = GradientRect.top + (GradientRect.Height()/2) * (i+1); 
			}
			if (i==0)
			{
				vertex[1].Red   = ((COLOR16)GetRValue(Color2))<<8;
				vertex[1].Green = ((COLOR16)GetGValue(Color2))<<8;
				vertex[1].Blue  = ((COLOR16)GetBValue(Color2))<<8;
			}
			else
			{
				vertex[1].Red   = ((COLOR16)GetRValue(Color1))<<8;
				vertex[1].Green = ((COLOR16)GetGValue(Color1))<<8;
				vertex[1].Blue  = ((COLOR16)GetBValue(Color1))<<8;
			}			
			vertex[1].Alpha = 0x0000;

			GRADIENT_RECT gRect;
			gRect.UpperLeft  = 0;
			gRect.LowerRight = 1;
			if (GradientType == gtHorizontalDouble)
				pDC->GradientFill(vertex,2,&gRect,1,GRADIENT_FILL_RECT_H);
			else
				pDC->GradientFill(vertex,2,&gRect,1,GRADIENT_FILL_RECT_V);
		}
	}
#else
	CBrush NewBrush(Color1);
	pDC->FillRect(GradientRect,&NewBrush);
	DeleteObject(NewBrush);
#endif

}
