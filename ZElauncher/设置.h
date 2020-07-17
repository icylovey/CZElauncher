#pragma once
#include "stdafx.h"
#include <Shlobj.h>

using namespace DuiLib;

class C…Ë÷√UI : public WindowImplBase
{
public:
	C…Ë÷√UI(){}
	~C…Ë÷√UI() {} //::PostQuitMessage(0L); }
	void OnFinalMessage(HWND hWnd);
	LPCTSTR GetWindowClassName()const;
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	UILIB_RESTYPE GetResourceType()const;
	LPCTSTR GetResourceID()const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void InitWindow();
private:
	void OnExit();
	void OnClick(CControlUI* pSender);
	void Notify(TNotifyUI& msg);
	void InitEditInfo();
	void SaveEditInfo();
	void GetNewPath(LPCTSTR pEditName);
	void GetImagePath(LPCTSTR pEditName);
	void SetDownloadHotMap();

private:
	bool Istip = false;
};