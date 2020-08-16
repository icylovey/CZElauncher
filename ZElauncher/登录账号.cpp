#include "登录账号.h"

bool IsLoginCreate = false;
bool is_btn_IElogin = false, is_btn_Login = false, is_btn_getsecurecode = false, is_btn_Logout = false, Is_IELogin = false;

LPCTSTR C登录账户UI::GetWindowClassName()const
{
	return _T("LoginingWnd");
}

CDuiString C登录账户UI::GetSkinFile()
{
	return _T("Login.xml");
}

CDuiString C登录账户UI::GetSkinFolder()
{
	return _T("Skin\\ListRes\\");
}

UILIB_RESTYPE C登录账户UI::GetResourceType()const
{
	return UILIB_FILE;
}

LPCTSTR C登录账户UI::GetResourceID()const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void C登录账户UI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* C登录账户UI::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void C登录账户UI::设置登录UI状态()
{
	if (m_IsLogin) {
		CContainerUI* pContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("panel_show")));
		if (!pContainer)MessageBox(NULL, _T("数据出错!"), NULL, NULL);
		pContainer->SetVisible(false);
		COptionUI* pOption = static_cast<COptionUI*>(m_pm.FindControl(_T("btn_Logout")));
		if (!pOption)MessageBox(NULL, _T("数据出错!"), NULL, NULL);
		pOption->SetVisible(true);
	}
	else {
		CContainerUI* pContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("panel_show")));
		if (!pContainer)MessageBox(NULL, _T("数据出错!"), NULL, NULL);
		pContainer->SetVisible(true);
		COptionUI* pOption = static_cast<COptionUI*>(m_pm.FindControl(_T("btn_Logout")));
		if (!pOption)MessageBox(NULL, _T("数据出错!"), NULL, NULL);
		pOption->SetVisible(false);
	}
}

void C登录账户UI::InitWindow()
{
	if (IsLoginCreate)__super::Close();
	IsLoginCreate = true;
	设置登录UI状态();
	初始化用户名编辑框();
}

void C登录账户UI::SaveEditInfo()
{
	_bstr_t CfgPath = GetCFGPath();
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("Edit_UserName")));
	if (!pEdit)return;
	CDuiString tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("Login_UserName"), tmpstr.GetData(), CfgPath);
	if (Is_IELogin)WritePrivateProfileString(_T("ZElauncher"), _T("Login_IELogin"), _T("true"), CfgPath);
	else WritePrivateProfileString(_T("ZElauncher"), _T("Login_IELogin"), _T("false"), CfgPath);
}

void C登录账户UI::OnExit(const TNotifyUI& msg)
{
	SaveEditInfo();
	IsLoginCreate = false;
	__super::Close();
}

C登录账户UI::C登录账户UI(bool Islogin)
{ 
	m_IsLogin = Islogin;
}

bool C登录账户UI::IELoginVerify()
{
	//获取个人信息;
	lib_http::CLibhttp http;
	std::string Cookies;
	std::string Result;
	http.GET("https://bbs.93x.net/plugin.php?id=xnet_steam_openid:SoftLogin", Result, "", Cookies);
	//JSON处理
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(Result, root)) {
		if (!root.isNull()) {
			Json::Value userdata = root["userdata"];
			if (userdata.isNull())return false;
			if (userdata["username"].isNull())return false;
			return true;
		}
	}
	return false;
}

