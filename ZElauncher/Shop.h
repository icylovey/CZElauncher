#pragma once
#include "stdafx.h"
#include <atlimage.h>

using namespace DuiLib;

class CShopUI :
	public CContainerUI
	, public INotifyUI
{
public:
	CShopUI(CPaintManagerUI& m_paintmanage);
	void GetShopinfo();
private:
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* Click);
	void OnCreate();
	std::string GetSkinHtml(const TCHAR* Page);
private:
	CPaintManagerUI* m_paintmanager;
	CPaintManagerUI m_pm;
	UINT m_PageNum;
	UINT m_pageNumMax;
};