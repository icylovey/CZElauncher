#include "Shop.h"

CShopUI::CShopUI(CPaintManagerUI& m_paintmanage)
{
	m_paintmanager = &m_paintmanage;
	m_PageNum = 1;
	CDialogBuilder Builder;
	CContainerUI* pShop = static_cast<CContainerUI*>(Builder.Create(_T("Shop.xml"), NULL, NULL, &m_pm));
	if (pShop) {
		this->Add(pShop);
		m_paintmanager->AddNotifier(this);
	}
	else {
		this->RemoveAll();
		return;
	}
}

void CShopUI::OnClick(CControlUI* Click)
{
	if (_tcscmp(Click->GetName(), _T("page_up")) == 0) { 
		m_PageNum--; 
		if (m_PageNum < 1)m_PageNum = 1;
		else GetShopinfo();
	}
	else if (_tcscmp(Click->GetName(), _T("page_next")) == 0) { 
		m_PageNum++; 
		if (m_PageNum > m_pageNumMax)m_PageNum = m_pageNumMax;
		else GetShopinfo();
	}
	else if (_tcsstr(Click->GetName(), _T("Label_Skin")))
	{
		CDuiString url = _T("https://bbs.93x.net/");
		url += Click->GetUserData();
		url.Replace(_T("amp;"), _T(""));
		ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_NORMAL);
	}
}

void CShopUI::Notify(TNotifyUI& msg)
{

	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnCreate();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);

}

std::string CShopUI::GetSkinHtml(const TCHAR* Page)
{
	std::string Result;
	{
		lib_http::http http;
		CDuiString url = _T("https://bbs.93x.net/plugin.php?id=dc_mall&uid=0&page=");
		url += Page;
		CDuiString Cookies = http.GetLocalCookies(_T("https://bbs.93x.net")).c_str();
		http.GET(url.GetData(), Result, _T(""), Cookies.GetData());
		//http.GET(url.GetData(), Result);
	}
	return Result;
}

void CShopUI::GetShopinfo()
{
	{
		for (int i = 0; i < 12; i++) {
			TCHAR szbuf[1024] = { 0 };
			_stprintf(szbuf, _T("panel_Skin%d"), i + 1);
			CContainerUI* pPanel = static_cast<CContainerUI*>(m_paintmanager->FindControl(szbuf));
			if (pPanel)pPanel->SetVisible(false);
		}
	}
	//获取正则表达式数据
	/*FILE* pFile = fopen("F:\\ProjectCode\\C++\\Other\\ZElauncher\\Debug\\ShopReg.db", "rb");
	if (!pFile) { MessageBox(NULL, _T("字符数据库丢失!无法获取商城数据!"), NULL, MB_OK); return; }
	fseek(pFile, 0L, SEEK_END);
	UINT sLen = ftell(pFile);
	char* pText = new char[sLen + 2];
	ZeroMemory(pText, sLen + 2);
	fseek(pFile, 0L, SEEK_SET);
	fread(pText, 1, sLen, pFile);
	fclose(pFile);*/
	TCHAR dbPath[MAX_PATH] = { 0 };
	GetRunPath(dbPath, sizeof(dbPath));
	_tcscat(dbPath, _T("\\bin\\Regex.db"));
	TCHAR* pTpattern = new TCHAR[4096];
	ZeroMemory(pTpattern, 4096);
	GetPrivateProfileString(_T("ZElauncher"), _T("Shop"), NULL, pTpattern, 4096, dbPath);
	if (_tcslen(pTpattern) < 5) { MessageBox(NULL, _T("字符数据库丢失!无法获取商城数据!"), NULL, MB_OK); delete[]pTpattern; return; }
	//获取商城网页数据
	TCHAR page[50] = { 0 };
	_stprintf(page, _T("%d"), m_PageNum);
	std::string Result = GetSkinHtml(page);
	if (Result.length() < 10) {
		MessageBox(NULL, _T("获取商城数据失败,请确认是否可访问93.net!"), NULL, MB_OK);
		return;
	}
	UINT nLen = Result.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, Result.c_str(), Result.length(), pStrHtml, nLen);
	/*UINT nLen2 = Result.size() * sizeof(TCHAR);
	char* pStrMulti = new char[nLen2];
	ZeroMemory(pStrMulti, nLen2);
	WideCharToMultiByte(CP_ACP, NULL, pStrHtml, nLen, pStrMulti, nLen2, NULL, NULL);
	Result = pStrMulti;
	delete[]pStrMulti;
	delete[]pStrHtml;*/
	if (wcsstr(pStrHtml,L"提示信息"))return;
