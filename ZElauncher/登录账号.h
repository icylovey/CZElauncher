#pragma once
#include "ZElauncher.h"
#include "stdafx.h"
#include <Shlobj.h>

using namespace DuiLib;
class CZElauncherMain;
class C登录账户UI : public WindowImplBase
{
public:
	C登录账户UI(){}
	C登录账户UI(bool Islogin);
	~C登录账户UI() {} //::PostQuitMessage(0L); }
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	UILIB_RESOURCETYPE GetResourceType()const;
	LPCTSTR GetResourceID()const;
	void OnFinalMessage(HWND hWnd);
	LPCTSTR GetWindowClassName()const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void InitWindow();
private:
	void IELoginUser();
	bool IELoginVerify();
	void LoginUser();
	bool LoginVerify(std::string token);
	void 创建POST请求数据(std::string& POST请求数据);
	void 初始化用户名编辑框();
	void 错误代码处理(std::string 返回文本);
	void 设置登录UI状态();
	void Logout();
	bool UserEvent(std::string ResultData, std::string& ret_token);
	void OnExit(const TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
	void SaveEditInfo();

private:
	bool m_Istip = false;
	bool m_IsLogin = false;
};