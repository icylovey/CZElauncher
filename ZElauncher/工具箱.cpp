#include "工具箱.h"

C工具箱UI::C工具箱UI(CPaintManagerUI& m_paintmanage)
{
	m_paintmanage_ = &m_paintmanage;
	CDialogBuilder Builder;
	CContainerUI* pTools = static_cast<CContainerUI*>(Builder.Create(_T("Tools.xml"), NULL, NULL, &m_pm2));
	if (pTools) {
		this->Add(pTools);
		m_paintmanage_->AddNotifier(this);
	}
	else {
		this->RemoveAll();
		return;
	}
}

void C工具箱UI::OnClick(CControlUI* Click)
{
	if (_tcscmp(Click->GetName(), _T("Button_mapclear")) == 0) { 
		_bstr_t szBuf = GetRunPath();;
		szBuf += _T("\\bin\\CSGO_MapsClear.exe");
		ShellExecute(NULL, _T("open"), szBuf, NULL, szBuf, SW_NORMAL);
	}
	else if (_tcscmp(Click->GetName(), _T("Button_cmd")) == 0) { 
		_bstr_t szBuf = GetRunPath();
		szBuf += _T("\\bin\\command.exe");
		ShellExecute(NULL, _T("open"), szBuf, NULL, szBuf, SW_NORMAL);
	}
	else if (_tcscmp(Click->GetName(), _T("Button_SteamUser")) == 0) {
		_bstr_t szBuf = GetRunPath();
		szBuf += _T("\\bin\\SteamUser.exe");
		ShellExecute(NULL, _T("open"), szBuf, NULL, szBuf, SW_NORMAL);
	}

}

void C工具箱UI::Notify(TNotifyUI& msg)
{

	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnCreate();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);

}

void C工具箱UI::OnCreate()
{
	
}


