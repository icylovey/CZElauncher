#include "Path.h"

zip_7Z::SavenZip pSavenZip = nullptr;
zip_7Z::SevenZipClearOwnerWindow pSavenZipClearOwnerWindow = nullptr;
zip_7Z::SevenZipSetOwnerWindowEx pSavenZipSetOwnerWindowEx = nullptr;
zip_7Z::ZipCallBackProc pZipCallBackProc = nullptr;

void __stdcall GetRunPath(TCHAR* PathBuf, UINT  buflen)
{
	GetModuleFileName(GetModuleHandle(NULL), PathBuf, buflen);
	TCHAR* pChar = _tcsrchr(PathBuf, '\\');
	if (pChar)pChar[0] = 0;
}

bool __stdcall GetCSGOPath(TCHAR* BufferStr, DWORD BufferLen) {
	//获取注册表
	TCHAR GamePathKey[] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 730\\");
	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 730
	HKEY hKey;
	int Rkey = RegOpenKeyEx(HKEY_LOCAL_MACHINE, GamePathKey, NULL, KEY_WOW64_64KEY | KEY_READ, &hKey);
	if (Rkey != ERROR_SUCCESS) {
		return false;
	}
	else {
		int Rread = RegQueryValueEx(hKey, _T("InstallLocation"), NULL, NULL, (LPBYTE)BufferStr, &BufferLen);
		if (Rread != ERROR_SUCCESS) {
			return false;
		}
		else {
			return true;
		}
	}
	return false;
}

void __stdcall GetSteamPath(TCHAR* BufferStr, DWORD BufferLen)
{
	//获取Steam目录
	const TCHAR SteamPathKey[] = _T("SOFTWARE\\Classes\\steam\\Shell\\Open\\Command");
	//HKEY_LOCAL_MACHINE\SOFTWARE\Classes\steam\Shell\Open\Command
	HKEY hKey;
	int Rkey = RegOpenKeyEx(HKEY_LOCAL_MACHINE, SteamPathKey, NULL, KEY_WOW64_64KEY | KEY_READ, &hKey);
	WCHAR KeyData[1024] = { 0 }, * wp = nullptr;
	DWORD DataSize = 1024;
	int Rread = RegQueryValueEx(hKey, NULL, NULL, NULL, (BYTE*)KeyData, &DataSize);
	if (KeyData[0] == '\"') {
		wp = (WCHAR*)((char*)KeyData + (sizeof(WCHAR)));
		wp = _tcschr(wp, '\"');
		ZeroMemory(wp, sizeof(TCHAR));
		//使用steam启动游戏
		wp = (WCHAR*)((char*)KeyData + (sizeof(WCHAR)));
		TCHAR* pchar = _tcsrchr(wp, '\\');
		ZeroMemory(pchar, sizeof(TCHAR));
		_tcscpy(BufferStr, wp);
	}
}

struct XSleep_Structure
{
	int duration;
	HANDLE eventHandle;
};

DWORD __stdcall XSleepThread(LPVOID pWaitTime)
{
	XSleep_Structure* sleep = (XSleep_Structure*)pWaitTime;

	Sleep(sleep->duration);
	SetEvent(sleep->eventHandle);

	return 0;
}

void __stdcall XSleep(int nWaitInMSecs)
{
	XSleep_Structure sleep;

	sleep.duration = nWaitInMSecs;
	sleep.eventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD threadId;
	CreateThread(NULL, 0, &XSleepThread, &sleep, 0, &threadId);

	MSG msg;

	while (::WaitForSingleObject(sleep.eventHandle, 0) == WAIT_TIMEOUT)
	{
		//get and dispatch messages
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	CloseHandle(sleep.eventHandle);
}

void _MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMulti, int cbMultiLen, LPWSTR lpWideByte, int CbWidelen)
{
	ZeroMemory(lpWideByte, CbWidelen);
	MultiByteToWideChar(CodePage, dwFlags, lpMulti, cbMultiLen, lpWideByte, CbWidelen);

}

void __stdcall Zip7ZInitiale()
{
	HMODULE hlibr = LoadLibrary(_T("bin\\7z.dll"));
	if (!hlibr)goto Exit;
	pSavenZip = reinterpret_cast<zip_7Z::SavenZip>(GetProcAddress(hlibr, "SevenZip"));
	if(!pSavenZip)goto Exit;
	pSavenZipClearOwnerWindow = reinterpret_cast<zip_7Z::SevenZipClearOwnerWindow>(GetProcAddress(hlibr, "SevenZipClearOwnerWindow"));
	if (!pSavenZipClearOwnerWindow)goto Exit;
	pSavenZipSetOwnerWindowEx = reinterpret_cast<zip_7Z::SevenZipSetOwnerWindowEx>(GetProcAddress(hlibr, "SevenZipSetOwnerWindowEx"));
	return;
	if (!pSavenZipSetOwnerWindowEx)goto Exit;
Exit:
	FreeLibrary(hlibr);
	MessageBox(NULL, _T("7Z初始化失败!"), NULL, MB_OK);
	return;
}

/*
BOOL __stdcall ArchiverProc(HWND hWnd, UINT uMsg, UINT nState, zip_7Z::tagExtractingInfoEx* lpeis)
{
	if (nState == 2)return TRUE;
	zip_7Z::tagExtractingInfoEx tagexinfo = { 0 };
	if(lpeis)CopyMemory(&tagexinfo, lpeis, sizeof(zip_7Z::tagExtractingInfoEx));
	UINT iPos2 = tagexinfo.exInfo.dwWriteSize;
	//UINT nStat=ns
	//if (pZipCallBackProc)pZipCallBackProc(NULL, nState, nPos);
	return FALSE;
}*/

bool __stdcall Zip7ZUnCompressed(const TCHAR* lpZipFile, const TCHAR* CompressedPWD, const TCHAR* szUncompressedDir, std::string& szOutPut, zip_7Z::ZipCallBackProc lpCallbackProc)
{
	_bstr_t sCmdline = _T("x \"");
	sCmdline += lpZipFile;
	sCmdline += _T("\" -o\"");
	sCmdline += szUncompressedDir;
	sCmdline += _T("\" \"*.*\" -r -hide -y");
	if (_tcslen(CompressedPWD) > 0) {
		sCmdline += _T(" -p");
		sCmdline += CompressedPWD;
	}
	//pSavenZipSetOwnerWindowEx(NULL, ArchiverProc); 设置解压回调
	char* szOut = new char[32768];
	ZeroMemory(szOut, 32768);
	pSavenZip(NULL, static_cast<char*>(sCmdline), szOut, 32768);
	pSavenZipClearOwnerWindow();
	szOutPut = szOut;
	if (szOutPut.find("error:") != std::string::npos)return false;
	else if (szOutPut.find("operation aborted") != std::string::npos)return false;
	else if (szOutPut.find("Everything is Ok") != std::string::npos)return true;
	else return false;
}