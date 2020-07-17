#include "查询服务器玩家.h"

CDuiString g_ServerName_LookPlayer;
C查询服务器玩家UI* pLookPlayer = nullptr;
CPaintManagerUI* g_paintmanager = nullptr;

LPCTSTR C查询服务器玩家UI::GetWindowClassName()const
{
	return _T("LookPlayerWnd");
}

CDuiString C查询服务器玩家UI::GetSkinFile()
{
	return _T("LookPlayer.xml");
}

CDuiString C查询服务器玩家UI::GetSkinFolder()
{
	return _T("Skin\\ListRes\\");
}

UILIB_RESTYPE C查询服务器玩家UI::GetResourceType()const
{
	return UILIB_FILE;
}

LPCTSTR C查询服务器玩家UI::GetResourceID()const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void C查询服务器玩家UI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* C查询服务器玩家UI::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void C查询服务器玩家UI::InitWindow()
{
	pLookPlayer = this;
	g_paintmanager = &m_pm;
	lib_Socket::UDPClient udp;
	udp.Initialize();
}

void C查询服务器玩家UI::OnExit(const TNotifyUI& msg)
{
	lib_Socket::UDPClient udp;
	udp.Release();
	__super::Close();
}

void __stdcall UdpProc(SOCKET nSock, unsigned int nIndex, void* pBuf, unsigned int BufLen, unsigned int ErrorCode, LPARAM lParam)
{
	if (((unsigned char*)pBuf)[4] == 0x41) {
		lib_Socket::UDPClient udp;
		unsigned char Key[30] = { 0xFF, 0xFF,0xFF,0xFF,0x55 };
		CopyMemory(&Key[5], &((char*)pBuf)[5], 4);
		CopyMemory(&Key[9], Key, 9);
		CopyMemory(&Key[18], Key, 9);
		udp.SendData(udp.QueryUDPSocket(0).Socket, Key, sizeof(Key));
		return;
	}
	else if (((char*)pBuf)[4] == 0x44) {
		char* pChar = &((char*)pBuf)[5];
		char* pCharMax = (char*)pBuf + BufLen;
		UINT nPlayerNum = *pChar;
		pChar++;
		for (UINT i = 0; i < nPlayerNum; i++) {
			TCHAR Buff[1024] = { 0 };
			UINT nPlayerIndex = *pChar;
			pChar ++;
			_MultiByteToWideChar(CP_UTF8, NULL, pChar, strlen(pChar), Buff, 1024);
			_bstr_t sPlayerName=_T(" ");
			if(_tcslen(Buff)<1)sPlayerName += _T("玩家正在下载资源");
			else sPlayerName += Buff;
			pChar += strlen(pChar) + 1;
			LONG nGrade;
			CopyMemory(&nGrade, pChar, sizeof(LONG));
			pChar += sizeof(LONG);
			float fTime;
			CopyMemory(&fTime, pChar, sizeof(LONG));
			pChar += sizeof(float);
			CListUI* pList = static_cast<CListUI*>(g_paintmanager->FindControl(_T("List_Player")));
			if (pList) {
				CListTextElementUI* pListElement = new CListTextElementUI;
				pListElement->SetTag(i);
				pList->Add(pListElement);
				pListElement->SetText(0, sPlayerName);
				_stprintf(Buff, _T(" %d"), nGrade);
				pListElement->SetText(1, Buff);
				UINT imin = (UINT)(fTime / 60) % 60;
				UINT ihour = (UINT)(fTime / 60 / 60) % 24;
				UINT iSecond = (UINT)fTime % 60;
				_stprintf(Buff, _T(" %02d:%02d:%02d"), ihour, imin, iSecond);
				pListElement->SetText(2, Buff);
			}
		}
	}
}

void C查询服务器玩家UI::GetPlayer(LPCTSTR lpServer)
{
	CTextUI* pText = static_cast<CTextUI*>(m_pm.FindControl(_T("Text_Out")));
	CDuiString Out = _T("当前服务器:");
	Out += g_ServerName_LookPlayer;
	if (pText)pText->SetText(Out);
	lib_Socket::UDPClient udp;
	CDuiString sServer = lpServer;
	UINT nPos = sServer.Find(_T(":"));
	if (nPos == -1) {
		MessageBox(NULL, _T("连接服务器失败"), NULL, MB_OK);
		return;
	}
	CDuiString ServerIp = sServer.Mid(0, sServer.GetLength() - (sServer.GetLength() - nPos));
	CDuiString sPort = sServer.Mid(nPos + 1, sServer.GetLength() - nPos - 1);
	UINT nPort = _ttoi(sPort.GetData());
	if (!udp.CreateUDP(ServerIp.GetData(), nPort)) {
		MessageBox(NULL, _T("连接服务器失败"), NULL, MB_OK);
		return;
	}
	udp.SetFunc(ServerIp.GetData(), nPort, (lib_Socket::pTCPEventFunc)UdpProc);
	/*unsigned char* pBuf = new unsigned char[4096];
	ZeroMemory(pBuf, 4096);
	for (int i = 0; i < 4; i++) {
		pBuf[i] = 0xff;
	}
	CopyMemory(&pBuf[4], "TSource Engine Query", 21);
	udp.SendData(ServerIp.GetData(), nPort, pBuf, strlen((char*)pBuf) + 1);
	delete[]pBuf;*/
	unsigned char pBuf2[30] = { 0xFF, 0xFF, 0xFF, 0xFF, 0x55, 0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF, 0x55, 0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF, 0x55, 0xFF, 0xFF, 0xFF, 0xFF };
	udp.SendData(udp.QueryUDPSocket(0).Socket, pBuf2, sizeof(pBuf2));
	
	
}

void C查询服务器玩家UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0) {
		::SetForegroundWindow(m_hWnd);//窗口创建完毕后,获取焦点;
		//GetPlayer(_T("127.0.0.1:27018"));
		GetPlayer(g_ServerName_LookPlayer);
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("closebtn")) == 0)OnExit(msg);

}