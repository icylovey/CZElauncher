#include "ZElauncher.h"

bool IsCreate = false;

LPCTSTR CZElauncherMain::GetWindowClassName() const
{
	return _T("ZElauncherForm");
};

UILIB_RESTYPE CZElauncherMain::GetResourceType() const
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

void CZElauncherMain::LogoutUser(_bstr_t Tiptext)
{
	CButtonUI* pLogin = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_Login")));
	if (!pLogin)return;
	CDuiString Tip = _T("{u}{a}");
	Tip += Tiptext.GetBSTR();
	Tip += _T("{/a}{/u}");;
	pLogin->SetText(Tip);
}

void InitializeCookies()
{
	TCHAR CfgPath[MAX_PATH] = { 0 };
	TCHAR Is_IELogin[20] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("Login_IELogin"), NULL, Is_IELogin, sizeof(Is_IELogin), CfgPath);
	if (_tcsstr(Is_IELogin, _T("true")) || _tcsstr(Is_IELogin, _T("TRUE"))) {
		lib_http::CLibhttp http;
		//暂时不使用cookie因为或登录失败;
		//gCookies = http.GetLocalCookies("https://bbs.93x.net").c_str();
		gCookies = "";
	}
	else {
		TCHAR cfgbuff[1024] = { 0 };
		GetRunPath(cfgbuff, sizeof(cfgbuff));
		_tcscat(cfgbuff, _T("\\bin\\Cookies.data"));
		_bstr_t FileName = cfgbuff;
		FILE* pFile = fopen(FileName, "rb+");
		if (!pFile) {
			FILE* pFile = fopen(FileName, "wb+");
			if(pFile)fclose(pFile);
			return;
		}
		fseek(pFile, SEEK_SET, 0);
		char* Cookiesbuff = new char[4096]();
		fread(Cookiesbuff, 4095, 1, pFile);
		fclose(pFile);
		gCookies = Cookiesbuff;
	}
}

void ServerHotDownloadJsonEvent(std::string &Resulthtml)
{
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(Resulthtml, root)) {
		if (root[(UINT)0].isNull()) {
			g_pZElauncher->OutTip2(_T("预热下载->未找到服务器游玩记录"));
			return;
		}
		Json::Value Json_userinfo = root[(UINT)0];
		_bstr_t nextmap = "", NominateLists = "", downloadurl = "", ServerIpinfo = "";
		if (!Json_userinfo["NextMap"].isNull())
			nextmap = Json_userinfo["NextMap"].asCString();
		if (!Json_userinfo["NominateLists"].isNull())
			NominateLists = Json_userinfo["NominateLists"].asCString();
		if (!Json_userinfo["downloadurlPreFix"].isNull())
			downloadurl = Json_userinfo["downloadurlPreFix"].asCString();
		if (!Json_userinfo["ListenServer"].isNull())
			ServerIpinfo = Json_userinfo["ListenServer"].asCString();
		_bstr_t str_outtip = "预热下载->下一张地图< ";
		//nextmap = L"ze_S_A_M_v1_7";
		_bstr_t map_name = "";
		if (nextmap.length() > 3)map_name = nextmap;
		else map_name = NominateLists;

		CDuiString map_tmp = map_name.GetBSTR();
		int mPos = map_tmp.Find(_T(","));
		if (mPos != -1) {
			//map_name = map_name.Mid(mPos + 1, map_name.Find(_T(",")) - (mPos + 1));
			map_name = map_tmp.Left(mPos);
		}
		/*
		_bstr_t sSearchText = pSearch->GetText().GetData();
			TCHAR* pPos = sSearchText;
			_tcstok(sSearchText, _T(","));
			if (!pPos)VectorType.emplace_back(sSearchText);
			else {
				while (pPos) {
					VectorType.emplace_back(pPos);
					pPos = _tcstok(NULL, _T(","));
				}
			}
		*/
		if (IsChineseMap) {
			str_outtip += g_pZElauncher->GetChineseMapName(map_name);
		}
		else str_outtip += map_name;
		str_outtip += " >";
		str_outtip += "  正在游玩[ ";
		for (UINT i = 0; i < g_ServerListInfo.size(); i++) {
			if (_tcsstr(g_ServerListInfo[i].ServerIp.GetBSTR(), ServerIpinfo.GetBSTR())) {
				ServerIpinfo = g_ServerListInfo[i].ServerName;
				break;
			}
		}
		str_outtip += ServerIpinfo;
		str_outtip += " ]";
		g_pZElauncher->OutTip2(str_outtip);
		if (map_name.length() > 3 && !pServer->IsFileExist(map_name))pServer->DownloadMap93x(map_name, false);
	}
}