void C登录账户UI::IELoginUser()
{
	Is_IELogin = true;
	//lib_http::CLibhttp http;
	//gCookies = http.GetLocalCookies("https://bbs.93x.net").c_str();
	gCookies = "";
	MessageBox(NULL, _T("登录账户后,请关闭IE.(记得勾选自动登录!)"), NULL, MB_OK | MB_TOPMOST);
	STARTUPINFO si;
	GetStartupInfo(&si);
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	if (CreateProcess(_T("C:\\Program Files\\Internet Explorer\\iexplore.exe"), (LPWSTR)_T("open https://bbs.93x.net"), NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi) == 0) {
		MessageBox(NULL, _T("IE进程创建失败,请确认是否安装IE.\r\n或手动打开IE进入 https://bbs.93x.net 登录账户."), NULL, MB_OK | MB_TOPMOST);
		return;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	if (IELoginVerify())
	{
		MessageBox(NULL, _T("IE成功登录账户!"), _T("Tip:"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
		OnExit(TNotifyUI());
	}
	else MessageBox(NULL, _T("IE登录失败!"), NULL, MB_OK | MB_TOPMOST);
	g_pZElauncher->InitLogin();
}

void C登录账户UI::创建POST请求数据(std::string& POST请求数据)
{
	POST请求数据 = "username=";
	strCoding 字符串编码;
	_bstr_t 用户名 = "";
	CEditUI* pEditUser = static_cast<CEditUI*>(m_pm.FindControl(_T("Edit_UserName")));
	CEditUI* pEditPWD = static_cast<CEditUI*>(m_pm.FindControl(_T("Edit_PassWord")));
	if (!pEditUser || !pEditPWD) {
		MessageBox(NULL, _T("登录失败,UI信息获取失败!"), NULL, NULL);
		return;
	}
	用户名 = pEditUser->GetText();
	std::string Tmpstr = 字符串编码.UrlUTF8(用户名);
	POST请求数据 += Tmpstr;
	POST请求数据 += "&timestamp=";
	char tmpbuff[1024] = { 0 };
	sprintf(tmpbuff, "%lld", time(NULL));
	POST请求数据 += tmpbuff;
	POST请求数据 += "&securecode=";
	_bstr_t 用户密码 = "";
	用户密码 = pEditPWD->GetText();
	std::string securecode;
	securecode.append(用户密码);
	POST请求数据 += Encode_sha512(securecode).c_str();
}

void C登录账户UI::初始化用户名编辑框()
{
	TCHAR tmpbuff[1024] = { 0 };
	_bstr_t CfgPath = GetCFGPath();
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("Edit_UserName")));
	if (!pEdit)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("Login_UserName"), NULL, tmpbuff, sizeof(tmpbuff), CfgPath);
	if (_tcslen(tmpbuff) > 3) {
		pEdit->SetText(tmpbuff);
	}
	GetPrivateProfileString(_T("ZElauncher"), _T("Login_IELogin"), NULL, tmpbuff, sizeof(tmpbuff), CfgPath);
	if (_tcsstr(tmpbuff, _T("true")))Is_IELogin = true;
}

void C登录账户UI::错误代码处理(std::string 返回文本)
{
	if (返回文本.find("username not found!") != std::string::npos) {
		int npos = 返回文本.find("username not found!") + strlen("username not found!");
		int npos2 = 返回文本.find("\"}");
		std::string errinfo = 返回文本.substr(npos, npos2 - npos);
		strCoding encode;
		errinfo = encode.usc2ToAnsi(errinfo);
		std::string errtip = "登录失败!\r\n未找到用户名";
		errtip += errinfo;
		MessageBoxA(NULL, errtip.c_str(), "错误:", MB_OK | MB_TOPMOST);
	}
	else if (返回文本.find("securecode not match") != std::string::npos) {
		MessageBoxA(NULL, "登录失败!\r\n登录器安全码错误!", "错误:", MB_OK | MB_TOPMOST);
	}
	else {
		int npos = 返回文本.find("errmsg\":\"") + strlen("errmsg\":\"");
		int npos2 = 返回文本.find("\"}");
		std::string errinfo = 返回文本.substr(npos, npos2 - npos);
		strCoding encode;
		errinfo = encode.usc2ToAnsi(errinfo);
		std::string errtip = "登录失败!\r\n";
		errtip += errinfo;
		MessageBoxA(NULL, errtip.c_str(), "错误:", MB_OK | MB_TOPMOST);
	}
}

bool C登录账户UI::UserEvent(std::string ResultData, std::string &ret_token)
{
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(ResultData, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		if (root.isNull()) {
			MessageBox(NULL, _T("Json数据获取失败."), NULL, NULL);
			return false;
		}
		if (!root["userdata"].isNull()) {
			Json::Value Json_用户数据 = root["userdata"];
			if (!Json_用户数据["user_token"].isNull()) {
				ret_token = Json_用户数据["user_token"].asCString();
			}
			else {
				MessageBox(NULL, _T("用户数据获取失败."), NULL, NULL);
				return false;
			}
		}
		else if (!root["errcode"].isNull()) {
			std::string errcode = root["errcode"].asString();
			if (strcmp(errcode.c_str(), "500") == 0) {
				错误代码处理(ResultData);
				return false;
			}
		}
	}
	return true;
}

