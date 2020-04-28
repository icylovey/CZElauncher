#include "ZElauncher.h"

CZElauncherMain* pZElauncher = nullptr;
bool IsCreate = false, IsSkinExist;
TCHAR Version[50];

LPCTSTR CZElauncherMain::GetWindowClassName() const
{
	return _T("ZElauncherForm");
};

UILIB_RESOURCETYPE CZElauncherMain::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else 
	if (!IsSkinExist)return UILIB_ZIPRESOURCE;
	else return UILIB_FILE;
#endif // _DEBUG

}

LPCTSTR CZElauncherMain::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

CDuiString CZElauncherMain::GetSkinFile()
{
	//return _T("115");
	return _T("skin.xml");
}

CDuiString CZElauncherMain::GetSkinFolder()
{
	return _T("Skin\\ListRes\\");
}

void CZElauncherMain::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CZElauncherMain::InitWindow()
{
	pZElauncher = this;
	TCHAR CfgPath[MAX_PATH] = { 0 };
	TCHAR Server_Switch[20] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("Server_Switch"), NULL, Server_Switch, sizeof(Server_Switch), CfgPath);
	if (_tcscmp(Server_Switch, _T("true")) == 0) {
		COptionUI* pButton = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("Server_Switch")));
		if (pButton)pButton->Selected(true);
	}
	_bstr_t sVersion = _T("{c #386382}当前版本：");
	sVersion += Version;
	sVersion += _T("{/c}");
	CTextUI* pText = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("VersionTip")));
	if (pText)pText->SetText(sVersion);
}

void CZElauncherMain::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnInitialize();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg);
	else if (_tcscmp(msg.sType, _T("selectchanged")) == 0)
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("Tabchild")));
		if (!pControl)return;
		COptionUI* pSwitch = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("Server_Switch")));
		if (!pSwitch)return;
		if (name == _T("Title_Server"))pControl->SelectItem(0);
		else if (name == _T("Title_award"))pControl->SelectItem(1);
		else if (name == _T("Title_Tools"))pControl->SelectItem(2);
		else if (name == _T("Title_shop")) {
			pControl->SelectItem(3);
			if (!IsCreateShop) {
				IsCreateShop = true;
				pShop->GetShopinfo();
			}
		}
		else if (name == _T("Title_Skin")) {
			pControl->SelectItem(4);
			if (!IsCreateSkin) {
				IsCreateSkin = true;
				pSkin->GetSkininfo();
			}
		}
		else if (name == _T("Server_Switch")) {
			TCHAR CfgPath[MAX_PATH] = { 0 };
			GetRunPath(CfgPath, sizeof(CfgPath));
			_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
			if (pSwitch->IsSelected())WritePrivateProfileString(_T("ZElauncher"), _T("Server_Switch"), _T("true"), CfgPath);
			else WritePrivateProfileString(_T("ZElauncher"), _T("Server_Switch"), _T("false"), CfgPath);
		}
	}
	__super::Notify(msg);
}

CControlUI* CZElauncherMain::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("Server")) == 0)return new CServerUI(m_PaintManager, this->m_hWnd);
	else if (_tcscmp(pstrClass, _T("Award")) == 0)return new CAwardUI(m_PaintManager);
	else if (_tcscmp(pstrClass, _T("Shop")) == 0) {
		pShop = new CShopUI(m_PaintManager);
		return pShop;
	}
	else if (_tcscmp(pstrClass, _T("Tools")) == 0)return new CToolsUI(m_PaintManager);
	else if (_tcscmp(pstrClass, _T("Skin")) == 0) {
		pSkin = new CSkinUI(m_PaintManager);
		return pSkin;
	}
	return NULL;
}

void CZElauncherMain::OnExit(const TNotifyUI& msg)
{
	::PostQuitMessage(0L);
	__super::Close();
	ExitProcess(0L);
}

