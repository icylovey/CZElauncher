#pragma once
#include "stdafx.h"
#include <atlimage.h>

using namespace DuiLib;

class CSkinUI :
	public CContainerUI
	, public INotifyUI
{
	struct SkinConvertInfo
	{
		_bstr_t sSkinConvertUrl;
		_bstr_t sSkinName;
		_bstr_t sSkinImg;
	};
public:
	CSkinUI(CPaintManagerUI& m_paintmanage);
	void static __stdcall RunThread();
	void GetSkininfo();
private:
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* Click);
	void OnCreate();
	std::string GetSkinHtml(const TCHAR* Page);
	CDuiString GetImgStr(LPCTSTR pUrl, UINT iFlag);
	void ConvertSkin(LPCTSTR pUrl);
private:
	CPaintManagerUI* m_paintmanager;
	CPaintManagerUI m_pm;
	CPaintManagerUI m_pTilelayout;
	CContainerUI* pSKin = nullptr;
	UINT m_PageNum;
	UINT m_pageNumMax;
	std::vector<SkinConvertInfo> m_SkinConvertInfo;
};