#include "Main.h"


int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	if (!strstr(lpCmdLine, "UpdateCSGOZElauncher"))ExitProcess(NULL);
	TCHAR hEventchar[] = _T("Update_shjd21312ioashdajshdwqniodashjdl232as");
	if (OpenEvent(EVENT_ALL_ACCESS, FALSE, hEventchar) != 0) {
		ExitProcess(NULL);
	}
	CreateEvent(NULL, FALSE, FALSE, hEventchar);
	CPaintManagerUI::SetInstance(hInstance);
	
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	CUpdateUI* pSteamUserUI = new CUpdateUI();
	if (!pSteamUserUI)return 0;
	pSteamUserUI->Create(NULL, _T("ZElauncher更新工具"), UI_WNDSTYLE_EX_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 900, 500);
	pSteamUserUI->CenterWindow();
	::ShowWindow(*pSteamUserUI, SW_SHOW);

	CPaintManagerUI::MessageLoop();
	::CoUninitialize();
	return 0;
}