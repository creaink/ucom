#pragma once

/*¿ØÖÆÌ¨µ÷ÊÔ*/
//#define USING_CONSLOE
#ifdef USING_CONSLOE

#include <iostream>
#include <consoleapi.h>
#pragma warning(disable:4996)

using std::cin;
using std::cout;

inline void InitDebugConsole(void){
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);
}
#endif