void _stdcall MenuProc_Main(HWND hWnd, MenuInfo* MenuInfo)
{
	if (pZElauncher) {
		if (_tcscmp(MenuInfo->MenuText.GetData(), _T("选项设置")) == 0)pZElauncher->OnSetting();
		else if (_tcscmp(MenuInfo->MenuText.GetData(), _T("默认")) == 0)pZElauncher->SetSkinName(_T("默认"));
		else if (_tcscmp(MenuInfo->MenuText.GetData(), _T("简约")) == 0)pZElauncher->SetSkinName(_T("简约"));
		else if (_tcscmp(MenuInfo->MenuText.GetData(), _T("卡通")) == 0)pZElauncher->SetSkinName(_T("卡通"));

	}
}

void __stdcall ThreadTip()
{
	if (MessageBox(NULL, _T("皮肤切换成功,重启后生效\r\n是否现在重启?"), _T("Tip"), MB_YESNO | MB_TOPMOST) == IDYES) {
		CDuiString sBat = _T("@echo off\r\n@ping 127.0.0.1 -n 1 >nul\r\nstart \"\" \"<文件名>\"\r\ndel Restart.bat");
		TCHAR szPath[MAX_PATH] = { 0 };
		TCHAR szShortPath[MAX_PATH] = { 0 };
		GetModuleFileName(GetModuleHandle(NULL), szPath, sizeof(szPath));
		GetShortPathName(szPath, szShortPath, sizeof(szShortPath));
		sBat.Replace(_T("<文件名>"), szPath);
		GetRunPath(szPath, sizeof(szPath));
		_bstr_t sRunBatFile = szPath;
		sRunBatFile += _T("\\restart.bat");
		FILE* pFile = fopen(sRunBatFile, "wb+");
		if (!pFile)return;
		_bstr_t sWriteBat = sBat.GetData();
		char* sTemp = static_cast<char*>(sWriteBat);
		fwrite(sTemp, 1, strlen(sTemp), pFile);
		fclose(pFile);
		ShellExecute(NULL, _T("open"), sRunBatFile, NULL, szPath, SW_HIDE);
		ExitProcess(NULL);
	}
}

void CZElauncherMain::SetSkinName(LPCTSTR SkinName)
{
	TCHAR RunPath[MAX_PATH] = { 0 };
	GetRunPath(RunPath, sizeof(RunPath));
	_bstr_t CfgPath = RunPath;
	CfgPath += _T("\\bin\\Config.cfg");
	_bstr_t sName = _T("");
	if (_tcscmp(SkinName, _T("默认")) == 0)sName = _T("default");
	else {
		sName = SkinName;
		sName += _T(".zip");
	}
	WritePrivateProfileString(_T("ZElauncher"), _T("Skin"), sName, CfgPath);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadTip, NULL, NULL, NULL);
}

void CZElauncherMain::OnSetting(bool IsModel)
{
	CSetting* pSetting = new CSetting();
	if (pSetting == NULL) return;
	pSetting->Create(NULL, _T("选项设置"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 900, 500);
	pSetting->CenterWindow();
	::ShowWindow(*pSetting, SW_SHOW);
	::SetWindowPos(*pSetting, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	if(IsModel)pSetting->ShowModal();
}

void CZElauncherMain::OnMenu(const TNotifyUI& msg)
{
	CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
	CDuiPoint point = msg.ptMouse;
	ClientToScreen(m_hWnd, &point);
	STRINGorID xml(IDR_XML2);
	pMenu->Init(NULL, xml, _T("xml"), point, MenuProc_Main);
}

void CZElauncherMain::OnInitialize()
{
	TCHAR CfgPath[MAX_PATH] = { 0 };
	TCHAR Initialize[20] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("Initialize"), NULL, Initialize, sizeof(Initialize), CfgPath);
	if (_tcscmp(Initialize, _T("true")) != 0) {
		if (!IsCreate) {
			OutTip(_T("请完成配置信息"));
			OnSetting(true);
			WritePrivateProfileString(_T("ZElauncher"), _T("Initialize"), _T("true"), CfgPath);
			IsCreate = true;
		}
	}
	else {
		OutTip(_T("获取配置信息完成"));
	}
	InitLogin();
	
}

void CZElauncherMain::OutTip(LPCTSTR pOutTip)
{
	CTextUI* pText = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("Outtip")));
	if (!pText)return;
	CDuiString Out = _T("{c #386382}");
	Out += pOutTip;
	Out += _T("{/c}");
	pText->SetText(Out);
}

