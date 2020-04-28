#pragma once
#include "stdafx.h"

using namespace DuiLib;
class CLookPlayerUI :
	public CWindowWnd
	,public INotifyUI
{
public:
	CLookPlayerUI(CPaintManagerUI& paintmanager, CDuiString Tip) { m_paintmanage = &paintmanager; m_LookPlayerUITip = Tip; };
	LPCTSTR GetWindowClassName()const { return _T("AwardTip"); };
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
	CDuiString m_LookPlayerUITip;
};

