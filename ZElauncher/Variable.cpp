#include "Variable.h"

_bstr_t gCookies = "";
CZElauncherMain* g_pZElauncher = nullptr;
std::vector<MapChineseName> g_MapChinese;
std::vector<ServerListInfo> g_ServerListInfo;
_bstr_t g_SteamID64 = "";
bool g_IsDownloadHotmap = false, IsSkinExist = false;
TCHAR Version[50]; 