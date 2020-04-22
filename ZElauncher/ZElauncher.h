#pragma once
#include "stdafx.h"
#include "Server.h"
#include "Award.h"
#include "Setting.h"
#include "Shop.h"
#include "Tools.h"
#include "Skin.h"

extern TCHAR Version[50];
extern bool IsSkinExist;

using namespace DuiLib;
class CZElauncherMain : 
	public WindowImplBase
{
public:
	CZElauncherMain() {}
	~CZElauncherMain() { ::PostQuitMessage(0L); }
	LPCTSTR  GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);
	void InitWindow();
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	LPCTSTR GetResourceID() const;
	UILIB_RESOURCETYPE GetResourceType() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void SetSkinName(LPCTSTR SkinName);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	void OnSetting(bool IsModel = false);
private:
	void Notify(TNotifyUI& msg);
	void OnExit(const TNotifyUI& msg);
	void OnClick(const TNotifyUI& msg);
	void OnMenu(const TNotifyUI& msg);
	void OnInitialize();
	void InitLogin();
	void LoginUser();
	void OutTip(LPCTSTR pOutTip);
private:
	const TCHAR* m_pMenuBtn = _T("menubtn");
	const TCHAR* m_pMinBtn = _T("minbtn");
	const TCHAR* m_pCloseBtn = _T("closebtn");
	const TCHAR* m_pBugBtn = _T("BUGbtn");
	const TCHAR* m_pBBSBtn = _T("BBSbtn");
	CShopUI* pShop = nullptr;
	CSkinUI* pSkin = nullptr;
	bool IsCreateShop = false;
	bool IsCreateSkin = false;

};

