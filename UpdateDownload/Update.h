#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <atlimage.h>

#include "resource.h"
#include "unzip.h"
#include "Duilib/StdAfx.h"
#include "../ZElauncher/json/json.h"
#include "../ZElauncher/ModuleSDK/lib_http.h"
#include "../ZElauncher/ModuleSDK/lib_Process.h"


#import <progid:VBScript.RegExp>named_guids
#ifdef _DEBUG
#include "Debug/3.tlh"
#endif // _DEBUG


using namespace DuiLib;
class CUpdateUI :
	public WindowImplBase
{
public:
	CUpdateUI() {}
	~CUpdateUI() { ::PostQuitMessage(0L); }
	LPCTSTR  GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	LPCTSTR GetResourceID() const;
	UILIB_RESTYPE GetResourceType() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void SetProgressText(LPCTSTR pString);
	void SetProgressValue(UINT Value);
private:
	void Notify(TNotifyUI& msg);					//消息处理事件
	void OnExit(const TNotifyUI& msg);				//程序离开事件
	void OnClick(const TNotifyUI& msg);				//按钮单击事件
	void OnCreate();
	CDuiString GetUpdateVersionUrl();
	void DownloadThread(CDuiString& Url);
private:
	CPaintManagerUI m_pListManager;

};