bool C登录账户UI::LoginVerify(std::string token)
{
	std::string RequestUrl = "https://bbs.93x.net/plugin.php?id=xnet_core_api:ForumLogin&token=", ReturnCookes = "";
	std::string 返回文本 = "";
	RequestUrl += token;
	lib_http::CLibhttp http;
	http.GET(true, RequestUrl.c_str(), 返回文本, "", "NotCookie", ReturnCookes);
	http.GET("https://bbs.93x.net/", 返回文本, "", ReturnCookes);
	UINT nLen = 返回文本.size() * sizeof(TCHAR);
	wchar_t* pStrHtml = new wchar_t[nLen];
	MultiByteToWideChar(CP_UTF8, NULL, 返回文本.c_str(), 返回文本.length(), pStrHtml, nLen);
	if (wcsstr(pStrHtml, L"找回密码") && wcsstr(pStrHtml, L"立即注册"))return false;
	_bstr_t cfgbuff = GetRunPath();
	cfgbuff += _T("\\bin\\Cookies.data");
	_bstr_t FileName = cfgbuff;
	FILE* pFile = fopen(FileName, "wb+");
	if (!pFile) {
		delete[]pStrHtml;
		MessageBox(NULL, _T("读取Cookies失败,请确认是否拥有文件操作权限!"), NULL, MB_OK);
		return false;
	}
	fseek(pFile, SEEK_SET, 0);
	fwrite((void*)ReturnCookes.c_str(), ReturnCookes.size(), 1, pFile);
	fclose(pFile);
	gCookies = ReturnCookes.c_str();
	g_pZElauncher->InitLogin();
	delete[]pStrHtml;
	return true;
}

void C登录账户UI::LoginUser()
{
	Is_IELogin = false;
	std::string 返回文本, POST请求数据, Tmpstr, token;
	创建POST请求数据(POST请求数据);
	lib_http::CLibhttp http;
	http.POST("https://bbs.93x.net/plugin.php?id=xnet_core_api:GetForumUserDetail", 返回文本, POST请求数据);
	if (!UserEvent(返回文本, token))return;
	if (LoginVerify(token))
	{
		MessageBox(NULL, _T("成功登录账户!"), _T("Tip:"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
		OnExit(TNotifyUI());
	}
	else MessageBox(NULL, _T("登录失败!"), NULL, MB_OK | MB_ICONERROR | MB_TOPMOST);

}

void C登录账户UI::Logout()
{
	_bstr_t cfgbuff = GetRunPath();
	cfgbuff += _T("\\bin\\Cookies.data");
	_bstr_t FileName = cfgbuff;
	FILE* pFile = fopen(FileName, "wb+");
	if (!pFile)return;
	fclose(pFile);
	g_pZElauncher->LogoutUser("未登录");
	m_IsLogin = false;
	设置登录UI状态();
	g_pZElauncher->IsCreateShop = false;
	gCookies = "";
	g_SteamID64 = "";
}

void C登录账户UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)::SetForegroundWindow(m_hWnd);//窗口创建完毕后,获取焦点;
	else if (_tcscmp(msg.pSender->GetName(), _T("closebtn")) == 0)OnExit(msg);
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_IElogin")) == 0) {
		SaveEditInfo();
		if (!is_btn_IElogin) {
			is_btn_IElogin = true;
			IELoginUser();
		}
		else {
			is_btn_IElogin = false;
		}
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_Login")) == 0) {
		SaveEditInfo();
		if (!is_btn_Login) {
			is_btn_Login = true;
			LoginUser();
		}
		else {
			is_btn_Login = false;
		}
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_getsecurecode")) == 0) {
		if (!is_btn_getsecurecode) {
			is_btn_getsecurecode = true;
			ShellExecute(NULL, _T("open"), _T("https://bbs.93x.net/plugin.php?id=xnet_steam_openid:SoftLogin_getsecurecode"), NULL, NULL, NULL);
		}
		else {
			is_btn_getsecurecode = false;
		}
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_Logout")) == 0) {
		if (!is_btn_Logout) {
			is_btn_Logout = true;
			Logout();
		}
		else {
			is_btn_Logout = false;
		}
	}
}