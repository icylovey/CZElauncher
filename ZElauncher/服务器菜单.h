#pragma once
#include "stdafx.h"

using namespace DuiLib;


class C服务器菜单UI :
	public CWindowWnd
{
public:
	C服务器菜单UI(HWND hParent);

private:
	//void Init(CMenuElementUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point);
private:
	HWND m_hParent;
};
