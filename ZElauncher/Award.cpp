#include "Award.h"
TCHAR G_TodayUIName[50] = { 0 };
CAwardUI::CAwardUI(CPaintManagerUI& m_paintmanage)
{
	m_paintmanager = &m_paintmanage;
	CDialogBuilder Builder;
	CContainerUI* pAward = static_cast<CContainerUI*>(Builder.Create(_T("Award.xml"), NULL, NULL, &m_pm));
	if (pAward) {
		this->Add(pAward);
		m_paintmanager->AddNotifier(this);
		DrawCalendar(pAward);
	}
	else {
		this->RemoveAll();
		return;
	}
}

void CAwardUI::DoEvent(TEventUI& event)
{
	__super::DoEvent(event);
}



void CAwardUI::OnClick(CControlUI* Click)
{
	if (_tcscmp(Click->GetName(), m_Signbtn.GetData()) == 0)Sign93x();
	else if (_tcscmp(Click->GetName(), _T("online_10")) == 0)online_Award(Url_Online_10, _T("50叶子"));
	else if (_tcscmp(Click->GetName(), _T("online_60")) == 0)online_Award(Url_Online_60, _T("100叶子"));
	else if (_tcscmp(Click->GetName(), _T("online_by")) == 0)online_Award(Url_Online_by, _T("100叶子"));
	else if (_tcscmp(Click->GetName(), _T("online_hj")) == 0)online_Award(Url_Online_hj, _T("200叶子"));
	else if (_tcscmp(Click->GetName(), _T("yaoyiyao")) == 0)yaoyiyao();
}

void CAwardUI::Sign93x()
{
	//获取签到Key和个人信息
	if (MessageBox(NULL, _T("由于X社接口限制原因,请求操作会很慢甚至假死. 请问是否继续操作?"), NULL, MB_YESNO) == IDNO)return;
	{
		lib_http::CLibhttp http;
		CDuiString Cookies = http.GetLocalCookies(_T("https://bbs.93x.net")).c_str();
		std::string Result;
		http.GET(_T("https://bbs.93x.net/plugin.php?id=xnet_steam_openid:SoftLogin"), Result, _T(""), Cookies.GetData());
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(Result, root)) {
			if (!root.isNull()) {
				Json::Value userdata = root["userdata"];
				if(userdata["user_auth"].isNull()){
					MessageBox(NULL, _T("签到失败,Key获取失败!"), NULL, NULL);
					return;
				}
				std::string sKey = userdata["user_auth"].asCString();
				std::string url = "https://bbs.93x.net/plugin.php?id=are_luo_qiandao:getawardsoftapi&auth=";
				url += sKey;
				Result = "";
				http.GET(url, Result);
				UINT nLen = Result.size() * sizeof(TCHAR);
				TCHAR* pStrHtml = new TCHAR[nLen];
				_MultiByteToWideChar(CP_UTF8, NULL, Result.c_str(), Result.length(), pStrHtml, nLen);
				if (wcsstr(pStrHtml, L"您已获得")) {
					CAwardTipUI* pAwardTip = new CAwardTipUI(*m_paintmanager, pStrHtml);
					if (pAwardTip == NULL) return;
					pAwardTip->Create(NULL, _T("AwardUI"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 900, 500);
					pAwardTip->CenterWindow();
					::ShowWindow(*pAwardTip, SW_SHOW);
				}
				else MessageBoxW(NULL, pStrHtml, NULL, MB_OK);
				SYSTEMTIME m_systime = { 0 };
				GetLocalTime(&m_systime);
				TCHAR szBuf[256] = { 0 };
				_stprintf(szBuf, _T("%d_Sign_%d"), m_systime.wMonth, m_systime.wDay);
				CDuiString szFile = CPaintManagerUI::GetInstancePath() + _T("\\bin\\Data.dat");
				WritePrivateProfileString(_T("CSGO"), szBuf, _T("true"), szFile);
				DrawCalendar(this);
			}
		}
		else MessageBox(NULL, _T("获取信息失败,Cookie过期请重新登录!"), NULL, NULL);
	}
}

void CAwardUI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
}

_bstr_t CAwardUI::GetFormhash(LPCTSTR pUrl)
{
	std::string strHtml;
	lib_http::CLibhttp http;
	CDuiString Cookies = http.GetLocalCookies(pUrl).c_str();
	http.GET(pUrl, strHtml, _T(""), Cookies.GetData());
	UINT nPos = strHtml.find("formhash=");
	if (nPos == std::string::npos)return _T("");
	nPos += strlen("formhash=");
	UINT nNextpos = strHtml.find("\">", nPos);
	if (nNextpos == std::string::npos)return _T("");
	std::string result = strHtml.substr(nPos, nNextpos - nPos);
	return result.c_str();
}

