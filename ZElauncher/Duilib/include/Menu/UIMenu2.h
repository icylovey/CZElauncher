#ifndef __UIMENU2_H__
#define __UIMENU2_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "observer_impl_base2.hpp"

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////

struct MenuInfo2 {
	UINT nIndex;
	CDuiString MenuText;
	bool IsSubMenu;
};

typedef void(_stdcall *MenuEvent2)(HWND hWnd, MenuInfo2* MenuInfo);

struct ContextMenuParam2
{
	// 1: remove all
	// 2: remove the sub menu
	WPARAM wParam;
	HWND hWnd;
};

enum MenuAlignment2
{
	eMenuAlignment_Left = 1 << 1,
	eMenuAlignment_Top = 1 << 2,
	eMenuAlignment_Right = 1 << 3,
	eMenuAlignment_Bottom = 1 << 4,
};

typedef class ObserverImpl2<BOOL, ContextMenuParam> ContextMenuObserver2;
typedef class ReceiverImpl2<BOOL, ContextMenuParam> ContextMenuReceiver2;

extern ContextMenuObserver2 s_context_menu_observer;

// MenuUI
extern const TCHAR* const kMenuUIClassName;// = _T("MenuUI");
extern const TCHAR* const kMenuUIInterfaceName;// = _T("Menu");

class CMenuUI2 : public CListUI
{
public:
	CMenuUI2();
	~CMenuUI2();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

	virtual void DoEvent(TEventUI& event);

    virtual bool Add(CControlUI* pControl);
    virtual bool AddAt(CControlUI* pControl, int iIndex);

    virtual int GetItemIndex(CControlUI* pControl) const;
    virtual bool SetItemIndex(CControlUI* pControl, int iIndex);
    virtual bool Remove(CControlUI* pControl, bool bDoNotDestroy=false);

	SIZE EstimateSize(SIZE szAvailable);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
};

/////////////////////////////////////////////////////////////////////////////////////
//

// MenuElementUI
extern const TCHAR* const kMenuElementUIClassName;// = _T("MenuElement");
extern const TCHAR* const kMenuElementUIInterfaceName;// = _T("MenuElement);

class CMenuElementUI2;
class CMenuWnd2 : public CWindowWnd, public ContextMenuReceiver2
{
public:
	CMenuWnd2(HWND hParent = NULL);
	void Init(CMenuElementUI2* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point, MenuEvent2 pMenuInfo);
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL Receive(ContextMenuParam2 param);

public:
	HWND m_hParent;
	POINT m_BasedPoint;
	STRINGorID m_xml;
	CDuiString m_sType;
	CPaintManagerUI m_pm;
	CMenuElementUI2* m_pOwner;
	CMenuUI2* m_pLayout;
};

class CMenuElementUI2 : public CListContainerElementUI
{
	friend CMenuWnd2;
public:
    CMenuElementUI2();
	~CMenuElementUI2();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

	void DrawItemText(HDC hDC, const RECT& rcItem);

	SIZE EstimateSize(SIZE szAvailable);

	bool Activate();

	void DoEvent(TEventUI& event);


	CMenuWnd2* GetMenuWnd();

	void CreateMenuWnd();

protected:
	MenuEvent2 m_menuevet;
	CMenuWnd2* m_pWindow;
};

} // namespace DuiLib

#endif // __UIMENU_H__
