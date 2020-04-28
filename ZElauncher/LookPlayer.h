#pragma once
#include "stdafx.h"
#include "ModuleSDK/Path.h"
#include <Shlobj.h>

using namespace DuiLib;

extern CDuiString g_ServerName_LookPlayer;

class CLookPlayerUI : public WindowImplBase
{
public:
	CLookPlayerUI(){}
	~CLookPlayerUI() {} //::PostQuitMessage(0L); }
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	UILIB_RESOURCETYPE GetResourceType()const;
	LPCTSTR GetResourceID()const;
	void OnFinalMessage(HWND hWnd);
	LPCTSTR GetWindowClassName()const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void InitWindow();
private:
	void OnExit(const TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
	void GetPlayer(LPCTSTR lpServer);
};