bool CAwardUI::IsStart_yaoyiyao(LPCTSTR pUrl)
{
	std::string Result;
	{
		lib_http::CLibhttp http;
		CDuiString Cookies = http.GetLocalCookies(pUrl).c_str();
		CDuiString url = pUrl;
		url += _T("plugin.php?id=yinxingfei_zzza:yinxingfei_zzza_hall");
		http.GET(url.GetData(), Result, _T(""), Cookies.GetData());
		if (Result.length() < 10) {
			MessageBox(NULL, _T("获取失败,Cookies过期或未登录!"), _T("Tip:"), MB_OK);
			return false;
		}
	}
	UINT nLen = Result.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, Result.c_str(), Result.length(), pStrHtml, nLen);
	std::wstring StrHtml = pStrHtml;
	if (StrHtml.find(L"请求来路不明,请返回") != std::wstring::npos) {
		MessageBox(NULL, _T("亲,Cookies过期或其他未知原因,导致领取失败!"), _T("Tip:"), MB_OK);
		return false;
	}
	else if (StrHtml.find(L"今天您已经摇过奖，请明天再试") != std::wstring::npos) {
		MessageBox(NULL, _T("亲,该今天您已经摇过奖，请明天再来."), _T("Tip:"), MB_OK);
		return false;
	}
	else if (StrHtml.find(L"需要先登录才能继续本操作") != std::wstring::npos) {
		MessageBox(NULL, _T("亲,Cookies过期或没有登录!"), _T("Tip:"), MB_OK);
		return false;
	}
	UINT pos = StrHtml.find(L"您需要 游戏在线");
	if (pos != std::wstring::npos) {
		TCHAR szbuf[256] = { 0 };
		pos = StrHtml.find(L"今日在线", pos);
		UINT nLen = _tcslen(_T("今日在线"));
		std::wstring Str_Min = StrHtml.substr(pos + nLen, StrHtml.find(_T("分钟"), pos) - pos - nLen);
		Str_Min = L"游戏时间不足,您当前游戏时间 " + Str_Min + L" 分钟";
		MessageBoxW(NULL, Str_Min.c_str(), L"Tip:", MB_OK);
		return false;
	}
	//所有失败结束后的最后询问是否开奖
	pos = StrHtml.find(L"恭喜您获得");
	if (pos != std::wstring::npos) {
		TCHAR szbuf[256] = { 0 };
		pos = StrHtml.find(L"恭喜您获得", pos);
		UINT nLen = _tcslen(_T("恭喜您获得"));
		std::wstring Str_Award = StrHtml.substr(pos + nLen, StrHtml.find(_T("奖励，明天记得再来摇摇乐哦"), pos) - pos - nLen);
		Str_Award = L"您今日摇摇乐可以获得 " + Str_Award + L" 奖励,请问是否现在开奖领取?";
		if (MessageBoxW(NULL, Str_Award.c_str(), L"Tip:", MB_YESNO) == IDYES)return true;
		else return false;
	}
	return true;
}

void CAwardUI::yaoyiyao()
{
	if (MessageBox(NULL, _T("由于X社接口限制原因,请求操作会很慢甚至假死. 请问是否继续操作?"), NULL, MB_YESNO) == IDNO)return;
	CDuiString htmlUrl = _T("https://bbs.93x.net/");
	if (!IsStart_yaoyiyao(htmlUrl))return;
	std::string Result;
	{
		lib_http::CLibhttp http;
		CDuiString Cookies = http.GetLocalCookies(htmlUrl.GetData()).c_str();
		CDuiString sFormhash = _T("formhash=");
		sFormhash += static_cast<TCHAR*>(GetFormhash(htmlUrl));
		htmlUrl += _T("/plugin.php?id=yinxingfei_zzza:yinxingfei_zzza_post");
		http.POST(htmlUrl.GetData(), Result, sFormhash.GetData(), _T(""), Cookies.GetData());
		if (Result.length() < 10) {
			MessageBox(NULL, _T("获取失败,Cookies过期或未登录!"), _T("Tip:"), MB_OK);
			return;
		}
	}
	UINT nLen = Result.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, Result.c_str(), Result.length(), pStrHtml, nLen);
	std::wstring StrHtml = pStrHtml;
	if (StrHtml.find(L"请求来路不明,请返回") != std::wstring::npos) {
		MessageBox(NULL, _T("亲,请求失败，出现未知错误!"), _T("Tip:"), MB_OK);
		return;
	}
	else if (StrHtml.find(L"今天您已经摇过奖，请明天再试") != std::wstring::npos) {
		MessageBox(NULL, _T("亲,该今天您已经摇过奖，请明天再来."), _T("Tip:"), MB_OK);
		return;
	}
	UINT pos = StrHtml.find(L"您需要 游戏在线");
	if (pos != std::wstring::npos) {
		TCHAR szbuf[256] = { 0 };
		pos = StrHtml.find(L"今日在线", pos);
		UINT nLen = _tcslen(_T("今日在线"));
		std::wstring Str_Min = StrHtml.substr(pos + nLen, StrHtml.find(_T("分钟"), pos) - pos - nLen);
		Str_Min = L"游戏时间不足,您当前游戏时间 " + Str_Min + L" 分钟";
		MessageBoxW(NULL, Str_Min.c_str(), L"Tip:", MB_OK);
		return;
	}
	MessageBox(NULL, _T("领取成功!"), _T("Tip:"), MB_OK);

}

