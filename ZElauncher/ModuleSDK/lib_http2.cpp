#include "lib_http.h"

namespace lib_http
{
	http::http()
	{
		
	}

	http::~http()
	{
		
	}
	std::string _fastcall http::GetHost(std::string url)
	{
		bool Ishttps;
		url.find("https://") == std::string::npos ? Ishttps = false : Ishttps = true;
		size_t strpos, coutof;
		Ishttps ? strpos = url.find("https://") + strlen("https://") : strpos = url.find("http://") + strlen("http://");
		url.find('/', strpos) == std::string::npos ? coutof = url.find('/', strpos) : coutof = url.find('/', strpos);
		std::string host;
		if (Ishttps)host = url.substr(strpos, coutof - strlen("https://"));
		else host = url.substr(strpos, coutof - strlen("http://"));
		strpos = host.find(':');
		if (strpos != std::wstring::npos)host = host.substr(0, strpos);
		return host;
	}
	std::wstring _fastcall http::GetHost(std::wstring url)
	{
		bool Ishttps;
		url.find(L"https://") == std::wstring::npos ? Ishttps = false : Ishttps = true;
		size_t strpos, coutof;
		Ishttps ? strpos = url.find(L"https://") + wcslen(L"https://") : strpos = url.find(L"http://") + wcslen(L"http://");
		url.find('/', strpos) == std::wstring::npos ? coutof = url.find('/', strpos) : coutof = url.find('/', strpos);
		std::wstring host;
		if (Ishttps)host = url.substr(strpos, coutof - wcslen(L"https://"));
		else host = url.substr(strpos, coutof - wcslen(L"http://"));
		strpos = host.find(':');
		if (strpos != std::wstring::npos)host = host.substr(0, strpos);
		return host;
	}
	std::string _fastcall http::GetURLPage(std::string url)
	{
		bool Ishttps;
		url.find("https://") == std::wstring::npos ? Ishttps = false : Ishttps = true;
		size_t strpos, coutof;
		Ishttps ? strpos = url.find("https://") + strlen("https://") : strpos = url.find("http://") + strlen("http://");
		coutof = url.find('/', strpos) + 1;
		if (coutof == std::wstring::npos)return "";
		std::string urlpage = url.substr(coutof, strpos - coutof);
		return urlpage;
	}
	std::wstring _fastcall http::GetURLPage(std::wstring url)
	{
		bool Ishttps;
		url.find(L"https://") == std::wstring::npos ? Ishttps = false : Ishttps = true;
		size_t strpos, coutof;
		Ishttps ? strpos = url.find(L"https://") + wcslen(L"https://") : strpos = url.find(L"http://") + wcslen(L"http://");
		coutof = url.find('/', strpos);
		if (coutof == std::wstring::npos)return L"";
		std::wstring urlpage= url.substr(coutof, strpos-coutof);
		return urlpage;
	}
	bool _fastcall http::POST(std::string url, std::string &ResultData, std::string RequestData, std::string Header, std::string Cookies, std::string ReturnCookies)
	{
		bool IsHttps;
		//把url转小写(不使用)
		//std::transform(url.begin(), url.end(), url.begin(), ::tolower);
		url.find("https") == std::string::npos ? IsHttps = false : IsHttps = true;
#pragma region 协议头处理
		if (Header.find("User-Agent:") == std::string::npos)Header.append("User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299\r\n");
		if (Header.find("Accept:") == std::string::npos)Header.append("Accept: */*\r\n");
		if (Header.find("Accept-Language:") == std::string::npos)Header.append("Accept-Language: zh-cn\r\n");
		if (Header.find("Content-Type:") == std::string::npos)Header.append("Content-Type: application/x-www-form-urlencoded\r\n");
		if (Header.find("Host:") == std::string::npos) {
			Header.append("Host: ");
			Header.append(GetHost(url));
			Header.append("\r\n");
		}
		if (Header.find("Referer:") == std::string::npos) {
			Header.append("Referer: ");
			Header.append(url);
			Header.append("\r\n");
		}
		if (!Cookies.empty()) {
			if (Header.find("Cookies:") == std::string::npos) {
				Header.append("Cookies: ");
				Header.append(Cookies);
				Header.append("\r\n");
			}
			else {
				Header.append(Cookies);
				Header.append("\r\n");
			}
		}
#pragma endregion
		//打开HTTP句柄;
		HINTERNET hInternetopen = InternetOpenA(Header.c_str(), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectA(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		DWORD RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		if (Cookies.empty())RequestFlg = RequestFlg | INTERNET_FLAG_NO_COOKIES;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestA(hInternetConnect, "POST", GetURLPage(url).c_str(), "HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		if (!HttpSendRequestA(hInternetRequest, Header.c_str(), Header.length(), (LPVOID)(RequestData.c_str()), RequestData.size()))return false;
		//new的异常处理
		char *pResultData = nullptr;
		try { pResultData = new char[1025]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		UINT  ResultLen = 0;
		do
		{
			ZeroMemory(pResultData, 1025);
			InternetReadFile(hInternetRequest, pResultData, 1024, reinterpret_cast<LPDWORD>(&ResultLen));
			ResultData.append(reinterpret_cast<char*>(pResultData));
		} while (ResultLen);
		char *pTmpQuery = nullptr;
		try { pTmpQuery = new char[4096]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		ZeroMemory(pTmpQuery, 4096 * sizeof(char));
		DWORD CookiesLength = 4095;
		HttpQueryInfoA(hInternetRequest, HTTP_QUERY_SET_COOKIE, pTmpQuery, &CookiesLength, NULL);
		ReturnCookies.append(pTmpQuery);
		InternetCloseHandle(hInternetopen);
		InternetCloseHandle(hInternetConnect);
		InternetCloseHandle(hInternetRequest);
		delete[]pResultData;
		delete[]pTmpQuery;
		bool Resultbool;
		ResultData.length() > 0 ? Resultbool = true : Resultbool = false;
		return Resultbool;
	}
	bool _fastcall http::POST(std::wstring url, std::string &ResultData, std::wstring RequestData, std::wstring Header, std::wstring Cookies, std::wstring ReturnCookies)
	{
		bool IsHttps;
		url.find(L"https") == std::string::npos ? IsHttps = false : IsHttps = true;
#pragma region 协议头处理
		if (Header.find(L"User-Agent:") == std::string::npos)Header.append(L"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299\r\n");
		if (Header.find(L"Accept:") == std::string::npos)Header.append(L"Accept: */*\r\n");
		if (Header.find(L"Accept-Language:") == std::string::npos)Header.append(L"Accept-Language: zh-cn\r\n");
		if (Header.find(L"Content-Type:") == std::string::npos)Header.append(L"Content-Type: application/x-www-form-urlencoded\r\n");
		if (Header.find(L"Host:") == std::string::npos) {
			Header.append(L"Host: ");
			Header.append(GetHost(url));
			Header.append(L"\r\n");
		}
		if (Header.find(L"Referer:") == std::string::npos) {
			Header.append(L"Referer: ");
			Header.append(url);
			Header.append(L"\r\n");
		}
		if (!Cookies.empty()) {
			if (Header.find(L"Cookies:") == std::string::npos) {
				Header.append(L"Cookies: ");
				Header.append(Cookies);
				Header.append(L"\r\n");
			}
			else {
				Header.append(Cookies);
				Header.append(L"\r\n");
			}
		}
#pragma endregion
		//打开HTTP句柄;
		HINTERNET hInternetopen = InternetOpenW(Header.c_str(), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectW(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		DWORD RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		if (Cookies.empty())RequestFlg = RequestFlg | INTERNET_FLAG_NO_COOKIES;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestW(hInternetConnect, L"POST", GetURLPage(url).c_str(), L"HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		_bstr_t tmpPost = RequestData.c_str();
		std::string PostData = static_cast<std::string>(tmpPost);
		if (!HttpSendRequestW(hInternetRequest, Header.c_str(), Header.length(), (LPVOID)(PostData.c_str()), PostData.size()))return false;
		//new的异常处理
		char *pResultData = nullptr;
		try { pResultData = new char[1025]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		UINT  ResultLen = 0;
		do
		{
			ZeroMemory(pResultData, 1025);
			InternetReadFile(hInternetRequest, pResultData, 1024, reinterpret_cast<LPDWORD>(&ResultLen));
			ResultData.append(reinterpret_cast<char*>(pResultData));
		} while (ResultLen);
		wchar_t *pTmpQuery = nullptr;
		try { pTmpQuery = new wchar_t[4096]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		ZeroMemory(pTmpQuery, 4096 * sizeof(wchar_t));
		DWORD CookiesLength = 4095;
		HttpQueryInfoW(hInternetRequest, HTTP_QUERY_SET_COOKIE, pTmpQuery, &CookiesLength, NULL);
		ReturnCookies.append(pTmpQuery);
		InternetCloseHandle(hInternetopen);
		InternetCloseHandle(hInternetConnect);
		InternetCloseHandle(hInternetRequest);
		delete[]pResultData;
		delete[]pTmpQuery;
		bool Resultbool;
		ResultData.length() > 0 ? Resultbool = true : Resultbool = false;
		return Resultbool;
	}
	bool _fastcall http::GET(std::string url, std::string &ResultData, std::string Header, std::string Cookies, std::string ReturnCookies)
	{
		bool IsHttps;
		url.find("https") == std::string::npos ? IsHttps = false : IsHttps = true;
#pragma region 协议头处理
		if (Header.find("User-Agent:") == std::string::npos)Header.append("User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299\r\n");
		if (Header.find("Accept:") == std::string::npos)Header.append("Accept: */*\r\n");
		if (Header.find("Accept-Language:") == std::string::npos)Header.append("Accept-Language: zh-cn\r\n");
		if (Header.find("Content-Type:") == std::string::npos)Header.append("Content-Type: application/x-www-form-urlencoded\r\n");
		if (Header.find("Host:") == std::string::npos) {
			Header.append("Host: ");
			Header.append(GetHost(url));
			Header.append("\r\n");
		}
		if (Header.find("Referer:") == std::string::npos) {
			Header.append("Referer: ");
			Header.append(url);
			Header.append("\r\n");
		}
		if (!Cookies.empty()) {
			if (Header.find("Cookies:") == std::string::npos) {
				Header.append("Cookies: ");
				Header.append(Cookies);
				Header.append("\r\n");
			}
			else {
				Header.append(Cookies);
				Header.append("\r\n");
			}
		}
#pragma endregion
		//打开HTTP句柄;
		HINTERNET hInternetopen = InternetOpenA(Header.c_str(), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectA(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		DWORD RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		if (Cookies.empty())RequestFlg = RequestFlg | INTERNET_FLAG_NO_COOKIES;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestA(hInternetConnect, "GET", GetURLPage(url).c_str(), "HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		if (!HttpSendRequestA(hInternetRequest, Header.c_str(), Header.length(), nullptr, NULL))return false;
		//new的异常处理
		char *pResultData = nullptr;
		try { pResultData = new char[1025]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		UINT  ResultLen = 0;
		do
		{
			ZeroMemory(pResultData, 1025);
			InternetReadFile(hInternetRequest, pResultData, 1024, reinterpret_cast<LPDWORD>(&ResultLen));
			ResultData.append(reinterpret_cast<char*>(pResultData), ResultLen);
		} while (ResultLen);
		char *pTmpQuery = nullptr;
		try { pTmpQuery = new char[4096]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		ZeroMemory(pTmpQuery, 4096 * sizeof(char));
		DWORD CookiesLength = 4095;
		HttpQueryInfoA(hInternetRequest, HTTP_QUERY_SET_COOKIE, pTmpQuery, &CookiesLength, NULL);
		ReturnCookies.append(pTmpQuery);
		InternetCloseHandle(hInternetopen);
		InternetCloseHandle(hInternetConnect);
		InternetCloseHandle(hInternetRequest);
		delete[]pResultData;
		delete[]pTmpQuery;
		bool Resultbool;
		ResultData.length() > 0 ? Resultbool = true : Resultbool = false;
		return Resultbool;
	}
	bool _fastcall http::GET(std::wstring url, std::string &ResultData, std::wstring Header, std::wstring Cookies, std::wstring ReturnCookies)
	{
		bool IsHttps;
		url.find(L"https") == std::string::npos ? IsHttps = false : IsHttps = true;
#pragma region 协议头处理
		if (Header.find(L"User-Agent:") == std::string::npos)Header.append(L"User-Agent: Mozilla/5.0 (compatible; WOW64; MSIE 10.0; Windows NT 6.2)\r\n");
		//if (Header.find(L"Accept:") == std::string::npos)Header.append(L"Accept: */*\r\n");
		if (Header.find(L"Accept-Language:") == std::string::npos)Header.append(L"Accept-Language: zh-cn\r\n");
		if (Header.find(L"Content-Type:") == std::string::npos)Header.append(L"Content-Type: application/x-www-form-urlencoded\r\n");
		if (Header.find(L"Host:") == std::string::npos) {
			Header.append(L"Host: ");
			Header.append(GetHost(url));
			Header.append(L"\r\n");
		}
		if (Header.find(L"Referer:") == std::string::npos) {
			Header.append(L"Referer: ");
			Header.append(url);
			Header.append(L"\r\n");
		}
		if (!Cookies.empty()) {
			if (Header.find(L"Cookies:") == std::string::npos) {
				Header.append(L"Cookies: ");
				Header.append(Cookies);
				Header.append(L"\r\n");
			}
			else {
				Header.append(Cookies);
				Header.append(L"\r\n");
			}
		}
#pragma endregion
		//打开HTTP句柄;
		HINTERNET hInternetopen = InternetOpenW(Header.c_str(), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectW(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		DWORD RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		if (Cookies.empty())RequestFlg = RequestFlg | INTERNET_FLAG_NO_COOKIES;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestW(hInternetConnect, L"GET", GetURLPage(url).c_str(), L"HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		if (!HttpSendRequestW(hInternetRequest, Header.c_str(), Header.size(), nullptr, NULL))return false;
		//new的异常处理
		char *pResultData = nullptr;
		try { pResultData = new char[1027]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		UINT  ResultLen = 0;
		do
		{
			ZeroMemory(pResultData, 1027);
			InternetReadFile(hInternetRequest, pResultData, 1024, reinterpret_cast<LPDWORD>(&ResultLen));
			ResultData.append(reinterpret_cast<char*>(pResultData), ResultLen);
		} while (ResultLen);
		wchar_t *pTmpQuery = nullptr;
		try { pTmpQuery = new wchar_t[4096]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		ZeroMemory(pTmpQuery, 4096 * sizeof(wchar_t));
		DWORD CookiesLength = 4095;
		HttpQueryInfoW(hInternetRequest, HTTP_QUERY_SET_COOKIE, pTmpQuery, &CookiesLength, NULL);
		ReturnCookies.append(pTmpQuery);
		InternetCloseHandle(hInternetopen);
		InternetCloseHandle(hInternetConnect);
		InternetCloseHandle(hInternetRequest);
		delete[]pResultData;
		delete[]pTmpQuery;
		bool Resultbool;
		ResultData.length() > 0 ? Resultbool = true : Resultbool = false;
		return Resultbool;
	}
	std::string _fastcall http::GetLocalCookies(std::string host)
	{
		char *TmpCookies = nullptr;
		DWORD CookiesLen = 4097;
		try { TmpCookies = new char[CookiesLen]; }
		catch (...) { return ""; }
		ZeroMemory(TmpCookies, 4097);
		if (!InternetGetCookieA(host.c_str(), nullptr, TmpCookies, &CookiesLen))return "";
		std::string Cookies = TmpCookies;
		delete[]TmpCookies;
		return Cookies;
	}
	std::wstring _fastcall http::GetLocalCookies(std::wstring host)
	{
		char *TmpCookies = nullptr;
		DWORD CookiesLen = 4097;
		try { TmpCookies = new char[CookiesLen]; }
		catch (...) { return L""; }
		ZeroMemory(TmpCookies, 4097);
		if (!InternetGetCookieW(host.c_str(), NULL, reinterpret_cast<LPWSTR>(TmpCookies), &CookiesLen))return L"";
		std::wstring Cookies(reinterpret_cast<LPWSTR>(TmpCookies));
		delete[]TmpCookies;
		return Cookies;
	}
	VARIANT _fastcall http::ExecuteJScript(std::string JScript, std::string FuncetionName, std::string ErrorLog)
	{
		IScriptControlPtr m_pScriptControlPtr;
		m_pScriptControlPtr.CreateInstance(__uuidof(ScriptControl));
		std::stringstream loger;
		VARIANT Result = { 0 };
		try {
			m_pScriptControlPtr->Language = "JScript";
			m_pScriptControlPtr->ExecuteStatement(JScript.c_str());
		}
		catch (_com_error &e) {
			MSScriptControl::IScriptErrorPtr Error = m_pScriptControlPtr->Error;
			long ErrorLine, ErrorCode;
			BSTR ErrorTip;
			Error->get_Number(&ErrorCode);
			Error->get_Line(&ErrorLine);
			Error->get_Description(&ErrorTip);
			loger << "JScript加载错误!\r\n";
			loger << "错误代码: [" << ErrorCode << "]\r\n";
			loger << "出错位置: [" << ErrorLine << "]行\r\n";
			loger << "提示信息: " << e.Description() << "\r\n";
			ErrorLog = loger.str();
			loger.clear();
			loger.str("");
			return Result;
		}
		try {
			Result = m_pScriptControlPtr->Eval(FuncetionName.c_str());
			m_pScriptControlPtr->Reset();
		}
		catch (_com_error &e) {
			MSScriptControl::IScriptErrorPtr Error = m_pScriptControlPtr->Error;
			long ErrorLine, ErrorCode;
			BSTR ErrorTip;
			Error->get_Number(&ErrorCode);
			Error->get_Line(&ErrorLine);
			Error->get_Description(&ErrorTip);
			loger << "JScript运行错误!\r\n";
			loger << "错误代码: [" << ErrorCode << "]\r\n";
			loger << "出错位置: [" << ErrorLine << "]行\r\n";
			loger << "提示信息: " << e.Description() << "\r\n";
			ErrorLog = loger.str();
			loger.clear();
			loger.str("");
			return Result;
		}
		loger.clear();
		loger.str("");
		m_pScriptControlPtr.Release();
		return Result;
	}
	VARIANT _fastcall http::ExecuteJScript(std::wstring JScript, std::wstring FuncetionName, std::wstring ErrorLog)
	{
		IScriptControlPtr m_pScriptControlPtr;
		m_pScriptControlPtr.CreateInstance(__uuidof(ScriptControl));
		std::wstringstream loger;
		VARIANT Result = { 0 };
		try {
			m_pScriptControlPtr->Language = "JScript";
			m_pScriptControlPtr->ExecuteStatement(JScript.c_str());
		}
		catch (_com_error &e) {
			MSScriptControl::IScriptErrorPtr Error = m_pScriptControlPtr->Error;
			long ErrorLine, ErrorCode;
			BSTR ErrorTip;
			Error->get_Number(&ErrorCode);
			Error->get_Line(&ErrorLine);
			Error->get_Description(&ErrorTip);
			loger << L"JScript加载错误!\r\n";
			loger << L"错误代码: [" << ErrorCode << L"]\r\n";
			loger << L"出错位置: [" << ErrorLine << L"]行\r\n";
			loger << L"提示信息: " << e.Description() << L"\r\n";
			ErrorLog = loger.str();
			loger.clear();
			loger.str(L"");
			return Result;
		}
		try {
			Result = m_pScriptControlPtr->Eval(FuncetionName.c_str());
			m_pScriptControlPtr->Reset();
		}
		catch (_com_error &e) {
			MSScriptControl::IScriptErrorPtr Error = m_pScriptControlPtr->Error;
			long ErrorLine, ErrorCode;
			BSTR ErrorTip;
			Error->get_Number(&ErrorCode);
			Error->get_Line(&ErrorLine);
			Error->get_Description(&ErrorTip);
			loger << L"JScript运行错误!\r\n";
			loger << L"错误代码: [" << ErrorCode << L"]\r\n";
			loger << L"出错位置: [" << ErrorLine << L"]行\r\n";
			loger << L"提示信息: " << e.Description() << L"\r\n";
			ErrorLog = loger.str();
			loger.clear();
			loger.str(L"");
			return Result;
		}
		loger.clear();
		loger.str(L"");
		m_pScriptControlPtr->Release();
		return Result;
	}
	bool _fastcall http::RegEx_FindString(std::string Regex, std::string DestString, bool IsCase /* = false */, bool IsMultiRows /* = true */, bool IsGlobal /* = true */)
	{
		return true;
	}
	bool _fastcall http::DownloadFile(std::string url, std::string FilePath, DownloadProc DownloadPr /* = nullptr */)
	{
		if (url.empty())return false;
		if (FilePath.empty())return false;
		bool IsHttps;
		url.find("https") == std::string::npos ? IsHttps = false : IsHttps = true;
		std::stringstream Header;
#pragma region 协议头处理
		Header << "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299\r\n";
		Header << "Accept: */*\r\n";
		Header << "Accept-Language: zh-cn\r\n";
		Header << "Content-Type: application/x-www-form-urlencoded\r\n";
		Header << "Host:" << GetHost(url) << "\r\n";
		Header << "Referer:" << url << "\r\n";
#pragma endregion
		//打开HTTP句柄;
		HINTERNET hInternetopen = InternetOpenA(Header.str().c_str(), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectA(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		DWORD RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestA(hInternetConnect, "GET", GetURLPage(url).c_str(), "HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		if (!HttpSendRequestA(hInternetRequest, Header.str().c_str(), Header.str().length(), nullptr, NULL))return false;
		//new的异常处理
		char *pResultData = nullptr;
		try { pResultData = new char[1025]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		UINT  ResultLen = 0;
		DWORD DownloadTickSize = 0, SecondTick = 0, DownloadSize = 0;
		time_t  ticksecond = time(NULL);
		std::ofstream file;
		file.open(FilePath, ios::out | ios::binary);
		char Tmpbuf[55] = { 0 };
		DWORD buflen = sizeof(Tmpbuf);
		HttpQueryInfo(hInternetRequest, HTTP_QUERY_CONTENT_LENGTH, Tmpbuf, &buflen, NULL);
		DWORD dwFileSize = atol(Tmpbuf);
		do
		{
			ZeroMemory(pResultData, 1025);
			InternetReadFile(hInternetRequest, pResultData, 1024, reinterpret_cast<LPDWORD>(&ResultLen));
			file.write(pResultData, ResultLen);
			DownloadTickSize += ResultLen;
			DownloadSize += ResultLen;
			if (time(NULL) - ticksecond > 1) {
				SecondTick = DownloadTickSize;
				DownloadTickSize = 0; 
				ticksecond = time(NULL);
			}
			if (DownloadPr) {
				if (!DownloadPr(dwFileSize, 1, DownloadSize, SecondTick))break;
			}
		} while (ResultLen);
		InternetCloseHandle(hInternetopen);
		InternetCloseHandle(hInternetConnect);
		InternetCloseHandle(hInternetRequest);
		delete[]pResultData;
		file.close();
		Header.clear();
		Header.str("");
		return true;
	}
	bool _fastcall http::DownloadFile(std::wstring url, std::wstring FilePath, DownloadProc DownloadPr /* = nullptr */)
	{
		if (url.empty())return false;
		if (FilePath.empty())return false;
		bool IsHttps;
		url.find(L"https") == std::wstring::npos ? IsHttps = false : IsHttps = true;
		std::wstring Header;
#pragma region 协议头处理
		Header = L"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299\r\n";
		Header += L"Accept: */*\r\n";
		Header += L"Accept-Language: zh-cn\r\n";
		Header += L"Content-Type: application/x-www-form-urlencoded\r\n";
		Header += L"Host:" + GetHost(url) + L"\r\n";
		Header += L"Referer:" + url + L"\r\n";
#pragma endregion
		//打开HTTP句柄;
		HINTERNET hInternetopen = InternetOpenW(Header.c_str(), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
#pragma region 获取端口
		{
			bool _Ishttps;
			url.find(L"https://") == std::wstring::npos ? _Ishttps = false : _Ishttps = true;
			size_t strpos, coutof;
			_Ishttps ? strpos = url.find(L"https://") + wcslen(L"https://") : strpos = url.find(L"http://") + wcslen(L"http://");
			url.find('/', strpos) == std::wstring::npos ? coutof = url.find('/', strpos) : coutof = url.find('/', strpos);
			std::wstring iprot;
			if (_Ishttps)iprot = url.substr(strpos, coutof - wcslen(L"https://"));
			else iprot = url.substr(strpos, coutof - wcslen(L"http://"));
			strpos = iprot.find(':') + 1;
			if (strpos != std::wstring::npos)iprot = iprot.substr(strpos, wcslen(iprot.c_str()));
			if (iprot.length() > 0)InterPort = _wtoi(iprot.c_str());
		}
#pragma endregion
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectW(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		DWORD RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestW(hInternetConnect, L"GET", GetURLPage(url).c_str(), L"HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		char Tmpbuf[55] = { 0 };
		DWORD buflen = sizeof(Tmpbuf);
		HttpQueryInfo(hInternetRequest, HTTP_QUERY_CONTENT_RANGE, Tmpbuf, &buflen, NULL);
		DWORD dwFileSize = atol(Tmpbuf);
		if (!HttpSendRequestW(hInternetRequest, Header.c_str(), Header.size(), nullptr, NULL))return false;
		//new的异常处理
		char *pResultData = nullptr;
		try { pResultData = new char[4096]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		UINT  ResultLen = 0;
		DWORD DownloadTickSize = 0, SecondTick = 0, DownloadSize = 0;
		time_t  ticksecond = time(NULL);
		std::ofstream file;
		file.open(FilePath, ios::out | ios::binary);
		bool IsFilef = false;
		do
		{
			ZeroMemory(pResultData, 4096);
			InternetReadFile(hInternetRequest, pResultData, 4095, reinterpret_cast<LPDWORD>(&ResultLen));
			if (!IsFilef) {
				if (strstr(pResultData, "找不到文件或目录")) {
					if (DownloadPr)DownloadPr(dwFileSize, 0, DownloadSize, SecondTick);
					InternetCloseHandle(hInternetopen);
					InternetCloseHandle(hInternetConnect);
					InternetCloseHandle(hInternetRequest);
					delete[]pResultData;
					file.close();
					Header.clear();
					return false;
				}
			}
			else IsFilef = true;
			file.write(pResultData, ResultLen);
			DownloadTickSize += ResultLen;
			DownloadSize += ResultLen;
			if (time(NULL) - ticksecond > 1) {
				SecondTick = DownloadTickSize;
				DownloadTickSize = 0;
				ticksecond = time(NULL);
			}
			if (DownloadPr) {
				if (!DownloadPr(dwFileSize, 1, DownloadSize, SecondTick))break;
			}
		} while (ResultLen);
		if (DownloadPr)DownloadPr(dwFileSize, 2, DownloadSize, SecondTick);
		InternetCloseHandle(hInternetopen);
		InternetCloseHandle(hInternetConnect);
		InternetCloseHandle(hInternetRequest);
		delete[]pResultData;
		file.close();
		Header.clear();
		return true;
	}
	
}