#pragma region 正则表达式算法
	CContainerUI* pPage = static_cast<CContainerUI*>(m_paintmanager->FindControl(_T("page_panel")));
	if (pPage)pPage->SetVisible(true);
	VBScript_RegExp_55::IRegExp2Ptr pRegexp(__uuidof(VBScript_RegExp_55::RegExp));
	pRegexp->PutGlobal(VARIANT_TRUE);
	pRegexp->put_Multiline(VARIANT_FALSE);
	pRegexp->PutIgnoreCase(VARIANT_FALSE);
	pRegexp->PutPattern(pTpattern);
	VBScript_RegExp_55::IMatchCollectionPtr Item = pRegexp->Execute(pStrHtml);
	srand(GetTickCount());
	for (int i = 0; i < Item->GetCount(); i++) {
		if (i > 12)break;
		VBScript_RegExp_55::IMatch2Ptr Match = Item->GetItem(i);
		if(!Match)continue;
		VBScript_RegExp_55::ISubMatchesPtr Submatch = Match->GetSubMatches();
		if (!Submatch)continue;
		//显示皮肤
		_bstr_t sText;
		TCHAR szbuf[1024] = { 0 };
		_stprintf(szbuf, _T("panel_Skin%d"), i + 1);
		CContainerUI* pPanel = static_cast<CContainerUI*>(m_paintmanager->FindControl(szbuf));
		if (pPanel)pPanel->SetVisible(true);
		//获取皮肤名称
		sText = static_cast<_bstr_t>(Submatch->GetItem(1L));
		_stprintf(szbuf, _T("Text_SkinName%d"), i + 1);
		CTextUI* pText = static_cast<CTextUI*>(m_paintmanager->FindControl(szbuf));;
		if (pText) {
			pText->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
			pText->SetText(sText);
		}
		//获取皮肤图片
		sText = static_cast<_bstr_t>(Submatch->GetItem(2L));
		_stprintf(szbuf, _T("Label_Skin%d"), i + 1);
		COptionUI* pLabel = static_cast<COptionUI*>(m_paintmanager->FindControl(szbuf));;
		if (pLabel) {
			//保存皮肤购买链接
			_bstr_t tmp23 = static_cast<_bstr_t>(Submatch->GetItem(0L));
			pLabel->SetUserData(tmp23);
			//设置皮肤图片
			_stprintf(szbuf, _T("Temp\\%d_%d.png"), i,m_PageNum);
			std::string ImgData;
			if (sText.length() > 3)
			{
				lib_http::http htp;
				std::wstring url;
				url = L"https://bbs.93x.net/";
				url += sText;
				htp.GET(url, ImgData);
				if (ImgData.find("404") == std::string::npos) {
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, ImgData.size());
					void* pData = GlobalLock(hGlobal);
					memcpy(pData, ImgData.c_str(), ImgData.size());
					GlobalUnlock(hGlobal);
					IStream* pStream = NULL;
					CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
					CImage img;
					img.Load(pStream);
					//m_paintmanager->RemoveImage(szbuf);
					m_paintmanager->AddImage(szbuf, (HBITMAP)img, img.GetWidth(), img.GetHeight(), false);
					img.Detach();
					pLabel->SetBkImage(szbuf);
					pStream->Release();
					GlobalFree(hGlobal);
				}
				else {
					pLabel->SetBkImage(_T("skin\\nopic.jpg"));
				}
			}
			else {
				pLabel->SetBkImage(_T("skin\\nopic.jpg"));
			}

		}
		//获取金钱数量
		sText = _T("{c #386382}价格:{/c}{c #FF0000}");
		sText += static_cast<_bstr_t>(Submatch->GetItem(3L));
		sText += _T("{/c}{c #386382}");
		sText += static_cast<_bstr_t>(Submatch->GetItem(4L));
		sText += _T("{/c}");
		_stprintf(szbuf, _T("Text_SkinMoney%d"), i + 1);
		pText = static_cast<CTextUI*>(m_paintmanager->FindControl(szbuf));
		if (pText)pText->SetText(sText);
		//获取VIP金钱数量
		sText = _T("{c #386382}VP价:{/c}{c #FF0000}");
		sText += static_cast<_bstr_t>(Submatch->GetItem(5L));
		sText += _T("{/c}{c #386382}");
		sText += static_cast<_bstr_t>(Submatch->GetItem(6L));
		sText += _T("{/c}");
		_stprintf(szbuf, _T("Text_SkinVip%d"), i + 1);
		pText = static_cast<CTextUI*>(m_paintmanager->FindControl(szbuf));
		if (pText)pText->SetText(sText);
	}
	pRegexp.Release();
	_stprintf(page, _T("%d/%d"), m_PageNum, m_pageNumMax);
	CTextUI* pPaget = static_cast<CTextUI*>(m_paintmanager->FindControl(_T("page_text")));
	if (pPaget)pPaget->SetText(page);
#pragma endregion
	{
		std::wstring shtml = pStrHtml;
		UINT Pos = shtml.find(L"<span title=\"共");
		if (Pos != std::wstring::npos) {
			m_pageNumMax = _wtoi(shtml.substr(Pos + wcslen(L"<span title=\"共"), shtml.find(L"页\">", Pos) - Pos - wcslen(L"页\">")).c_str());
		}
	}
	delete[]pStrHtml;
}

void __stdcall ThreadCreate(CShopUI* pShopc)
{
	if (pShopc)pShopc->GetShopinfo();
}

void CShopUI::OnCreate()
{
	m_pageNumMax = m_PageNum;
	/*std::list<std::future<void>> lk;
	auto __p = std::async(std::launch::async, &CShopUI::GetSkininfo, this);
	lk.push_back(std::move(__p));*/
	//GetSkininfo();
	//HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadCreate, this, NULL, NULL);
}

