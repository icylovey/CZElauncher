#pragma once
#include <iostream>
#include <comutil.h>
#include <vector>
#include "ZElauncher.h"

class CZElauncherMain;
struct MapChineseName
{
	_bstr_t Map_en = "";
	_bstr_t Map_cn = "";
	_bstr_t MapDownloadurl = "";
};
struct ServerListInfo  
{
	_bstr_t ServerName = "";
	_bstr_t ServerIp = "";
};

extern _bstr_t gCookies;
extern CZElauncherMain* g_pZElauncher;
extern std::vector<MapChineseName> g_MapChinese;
extern std::vector<ServerListInfo> g_ServerListInfo;
extern _bstr_t g_SteamID64;
extern bool g_IsDownloadHotmap;
extern TCHAR Version[50];
extern bool IsSkinExist;
