#include "SteamUser.h"

LPCTSTR CSteamUserMain::GetWindowClassName() const
{
	return _T("SteamUserForm");
};

UILIB_RESOURCETYPE CSteamUserMain::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_ZIPRESOURCE;
#endif // _DEBUG
}

LPCTSTR CSteamUserMain::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

CDuiString CSteamUserMain::GetSkinFile()
{
	return _T("SteamUser.xml");
}

CDuiString CSteamUserMain::GetSkinFolder()
{
	return _T("Skin\\ListRes");
}

void CSteamUserMain::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

_bstr_t CSteamUserMain::GetPattern()
{
	TCHAR szBuf[MAX_PATH] = { 0 };
	GetRunPath(szBuf, sizeof(szBuf));
	_tcscat(szBuf, _T("\\Regex.db"));
	TCHAR* pTpattern = new TCHAR[4096];
	ZeroMemory(pTpattern, 4096);
	GetPrivateProfileString(_T("ZElauncher"), _T("SteamUser"), NULL, pTpattern, 4096, szBuf);
	_bstr_t Reuslt = pTpattern;
	return pTpattern;
}

_bstr_t CSteamUserMain::GetUserData()
{
	TCHAR szBuf[MAX_PATH] = { 0 };
	GetRunPath(szBuf, sizeof(szBuf));
	_tcscat(szBuf, _T("\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("SteamPath"), NULL, szBuf, sizeof(szBuf), szBuf);
	_bstr_t FileName = szBuf;
	FileName += _T("\\config\\loginusers.vdf");
	FILE* pFile = fopen(FileName, "rb");
	if (!pFile)return _T("");
	fseek(pFile, 0, SEEK_END);
	UINT nSize = ftell(pFile);
	char* pStrText = new char[nSize];
	TCHAR* pWStrText = new TCHAR[nSize * sizeof(TCHAR) + sizeof(TCHAR)];
	fseek(pFile, 0, SEEK_SET);
	ZeroMemory(pStrText, nSize);
	fread(pStrText, 1, nSize, pFile);
	fclose(pFile);
	MultiByteToWideChar(CP_UTF8, NULL, pStrText, nSize, pWStrText, nSize * sizeof(TCHAR) + sizeof(TCHAR));
	_bstr_t Result = pWStrText;
	delete[]pStrText;
	delete[]pWStrText;
	return Result;
}

void CSteamUserMain::GetUserInfo()
{
	/*for (int i = 0; i < 20; i++) {
		CPaintManagerUI m_Paint;
		CDialogBuilder builder;
		CControlUI* pControl = builder.Create(_T("SkinConvert_Button.xml"), NULL, NULL, &m_Paint);
		CTileLayoutUI* plist = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("TitlelayoutUI1")));
		if (plist) plist->Add(pControl);
	}
	return;*/
	_bstr_t UserData = GetUserData();
	if (UserData.length() < 10) {
		MessageBox(NULL, _T("获取用户数据失败,请确认Steam目录是否配置正确!"), NULL, MB_OK);
		ExitProcess(NULL);
	}
	VBScript_RegExp_55::IRegExp2Ptr pRegexp(__uuidof(VBScript_RegExp_55::RegExp));
	pRegexp->PutGlobal(VARIANT_TRUE);
	pRegexp->put_Multiline(VARIANT_FALSE);
	pRegexp->PutIgnoreCase(VARIANT_FALSE);
	std::string pTpattern = (char*)GetPattern();
	pRegexp->PutPattern(pTpattern.c_str());
	VBScript_RegExp_55::IMatchCollectionPtr Item = pRegexp->Execute(UserData);
	UINT ir = Item->GetCount();
	for (int i = 0; i < Item->GetCount(); i++) {
		VBScript_RegExp_55::IMatch2Ptr Match = Item->GetItem(i);
		if (!Match)continue;
		VBScript_RegExp_55::ISubMatchesPtr Submatch = Match->GetSubMatches();
		if (!Submatch)continue;
		_bstr_t sSteamID = static_cast<_bstr_t>(Submatch->GetItem(0L));
		_bstr_t sUser = static_cast<_bstr_t>(Submatch->GetItem(1L));
		_bstr_t sNickName = static_cast<_bstr_t>(Submatch->GetItem(2L));
		{
			//CPaintManagerUI m_Paint;
			CDialogBuilder builder;
			CControlUI* pControl = builder.Create(_T("xml\\Steam_Button.xml"), NULL, NULL, &m_pListManager);
			CListUI* plist = static_cast<CListUI*>(m_PaintManager.FindControl(_T("List1")));
			srand(GetTickCount64());
			if (plist) {
				plist->Add(pControl);
				CLabelUI* pLabel = static_cast<CLabelUI*>(m_pListManager.FindSubControlByName(pControl, _T("Image")));
				if (!pLabel)continue;
				pLabel->SetBkImage(GetImageStr(sSteamID));
				CTextUI* pText = static_cast<CTextUI*>(m_pListManager.FindSubControlByName(pControl, _T("Text_nickname")));
				if (!pText)continue;
				pText->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
				pText->SetText(sNickName);
				CButtonUI* plogin = static_cast<CButtonUI*>(m_pListManager.FindSubControlByName(pControl, _T("Button_Login")));
				if (!plogin)continue;
				plogin->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
				plogin->SetUserData(sUser);
			}
		}
	}
	pRegexp.Release();

}

CDuiString CSteamUserMain::GetImageStr(LPCTSTR pSteamID)
{
	lib_http::http http;
	std::string HtmlData;
	CDuiString Url = _T("http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=2B4F7DEC13030AC9E59898514A14AFEB&steamids=");
	Url += pSteamID;
	http.GET(Url.GetData(), HtmlData);
	if (HtmlData.size() < 10)return _T("");
#pragma region 获取个人信息数据
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(HtmlData, root)) {
		if (root.isNull())return _T("");
		Json::Value response = root["response"];
		if (response.isNull())return _T("");
		Json::Value players = response["players"];
		if (players.isNull())return _T("");
		_bstr_t imgurl = players[(UINT)0]["avatarfull"].asCString();
		if (imgurl.length() < 3)return _T("");
		HtmlData = "";
		http.GET((std::wstring)imgurl, HtmlData);
	}
#pragma endregion
	if (HtmlData.find("404") != std::string::npos)return _T("");
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, HtmlData.size());
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, HtmlData.c_str(), HtmlData.size());
	GlobalUnlock(hGlobal);
	IStream* pStream = NULL;
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	CImage img;
	img.Load(pStream);
	TCHAR szbuf[MAX_PATH] = { 0 };
	_stprintf(szbuf, _T("%s.png"), pSteamID);
	m_PaintManager.RemoveImage(szbuf);
	m_PaintManager.AddImage(szbuf, (HBITMAP)img, img.GetWidth(), img.GetHeight(), false);
	img.Detach();
	pStream->Release();
	GlobalFree(hGlobal);
	return szbuf;

}

