#include "论坛.h"

std::string g_BBSListUrl = "";

C论坛UI::~C论坛UI()
{
}

C论坛UI::C论坛UI(CPaintManagerUI& m_paintmanager)
{
	m_paintmanager_ = &m_paintmanager;
	CContainerUI* pBBS = static_cast<CContainerUI*>(builder.Create(_T("BBS.xml"), NULL, NULL, &m_PaintManager2));
	if (pBBS) {
		this->Add(pBBS);
		m_paintmanager_->AddNotifier(this);
	}
	else {
		this->RemoveAll();
		return;
	}
}

void C论坛UI::OnClick(CControlUI* Click)
{
	if (_tcscmp(Click->GetName(), _T("bbs_ze")) == 0)GetBBSList("https://bbs.93x.net/forum.php?mod=forumdisplay&fid=38");
	else if (_tcscmp(Click->GetName(), _T("bbs_discuss")) == 0)GetBBSList("https://bbs.93x.net/forum.php?mod=forumdisplay&fid=39");
	else if (_tcscmp(Click->GetName(), _T("bbs_ttt")) == 0)GetBBSList("https://bbs.93x.net/forum.php?mod=forumdisplay&fid=114");
	else if (_tcscmp(Click->GetName(), _T("bbs_Skin")) == 0)GetBBSList("https://bbs.93x.net/forum.php?mod=forumdisplay&fid=279");
}

void C论坛UI::MenuClick(CControlUI* Click)
{
	if(_tcscmp(Click->GetText(),_T("使用浏览器查看帖子"))==0)BrowserBBS();
	else if (_tcscmp(Click->GetText(), _T("刷新帖子")) == 0)ShowBBSList();
	else if (_tcscmp(Click->GetText(), _T("查看帖子")) == 0)OnLookBBS();
	else if (_tcscmp(Click->GetText(), _T("切换显示置顶帖子")) == 0) {
		禁止置顶帖显示 = !禁止置顶帖显示;
		ShowBBSList();
	}
}

void C论坛UI::BrowserBBS()
{
	CListUI* plist = static_cast<CListUI*>(m_paintmanager_->FindControl(_T("List_bbs")));
	if (!plist)return;
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(plist->GetItemAt(plist->GetCurSel()));
	if (!pItem)return;
	CDuiString url = pItem->GetUserData();
	url.Replace(_T("amp;"), _T(""));
	ShellExecute(NULL, _T(""), url, NULL, NULL, SW_SHOW);
}

void C论坛UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnCreate();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
	else if (_tcscmp(msg.sType, _T("itemclick")) == 0)MenuClick(msg.pSender);
	else if (_tcscmp(msg.sType, _T("menu")) == 0 && _tcscmp(msg.pSender->GetName(), _T("List_bbs")) == 0) {
		CDuiPoint point(0, 0);
		GetCursorPos(&point);
		STRINGorID xml(_T("Menu_BBS.xml"));
		CMenuWnd* pMenu = CMenuWnd::CreateMenu(nullptr, xml, point, m_paintmanager_);

	}
	//else if (_tcscmp(msg.sType, _T("itemactivate")) == 0)OnLookBBS();
}

void C论坛UI::OnCreate()
{
	CListUI* plistbkimg = static_cast<CListUI*>(m_paintmanager_->FindControl(_T("List_bbs")));
	if (plistbkimg) {
		_bstr_t cfgbuff = GetCFGPath();
		TCHAR* pszbuff = new TCHAR[MAX_PATH]();
		GetPrivateProfileString(_T("ZElauncher"), _T("edit_listbkimge"), NULL, pszbuff, (MAX_PATH * sizeof(TCHAR)), cfgbuff);
		if (_tcslen(pszbuff) > 5) {
			plistbkimg->SetItemBkColor(RGB(0xFF, 0xE7, 0xE7));
			plistbkimg->SetBkImage(pszbuff);
		}
	}
	std::thread t1 = std::thread(&C论坛UI::GetBBSList, this, (_bstr_t)"https://bbs.93x.net/forum.php?mod=forumdisplay&fid=38", NULL);
	t1.detach();
	std::thread t2 = std::thread(&C论坛UI::Threadlaba, this);
	t2.detach();
}

void C论坛UI::Threadlaba()
{
	for (;;) {
		if (!是否正在获取喇叭信息) {
			CTextUI* pText = static_cast<CTextUI*>(m_paintmanager_->FindControl(_T("bbs_say")));
			if (!pText || 喇叭信息.size() < 1)continue;
			_bstr_t sText = 喇叭信息[m_SaynCount].c_str();
			m_SaynCount++;
			if (m_SaynCount >= 喇叭信息.size())m_SaynCount = 0;
			pText->SetText(sText);
		}
		Sleep(3000);
	}
}

