#include "lib_http.h"

namespace lib_http
{
	CLibhttp::CLibhttp()
	{
		
	}

	CLibhttp::~CLibhttp()
	{
		
	}
	std::string _fastcall CLibhttp::GetHost(std::string url)
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
	std::wstring _fastcall CLibhttp::GetHost(std::wstring url)
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
	std::string _fastcall CLibhttp::GetURLPage(std::string url)
	{
		bool Ishttps;
		url.find("https://") == std::wstring::npos ? Ishttps = false : Ishttps = true;
		size_t strpos, coutof;
		Ishttps ? strpos = url.find("https://") + strlen("https://") : strpos = url.find("http://") + strlen("http://");
		coutof = url.find('/', strpos);
		if (coutof == std::wstring::npos)return "/";
		std::string urlpage = url.substr(coutof, strpos - coutof);
		return urlpage;
	}
	std::wstring _fastcall CLibhttp::GetURLPage(std::wstring url)
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
	bool _fastcall CLibhttp::POST(std::string url, std::string &ResultData, std::string RequestData, std::string Header, std::string Cookies, std::string ReturnCookies)
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
			if (Header.find("Cookie:") == std::string::npos) {
				Header.append("Cookie: ");
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
		HINTERNET hInternetopen = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64)", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectA(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		int RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		if (!Cookies.empty())RequestFlg = RequestFlg | INTERNET_FLAG_NO_COOKIES;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestA(hInternetConnect, "POST", GetURLPage(url).c_str(), "HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		if (!HttpSendRequestA(hInternetRequest, Header.c_str(), Header.size(), (LPVOID)(RequestData.c_str()), RequestData.size()))return false;
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
	bool _fastcall CLibhttp::POST(std::wstring url, std::string &ResultData, std::wstring RequestData, std::wstring Header, std::wstring Cookies, std::wstring ReturnCookies)
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
			if (Header.find(L"Cookie:") == std::string::npos) {
				Header.append(L"Cookie: ");
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
		HINTERNET hInternetopen = InternetOpenW(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64)", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectW(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		int RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		if (!Cookies.empty())RequestFlg = RequestFlg | INTERNET_FLAG_NO_COOKIES;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestW(hInternetConnect, L"POST", GetURLPage(url).c_str(), L"HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		_bstr_t tmpPost = RequestData.c_str();
		std::string PostData = static_cast<std::string>(tmpPost);
		if (!HttpSendRequestW(hInternetRequest, Header.c_str(), Header.size(), (LPVOID)(PostData.c_str()), PostData.size()))return false;
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
	bool _fastcall CLibhttp::GET(std::string url, std::string &ResultData, std::string Header, std::string Cookies, std::string ReturnCookies)
	{
		bool IsHttps;
		url.find("https") == std::string::npos ? IsHttps = false : IsHttps = true;
#pragma region 协议头处理
		//if (Header.find("User-Agent:") == std::string::npos)Header.append("User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299\r\n");
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
			if (Header.find("Cookie:") == std::string::npos) {
				Header.append("Cookie: ");
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
		HINTERNET hInternetopen = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64)", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectA(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		int RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		if (!Cookies.empty())RequestFlg |= INTERNET_FLAG_NO_COOKIES;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestA(hInternetConnect, "GET", GetURLPage(url).c_str(), "HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		if (!HttpSendRequestA(hInternetRequest, Header.c_str(), Header.size(), NULL, NULL))return false;
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
		HttpQueryInfoA(hInternetRequest, HTTP_QUERY_RAW_HEADERS_CRLF, pTmpQuery, &CookiesLength, NULL);
		std::string tmpstr = GetCookies(pTmpQuery);
		ReturnCookies.append(tmpstr);
		InternetCloseHandle(hInternetopen);
		InternetCloseHandle(hInternetConnect);
		InternetCloseHandle(hInternetRequest);
		delete[]pResultData;
		delete[]pTmpQuery;
		bool Resultbool;
		ResultData.length() > 0 ? Resultbool = true : Resultbool = false;
		return Resultbool;
	}
	std::string __fastcall CLibhttp::GetCookies(const char* httpHeader)
	{
		if (strlen(httpHeader) < 10)return "";
		int nPos = 0, nPos2 = 0;
		std::string ResultCookies = "", event_htmldata = httpHeader;
		for (;;) {
			nPos = event_htmldata.find("Set-Cookie: ", nPos);
			if (nPos == std::string::npos)break;
			nPos += strlen("Set-Cookie: ");
			nPos2 = event_htmldata.find(";", nPos) + strlen(";");
			ResultCookies += event_htmldata.substr(nPos, nPos2 - nPos);
		}
		return ResultCookies;
	}
	bool _fastcall CLibhttp::GET(bool forbidjump, const char* url, std::string& ResultData, std::string Header, std::string Cookies, std::string &ReturnCookies)
	{
		bool IsHttps;
		strstr(url, "https") ? IsHttps = true : IsHttps = false;
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
			if (Header.find("Cookie:") == std::string::npos) {
				Header.append("Cookie: ");
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
		int RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		if (!Cookies.empty())RequestFlg = RequestFlg | INTERNET_FLAG_NO_COOKIES;
		if (forbidjump)RequestFlg = RequestFlg | INTERNET_FLAG_NO_AUTO_REDIRECT;
		IsHttps ? RequestFlg |= INTERNET_FLAG_SECURE : RequestFlg |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS;
		HINTERNET hInternetRequest = HttpOpenRequestA(hInternetConnect, "GET", GetURLPage(url).c_str(), "HTTP/1.1", NULL, NULL, RequestFlg, NULL);
		if (!hInternetRequest) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); return false; }
		if (!HttpSendRequestA(hInternetRequest, Header.c_str(), Header.size(), nullptr, NULL))return false;
		//new的异常处理
		char* pResultData = nullptr;
		try { pResultData = new char[1025]; }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		UINT  ResultLen = 0;
		do
		{
			ZeroMemory(pResultData, 1025);
			InternetReadFile(hInternetRequest, pResultData, 1024, reinterpret_cast<LPDWORD>(&ResultLen));
			ResultData.append(reinterpret_cast<char*>(pResultData), ResultLen);
		} while (ResultLen);
		char* pTmpQuery = nullptr;
		try { pTmpQuery = new char[6056](); }
		catch (...) { InternetCloseHandle(hInternetConnect); InternetCloseHandle(hInternetopen); InternetCloseHandle(hInternetRequest); return false; }
		DWORD CookiesLength = 6055;
		HttpQueryInfoA(hInternetRequest, HTTP_QUERY_RAW_HEADERS_CRLF, pTmpQuery, &CookiesLength, NULL);
		std::string tmpstr = GetCookies(pTmpQuery);
		ReturnCookies.append(tmpstr);
		InternetCloseHandle(hInternetopen);
		InternetCloseHandle(hInternetConnect);
		InternetCloseHandle(hInternetRequest);
		delete[]pResultData;
		delete[]pTmpQuery;
		bool Resultbool;
		ResultData.length() > 0 ? Resultbool = true : Resultbool = false;
		return Resultbool;
	}
	bool _fastcall CLibhttp::GET(std::wstring url, std::string &ResultData, std::wstring Header, std::wstring Cookies, std::wstring ReturnCookies)
	{
		bool IsHttps;
		url.find(L"https") == std::string::npos ? IsHttps = false : IsHttps = true;
#pragma region 协议头处理
		if (Header.find(L"User-Agent:") == std::string::npos)Header.append(L"User-Agent: Mozilla/5.0 (compatible; WOW64; MSIE 10.0; Windows NT 6.2)\r\n");
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
			if (Header.find(L"Cookie:") == std::string::npos) {
				Header.append(L"Cookie: ");
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
		HINTERNET hInternetopen = InternetOpenW(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64)", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
		if (!hInternetopen)return false;
		//访问端口;
		INTERNET_PORT InterPort;
		IsHttps ? InterPort = INTERNET_DEFAULT_HTTPS_PORT : InterPort = INTERNET_DEFAULT_HTTP_PORT;
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectW(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		int RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
		if (!Cookies.empty())RequestFlg = RequestFlg | INTERNET_FLAG_NO_COOKIES;
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
	std::string _fastcall CLibhttp::GetLocalCookies(std::string host)
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
	std::wstring _fastcall CLibhttp::GetLocalCookies(std::wstring host)
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
	VARIANT _fastcall CLibhttp::ExecuteJScript(std::string JScript, std::string FuncetionName, std::string ErrorLog)
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
	VARIANT _fastcall CLibhttp::ExecuteJScript(std::wstring JScript, std::wstring FuncetionName, std::wstring ErrorLog)
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
	bool _fastcall CLibhttp::RegEx_FindString(std::string Regex, std::string DestString, bool IsCase /* = false */, bool IsMultiRows /* = true */, bool IsGlobal /* = true */)
	{
		return true;
	}
	bool _fastcall CLibhttp::DownloadFile(std::string url, std::string FilePath, DownloadProc DownloadPr /* = nullptr */)
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
		int RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
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
	bool _fastcall CLibhttp::DownloadFile(std::wstring url, std::wstring FilePath, DownloadProc DownloadPr /* = nullptr */)
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
			strpos = iprot.find(':');
			if (strpos != std::wstring::npos)
			{
				iprot = iprot.substr(strpos + 1, wcslen(iprot.c_str()));
				if (iprot.length() > 0)InterPort = _wtoi(iprot.c_str());
			}
		}
#pragma endregion
		//创建Internet连接;
		HINTERNET hInternetConnect = InternetConnectW(hInternetopen, GetHost(url).c_str(), InterPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
		if (!hInternetConnect) { InternetCloseHandle(hInternetopen); return false; }
		int RequestFlg = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY;
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
		/*char Tmpbuf2[2055] = { 0 };
		DWORD buflen2 = sizeof(Tmpbuf2);
		HttpQueryInfoA(hInternetRequest, HTTP_QUERY_RAW_HEADERS_CRLF, Tmpbuf2, &buflen2, NULL);*/
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
	
#pragma region http2
	bool IsInitalize = false;
	WSADATA httpwsadata = { 0 };
	bool __fastcall CLibhttp2::Initalize()
	{
		if (!IsInitalize)WSAStartup(MAKEWORD(2, 2), &httpwsadata) == 0 ? IsInitalize = true : IsInitalize = false;
		return IsInitalize;
	}
	void __fastcall CLibhttp2::UnInitalize()
	{
		WSACleanup();
		IsInitalize = false;
	}
	std::string __fastcall CLibhttp2::GetHost(std::string url)
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
	std::wstring __fastcall CLibhttp2::GetHost(std::wstring url)
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
	std::string __fastcall CLibhttp2::GetURLPage(std::string url)
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
	std::wstring __fastcall CLibhttp2::GetURLPage(std::wstring url)
	{
		bool Ishttps;
		url.find(L"https://") == std::wstring::npos ? Ishttps = false : Ishttps = true;
		size_t strpos, coutof;
		Ishttps ? strpos = url.find(L"https://") + wcslen(L"https://") : strpos = url.find(L"http://") + wcslen(L"http://");
		coutof = url.find('/', strpos);
		if (coutof == std::wstring::npos)return L"";
		std::wstring urlpage = url.substr(coutof, strpos - coutof);
		return urlpage;
	}
	UINT __fastcall CLibhttp2::GetProt(TCHAR* url)
	{
		UINT iProt = 0;
		TCHAR* pProt = _tcsstr(url + 7, _T(":"));
		if (pProt) {
			pProt++;
			TCHAR* pProtEnd = _tcsstr(pProt, _T("/"));
			if (!pProtEnd)return iProt;
			pProtEnd[0] = 0;
			iProt = _tstoi(pProtEnd);
		}
		return iProt;
	}
	CLibString __fastcall CLibhttp2::GethtmlData(std::string& ResultData)
	{
		UINT nPos = ResultData.find("\r\n\r\n") + strlen("\r\n\r\n");
		if (nPos == std::string::npos)return "";
		m_htmldata = ResultData.substr(nPos, ResultData.length() - nPos).c_str();
		m_htmlHeader = ResultData.substr(0, nPos).c_str();
		return m_htmldata;
	}
	CLibString __fastcall CLibhttp2::GetCookies()
	{
		if (strlen(m_htmlHeader) < 10)return _T("");
		UINT nPos = 0;
		CLibString pCookies;
		CLibString ReusltCookies;
		do
		{
			nPos = m_htmlHeader.Find("Set-Cookie: ", nPos) + strlen("\r\n");
			pCookies = m_htmlHeader.Mid("Set-Cookie: ", "\r\n", nPos);
			ReusltCookies += pCookies;
		} while (_tcslen(pCookies) > 1);;
		return ReusltCookies;
	}
	bool __fastcall CLibhttp2::Get(const TCHAR* Url, std::string& ResultData, const TCHAR* Header /* = nullptr */, const TCHAR* Cookies /* = nullptr */)
	{
		bool Ishttps;
		_tcsstr(Url, _T("https")) ? Ishttps = true : Ishttps = false;
		lib_String::CLibString cstr_url = Url;
		UINT iport = GetProt(cstr_url);
		if (iport < 1) {
			_tcsstr(Url, _T("https://")) ? iport = 443 : iport = 80;
		}
		SOCKADDR_IN Addr = { 0 };
		Addr.sin_family = AF_INET;
		Addr.sin_port = htons(iport);
		CLibString ServAddr = GetHost(Url).c_str();
		hostent* pHost = gethostbyname(ServAddr);
		Addr.sin_addr = *(in_addr*)pHost->h_addr;
		SOCKET Socket = socket(AF_INET, SOCK_STREAM, NULL);
		if (Socket == INVALID_SOCKET)return false;
		if (connect(Socket, (sockaddr*)&Addr, sizeof(Addr)) == INVALID_SOCKET) {
			closesocket(Socket);
			return false;
		}
		/*
		GET / HTTP/1.1
		Host: bilibili.com
		Connection: keep-alive
		Upgrade-Insecure-Requests: 1
		User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Safari/537.36
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,; q = 0.8, application / signed - exchange; v = b3
		Accept - Encoding: gzip, deflate
		Accept - Language : zh - CN, zh; q = 0.9
		Cookie: CURRENT_FNVAL = 16;llJR)m;
		*/
#pragma region Http请求协议头
		CLibString RequestData = _T("GET ");
		RequestData += GetURLPage(Url).c_str();
		RequestData += " HTTP/1.1\r\n";

		if (Header) {
			RequestData += Header;
			RequestData += _T("\r\n");
		}
		else {
			RequestData += _T("User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64)\r\n");
			RequestData += _T("Connection: keep-alive\r\n");
			RequestData += _T("Accept: text/html,*/*\r\n");
			RequestData += _T("Accept-Language: zh-CN,zh;\r\n");
			RequestData += _T("Content-Type: application/x-www-form-urlencoded\r\n");

			RequestData += _T("Host: ");
			RequestData += GetHost(Url).c_str();
			RequestData += _T("\r\n");

			RequestData += _T("Referer: ");
			RequestData += Url;
			RequestData += _T("\r\n");
		}
		if (Cookies) {
			RequestData += _T("Cookie: ");
			RequestData += Cookies;
			RequestData += _T("\r\n");
		}
		RequestData += _T("\r\n");
#pragma endregion
#pragma region http数据发送
		if (!Ishttps) {
			if (send(Socket, RequestData, RequestData.Size(), NULL) == SOCKET_ERROR) {
				closesocket(Socket);
				return false;
			}
			char* pBuf = new char[4096]();
			int ret = 0;
			fd_set rfds;
			do
			{
				FD_ZERO(&rfds);
				FD_SET(Socket, &rfds);
				ZeroMemory(pBuf, 4096);
				ret = 0;
				timeval t = { 1,0 };
				int nRetAll = select(0, &rfds, NULL, NULL, &t);
				if (nRetAll > 0) {
					if (!FD_ISSET(Socket, &rfds))continue;
					else {
						ret = recv(Socket, pBuf, 4095, NULL);
					}
				}
				ResultData.append(pBuf);
			} while (ret > 0);
			closesocket(Socket);
			ResultData = GethtmlData(ResultData);
			CLibString r = GetCookies();
			return true;
		}
		else {
			//https请求
			//添加SSL错误信息;
			SSL_load_error_strings();
			//添加SSL加密/HASH算法;
			SSLeay_add_ssl_algorithms();
			//客户端，服务端选择SSLv23_server_method() 
			const SSL_METHOD* meth = SSLv23_client_method();
			//建立新的SSL上下文 
			SSL_CTX* ctx = SSL_CTX_new(meth);
			if (ctx == NULL) { closesocket(Socket); return false; }
			int ret;
			SSL* ssl = SSL_new(ctx);
			if (ssl == NULL) { closesocket(Socket); return false; }
			//将SSL与TCP SOCKET 连接 
			SSL_set_fd(ssl, Socket);
			//SSL连接 
			ret = SSL_connect(ssl);
			if (ret == -1) { closesocket(Socket); return false; }
			ret = SSL_write(ssl, reinterpret_cast<void*>((char*)RequestData), RequestData.Size());
			if (ret == -1) { closesocket(Socket); return false; }
			char* pBuf = new char[4096]();
			fd_set rfds;
			do {
				FD_ZERO(&rfds);
				FD_SET(Socket, &rfds);
				ZeroMemory(pBuf, 4095);
				ret = 0;
				timeval t = { 1,0 };
				int nRetAll = select(0, &rfds, NULL, NULL, &t);
				if (nRetAll > 0) {
					if (!FD_ISSET(Socket, &rfds))continue;
					else {
						ret = SSL_read(ssl, pBuf, 4095);
					}
				}
				ResultData.append(pBuf);
			} while (ret);
			SSL_shutdown(ssl);
			SSL_free(ssl);
			SSL_CTX_free(ctx);
			closesocket(Socket);
			ResultData = GethtmlData(ResultData);
			return true;
		}
#pragma endregion
	}
#pragma endregion
}

