#pragma once
#include "stdafx.h"
#include "Award_Tip.h"
#include <time.h>

using namespace DuiLib;

extern TCHAR G_TodayUIName[50];
#define Url_Online_10 _T("https://bbs.93x.net/plugin.php?id=levgift:l&fh=1ae5e936&m=_getaward1.5.1")
#define Url_Online_60 _T("https://bbs.93x.net/plugin.php?id=levgift:l&fh=1ae5e936&m=_getaward1.7.1")
#define Url_Online_by _T("https://bbs.93x.net/plugin.php?id=levgift:l&fh=1ae5e936&m=_getaward1.20.1")
#define Url_Online_hj _T("https://bbs.93x.net/plugin.php?id=levgift:l&fh=1ae5e936&m=_getaward1.21.1")

class CAwardUI :
	public CContainerUI
	,public INotifyUI
{
public:
	CAwardUI(CPaintManagerUI& m_paintmanage);
private:
	void DrawCalendar(CContainerUI* pAwarad);
	int GetMonthDays(int iY, int iM);
	void DoEvent(TEventUI& event);
private:
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* Click);
	void Sign93x();
	void online_Award(LPCTSTR pUrl, LPCTSTR szAward);
	void yaoyiyao();
	_bstr_t GetFormhash(LPCTSTR pUrl);
	bool IsStart_yaoyiyao(LPCTSTR pUrl);
private:
	CPaintManagerUI* m_paintmanager;
	CPaintManagerUI m_pm;
	CDuiString m_Signbtn;
};