void C论坛UI::ShowBBSList()
{
	std::thread t1 = std::thread(&C论坛UI::GetBBSList, this, (_bstr_t)g_BBSListUrl.c_str(), NULL);
	t1.detach();
}

std::vector<shared_ptr<HtmlElement>> C论坛UI::GetTagValue(const char* name, const char* tagname, shared_ptr<HtmlElement>& tbody)
{
	std::vector<shared_ptr<HtmlElement>> tagvalue = tbody->GetElementByClassName(name);
	std::vector<shared_ptr<HtmlElement>> Result;
	if (tagvalue.size() < 1)return Result;
	return tagvalue[0]->GetElementByClassName(tagname);
}

void C论坛UI::OnLookBBS()
{
	/*MessageBox(NULL, L"很抱歉此项功能还没有完成,请耐心等待.", L"Tip", MB_ICONQUESTION);
	return;*/
	CListUI* pList = static_cast<CListUI*>(m_paintmanager_->FindControl(_T("List_bbs")));
	if (!pList)return;
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(pList->GetCurSel()));
	if (!pItem) {
		MessageBox(NULL, L"请选择一个主题在使用该功能!", NULL, NULL);
		return;
	}
	CDuiString ItemText = pItem->GetUserData();
	ItemText.Replace(_T("amp;"), _T(""));
	g_BBSUrl = ItemText;
	g_BBSTitle = pItem->GetText(1);
	C查看帖子UI* pSetting = new C查看帖子UI(m_paintmanager_);
	if (pSetting == NULL) return;
	pSetting->Create(NULL, _T("查看帖子"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 900, 500);
	pSetting->CenterWindow();
	pSetting->ShowModal();
	//::ShowWindow(*pSetting, SW_SHOW);
	//::SetWindowPos(*pSetting, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

bool IsThread3 = false;

void C论坛UI::GetBBSList(_bstr_t url, UINT type /* = NULL */)
{
	if (IsThread3)return;
	IsThread3 = true;
	g_BBSListUrl = url;
	CListUI* pList = static_cast<CListUI*>(m_paintmanager_->FindControl(_T("List_bbs")));
	if (!pList) { IsThread3 = false; return; }
	pList->RemoveAll();
	主题信息.clear();
	lib_http::CLibhttp http;
	std::string htmldata;
	http.GET(url.GetBSTR(), htmldata, L"", gCookies.GetBSTR());
	if (htmldata.length() < 10) { IsThread3 = false; return; }
	//编码转换
	std::string htmlDecode = DecodeToString(CP_UTF8, htmldata);
	if (strstr(htmlDecode.c_str(), "出于用户隐私考虑,这个板块需要350分钟游戏时间方可进入")) {
		MessageBox(NULL, L"出于用户隐私考虑,这个板块需要350分钟游戏时间方可进入!", NULL, MB_ICONWARNING);
		IsThread3 = false;
		return;
	}
	HtmlParser parser;
	shared_ptr<HtmlDocument> doc = parser.Parse(htmlDecode.c_str(), htmlDecode.size());
	//释放缓冲区;
	/*delete[]pStrMulti;
	delete[]pStrHtml*/;
	//html解析
	CBBSUI::主题 Tmp_ = { "" };
	_bstr_t tmpText = "";

	GetSaylaba(doc);
	//主题帖子列表获取处理
	shared_ptr<HtmlElement> BBSlist = std::move(doc->GetElementById("threadlisttableid"));
	if (!BBSlist) {
		MessageBox(NULL, L"获取数据失败,请重试!", NULL, NULL);
		IsThread3 = false;
		return;
	}
	std::vector<shared_ptr<HtmlElement>> threadlisttableid = std::move(BBSlist->GetElementByTagName("tbody"));
	const char Element_name[3][10] = { "common","lock","new" };
	const char Element_type[3][15] = { "<置顶帖>","<锁定帖>","<未分类>" };
	for (auto tbody : threadlisttableid) {
		Tmp_ = { "" };
		//获取主题帖子标题
		for (UINT j = 0; j < 3; j++) {
			//获取主题Element;
			std::vector<shared_ptr<HtmlElement>> Element = std::move(tbody->GetElementByClassName(Element_name[j]));
			if (Element.size() < 1)continue;
			if (strstr(tbody->html().c_str(), "置顶主题"))tmpText = Element_type[j];
			else {
				//获取主题类型名称;
				std::vector<shared_ptr<HtmlElement>> eletype = Element[0]->GetElementByTagName("em");
				if (eletype.size() > 0) {
					tmpText = eletype[0]->text().c_str();
				}
				else tmpText = Element_type[2];
			}
			//std::string htmltitle = tbody->html();
			Tmp_.typename_ = tmpText;
			//获取主题名称和链接;
			std::vector<shared_ptr<HtmlElement>> sxst = std::move(Element[0]->GetElementByClassName("s xst"));
			if (sxst.size() < 1)continue;
			Tmp_.主题名 = sxst[0]->text().c_str();
			Tmp_.hrefurl = sxst[0]->GetAttribute("href").c_str();
			//判断是否有红包;
			std::vector<shared_ptr<HtmlElement>> award = std::move(Element[0]->GetElementByTagName("font"));
			if (award.size() > 1) {
				const char* pChar = award[1]->GetValue().c_str();
				if (strstr(pChar, "红包")) {
					Tmp_.是否有红包 = true;
					Tmp_.红包类型 = pChar;
				}
			}
			Tmp_.type = 0x2F + j;
			break;
		}
		//获取主题回复和查看量
		std::vector<shared_ptr<HtmlElement>> num = std::move(tbody->GetElementByClassName("num"));
		if (num.size() > 0) {
			std::vector<shared_ptr<HtmlElement>> sxst = std::move(num[0]->GetElementByClassName("xi2"));
			if (sxst.size() > 0) {
				tmpText = sxst[0]->text().c_str();
				Tmp_.回复量 = std::move(tmpText);
				tmpText = num[0]->text().c_str();
				Tmp_.查看量 = std::move(tmpText);
			}
		}
		if (Tmp_.主题名.length() > 1)主题信息.emplace_back(Tmp_);
	}

	srand((UINT)time(NULL));
	for (auto ListBBS : 主题信息) {
		if (禁止置顶帖显示) {
			if (strstr(ListBBS.typename_.c_str(), Element_type[0]) || strstr(ListBBS.typename_.c_str(), Element_type[1]))continue;
		}
		CListTextElementUI* pItemText = new CListTextElementUI();
		pItemText->SetTag(pItemText->GetTag() + 1);
		pList->Add(pItemText);
		_bstr_t sText = ListBBS.typename_.c_str();
		pItemText->SetText(0, sText);

		sText = ListBBS.主题名.c_str();
		pItemText->SetText(1, sText);

		sText = ListBBS.红包类型.c_str();
		pItemText->SetText(2, sText);

		sText = ListBBS.回复量.c_str();
		sText += "/";
		sText += ListBBS.查看量.c_str();
		pItemText->SetText(3, sText);

		sText = ListBBS.hrefurl.c_str();
		pItemText->SetUserData(sText.GetBSTR());

		//if (ListBBS.type > 1)pItemText->SetTextColor_(RGB(rand() % 255, rand() % 255, rand() % 255));
		if (strstr(ListBBS.typename_.c_str(), Element_type[0]) || strstr(ListBBS.typename_.c_str(), Element_type[1]))
			pItemText->SetTextColor_(RGB(rand() % 255, rand() % 255, rand() % 255));
	}
	IsThread3 = false;
}

void C论坛UI::GetSaylaba(shared_ptr<HtmlDocument>& doc)
{
	是否正在获取喇叭信息 = true;
	喇叭信息.clear();
	m_SaynCount = 0;
	shared_ptr<HtmlElement> laba = std::move(doc->GetElementById("laba"));
	std::string tmptext;
	std::vector<shared_ptr<HtmlElement>> tbodyroot = std::move(doc->SelectElement("//tr[@style]"));
	if (tbodyroot.size() < 1)return;
	std::vector<shared_ptr<HtmlElement>> tbodyroot_;
	tbodyroot[0]->SelectElement("//div[@style]", tbodyroot_);
	tmptext = std::move(tbodyroot[0]->html());
	_bstr_t Tmpstr = tbodyroot[0]->text().c_str();
	CDuiString strin = Tmpstr.GetBSTR();
	strin.Replace(L"\t\n\t", L"  ");
	strin.Replace(L"\t", L"  ");
	int npos = -1;
	do {
		Tmpstr = strin.Mid(npos + 1, strin.Find(L"\n", npos + 1) - (npos + 1));
		npos = strin.Find(L"\n", npos + 1);
		tmptext = Tmpstr;
		if (tmptext.length() > 2 && tmptext.find("小时前") == std::string::npos && tmptext.find("天前") == std::string::npos)喇叭信息.emplace_back(tmptext);
	} while (npos != -1);
	是否正在获取喇叭信息 = false;
}