void __stdcall TipThread()
{
	MessageBox(NULL, _T("正在加载用户数据,请耐心等待o(∩_∩)o "), _T("Tip"), MB_OK);
}

void CSteamUserMain::InitWindow()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)TipThread, NULL, NULL, NULL);
	GetUserInfo();

}

void CSteamUserMain::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)return;
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg);
	__super::Notify(msg);
}

CControlUI* CSteamUserMain::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CSteamUserMain::OnExit(const TNotifyUI& msg)
{
	::PostQuitMessage(0L);
	__super::Close();
	ExitProcess(0L);
}

void CSteamUserMain::LoginInSteamUser(CControlUI* pControl)
{
	if (!pControl)MessageBox(NULL, _T("帐号切换失败!"), NULL, MB_OK);
	CDuiString sUserName = pControl->GetUserData();
	if (sUserName.GetLength() < 2)MessageBox(NULL, _T("帐号切换失败,数据有误!"), NULL, MB_OK);
	WriteRegiter(sUserName);
}

//结束进程(进程ID);
BOOL CSteamUserMain::TerminateProcess(DWORD pid) {
	DWORD lpid = pid;
	if (!lpid)return FALSE;
	HANDLE HProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, lpid);
	if (!HProcess)return FALSE;
	::TerminateProcess(HProcess, NULL);
	CloseHandle(HProcess);
	return TRUE;
}

void CSteamUserMain::WriteRegiter(LPCTSTR pUserName)
{
	TCHAR GamePathKey[] = _T("Software\\Valve\\Steam");
	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 730
	HKEY hKey = NULL;
	int Rkey = RegOpenKeyEx(HKEY_CURRENT_USER, GamePathKey, NULL, KEY_WOW64_64KEY | KEY_WRITE | KEY_READ, &hKey);
	if (Rkey != ERROR_SUCCESS) {
		switch (Rkey)
		{
		case ERROR_FILE_NOT_FOUND:
			MessageBox(NULL, _T("帐号切换失败,没有操作权限!"), NULL, MB_OK);;
			break;
		case ERROR_BAD_PATHNAME:
			MessageBox(NULL, _T("帐号切换失败,注册表键值操作失败!"), NULL, MB_OK);;
			break;
		default:
			MessageBox(NULL, _T("帐号切换失败,未知错误!"), NULL, MB_OK);;
			break;
		}
	}
	CDuiString sText = pUserName;
	DWORD cbData = sText.GetLength() * sizeof(TCHAR);
	Rkey = RegSetValueEx(hKey, _T("AutoLoginUser"), NULL, REG_SZ, (BYTE*)sText.GetData(), cbData);
	if (Rkey != ERROR_SUCCESS) {
		switch (Rkey)
		{
		case ERROR_FILE_NOT_FOUND:
			MessageBox(NULL, _T("帐号切换失败,没有操作权限!"), NULL, MB_OK);;
			break;
		case ERROR_BAD_PATHNAME:
			MessageBox(NULL, _T("帐号切换失败,注册表键值操作失败!"), NULL, MB_OK);;
			break;
		default:
			MessageBox(NULL, _T("帐号切换失败,未知错误!"), NULL, MB_OK);;
			break;
		}
	}
	lib_Process::Process process;
	TerminateProcess(process.ProcessNameToPid(_T("steam.exe")));
	TerminateProcess(process.ProcessNameToPid(_T("Steam.exe")));
	TerminateProcess(process.ProcessNameToPid(_T("STEAM.exe")));
	TCHAR SteamPath[MAX_PATH] = { 0 };
	TCHAR FilePath[MAX_PATH] = { 0 };
	TCHAR CfgPath[MAX_PATH] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("SteamPath"), NULL, SteamPath, sizeof(SteamPath), CfgPath);
	_tcscpy(FilePath, SteamPath);
	_tcscat(FilePath, _T("\\steam.exe"));
	ShellExecute(NULL, _T("open"), _T("explorer.exe"), FilePath, SteamPath, SW_NORMAL);
}

void CSteamUserMain::OnClick(const TNotifyUI& msg)
{
	OutputDebugString(msg.pSender->GetName());
	OutputDebugString(L"\n");
	if (_tcscmp(msg.pSender->GetName(), _T("closebtn")) == 0)OnExit(msg);
	else if (_tcscmp(msg.pSender->GetName(), _T("minbtn")) == 0)SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	else if (_tcscmp(msg.pSender->GetName(), _T("Button_Login")) == 0)LoginInSteamUser(msg.pSender);
}
