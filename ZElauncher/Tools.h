#pragma once
#include "stdafx.h"
#include <atlimage.h>

using namespace DuiLib;

class CToolsUI :
	public CContainerUI
	, public INotifyUI
{
public:
	CToolsUI(CPaintManagerUI& m_paintmanage);

private:
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* Click);
	void OnCreate();

private:
	CPaintManagerUI* m_paintmanager;
	CPaintManagerUI m_pm;
};