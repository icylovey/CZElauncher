#pragma once
#include "stdafx.h"

using namespace DuiLib;


class CServerMenu :
	public CWindowWnd
{
public:
	CServerMenu(HWND hParent);

private:
	//void Init(CMenuElementUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point);
private:
	HWND m_hParent;
};