void __stdcall ThreadhotProc(HWND hWnd,UINT uMsg,UINT_PTR arg3,DWORD arg4)
{
	for (;;) {
		if (g_pZElauncher->UserIslogin()) {
			if (g_IsDownloadHotmap) {
				lib_http::CLibhttp http;
				std::string Resulthtml;
				_bstr_t map_HotUrl = "http://c.93x.net/yunget/userinfo.php?data=";
				map_HotUrl += g_SteamID64;
				http.GET(map_HotUrl.GetBSTR(), Resulthtml, _T(""), gCookies.GetBSTR());
				if (Resulthtml.length() < 10) {
					g_pZElauncher->OutTip2(_T("预热下载->未找到服务器游玩记录"));
					continue;
				}
				ServerHotDownloadJsonEvent(Resulthtml);
			}
			else g_pZElauncher->OutTip2(_T("预热下载->已关闭预热下载功能!"));
		}
		else g_pZElauncher->OutTip2(_T("预热下载->您未登录,请先登录!"));
		
		Sleep(2000);
	}
}

void CZElauncherMain::InitWindow()
{
	g_pZElauncher = this;
	InitializeCookies();
	TCHAR CfgPath[MAX_PATH] = { 0 };
	TCHAR Server_Switch[20] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("Server_Switch"), NULL, Server_Switch, sizeof(Server_Switch), CfgPath);
	if (_tcscmp(Server_Switch, _T("true")) == 0) {
		COptionUI* pButton = static_cast<COptionUI*>(m_pm.FindControl(_T("Server_Switch")));
		if (pButton)pButton->Selected(true);
	}
	_bstr_t sVersion = _T("{c #386382}当前版本：");
	sVersion += Version;
	sVersion += _T("{/c}");
	CTextUI* pText = static_cast<CTextUI*>(m_pm.FindControl(_T("VersionTip")));
	if (pText)pText->SetText(sVersion);
	CControlUI* pbkimg = static_cast<CControlUI*>(m_pm.FindControl(_T("img_bkimage")));
	if (pbkimg) {
		TCHAR* pszbuff = new TCHAR[MAX_PATH]();
		GetPrivateProfileString(_T("ZElauncher"), _T("edit_bkimge"), NULL, pszbuff, (MAX_PATH * sizeof(TCHAR)), CfgPath);
		if (_tcslen(pszbuff) > 5)pbkimg->SetBkImage(pszbuff);
	}
	std::thread t1 = std::thread(&CZElauncherMain::启动获取ZE地图中文名表, this);
	t1.detach();
	//SetTimer(m_hWnd, 12313, 2000, (TIMERPROC)Timeproc);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadhotProc, NULL, NULL, NULL);
}

void CZElauncherMain::启动获取ZE地图中文名表()
{
	std::thread t2 = std::thread(&CZElauncherMain::Get93xServerListInfo, this);
	t2.detach();
	TCHAR Maptype[9][10] = { L"ze",L"ttt",L"jb",L"mgdk" ,L"newmaps" ,L"bhop" ,L"zm" ,L"awp",L"35hp" };
	for (int i = 0; i < 5; i++) {
		std::thread t1 = std::thread(&CZElauncherMain::获取ZE地图中文名表, this, Maptype[i]);
		t1.join();
	}
	pServer->RefreshServer();
}

