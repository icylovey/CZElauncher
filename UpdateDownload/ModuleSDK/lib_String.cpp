#include "lib_String.h"

namespace lib_String
{
	CLibString::CLibString()
	{
		m_lpstr = new char[5]();
		m_lpstrw = new wchar_t[5]();
	}
	CLibString::~CLibString()
	{
		delete[]m_lpstr;
		delete[]m_lpstrw;
		m_lpstr = nullptr;
		m_lpstrw = nullptr;
	}
	CLibString::CLibString(const char* lpstr)
	{
		UINT sLen = strlen(lpstr);
		m_lpstr = new char[sLen + 1]();
		strcpy(m_lpstr, lpstr);
		UINT wslen = sLen * sizeof(wchar_t);
		m_lpstrw = new wchar_t[wslen + 2]();
		MultiByteToWideChar(CP_ACP, NULL, m_lpstr, sLen, m_lpstrw, wslen + 2);
	}
	CLibString::CLibString(const wchar_t* lpstr)
	{
		UINT wsLen = wcslen(lpstr) * sizeof(wchar_t);
		m_lpstrw = new wchar_t[wsLen + 2]();
		wcscpy(m_lpstrw, lpstr);
		UINT slen = wcslen(lpstr) * sizeof(wchar_t);
		m_lpstr = new char[slen + 2]();
		WideCharToMultiByte(CP_ACP, NULL, m_lpstrw, wsLen, m_lpstr, slen + 2, NULL, NULL);
	}

	UINT CLibString::Size()const
	{
		return strlen(m_lpstr);
	}
	UINT CLibString::wSize()const
	{
		return strlen(m_lpstr) * sizeof(wchar_t);
	}
	UINT CLibString::Length()const
	{
		return strlen(m_lpstr);
	}

	CLibString::operator char* ()const noexcept
	{
		return m_lpstr;
	}
	CLibString::operator const char* ()const noexcept
	{
		return m_lpstr;
	}
	CLibString::operator const wchar_t* ()const noexcept
	{
		return m_lpstrw;
	}
	CLibString::operator wchar_t* ()const noexcept
	{
		return m_lpstrw;
	}

	const CLibString& CLibString::operator+(const CLibString& lpstr)
	{
		UINT sLen = strlen(lpstr.m_lpstr) + Size();
		char* sTemp = new char[sLen + 1]();
		strcpy(sTemp, m_lpstr);
		strcat(sTemp, lpstr.m_lpstr);
		delete[]m_lpstr;
		m_lpstr = sTemp;

		UINT wslen = wcslen(lpstr.m_lpstrw) * sizeof(wchar_t) + wSize();
		wchar_t* sTemp2 = new wchar_t[wslen + 2]();
		wcscpy(sTemp2, m_lpstrw);
		wcscat(sTemp2, lpstr.m_lpstrw);
		delete[]m_lpstrw;
		m_lpstrw = sTemp2;
		return *this;
	}
	void CLibString::operator+=(const CLibString& lpstr)
	{
		UINT sLen = strlen(m_lpstr) + strlen(lpstr) + 2;
		char* pTemp = new char[sLen + 1]();
		strcpy(pTemp, m_lpstr);
		strcat(pTemp, lpstr);
		delete[]m_lpstr;
		m_lpstr = pTemp;

		sLen *= sizeof(wchar_t);
		wchar_t* pTemp2 = new wchar_t[sLen + 2]();
		wcscpy(pTemp2, m_lpstrw);
		sLen = strlen(lpstr) * sizeof(wchar_t);
		wchar_t* pTemp3 = new wchar_t[sLen + 2]();
		MultiByteToWideChar(CP_ACP, NULL, lpstr, strlen(lpstr), pTemp3, sLen);
		wcscat(pTemp2, pTemp3);
		delete[]pTemp3;
		delete[]m_lpstrw;
		m_lpstrw = pTemp2;
		return;
	}
	void CLibString::operator=(const CLibString& lpstr)
	{
		UINT sLen = strlen(lpstr.m_lpstr);
		if (sLen < 1) {
			delete[]m_lpstr;
			delete[]m_lpstrw;
			m_lpstr = new char[5]();
			m_lpstrw = new wchar_t[5]();
			return;
		}
		char* pTemp = new char[sLen + 1]();
		strcpy(pTemp, lpstr.m_lpstr);
		delete[]m_lpstr;
		m_lpstr = pTemp;

		wchar_t* pTemp2 = new wchar_t[sLen * sizeof(wchar_t) + 2]();
		wcscpy(pTemp2, lpstr.m_lpstrw);
		delete[]m_lpstrw;
		m_lpstrw = pTemp2;
	}
	bool CLibString::operator==(const CLibString& lpstr)
	{
		return (Compare(lpstr) == 0);
	}
	bool CLibString::operator==(const char* lpstr)
	{
		return (Compare(lpstr) == 0);
	}
	bool CLibString::operator==(const wchar_t* lpstr)
	{
		return (Compare(lpstr) == 0);
	}

