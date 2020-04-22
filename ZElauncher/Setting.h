#pragma once
#include "stdafx.h"
#include <Shlobj.h>

using namespace DuiLib;

class CSetting : public WindowImplBase
{
public:
	CSetting(){}
	~CSetting() {} //::PostQuitMessage(0L); }
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
	void InitEditInfo();
	void SaveEditInfo();
	void GetNewPath(LPCTSTR pEditName);
};