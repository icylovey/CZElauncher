#pragma once
#include <windows.h>
#include <iostream>
#include <UIlib.h>

#include "SteamUser.h"

#pragma warning(disable:4996)
#ifdef _DEBUG
#pragma comment(lib,"Duilib_d.lib")
#else
#pragma comment(lib,"Duilib.lib")
#endif // DEBUG

#pragma comment(lib,"ComCtl32.lib") 