void CZElauncherMain::InitLogin()
{
	CButtonUI* pLogin = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Button_Login")));
	if (!pLogin)return;
	//获取个人信息;
	lib_http::http http;
	CDuiString Cookies = http.GetLocalCookies(_T("https://bbs.93x.net")).c_str();
	std::string Result;
	http.GET(_T("https://bbs.93x.net/plugin.php?id=xnet_steam_openid:SoftLogin"), Result, _T(""), Cookies.GetData());
	//JSON处理
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(Result, root)) {
		if (!root.isNull()) {
			Json::Value userdata = root["userdata"];
			if (userdata.isNull())return;
			if (userdata["username"].isNull())return;
			TCHAR pUserName[4096] = { 0 };
			std::string tmpstr = userdata["username"].asCString();
			MultiByteToWideChar(CP_UTF8, NULL, tmpstr.c_str(), tmpstr.size(), pUserName, sizeof(pUserName));
			CDuiString NickName = _T("{u}{a}");
			NickName += pUserName;
			NickName += _T("{/a}{/u}");
			pLogin->SetText(NickName);
			OutTip(_T("帐号登录成功!"));
		}
	}
	else OutTip(_T("登录失败,Cookies过期或从未登陆过!"));
}

void CZElauncherMain::LoginUser()
{
	CButtonUI* pLogin = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Button_Login")));
	if (!pLogin)return;
	if (_tcscmp(pLogin->GetText(), _T("{u}{a}未登录{/a}{/u}")) != 0) {
		MessageBox(NULL, _T("您已经登录,不用重复登录.\r\n如果想要注销,打开IE登录论坛注销即可!"), NULL, MB_OK);
	}
	else {
		//if (_tcscmp(pLogin->GetText(), _T("{u}{a}未登录{/a}{/u}")) == 0)ShellExecute(0, _T("open"), _T("iexplore.exe"), _T("https://bbs.93x.net"), NULL, NULL);
		MessageBox(NULL, _T("登录账户后,请关闭IE.(记得勾选自动登录!)"), NULL, MB_OK);
		STARTUPINFO si;
		GetStartupInfo(&si);
		PROCESS_INFORMATION pi;
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		if (CreateProcess(_T("C:\\Program Files\\Internet Explorer\\iexplore.exe"), (LPWSTR)_T("open https://bbs.93x.net"), NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi) == 0) {
			MessageBox(NULL, _T("IE进程创建失败,请确认是否安装IE.\r\n或手动打开IE进入 https://bbs.93x.net 登录账户."), NULL, MB_OK);
			return;
		}
		WaitForSingleObject(pi.hProcess, INFINITE);
		InitLogin();
	}
}

void CZElauncherMain::OnClick(const TNotifyUI& msg)
{
	if (_tcscmp(msg.pSender->GetName(), m_pCloseBtn) == 0)OnExit(msg);
	else if (_tcscmp(msg.pSender->GetName(), m_pMinBtn) == 0)SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	else if (_tcscmp(msg.pSender->GetName(), m_pMenuBtn) == 0)OnMenu(msg);
	else if (_tcscmp(msg.pSender->GetName(), m_pBBSBtn) == 0)ShellExecute(NULL, _T("open"), _T("https://bbs.93x.net/"), NULL, NULL, NULL);
	else if (_tcscmp(msg.pSender->GetName(), m_pBugBtn) == 0)ShellExecute(NULL, _T("open"), _T("https://bbs.93x.net/forum.php?mod=viewthread&tid=216568"), NULL, NULL, NULL);
	else if (_tcscmp(msg.pSender->GetName(), _T("Button_Login")) == 0)LoginUser();
	
}

LRESULT CZElauncherMain::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*TCHAR buf[256] = { 0 };
	_stprintf(buf, _T("%d---%X\n"), uMsg, uMsg);
	OutputDebugString(buf);
	switch (uMsg)
	{
	case WM_CREATE:
		OutputDebugStringA("asdasdas\n");
		break;
	default:
		break;
	}*/
	return __super::HandleMessage(uMsg, wParam, lParam);
}