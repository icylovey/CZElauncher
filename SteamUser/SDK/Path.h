#pragma once
#include "stdafx.h"

void _stdcall GetRunPath(TCHAR* PathBuf, UINT  buflen);

bool _stdcall GetCSGOPath(TCHAR* BufferStr, DWORD BufferLen);

void _stdcall GetSteamPath(TCHAR* BufferStr, DWORD BufferLen);

void _stdcall XSleep(int nWaitInMSecs);

void _MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMulti, int cbMultiLen, LPWSTR lpWideByte, int CbWidelen);