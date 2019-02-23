/*
 *
 *	ChartString.h
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

#include <string>
#include <sstream>

#if defined _UNICODE || defined UNICODE
	typedef std::wstring TChartString;
	typedef std::wstringstream TChartStringStream;
#else
	typedef std::string TChartString;
	typedef std::stringstream TChartStringStream;
#endif