void CZElauncherMain::MenuClick(CControlUI* Click)
{
	if (_tcscmp(Click->GetText().GetData(), _T("选项设置")) == 0)OnSetting();
	else if (_tcscmp(Click->GetText().GetData(), _T("默认")) == 0)SetSkinName(_T("默认"));
	else if (_tcscmp(Click->GetText().GetData(), _T("简约")) == 0)SetSkinName(_T("简约"));
	else if (_tcscmp(Click->GetText().GetData(), _T("卡通")) == 0)SetSkinName(_T("卡通"));
}

void CZElauncherMain::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnInitialize();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg);
	else if (_tcscmp(msg.sType, _T("itemclick")) == 0)MenuClick(msg.pSender);
	else if (_tcscmp(msg.sType, _T("selectchanged")) == 0)
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("Tabchild")));
		if (!pControl)return;
		COptionUI* pSwitch = static_cast<COptionUI*>(m_pm.FindControl(_T("Server_Switch")));
		if (!pSwitch)return;
		if (name == _T("Title_Server"))pControl->SelectItem(0);
		else if (name == _T("Title_award"))pControl->SelectItem(1);
		else if (name == _T("Title_Tools"))pControl->SelectItem(2);
		else if (name == _T("Title_shop")) {
			pControl->SelectItem(3);
			if (!IsCreateShop) {
				IsCreateShop = true;
				pShop->启动获取商城数据函数();
			}
		}
		else if (name == _T("Title_Skin")) {
			pControl->SelectItem(4);
			if (!IsCreateSkin) {
				IsCreateSkin = true;
				pSkin->获取新手皮肤数据();
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
	//__super::Notify(msg);
}

void CZElauncherMain::Get93xServerListInfo()
{
	//获取网页Json内容
	std::string StrResult;
	{
		lib_http::CLibhttp phttp;
		string url = "https://serverssoftware.93x.net/api/servers?game=csgo";
		if (!phttp.GET(url, StrResult))MessageBox(NULL, _T("获取服务器信息失败!"), NULL, NULL);
	}
	//编码转换
	UINT nLen = StrResult.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, StrResult.c_str(), StrResult.length(), pStrHtml, nLen);
	UINT nLen2 = StrResult.size() * sizeof(TCHAR);
	char* pStrMulti = new char[nLen2];
	ZeroMemory(pStrMulti, nLen2);
	WideCharToMultiByte(CP_ACP, NULL, pStrHtml, nLen, pStrMulti, nLen2, NULL, NULL);
#pragma region 服务器列表Json获取
	Json::Reader reader;
	Json::Value root;
	int listcount = 0;
	if (reader.parse(pStrMulti, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		//未排序
		for (UINT i = 0; i < root.size(); i++) {
			std::string tmpStr;
			if (root[i]["name"].isNull())break;
			tmpStr = root[i]["name"].asCString();
			if (tmpStr.empty())break;
			_bstr_t sText = "";
			sText = root[i]["name"].asCString();
			ServerListInfo tmpServerListInfo;
			tmpServerListInfo.ServerName = sText;

			TCHAR tmpbuf[250] = { 0 };
			if (root[i]["port"].isNull())continue;
			sText = root[i]["ip"].asCString();
			_bstr_t port = root[i]["port"].asCString();
			_stprintf(tmpbuf, _T("%s:%s"), static_cast<TCHAR*>(sText), static_cast<TCHAR*>(port));
			tmpServerListInfo.ServerIp = tmpbuf;
			g_ServerListInfo.push_back(tmpServerListInfo);
		}

	}
#pragma endregion
	delete[] pStrHtml;
	delete[] pStrMulti;
}

_bstr_t CZElauncherMain::GetChineseMapName(_bstr_t Map_englishName)
{
	_bstr_t Result_map = "";
	for (UINT j = 0; j < g_MapChinese.size(); j++) {
		if (strcmp(Map_englishName, g_MapChinese[j].Map_en) == 0 && g_MapChinese[j].Map_cn.length() > 1) {
			Result_map = g_MapChinese[j].Map_cn;
			Result_map += "<";
			Result_map += g_MapChinese[j].Map_en;
			Result_map += ">";
			break;
		}
	}
	return Result_map;
}

CControlUI* CZElauncherMain::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("Server")) == 0)return new C服务器UI(m_pm, this->m_hWnd);
	else if (_tcscmp(pstrClass, _T("Award")) == 0)return new C礼包UI(m_pm);
	else if (_tcscmp(pstrClass, _T("Shop")) == 0) {
		pShop = new C商城UI(m_pm);
		return pShop;
	}
	else if (_tcscmp(pstrClass, _T("Tools")) == 0)return new C工具箱UI(m_pm);
	else if (_tcscmp(pstrClass, _T("Skin")) == 0) {
		pSkin = new C新手皮肤UI(m_pm);
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
	C设置UI* pSetting = new C设置UI();
	if (pSetting == NULL) return;
	pSetting->Create(NULL, _T("选项设置"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 900, 500);
	pSetting->CenterWindow();
	::ShowWindow(*pSetting, SW_SHOW);
	::SetWindowPos(*pSetting, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	if (IsModel)pSetting->ShowModal();
}

void CZElauncherMain::获取ZE地图中文名表(LPCTSTR maptype)
{
	lib_http::CLibhttp http;
	std::string htmlResult;
	_bstr_t url = "https://xnetsoft2.93x.net/mapdata/";
	url += maptype;
	url += ".html";
	http.GET(url.GetBSTR(), htmlResult);
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(htmlResult, root)) {
		if (root[(UINT)0].isNull())return;
		for (UINT i = 0; i < root.size(); i++) {
			if(root[i].isNull())break;
			Json::Value json_Array = root[i];
			_bstr_t map_cn = "", map_en = "", map_downloadurl = "";
			if (!json_Array["cn_name"].isNull())
				map_cn = json_Array["cn_name"].asCString();
			if (!json_Array["mapname"].isNull())
				map_en = json_Array["mapname"].asCString();
			if (!json_Array["download"].isNull())
				map_downloadurl = json_Array["download"].asCString();
			MapChineseName tmp;
			tmp.Map_cn = map_cn;
			tmp.Map_en = map_en;
			tmp.MapDownloadurl = map_downloadurl;
			g_MapChinese.push_back(tmp);
		}
	}
}

void CZElauncherMain::OnLogin(bool IsLogin, bool IsModel)
{
	C登录账户UI* pLogin = new C登录账户UI(IsLogin);
	if (pLogin == NULL) return;
	pLogin->Create(NULL, _T("账户登录"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 900, 500);
	pLogin->CenterWindow();
	::ShowWindow(*pLogin, SW_SHOW);
	::SetWindowPos(*pLogin, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	if (IsModel)pLogin->ShowModal();
}

void CZElauncherMain::OnMenu(const TNotifyUI& msg)
{
	CDuiPoint point(0, 0);
	GetCursorPos(&point);
	STRINGorID xml(_T("Menu_Main.xml"));
	CMenuWnd* pMenu = CMenuWnd::CreateMenu(nullptr, xml, point, &m_pm);
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
	GetPrivateProfileString(_T("ZElauncher"), _T("DownloadHotmap"), NULL, Initialize, sizeof(Initialize), CfgPath);
	if (_tcscmp(Initialize, _T("true")) == 0) {
		g_IsDownloadHotmap = true;
	}
	std::thread t1 = std::thread(&CZElauncherMain::InitLogin, this);
	t1.detach();

}

void CZElauncherMain::OutTip(_bstr_t pOutTip)
{
	CTextUI* pText = static_cast<CTextUI*>(m_pm.FindControl(_T("Outtip")));
	if (!pText)return;
	CDuiString Out = _T("{c #FF0000}");//#386382
	Out += pOutTip.GetBSTR();
	Out += _T("{/c}");
	pText->SetText(Out);
}

void CZElauncherMain::OutTip2(_bstr_t pOutTip2)
{
	CTextUI* pText = static_cast<CTextUI*>(m_pm.FindControl(_T("Outtip2")));
	if (!pText)return;
	CDuiString Out = _T("{c #FF0000}");
	Out += pOutTip2.GetBSTR();
	Out += _T("{/c}");
	pText->SetText(Out);
}

void CZElauncherMain::InitLogin()
{
	CButtonUI* pLogin = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_Login")));
	if (!pLogin)return;
	//获取个人信息;
	lib_http::CLibhttp http;
	std::string Cookies;
	Cookies.append(gCookies);
	std::string Result;
	http.GET("https://bbs.93x.net/plugin.php?id=xnet_steam_openid:SoftLogin", Result, "", Cookies.c_str());
	/*FILE* pfile = fopen("C:\\Users\\Administrator\\Desktop\\新建文件夹\\僵尸逃跑启动器\\html.html", "wb+");
	fwrite(Result.c_str(), Result.size(), 1, pfile);
	fclose(pfile);*/
	//JSON处理
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(Result, root)) {
		if (!root.isNull()) {
			Json::Value userdata = root["userdata"];
			if (userdata.isNull())return;
			if (userdata["username"].isNull())return;
			//TCHAR pUserName[4096] = { 0 };
			//std::string tmpstr = userdata["username"].asCString();
			//MultiByteToWideChar(CP_UTF8, NULL, tmpstr.c_str(), tmpstr.size(), pUserName, sizeof(pUserName));
			_bstr_t NickName = _T("{u}{a}");
			NickName += userdata["username"].asCString();
			NickName += _T("{/a}{/u}");
			pLogin->SetText(NickName);
			if (!userdata["steamid64"].isNull())g_SteamID64 = userdata["steamid64"].asCString();
			OutTip(_T("帐号登录成功!"));
		}
	}
	else {
		g_SteamID64 = "";
		OutTip(_T("登录失败,Cookies过期或从未登陆过!"));
		pLogin->SetText(_T("{u}{a}登录失败{/a}{/u}"));
	}
}

bool CZElauncherMain::UserIslogin()
{
	CButtonUI* pLogin = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_Login")));
	if (!pLogin)return false;
	if (_tcscmp(pLogin->GetText(), _T("{u}{a}未登录{/a}{/u}")) == 0 || _tcscmp(pLogin->GetText(), _T("{u}{a}登录失败{/a}{/u}")) == 0 || _tcscmp(pLogin->GetText(), _T("{u}{a}Cookies获取为空{/a}{/u}")) == 0)
		return false;
	else return true;
}

void CZElauncherMain::OnClick(const TNotifyUI& msg)
{
	if (_tcscmp(msg.pSender->GetName(), m_pCloseBtn) == 0)OnExit(msg);
	else if (_tcscmp(msg.pSender->GetName(), m_pMinBtn) == 0)SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	else if (_tcscmp(msg.pSender->GetName(), m_pMenuBtn) == 0)OnMenu(msg);
	else if (_tcscmp(msg.pSender->GetName(), m_pBBSBtn) == 0)ShellExecute(NULL, _T("open"), _T("https://bbs.93x.net/"), NULL, NULL, NULL);
	else if (_tcscmp(msg.pSender->GetName(), m_pBugBtn) == 0)ShellExecute(NULL, _T("open"), _T("https://bbs.93x.net/forum.php?mod=viewthread&tid=216568"), NULL, NULL, NULL);
	else if (_tcscmp(msg.pSender->GetName(), _T("Button_Login")) == 0) {
		if (!UserIslogin())
			g_pZElauncher->OnLogin(false);
		else {
			g_pZElauncher->OnLogin(true);
		}
	}
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