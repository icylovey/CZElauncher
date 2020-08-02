#include "Main.h"
using namespace DuiLib;
//检测版本更新
void __stdcall GetUpdateVersion(LPCTSTR sVersion)
{
	_tcscpy(Version, sVersion);
	std::string htmldata;
	{
		lib_http::CLibhttp http;
		http.GET(_T("http://blog.sina.com.cn/s/blog_155297e510102z9f9.html"), htmldata);
	}
	//转换编码
	UINT nLen = htmldata.size() * sizeof(TCHAR);
	TCHAR* pTmpHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, htmldata.c_str(), htmldata.length(), pTmpHtml, nLen);
	_bstr_t Version = sVersion;
	std::wstring html = pTmpHtml;
	//获取版本号
	UINT nPos = html.find(L"#Version#") + wcslen(L"#Version#");
	if (nPos == std::string::npos)MessageBox(NULL, _T("获取版本错误!"), NULL, MB_OK | MB_TOPMOST);
	_bstr_t NowVersion = html.substr(nPos, html.find(L"$Version$", nPos) - nPos).c_str();
	//获取更新地址
	nPos = html.find(L"#UpdateUrl#") + wcslen(L"#UpdateUrl#");
	if (nPos == std::string::npos)MessageBox(NULL, _T("获取版本错误!"), NULL, MB_OK | MB_TOPMOST);
	CDuiString Updateurl = html.substr(nPos, html.find(L"$UpdateUrl$", nPos) - nPos).c_str();
	Updateurl.Replace(_T("amp;"), _T(""));
	//获取更新信息
	nPos = html.find(L"#UpdateInfo#") + wcslen(L"#UpdateInfo#");
	if (nPos == std::string::npos)MessageBox(NULL, _T("获取版本错误!"), NULL, MB_OK | MB_TOPMOST);
	CDuiString UpdateTip = _T("\t有新版本发布,是否前往更新!\r\n\r\n");
	UpdateTip += html.substr(nPos, html.find(L"$UpdateInfo$", nPos) - nPos).c_str();
	UpdateTip.Replace(_T("\\r\\n"), _T("\r\n"));
	if (_tcscmp(Version, NowVersion) != 0) {
		if (MessageBox(NULL, UpdateTip, NULL, MB_YESNO | MB_TOPMOST) == IDYES) {
			if (Updateurl.GetLength() < 3) {
				MessageBox(NULL, _T("更新地址获取错误!"), NULL, MB_OK | MB_TOPMOST);
				ExitProcess(NULL);
			}
			else {
				ShellExecute(NULL, _T("open"), _T("UpdateDownload.exe"),_T("UpdateCSGOZElauncher"), NULL, SW_NORMAL);
				ExitProcess(NULL);
			}
		}
	}
}
//禁止重复运行
void __stdcall NotRepeat()
{
	TCHAR hEventchar[] = _T("ZElauncher_shjdioashdajshdwqniodashjdlas");
	if (OpenEvent(EVENT_ALL_ACCESS, FALSE, hEventchar) != 0) {
		ExitProcess(NULL);
	}
	CreateEvent(NULL, FALSE, FALSE, hEventchar);
}

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	NotRepeat();

	GetUpdateVersion(_T("v1.2.4"));
	Zip7ZInitiale();

	CPaintManagerUI::SetInstance(hInstance);
	TCHAR RunPath[MAX_PATH] = { 0 };
	TCHAR SkinName[MAX_PATH] = { 0 };
	GetRunPath(RunPath, sizeof(RunPath));
	_bstr_t CfgPath = RunPath;
	CfgPath += _T("\\bin\\Config.cfg");
	GetPrivateProfileString(_T("ZElauncher"), _T("Skin"), NULL, SkinName, sizeof(SkinName), CfgPath);
	_bstr_t SkinFile = RunPath;
	SkinFile += _T("\\Skin\\");
	SkinFile += SkinName;
	FILE* pFile = fopen(SkinFile, "rb");
	if (pFile) {
		CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath() + _T("Skin"));
		CPaintManagerUI::SetResourceZip(SkinName);
		IsSkinExist = true;
		fclose(pFile);
	}
	else if (_tcscmp(SkinName, _T("default")) != 0)
	{
		IsSkinExist = false;
		WritePrivateProfileString(_T("ZElauncher"), _T("Skin"), _T("default"), CfgPath);
		MessageBox(NULL, _T("找不到皮肤主题文件,使用默认皮肤!"), NULL, MB_OK | MB_TOPMOST);
	}
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CZElauncherMain* pFrame = new CZElauncherMain();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("CSGO僵尸逃跑社区服"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW | WS_MINIMIZEBOX, 0, 0, 900, 500);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();
	::CoUninitialize();
	return 0;
}
