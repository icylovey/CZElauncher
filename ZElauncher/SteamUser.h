#pragma once
#include "stdafx.h"

using namespace DuiLib;
class CSteamUserUI :
	public CWindowWnd
	, public INotifyUI
{
public:
	CSteamUserUI(CPaintManagerUI& paintmanager, CDuiString Tip) { m_paintmanage = &paintmanager; };
	LPCTSTR GetWindowClassName()const { return _T("SteamUser"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void Notify(TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void OnClick(CControlUI* pSender);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnClose();
	void OnTip();
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	CPaintManagerUI* m_paintmanage;
	CPaintManagerUI m_pm;
};