void CAwardUI::online_Award(LPCTSTR pUrl, LPCTSTR szAward)
{
	if (MessageBox(NULL, _T("由于X社接口限制原因,请求操作会很慢甚至假死. 请问是否继续操作?"), NULL, MB_YESNO) == IDNO)return;
	std::string Result;
	{
		lib_http::CLibhttp http;
		CDuiString Cookies = http.GetLocalCookies(_T("https://bbs.93x.net")).c_str();
		http.GET(pUrl, Result, _T(""), Cookies.GetData());
		if (Result.length() < 10) {
			MessageBox(NULL, _T("获取失败,Cookies过期或未登录!"), _T("Tip:"), MB_OK);
			return;
		}
	}
	UINT nLen = Result.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, Result.c_str(), Result.length(), pStrHtml, nLen);
	std::wstring StrHtml = pStrHtml;
#pragma region 获取返回结果
	VBScript_RegExp_55::IRegExp2Ptr pRegexp(__uuidof(VBScript_RegExp_55::RegExp));
	VBScript_RegExp_55::IMatchCollectionPtr sResult;
	CDuiString sTip = _T("");
	TCHAR dbPath[MAX_PATH] = { 0 };
	GetRunPath(dbPath, sizeof(dbPath));
	_tcscat(dbPath, _T("\\bin\\Regex.db"));
	TCHAR* pTpattern = new TCHAR[4096];
	ZeroMemory(pTpattern, 4096);
	//读取表达式数据
	GetPrivateProfileString(_T("ZElauncher"), _T("SkinConvertResult"), NULL, pTpattern, 4096, dbPath);
	if (_tcslen(pTpattern) < 5) { MessageBox(NULL, _T("字符数据库丢失!无法获取新手礼包数据!"), NULL, MB_OK); goto Exit; }
	pRegexp->PutGlobal(VARIANT_TRUE);
	pRegexp->put_Multiline(VARIANT_FALSE);
	pRegexp->PutIgnoreCase(VARIANT_FALSE);
	pRegexp->PutPattern(pTpattern);
	sResult = pRegexp->Execute(pStrHtml);
	if (sResult->GetCount() > 0) {
		VBScript_RegExp_55::IMatch2Ptr Match = sResult->GetItem(0L);
		if (!Match) { MessageBox(NULL, _T("获取返回数据失败!"), NULL, MB_OK); goto Exit; }
		VBScript_RegExp_55::ISubMatchesPtr Submatch = Match->GetSubMatches();
		if (!Submatch) { MessageBox(NULL, _T("获取返回数据失败!"), NULL, MB_OK); goto Exit; }
		_bstr_t tmpbstr = static_cast<_bstr_t>(Submatch->GetItem(0L));
		sTip = static_cast<TCHAR*>(tmpbstr);
		sTip.Replace(_T("<br>"), _T("\r\n"));
		sTip.Replace(_T("</p>"), _T(""));
		if (sTip.Find(_T("领取成功")) != -1) {
			sTip += _T("\r\n获得 ");
			sTip += szAward;
		}
		MessageBox(NULL, sTip, _T("Tip:"), MB_OK);
	}
	else MessageBox(NULL, sTip, _T("领取礼包过程中,出现未知错误!"), MB_OK);
Exit:
	delete[]pTpattern;
	delete[]pStrHtml;
	pRegexp.Release();
#pragma endregion
}

