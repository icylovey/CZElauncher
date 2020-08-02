#include "Update.h"

CUpdateUI* g_pUpdate = nullptr;

LPCTSTR CUpdateUI::GetWindowClassName() const
{
	return _T("Update");
};

UILIB_RESTYPE CUpdateUI::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_ZIPRESOURCE;
#endif // _DEBUG
}

LPCTSTR CUpdateUI::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

CDuiString CUpdateUI::GetSkinFile()
{
	return _T("Update.xml");
}

CDuiString CUpdateUI::GetSkinFolder()
{
	return _T("Skin\\ListRes");
}

void CUpdateUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CUpdateUI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnCreate();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg);
	__super::Notify(msg);
}

//检测版本更新
CDuiString CUpdateUI::GetUpdateVersionUrl()
{
	std::string htmldata;
	lib_http::CLibhttp http;
	http.GET(_T("http://blog.sina.com.cn/s/blog_155297e510102z9f9.html"), htmldata);
	//转换编码
	UINT nLen = htmldata.size() * sizeof(TCHAR);
	TCHAR* pTmpHtml = new TCHAR[nLen]();
	MultiByteToWideChar(CP_UTF8, NULL, htmldata.c_str(), htmldata.length(), pTmpHtml, nLen);

	std::wstring html = pTmpHtml;
	//获取版本号
	UINT nPos = html.find(L"#Version#") + wcslen(L"#Version#");
	if (nPos == std::wstring::npos)MessageBox(NULL, _T("获取版本错误!"), NULL, MB_OK | MB_TOPMOST);
	CDuiString NewVersion = html.substr(nPos, html.find(L"$Version$", nPos) - nPos).c_str();
	//获取更新地址
	nPos = html.find(L"#UpdateUrl#") + wcslen(L"#UpdateUrl#");
	if (nPos == std::wstring::npos)MessageBox(NULL, _T("获取版本错误!"), NULL, MB_OK | MB_TOPMOST);
	CDuiString Updateurl = html.substr(nPos, html.find(L"$UpdateUrl$", nPos) - nPos).c_str();
	Updateurl.Replace(_T("amp;"), _T(""));
	//获取更新信息
	nPos = html.find(L"#UpdateInfo#") + wcslen(L"#UpdateInfo#");
	if (nPos == std::wstring::npos)MessageBox(NULL, _T("获取版本错误!"), NULL, MB_OK | MB_TOPMOST);
	CDuiString UpdateTip = _T("\t有新版本发布 ");
	UpdateTip += NewVersion;
	UpdateTip += _T("\r\n\r\n");
	UpdateTip += html.substr(nPos, html.find(L"$UpdateInfo$", nPos) - nPos).c_str();
	UpdateTip.Replace(_T("\\r\\n"), _T("\r\n"));
	CTextUI* pText = static_cast<CTextUI*>(m_pm.FindControl(_T("UpdateInfo")));
	if (pText)pText->SetText(UpdateTip);
	return std::move(Updateurl);
}

bool __stdcall DownloadProc_(size_t FileSize, UINT nState, size_t DownloadSize, size_t SecondSize)
{
	CDuiString Temp;
	if (nState == 2)
	{
		Temp = _T("文件更新下载完成");
		g_pUpdate->SetProgressText(Temp.GetData());
		g_pUpdate->SetProgressValue(50);
	}
	else if (nState == 1)
	{
		TCHAR szTemp[1024] = { 0 };
		Temp = _T("正在更新<CSGO僵尸逃跑社区登录器> 下载速度 %.2lf%s/S");
		float nSize = static_cast<float>(SecondSize), nDownlen = 0;
		TCHAR dwType[5] = _T("B");
		nDownlen = nSize / 1024;
		if (nDownlen >= 1) {
			nSize = nDownlen;
			_tcscpy(dwType, _T("KB"));
			nDownlen /= 1024;
			if (nDownlen >= 1) {
				nSize = nDownlen;
				_tcscpy(dwType, _T("MB"));
				nDownlen /= 1024;
				if (nDownlen >= 1) {
					nSize = nDownlen;
					_tcscpy(dwType, _T("GB"));
				}
				else nDownlen = nSize;
			}
			else nDownlen = nSize;
		}
		else nDownlen = nSize;
		_stprintf(szTemp, Temp.GetData(), nDownlen, dwType);
		g_pUpdate->SetProgressText(szTemp);
		if (!FileSize)FileSize = 1;
		g_pUpdate->SetProgressValue(DownloadSize / FileSize * 50);

	}
	return true;
}

