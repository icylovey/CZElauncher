#pragma once
#include "stdafx.h"
#include <atlimage.h>

using namespace DuiLib;

class C新手皮肤UI :
	public CContainerUI
	, public INotifyUI
{
	struct SkinConvertInfo
	{
		_bstr_t sSkinConvertUrl = "";
		_bstr_t sSkinName = "";
		_bstr_t sSkinImg = "";
	};
public:
	C新手皮肤UI(CPaintManagerUI& m_paintmanage);
	void 启动线程();
	void 获取新手皮肤数据();
private:
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* Click);
	void OnCreate();
	std::string 获取皮肤html数据(const TCHAR* Page);
	CDuiString 获取图片地址(LPCTSTR pUrl, UINT iFlag);
	void 兑换皮肤(LPCTSTR pUrl);
private:
	CPaintManagerUI* m_paintmanager;
	CPaintManagerUI m_pm;
	CPaintManagerUI m_pTilelayout;
	CContainerUI* pSKin = nullptr;
	UINT m_PageNum;
	UINT m_pageNumMax;
	std::vector<SkinConvertInfo> m_SkinConvertInfo;
};