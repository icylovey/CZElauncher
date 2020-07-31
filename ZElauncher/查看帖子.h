#pragma once
#include "stdafx.h"
#include "ModuleSDK/Path.h"
#include <Shlobj.h>

using namespace DuiLib;

extern CDuiString g_BBSUrl;

class C查看帖子UI : public WindowImplBase
{
public:
	C查看帖子UI(){}
	~C查看帖子UI() {} //::PostQuitMessage(0L); }
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	//UILIB_RESOURCETYPE GetResourceType()const;
	UILIB_RESTYPE GetResourceType()const;
	LPCTSTR GetResourceID()const;
	void OnFinalMessage(HWND hWnd);
	LPCTSTR GetWindowClassName()const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void InitWindow();
private:
	void OnExit(const TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
	void 获取帖子内容();
private:
	CPaintManagerUI m_pTilelayout;
};