#pragma once
#include "stdafx.h"
#include "查看帖子.h"
#include "htmlsdk/html_parser.hpp"

using namespace DuiLib;

namespace CBBSUI
{
	struct 主题
	{
		std::string 主题名;
		std::string 回复量;
		std::string 查看量;
		std::string hrefurl;
		bool 是否有红包;
		std::string 红包类型;
		std::string 红包总金额;
		std::string 红包剩余金额;
		UINT type;
		std::string typename_;
	};
	struct 喇叭
	{
		std::string 喇叭内容;
		std::string 发送用户;
		std::string 发送时间;
		UINT type;
	};
}

class C论坛UI :
	public CContainerUI
	,public INotifyUI
{
public:
	C论坛UI(CPaintManagerUI& m_paintmanager);;
	~C论坛UI();
	void ShowBBSList();
private:
	void Notify(TNotifyUI& msg);
	void MenuClick(CControlUI* Click);
	void OnCreate();
	void OnClick(CControlUI* Click);
	void GetBBSList(_bstr_t url, UINT type = NULL);
	void GetSaylaba(shared_ptr<HtmlDocument>& doc);
	void Threadlaba();
	std::vector<std::shared_ptr<HtmlElement>> GetTagValue(const char* name, const char* tagname, shared_ptr<HtmlElement>& tbody);
	void BrowserBBS();
	void OnLookBBS();
private:
	CDialogBuilder builder;
	CPaintManagerUI m_pm2;
	CPaintManagerUI* m_paintmanager_;
	std::vector<CBBSUI::主题> 主题信息;
	std::vector<std::string> 喇叭信息;
	UINT m_SaynCount;
	bool 是否正在获取喇叭信息;
	bool 禁止置顶帖显示;
};