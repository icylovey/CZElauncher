#pragma once
#include "stdafx.h"
#include <Shlobj.h>

using namespace DuiLib;

class C…Ë÷√UI : public CWindowWnd, public INotifyUI
{
public:
	C…Ë÷√UI(){}
	~C…Ë÷√UI() {} //::PostQuitMessage(0L); }
	void OnFinalMessage(HWND hWnd);
	LPCTSTR GetWindowClassName()const;
private:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnClick(CControlUI* pSender);
	void OnExit();
	void Notify(TNotifyUI& msg);
	void InitEditInfo();
	void SaveEditInfo();
	void GetNewPath(LPCTSTR pEditName);
	void GetImagePath(LPCTSTR pEditName);
	void SetDownloadHotMap();

private:
	bool Istip = false;
	CPaintManagerUI m_pm;
};