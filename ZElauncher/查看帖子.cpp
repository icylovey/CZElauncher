#include "查看帖子.h"

CDuiString g_BBSUrl = L"";

LPCTSTR C查看帖子UI::GetWindowClassName()const
{
	return _T("查看帖子");
}

CDuiString C查看帖子UI::GetSkinFile()
{
	return _T("BBS_look.xml");
}

CDuiString C查看帖子UI::GetSkinFolder()
{
	return _T("Skin\\ListRes\\");
}

UILIB_RESTYPE C查看帖子UI::GetResourceType()const
{
	return UILIB_FILE;
}

LPCTSTR C查看帖子UI::GetResourceID()const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void C查看帖子UI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* C查看帖子UI::CreateControl(LPCTSTR pstrClass)
{
	return nullptr;
}

void C查看帖子UI::InitWindow()
{
	
}

void C查看帖子UI::OnExit(const TNotifyUI& msg)
{
	
	__super::Close();
}


void C查看帖子UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0) {
		::SetForegroundWindow(m_hWnd);//窗口创建完毕后,获取焦点;
		获取帖子内容();
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("closebtn")) == 0)OnExit(msg);

}

void C查看帖子UI::获取帖子内容()
{
	CListUI* pTilelayout = static_cast<CListUI*>(m_pm.FindControl(_T("Table_LookBBS")));
	if (!pTilelayout)return;
	for (UINT i = 0; i < 20; i++) {
		CDialogBuilder BuilderXml;
		CControlUI* pNewXml = BuilderXml.Create(_T("xml\\BBS_Nick.xml"), NULL, NULL, m_paintmanager);
		if (!pNewXml)return;
		pTilelayout->Add(pNewXml);
	}
}