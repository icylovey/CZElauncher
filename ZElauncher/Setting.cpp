#include "Setting.h"

bool IsSettingCreate = false;

LPCTSTR CSetting::GetWindowClassName()const
{
	return _T("SettingWnd");
}

CDuiString CSetting::GetSkinFile()
{
	return _T("Setting.xml");
}

CDuiString CSetting::GetSkinFolder()
{
	return _T("Skin\\ListRes\\");
}

UILIB_RESOURCETYPE CSetting::GetResourceType()const
{
	return UILIB_FILE;
}

LPCTSTR CSetting::GetResourceID()const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CSetting::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* CSetting::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CSetting::InitEditInfo()
{
	//获取CSGO目录
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_csgo")));
	if (!pEdit)return;
	TCHAR csgobuff[1024] = { 0 };
	TCHAR steambuff[1024] = { 0 };
	TCHAR cfgbuff[1024] = { 0 };
	GetRunPath(cfgbuff, sizeof(cfgbuff));
	_tcscat(cfgbuff, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), NULL, csgobuff, sizeof(csgobuff), cfgbuff);
	if (_tcslen(csgobuff) > 3) {
		pEdit->SetText(csgobuff);
	}
	else
	{
		GetCSGOPath(csgobuff, sizeof(csgobuff));
		pEdit->SetText(csgobuff);
	}
	//获取Steam目录
	pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_steam")));
	if (!pEdit)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("SteamPath"), NULL, steambuff, sizeof(steambuff), cfgbuff);
	if (_tcslen(steambuff) > 3) {
		pEdit->SetText(steambuff);
	}
	else
	{
		GetSteamPath(steambuff, sizeof(steambuff));
		pEdit->SetText(steambuff);
	}
}

void CSetting::InitWindow()
{
	if (IsSettingCreate)__super::Close();
	IsSettingCreate = true;
	InitEditInfo();
}

void CSetting::SaveEditInfo()
{
	TCHAR cfgbuff[1024] = { 0 };
	GetRunPath(cfgbuff, sizeof(cfgbuff));
	_tcscat(cfgbuff, _T("\\bin\\Config.cfg"));
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_csgo")));
	if (!pEdit)return;
	CDuiString tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), tmpstr.GetData(), cfgbuff);
	pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_steam")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("SteamPath"), tmpstr.GetData(), cfgbuff);
}

void CSetting::OnExit(const TNotifyUI& msg)
{
	IsSettingCreate = false;
	SaveEditInfo();
	__super::Close();
}

void  CSetting::GetNewPath(LPCTSTR pEditName)
{
	TCHAR szBuffer[1024] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("请选择游戏目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (!idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(pEditName));
	if (!pEdit)return;
	pEdit->SetText(szBuffer);
	/*::SendMessage(m_hWnd, WM_LBUTTONDOWN, NULL, NULL);
	::SendMessage(m_hWnd, WM_LBUTTONUP, NULL, NULL);*/
}

void CSetting::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)::SetForegroundWindow(m_hWnd);//窗口创建完毕后,获取焦点;
	else if (_tcscmp(msg.pSender->GetName(), _T("closebtn")) == 0)OnExit(msg);
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_getcsgopath")) == 0) {
		/*std::list<std::future<void>>lk;
		auto __p = std::async(std::launch::async, &CSetting::GetNewPath, this, _T("edit_csgo"));
		lk.push_back(std::move(__p));*/
		GetNewPath(_T("edit_csgo"));
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_getsteampath")) == 0)GetNewPath(_T("edit_steam"));
}