void CUpdateUI::SetProgressValue(UINT Value)
{
	CProgressUI* pProgress = static_cast<CProgressUI*>(m_pm.FindControl(_T("ProgressBar")));
	if (pProgress)pProgress->SetValue(Value);
}

void CUpdateUI::SetProgressText(LPCTSTR pString)
{
	CProgressUI* pProgress = static_cast<CProgressUI*>(m_pm.FindControl(_T("ProgressBar")));
	if (pProgress)pProgress->SetText(pString);
}

void CUpdateUI::DownloadThread(CDuiString& Url)
{
	wchar_t* TempPath = new wchar_t[MAX_PATH]();
	GetTempPath(MAX_PATH, TempPath);
	wcscat(TempPath, L"\\Temp.zip");

	wchar_t* ZipPath = new wchar_t[MAX_PATH]();
	GetModuleFileName(NULL, ZipPath, MAX_PATH);
	wcsrchr(ZipPath, '\\')[0] = 0;

	lib_http::CLibhttp http;
	http.DownloadFile(Url.GetData(), TempPath, DownloadProc_);
	HZIP hzip = OpenZip(TempPath, NULL);
	if (!hzip)SetProgressText(L"压缩包打开失败!");

	SetUnzipBaseDir(hzip, ZipPath);
	SHCreateDirectoryEx(NULL, ZipPath, NULL);
	ZIPENTRY zipentry = { 0 };
	GetZipItem(hzip, -1, &zipentry);
	if (zipentry.index <= 0) {
		SetProgressText(L"压缩包损坏或不完整!");
		CloseZip(hzip);
	}
	ZIPENTRY TmpZipEntry = { 0 };
	for (int i = 0; i < zipentry.index; i++) {
		GetZipItem(hzip, i, &TmpZipEntry);
		UnzipItem(hzip, i, TmpZipEntry.name);
		CDuiString tmpstr = L"正在解压-> ";
		tmpstr += TmpZipEntry.name;
		SetProgressText(tmpstr);
		SetProgressValue(50 + ((i / zipentry.index) * 50));
	}
	SetProgressValue(100);
	SetProgressText(L"CSGO僵尸逃跑社区登录器  更新完成!");
	DeleteFile(TempPath);
	delete[]TempPath;

	CDuiString RunFile = ZipPath;
	RunFile += "\\ZElauncher.exe";
	if (MessageBox(NULL, _T("更新完成,是否启动主程序?"), NULL, MB_YESNO | MB_TOPMOST | MB_ICONQUESTION) == IDYES)
		ShellExecute(NULL, _T("open"), RunFile.GetData(), NULL, NULL, NULL);

	_bstr_t Cmdline = "cmd /c ping 127.0.0.1 -n 5 &del \"";
	Cmdline += ZipPath;
	Cmdline += "\\UpdateDownload.exe\"";
	Cmdline += "&ren old_update.exe UpdateDownload.exe ";
	Cmdline += "&del update.bat";
	_bstr_t cmdfile = ZipPath;
	cmdfile += "\\update.bat";
	FILE* pFile = fopen(cmdfile, "wb+");
	if (pFile) {
		fwrite((char*)Cmdline, Cmdline.length(), 1, pFile);
		fclose(pFile);
	}
	
	ShellExecute(NULL, _T("open"), cmdfile, NULL, NULL, SW_HIDE);
	ExitProcess(NULL);
}

void CUpdateUI::OnCreate()
{
	g_pUpdate = this;
	CDuiString Url = GetUpdateVersionUrl();
	
	std::thread t1 = std::thread(&CUpdateUI::DownloadThread, this, Url);
	t1.detach();
}

CControlUI* CUpdateUI::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CUpdateUI::OnExit(const TNotifyUI& msg)
{
	::PostQuitMessage(0L);
	__super::Close();
	ExitProcess(0L);
}

void CUpdateUI::OnClick(const TNotifyUI& msg)
{
	if (_tcscmp(msg.pSender->GetName(), _T("closebtn")) == 0)OnExit(msg);
	else if (_tcscmp(msg.pSender->GetName(), _T("minbtn")) == 0)SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);

}
