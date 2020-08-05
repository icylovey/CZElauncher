#pragma once
#include "stdafx.h"
#include "查询服务器玩家.h"

using namespace DuiLib;
class C服务器UI;
extern bool IsChineseMap;
extern C服务器UI* pServer;
namespace Server {
	struct ServerInfo
	{
		std::string name;
		UINT type;
		bool Is93x;
	};
}

class C服务器UI :
	public CContainerUI
	,public INotifyUI
{
public:
	C服务器UI(CPaintManagerUI& m_pm, HWND hWnd);
	~C服务器UI();
	void RefreshServer();
	void ShowServerList(const char* pName, UINT type = NULL, bool Is93x = false);
	void CopyServerInfo(UINT nIndex);
	void JoinServer();
	void DownloadMap93x(LPCTSTR lpMapName, bool IsRunGame);
	void static DownloadMapjsly(LPCTSTR lpMapName);
	void static DownloadMapfys(LPCTSTR lpMapName);
	void RunGame(LPCTSTR lpServer);
	void OnLookPlayer();
	bool IsFileExist(LPCTSTR lpFileName);
private:
	void Notify(TNotifyUI& msg);
	//新版获取服务器列表
	void GetServerList2(const char* pName, UINT type = NULL);
	void Get93xServerList2(const char* pName, UINT type = NULL);
	void MenuClick(CControlUI* Click);
	void OnCreate();
	void OnClick(CControlUI* Click);
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