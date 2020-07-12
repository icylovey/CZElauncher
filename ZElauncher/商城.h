#pragma once
#include "stdafx.h"
#include <atlimage.h>

using namespace DuiLib;

class C商城UI :
	public CContainerUI
	, public INotifyUI
{
public:
	C商城UI(CPaintManagerUI& m_paintmanage);
	void 启动获取商城数据函数();
private:
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* Click);
	void OnCreate();
	void 获取商城数据();
	void 设置图片可视状态();
	bool 判断正则表达式数据库是否存在(TCHAR* 正则表达式);
	std::string 获取皮肤html数据(const TCHAR* Page);
private:
	CPaintManagerUI* m_paintmanager;
	CPaintManagerUI m_pm;
	UINT m_PageNum;
	UINT m_pageNumMax;
};