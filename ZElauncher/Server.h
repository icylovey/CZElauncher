#pragma once
#include "stdafx.h"
#include "LookPlayer.h"

using namespace DuiLib;
using namespace lib_http;

namespace Server {
	struct ServerInfo
	{
		std::string name;
		UINT type;
		bool Is93x;
	};
}

class CServerUI :
	public CContainerUI
	,public INotifyUI
{
public:
	CServerUI(CPaintManagerUI& m_paintmanager, HWND hWnd);
	~CServerUI();
	void ShowServerList(const char* pName, UINT type = NULL, bool Is93x = false);
	void CopyServerInfo(UINT nIndex);
	void JoinServer();
	void static DownloadMap93x(LPCTSTR lpMapName);
	void static DownloadMapjsly(LPCTSTR lpMapName);
	void static DownloadMapfys(LPCTSTR lpMapName);
	void OutPutTip(LPCTSTR lpOutput);
	void OutPutTip2(LPCTSTR lpOutput);
	void RunGame(LPCTSTR lpServer);
	void OnLookPlayer();
private:
	void Notify(TNotifyUI& msg);
	void GetServerList(const char* pName, UINT type = NULL);
	void Get93xServerList(const char* pName, UINT type = NULL);
	//新版获取服务器列表
	void GetServerList2(const char* pName, UINT type = NULL);
	void Get93xServerList2(const char* pName, UINT type = NULL);
	void OnCreate();
	void OnClick(CControlUI* Click);
	bool IsFileExist(LPCTSTR lpFileName);
	//编辑框清空
	void ClearEdit();
	//LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//LPCTSTR GetWindowClassName()const;
private:
	CPaintManagerUI* m_paManager;
	CPaintManagerUI m_pTmpManager;
	CDialogBuilder builder;
	HWND m_hWnd;
	static bool Is93x;
};