/*
void CAwardUI::online_Award(LPCTSTR pUrl, LPCTSTR szAward)
{
	std::string Result;
	{
		lib_http::CLibhttp http;
		CDuiString Cookies = http.GetLocalCookies(_T("https://bbs.93x.net")).c_str();
		http.GET(pUrl, Result, _T(""), Cookies.GetData());
		if (Result.length() < 10) {
			MessageBox(NULL, _T("获取失败,Cookies过期或未登录!"), _T("Tip:"), MB_OK);
			return;
		}
	}
	UINT nLen = Result.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, Result.c_str(), Result.length(), pStrHtml, nLen);
	std::wstring StrHtml = pStrHtml;
	if (StrHtml.find(L"登陆后才可以领取在线礼包") != std::wstring::npos) {
		MessageBox(NULL, _T("亲,登陆后才可以领取在线礼包哦."), _T("Tip:"), MB_OK);
		return;
	}
	if (StrHtml.find(L"该礼包今日已领取过了") != std::wstring::npos) {
		MessageBox(NULL, _T("亲,该礼包今日已领取过了."), _T("Tip:"), MB_OK);
		return;
	}
	if (StrHtml.find(L"礼包领取成功") != std::wstring::npos) {
		MessageBox(NULL, _T("恭喜您礼包领取成功,获得 50 叶子."), _T("Tip:"), MB_OK);
		return;
	}
	UINT pos = StrHtml.find(L"游戏时间不足");
	if (pos != std::wstring::npos) {
		TCHAR szbuf[256] = { 0 };
		pos = StrHtml.find(L"当前", pos);
		UINT nLen = _tcslen(_T("当前"));
		std::wstring Str_Min = StrHtml.substr(pos + nLen, StrHtml.find(_T("分钟"), pos) - pos - nLen);
		Str_Min = L"游戏时间不足,您当前游戏时间 " + Str_Min + L" 分钟";
		MessageBoxW(NULL, Str_Min.c_str(), L"Tip:", MB_OK);
	}

}*/

void CAwardUI::DrawCalendar(CContainerUI* pAwarad)
{
	SYSTEMTIME m_systime = { 0 };
	GetLocalTime(&m_systime);
	time_t TmpTime;
	time(&TmpTime);
	tm* pTime = localtime(&TmpTime);
	pTime->tm_mday = 1;
	TmpTime = mktime(pTime);
	pTime = localtime(&TmpTime);
	int iWeekDays = m_systime.wDayOfWeek;
	//本月天数
	int iMonthDays = GetMonthDays(m_systime.wYear, m_systime.wMonth);
	//下月天数
	int iNextMonthDays = 0;
	CButtonUI* pButtonUI = nullptr;
	TCHAR szBuf[256] = { 0 };
	UINT iDay = 0;
	_stprintf(G_TodayUIName, _T("OptionUI%d"), pTime->tm_wday + m_systime.wDay);
	CTextUI* pText = static_cast<CTextUI*>(m_pm.FindSubControlByName(pAwarad, _T("Text_yyd")));
	if (pText) { _stprintf(szBuf, _T("%d 月"), m_systime.wMonth); pText->SetText(szBuf); }
	CDuiString szFile = CPaintManagerUI::GetInstancePath() + _T("\\bin\\Data.dat");
	GetPrivateProfileString(_T("CSGO"), _T("Date_Year"), NULL, szBuf, sizeof(szBuf), szFile);
	UINT nYear = _wtoi(szBuf);
	if (nYear != m_systime.wYear) {
		DeleteFile(szFile);
		_stprintf(szBuf, _T("%d"), m_systime.wYear);
		WritePrivateProfileString(_T("CSGO"), _T("Date_Year"), szBuf, szFile);
	}
	for (int i = pTime->tm_wday; i < (iMonthDays + pTime->tm_wday); i++)
	{
		_stprintf(szBuf, _T("OptionUI%d"), i + 1);
		pButtonUI = static_cast<CButtonUI*>(m_pm.FindSubControlByName(pAwarad, szBuf));
		if (!pButtonUI)continue;
		iDay++;
		if (iDay < m_systime.wDay)pButtonUI->SetEnabled(false);
		if (iDay == m_systime.wDay)m_Signbtn = szBuf;
		_stprintf(szBuf, _T("%d"), iDay);
		pButtonUI->SetText(szBuf);
		pButtonUI->SetTextColor(RGB(255, 0, 0));
		_stprintf(szBuf, _T("%d_Sign_%d"), m_systime.wMonth, iDay);
		GetPrivateProfileString(_T("CSGO"), szBuf, NULL, szBuf, sizeof(szBuf), szFile);
		if (_tcscmp(szBuf, _T("true")) == 0) {
			pButtonUI->SetBkImage(_T("icon\\icon\\sign.png"));
			pButtonUI->SetTextColor(RGB(0, 0, 255));
			pButtonUI->SetDisabledTextColor(RGB(0, 0, 255));
			pButtonUI->SetHotImage(_T(""));
		}
	}
}//绘制日历

int CAwardUI::GetMonthDays(int iY, int iM)
{
	int iTotalDay = 31;
	if (iM == 2)
	{
		//闰年可以被4或者400整除 但是不能被100整除
		if (iY % 4 == 0 && iY % 100 != 0 || iY % 400 == 0)
			iTotalDay = 29;
		else
			iTotalDay = 28;
	}
	else if (iM == 4 || iM == 6 || iM == 9 || iM == 11)
		iTotalDay = 30;
	return iTotalDay;
}
