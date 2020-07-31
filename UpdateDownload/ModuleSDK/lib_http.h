#ifndef __lib_htpp_h
#define __lib_http_h
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <wininet.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <tchar.h>
#include <openssl/ssl.h>

#include "lib_String.h"

#import <msscript.ocx>
//#import <vbscript.dll>rename("RGB","rgb")

#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libssl.lib")
#pragma warning(disable:4996)

namespace lib_http
{
	using namespace std;
	using namespace MSScriptControl;
	using namespace lib_String;
	//下载回调函数,返回true继续下载,返回false停止下载;
	//@parameter 下载文件总大小
	//@parameter 当前下载数据大小
	//@parameter 每秒下载数据大小
	typedef bool(__stdcall *DownloadProc)(size_t FileSize, UINT nState, size_t DownloadSize, size_t SecondSize);
	class CLibhttp
	{
	public:
		CLibhttp();
		~CLibhttp();
		//POST请求
		//@parameter 请求地址
		//@parameter 返回数据
		//@parameter 请求数据
		//@parameter 请求协议头
		//@parameter 请求Cookies
		//@parameter 返回Cokies
		bool _fastcall POST(std::string url, std::string &ResultData, std::string RequestData = "", std::string Header = "", std::string Cookies = "", std::string ReturnCookies = "");
		//POST请求
		//@parameter 请求地址
		//@parameter 返回数据(返回string类型,转换交给函数外)
		//@parameter 请求数据
		//@parameter 请求协议头
		//@parameter 请求Cookies
		//@parameter 返回Cokies
		bool _fastcall POST(std::wstring url, std::string &ResultData, std::wstring RequestData = L"", std::wstring Header = L"", std::wstring Cookies = L"", std::wstring ReturnCookies = L"");
		//GET请求
		//@parameter 请求地址
		//@parameter 返回数据
		//@parameter 请求协议头
		//@parameter 请求Cookies
		//@parameter 返回Cokies
		bool _fastcall GET(std::string url, std::string &ResultData, std::string Header = "", std::string Cookies = "", std::string ReturnCookies = "");
		//GET请求
		//@parameter 请求地址
		//@parameter 返回数据(返回string类型,转换交给函数外)
		//@parameter 请求协议头
		//@parameter 请求Cookies
		//@parameter 返回Cokies
		bool _fastcall GET(std::wstring url, std::string &ResultData, std::wstring Header = L"", std::wstring Cookies = L"", std::wstring ReturnCookies = L"");
		//从协议头中获取Cookies
		std::string __fastcall GetCookies(const char* httpHeader);
		//GET禁止重定向
		bool _fastcall GET(bool forbidjump, const char* url, std::string& ResultData, std::string Header, std::string Cookies, std::string& ReturnCookies);
		//下载文件
		//@parameter 下载地址
		//@parameter 保存文件目录
		//@parameter 回调函数 DownloadProc(size_t FileSize, UINT nState, size_t DownloadSize, size_t SecondSize);
		bool _fastcall DownloadFile(std::string url, std::string FilePath, DownloadProc DownloadPr = nullptr);
		//下载文件
		//@parameter 下载地址
		//@parameter 保存文件目录
		//@parameter 回调函数 DownloadProc(size_t FileSize, UINT nState, size_t DownloadSize, size_t SecondSize);
		bool _fastcall DownloadFile(std::wstring url, std::wstring FilePath, DownloadProc DownloadPr = nullptr);
		//获取指定网址本地IE Cookies
		//@parameter 主机Url地址
		std::string _fastcall GetLocalCookies(std::string host);
		//获取指定网址本地IE Cookies
		//@parameter 主机Url地址
		std::wstring _fastcall GetLocalCookies(std::wstring host);
		//获取网页页面地址
		//@parameter url
		std::string _fastcall GetURLPage(std::string url);
		//获取网页页面地址
		//@parameter url
		std::wstring _fastcall GetURLPage(std::wstring url);
		//获取指定Url的Host地址
		//@parameter Url
		std::string _fastcall GetHost(std::string url);
		//获取指定Url的Host地址
		//@parameter Url
		std::wstring _fastcall GetHost(std::wstring url);
		//运行JScript
		//@parameter JScript代码
		//@parameter JScript函数名及参数func(a,b)
		VARIANT _fastcall ExecuteJScript(std::string JScript, std::string FuncetionName, std::string ErrorLog = "");
		//运行JScript
		//@parameter JScript代码
		//@parameter JScript函数名及参数func(a,b)
		VARIANT _fastcall ExecuteJScript(std::wstring JScript, std::wstring FuncetionName, std::wstring ErrorLog = L"");
		//正则表达式
		//@parameter 正则表达式
		//@parameter 搜索目标字符串
		//@parameter 是否区分大小写
		//@parameter 是否匹配多行
		//@parameter 是否全局匹配
		bool _fastcall RegEx_FindString(std::string Regex, std::string DestString, bool IsCase = false, bool IsMultiRows = true, bool IsGlobal = true);
		//正则表达式
		//@parameter 正则表达式
		//@parameter 搜索目标字符串
		//@parameter 是否区分大小写
		//@parameter 是否匹配多行
		//@parameter 是否全局匹配
		bool _fastcall RegEx_FindString(std::wstring Regex, std::wstring DestString, bool IsCase = false, bool IsMultiRows = true, bool IsGlobal = true);
	private:

	};
	//Http Socket方式请求;
	class CLibhttp2
	{
	public:
		CLibhttp2() {};
		~CLibhttp2() {};
		bool __fastcall Initalize();
		void __fastcall UnInitalize();
		std::string __fastcall GetHost(std::string url);
		std::wstring __fastcall GetHost(std::wstring url);
		std::string __fastcall GetURLPage(std::string url);
		std::wstring __fastcall GetURLPage(std::wstring url);
		UINT __fastcall GetProt(TCHAR* url);
		lib_String::CLibString __fastcall GetCookies();
		lib_String::CLibString __fastcall GethtmlData(std::string& ResultData);
		bool __fastcall Get(const TCHAR* Url, std::string& ResultData, const TCHAR* Header = nullptr, const TCHAR* Cookies = nullptr);
	private:
		lib_String::CLibString m_htmlHeader;
		lib_String::CLibString m_htmldata;

	};
}

#endif
