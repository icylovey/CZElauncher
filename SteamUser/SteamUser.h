#pragma once
#include <UIlib.h>
#include <atlimage.h>

#include "resource.h"
#include "SDK/Path.h"
#include "../ZElauncher/json/json.h"
#include "../ZElauncher/ModuleSDK/lib_http.h"
#include "../ZElauncher/ModuleSDK/lib_Process.h"


#import <progid:VBScript.RegExp>named_guids
#ifdef _DEBUG
#include "Debug/3.tlh"
#endif // _DEBUG


using namespace DuiLib;
class CSteamUserMain :
	public WindowImplBase
{
public:
	CSteamUserMain() {}
	~CSteamUserMain() { ::PostQuitMessage(0L); }
	LPCTSTR  GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);
	void InitWindow();
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	LPCTSTR GetResourceID() const;
	UILIB_RESOURCETYPE GetResourceType() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	BOOL TerminateProcess(DWORD pid);
private:
	void Notify(TNotifyUI& msg);					//消息处理事件
	void OnExit(const TNotifyUI& msg);				//程序离开事件
	void OnClick(const TNotifyUI& msg);				//按钮单击事件
	void GetUserInfo();								//获取用户信息
	void LoginInSteamUser(CControlUI* pControl);	//切换Steam账号
	void WriteRegiter(LPCTSTR pUserName);			//写入登录账户名
	_bstr_t GetUserData();							//获取用户数据
	_bstr_t GetPattern();							//获取正则表达式
	CDuiString GetImageStr(LPCTSTR pSteamID);		//获取个人头像数据
private:
	CPaintManagerUI m_pListManager;

};