	int CLibString::Compare(const CLibString& lpstr)
	{
		return strcmp(m_lpstr, lpstr);
	}
	void CLibString::Decode(UINT nFlag)
	{
		UINT slen = strlen(m_lpstr) * sizeof(wchar_t);
		wchar_t* szTemp = new wchar_t[slen + 2]();
		MultiByteToWideChar(nFlag, NULL, m_lpstr, Length(), szTemp, slen);
		slen = wcslen(m_lpstrw);
		char* szTemp2 = new char[slen * sizeof(wchar_t) + 2]();
		WideCharToMultiByte(CP_ACP, NULL, szTemp, slen, szTemp2, slen * sizeof(wchar_t), NULL, NULL);
		delete[]m_lpstrw;
		delete[]m_lpstr;
		m_lpstrw = szTemp;
		m_lpstr = szTemp2;
	}
	int CLibString::Find(const char* lpFindstr, UINT nPos)
	{
		if (nPos >= Length())return -1;
		if (!lpFindstr)return -1;
		UINT sLen = strlen(lpFindstr) * sizeof(wchar_t);
		wchar_t* pTemp = new wchar_t[sLen + 2]();
		MultiByteToWideChar(CP_ACP, NULL, lpFindstr, strlen(lpFindstr), pTemp, sLen + 2);
		wchar_t* pFind = m_lpstrw + nPos;
		wchar_t* ptmpc = wcsstr(pFind, pTemp);
		if (!ptmpc)return -1;
		return ptmpc - m_lpstrw;
	}
	int CLibString::Find(const wchar_t* lpFindstr, UINT nPos)
	{
		if (nPos >= Length())return -1;
		if (!lpFindstr)return -1;
		//if (!::IsBadStringPtrW(lpFindstr, -1))return -1;
		wchar_t* pFind = m_lpstrw + nPos;
		wchar_t* ptmpc = wcsstr(pFind, lpFindstr);
		if (!ptmpc)return -1;
		return ptmpc - m_lpstrw;
	}
	CLibString CLibString::Left(const CLibString& lpFindStr, UINT nCount)const
	{
		if (nCount < 1)return _T("");
		wchar_t* pTemp = wcsstr(m_lpstrw, lpFindStr);
		if (!pTemp)return _T("");
		if (pTemp < (m_lpstrw + wcslen(m_lpstrw)))pTemp[0] = 0;
		return CLibString(m_lpstrw);
	}
	CLibString CLibString::Right(const CLibString& lpFindStr, UINT nCount)const
	{
		if (nCount < 1)return _T("");
		wchar_t* pTemp = wcsstr(m_lpstrw, lpFindStr);
		wchar_t* pMaxTemp = m_lpstrw + wcslen(m_lpstrw);
		if (nCount + pTemp <= pMaxTemp)pTemp[nCount] = 0;
		return CLibString(pTemp);
	}
	CLibString CLibString::Mid(const CLibString& lpLeftStr, const CLibString& lpRightStr, UINT nPos)const
	{
		UINT ipos = nPos * sizeof(wchar_t);
		if (m_lpstrw + ipos > m_lpstrw + wcslen(m_lpstrw))return _T("");
		wchar_t* pLeft = wcsstr(m_lpstrw + ipos, lpLeftStr);
		if (!pLeft)return _T("");
		wchar_t* pRight = wcsstr(pLeft, lpRightStr);
		if (!pLeft)return _T("");
		pLeft += wcslen(lpLeftStr);
		UINT sLen = (pRight - pLeft) * sizeof(wchar_t);
		wchar_t* szTemp = new wchar_t[sLen + 2]();
		memcpy(szTemp, pLeft, sLen);
		CLibString Result = szTemp;
		delete[]szTemp;
		return Result;
	}

}