#include "Path.h"

void _stdcall GetRunPath(TCHAR* PathBuf, UINT  buflen)
{
	GetModuleFileName(GetModuleHandle(NULL), PathBuf, buflen);
	TCHAR* pChar = _tcsrchr(PathBuf, '\\');
	if (pChar)pChar[0] = 0;
}

bool _stdcall GetCSGOPath(TCHAR* BufferStr, DWORD BufferLen) {
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

void _stdcall GetSteamPath(TCHAR* BufferStr, DWORD BufferLen)
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

// This structure is used internally by the XSleep function 
struct XSleep_Structure
{
	int duration;
	HANDLE eventHandle;
};


//////////////////////////////////////////////////////////////////////
// Function  : XSleepThread()
// Purpose   : The thread which will sleep for the given duration
// Returns   : DWORD WINAPI
// Parameters:       
//  1. pWaitTime -
//////////////////////////////////////////////////////////////////////
DWORD _stdcall XSleepThread(LPVOID pWaitTime)
{
	XSleep_Structure* sleep = (XSleep_Structure*)pWaitTime;

	Sleep(sleep->duration);
	SetEvent(sleep->eventHandle);

	return 0;
}

void _stdcall XSleep(int nWaitInMSecs)
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