#include "Main.h"


int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	TCHAR hEventchar[] = _T("SteamUser_shjd21312ioashdajshdwqniodashjdlas");
	if (OpenEvent(EVENT_ALL_ACCESS, FALSE, hEventchar) != 0) {
		ExitProcess(NULL);
	}
	CreateEvent(NULL, FALSE, FALSE, hEventchar);
	CPaintManagerUI::SetInstance(hInstance);
	
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	CSteamUserMain* pSteamUserUI = new CSteamUserMain();
	if (!pSteamUserUI)return 0;
	pSteamUserUI->Create(NULL, _T("SteamÕÊºÅÇÐ»»¹¤¾ß"), UI_WNDSTYLE_EX_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 900, 500);
	pSteamUserUI->CenterWindow();
	::ShowWindow(*pSteamUserUI, SW_SHOW);

	CPaintManagerUI::MessageLoop();
	::CoUninitialize();
	return 0;
}