#pragma once
#include "../stdafx.h"
#include <comutil.h>

namespace zip_7Z{
	struct tagExtractingInfo
	{
		UINT dwFileSize;
		UINT dwWriteSize;
		char szSourceFileName[513];
		char dummy1[3];
		char szDestFileName[513];
		char dummy[3];
	};
	struct tagExtractingInfoEx
	{
		tagExtractingInfo exInfo;
		UINT dwCompressedSize;
		UINT dwCRC;
		UINT uOSType;
		UINT wRatio;
		UINT wDate;
		UINT wTime;
		char szAttribute[8];
		char szMode[8];
	};
	typedef void(__stdcall *SavenZip)(HWND hWnd, const char* szCmdline, LPVOID szOutputBuf, DWORD szBufsize);
	//typedef BOOL(__stdcall *ArchiverProc)(HWND hWnd, UINT uMsg, UINT nState, tagExtractingInfoEx* lpeis);
	typedef void(__stdcall *SevenZipSetOwnerWindowEx)(HWND hWnd, LPVOID lpArchProc);
	typedef void(__stdcall *SevenZipClearOwnerWindow)();
	typedef void(__stdcall *ZipCallBackProc)(HWND hWnd, UINT nState, UINT nRatePos);
}

_bstr_t __stdcall GetRunPath();

_bstr_t __stdcall GetCFGPath();

bool __stdcall GetCSGOPath(TCHAR* BufferStr, DWORD BufferLen);

void __stdcall GetSteamPath(TCHAR* BufferStr, DWORD BufferLen);

void __stdcall XSleep(int nWaitInMSecs);

bool __stdcall Zip7ZUnCompressed(const TCHAR* lpZipFile, const TCHAR* CompressedPWD, const TCHAR* szUncompressedDir, std::string& szOutPut, zip_7Z::ZipCallBackProc lpCallbackProc);

void __stdcall Zip7ZInitiale();

//±àÂë½âÂë
std::string __stdcall DecodeToString(UINT Codepage, std::string& EncodeChar);

std::string __stdcall DecodeToString(UINT Codepage, std::wstring& EncodeChar);