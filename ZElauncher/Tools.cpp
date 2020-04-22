#include "Tools.h"

CToolsUI::CToolsUI(CPaintManagerUI& m_paintmanage)
{
	m_paintmanager = &m_paintmanage;
	CDialogBuilder Builder;
	CContainerUI* pTools = static_cast<CContainerUI*>(Builder.Create(_T("Tools.xml"), NULL, NULL, &m_pm));
	if (pTools) {
		this->Add(pTools);
		m_paintmanager->AddNotifier(this);
	}
	else {
		this->RemoveAll();
		return;
	}
}

void CToolsUI::OnClick(CControlUI* Click)
{
	if (_tcscmp(Click->GetName(), _T("Button_mapclear")) == 0) { 
		TCHAR szBuf[1024] = { 0 };
		GetRunPath(szBuf, sizeof(szBuf));
		_tcscat(szBuf, _T("\\bin\\CSGO_MapsClear.exe"));
		ShellExecute(NULL, _T("open"), szBuf, NULL, szBuf, SW_NORMAL);
	}
	else if (_tcscmp(Click->GetName(), _T("Button_cmd")) == 0) { 
		TCHAR szBuf[1024] = { 0 };
		GetRunPath(szBuf, sizeof(szBuf));
		_tcscat(szBuf, _T("\\bin\\command.exe"));
		ShellExecute(NULL, _T("open"), szBuf, NULL, szBuf, SW_NORMAL);
	}
	else if (_tcscmp(Click->GetName(), _T("Button_SteamUser")) == 0) {
		TCHAR szBuf[1024] = { 0 };
		GetRunPath(szBuf, sizeof(szBuf));
		_tcscat(szBuf, _T("\\bin\\SteamUser.exe"));
		ShellExecute(NULL, _T("open"), szBuf, NULL, szBuf, SW_NORMAL);
	}

}

void CToolsUI::Notify(TNotifyUI& msg)
{

	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnCreate();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);

}

void CToolsUI::OnCreate()
{
	
}


