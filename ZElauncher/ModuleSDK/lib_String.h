#pragma once
#ifndef _Lib_String_H
#define _Lib_String_H
#include <windows.h>
#include <tchar.h>

#pragma warning(disable:4996)
namespace lib_String
{

	class CLibString
	{
	public:
		CLibString();
		~CLibString();
		CLibString(const char* lpstr);
		CLibString(const wchar_t* lpstr);

		//运算符重载
		const CLibString& operator+(const CLibString& lpstr);
		void operator+=(const CLibString& lpstr);
		void operator=(const CLibString& lpstr);
		bool operator==(const CLibString& lpstr);
		bool operator==(const char* lpstr);
		bool operator==(const wchar_t* lpstr);

		//返回类型重载
		operator const wchar_t* () const noexcept;
		operator wchar_t* () const noexcept;
		operator const char* () const noexcept;
		operator char* () const noexcept;

		//获取大小
		UINT wSize()const;
		UINT Size()const;
		//获取字符串长度
		UINT Length()const;
		//解码
		//@parameter 转换类型
		void Decode(UINT nFlag);
		//寻找文本
		//@parameter 欲寻找文本
		int Find(const char* lpFindstr, UINT nPos = NULL);
		//寻找文本
		//@parameter 欲寻找文本
		int Find(const wchar_t* lpFindstr, UINT nPos = NULL);
		//取文本左边
		//@parameter 欲寻找文本
		//@parameter 取出文本数量
		CLibString Left(const CLibString& lpFindStr, UINT nCount)const;
		//取文本右边
		//@parameter 欲寻找文本
		//@parameter 取出文本数量
		CLibString Right(const CLibString& lpFindStr, UINT nCount)const;
		//取文本中间
		//@parameter 欲寻找左边文本
		//@parameter 欲寻找右边文本
		CLibString Mid(const CLibString& lpLeftStr, const CLibString& lpRightStr, UINT nPos = NULL)const;
		//文本比较
		//@paramaeter 欲比较文本
		int Compare(const CLibString& lpstr);
	private:
		mutable char* m_lpstr;
		mutable wchar_t* m_lpstrw;
	};
}

#endif // !_Lib_String_H

