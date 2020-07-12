#include "设置.h"

bool IsSettingCreate = false;


LPCTSTR C设置UI::GetWindowClassName()const
{
	return _T("SettingWnd");
}

void C设置UI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

void C设置UI::InitEditInfo()
{
	//获取CSGO目录
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_csgo")));
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
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_steam")));
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
	//获取背景图片目录
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_bkimge")));
	if (!pEdit)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("edit_bkimge"), NULL, steambuff, sizeof(steambuff), cfgbuff);
	if (_tcslen(steambuff) > 3) {
		pEdit->SetText(steambuff);
	}
	//获取服务器列表背景图片目录
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_listbkimge")));
	if (!pEdit)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("edit_listbkimge"), NULL, steambuff, sizeof(steambuff), cfgbuff);
	if (_tcslen(steambuff) > 3) {
		pEdit->SetText(steambuff);
	}
	//获取预热下载状态
	DuiLib::CCheckBoxUI* pOption = static_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(_T("Option_HotMap")));
	if (!pOption)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("DownloadHotmap"), NULL, steambuff, sizeof(steambuff), cfgbuff);
	if (_tcscmp(steambuff, _T("true")) == 0) {
		pOption->SetCheck(true, false);
	}
}

LRESULT C设置UI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT C设置UI::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("Setting.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	if (IsSettingCreate)__super::Close();
	IsSettingCreate = true;
	InitEditInfo();
	return 0;
}

void C设置UI::SaveEditInfo()
{
	TCHAR cfgbuff[1024] = { 0 };
	GetRunPath(cfgbuff, sizeof(cfgbuff));
	_tcscat(cfgbuff, _T("\\bin\\Config.cfg"));
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_csgo")));
	if (!pEdit)return;
	CDuiString tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), tmpstr.GetData(), cfgbuff);
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_steam")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("SteamPath"), tmpstr.GetData(), cfgbuff);

	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_listbkimge")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("edit_listbkimge"), tmpstr.GetData(), cfgbuff);

	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_bkimge")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("edit_bkimge"), tmpstr.GetData(), cfgbuff);

	if (g_IsDownloadHotmap)tmpstr = _T("true");
	else tmpstr = _T("false");
	WritePrivateProfileString(_T("ZElauncher"), _T("DownloadHotmap"), tmpstr.GetData(), cfgbuff);
}

void C设置UI::OnExit()
{
	IsSettingCreate = false;
	SaveEditInfo();
	if (!Istip) { Istip = true; MessageBox(NULL, _T("部分设置需要重启才能生效!"), NULL, MB_TOPMOST); }
	__super::Close();
}

void C设置UI::GetNewPath(LPCTSTR pEditName)
{
	TCHAR szBuffer[1024] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("请选择游戏目录:");
	bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (!idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(pEditName));
	if (!pEdit)return;
	pEdit->SetText(szBuffer);
	/*::SendMessage(m_hWnd, WM_LBUTTONDOWN, NULL, NULL);
	::SendMessage(m_hWnd, WM_LBUTTONUP, NULL, NULL);*/
}

void C设置UI::GetImagePath(LPCTSTR pEditName)
{
	TCHAR szBuffer[1024] = { 0 };
	OPENFILENAME ofn = { sizeof(OPENFILENAME),0 };
	ofn.hwndOwner = this->m_hWnd;
	ofn.lpstrFileTitle = const_cast<LPWSTR>(_T("打开图片文件"));
	ofn.lpstrFile = const_cast<LPWSTR>(_T("*.*\0*.*\0\0"));
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT
	BOOL bSel = GetOpenFileName(&ofn);
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(pEditName));
	if (!pEdit)return;
	if (_tcslen(szBuffer) > 3)pEdit->SetText(szBuffer);
}

void C设置UI::SetDownloadHotMap()
{
	DuiLib::CCheckBoxUI* pOption = static_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(_T("Option_HotMap")));
	if (!pOption)return;
	g_IsDownloadHotmap = pOption->IsSelected();
	//g_IsDownloadHotmap = !g_IsDownloadHotmap;
}

void C设置UI::OnClick(CControlUI* pSender)
{
	if (_tcscmp(pSender->GetName(), _T("closebtn")) == 0)OnExit();
	else if (_tcscmp(pSender->GetName(), _T("btn_getcsgopath")) == 0)GetNewPath(_T("edit_csgo"));
	else if (_tcscmp(pSender->GetName(), _T("btn_getsteampath")) == 0)GetNewPath(_T("edit_steam"));
	else if (_tcscmp(pSender->GetName(), _T("btn_bkimge_path")) == 0)GetImagePath(_T("edit_bkimge"));
	else if (_tcscmp(pSender->GetName(), _T("btn__listbkimge_path")) == 0)GetImagePath(_T("edit_listbkimge"));
	else if (_tcscmp(pSender->GetName(), _T("Option_HotMap")) == 0)SetDownloadHotMap();
}

void C设置UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)::SetForegroundWindow(m_hWnd);//窗口创建完毕后,获取焦点;
	if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
	OutputDebugString(msg.sType);
	OutputDebugString(L"\n");
}