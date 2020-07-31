#pragma once
#include <iostream>
#include <comutil.h>
#include <vector>
#include "ZElauncher.h"

class CZElauncherMain;
struct MapChineseName
{
	DuiLib::CDuiString Map_en;
	DuiLib::CDuiString Map_cn;
	DuiLib::CDuiString MapDownloadurl;
};
struct ServerListInfo  
{
	DuiLib::CDuiString ServerName;
	DuiLib::CDuiString ServerIp;
};

extern _bstr_t gCookies;
extern CZElauncherMain* g_pZElauncher;
extern std::vector<MapChineseName> g_MapChinese;
extern std::vector<ServerListInfo> g_ServerListInfo;
extern _bstr_t g_SteamID64;
extern bool g_IsDownloadHotmap;
extern TCHAR Version[50];
extern bool IsSkinExist;
