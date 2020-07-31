// ChineseCode.h: interface for the CChineseCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_)
#define AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

class ChineseCode
{
public:
	static void UTF_8ToUnicode(wchar_t* pOut, char* pText);  // 把UTF-8转换成Unicode
	static void UnicodeToUTF_8(char* pOut, wchar_t* pText);  //Unicode 转换成UTF-8
	static void UnicodeToGB2312(char* pOut, wchar_t uData);  // 把Unicode 转换成 GB2312 
	static void Gb2312ToUnicode(wchar_t* pOut, char* gbBuffer);// GB2312 转换成　Unicode
	static void GB2312ToUTF_8(string& pOut, char* pText, int pLen);//GB2312 转为 UTF-8
	static void UTF_8ToGB2312(string& pOut, char* pText, int pLen);//UTF-8 转为 GB2312
};

class strCoding
{
public:
	strCoding(void);
	~strCoding(void);
	//utf_8转为gb2312
	void UTF_8ToGB2312(string& pOut, char* pText, int pLen);
	//gb2312 转utf_8
	void GB2312ToUTF_8(string& pOut, char* pText, int pLen); 
	//urlgb2312编码
	string UrlGB2312(char* str);                           
	//urlutf8 编码
	string UrlUTF8(char* str);                             
	//urlutf8解码
	string UrlUTF8Decode(string str);                  
	//urlgb2312解码
	string UrlGB2312Decode(string str);   
	std::string usc2ToAnsi(const std::string& instr);
private:
	void Gb2312ToUnicode(WCHAR* pOut, char* gbBuffer);
	void UTF_8ToUnicode(WCHAR* pOut, char* pText);
	void UnicodeToUTF_8(char* pOut, WCHAR* pText);
	void UnicodeToGB2312(char* pOut, WCHAR uData);
	char CharToInt(char ch);
	char StrToBin(char* str);

};

class CSHA512
{
protected:
	typedef unsigned char uint8; 
	typedef unsigned int uint32;
	typedef unsigned long long uint64;
	const static uint64 sha512_k[]; 
	static const unsigned int SHA384_512_BLOCK_SIZE = (1024 / 8);

public:
	void init(); 
	void update(const unsigned char* message, unsigned int len); 
	void final(unsigned char* digest); static const unsigned int DIGEST_SIZE = (512 / 8);

protected:
	void transform(const unsigned char* message, unsigned int block_nb); 
	unsigned int m_tot_len; 
	unsigned int m_len; 
	unsigned char m_block[2 * SHA384_512_BLOCK_SIZE];
	uint64 m_h[8];
};
std::string Encode_sha512(std::string input);

#define SHA2_SHFR(x, n)    (x >> n)
#define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA512_F1(x) (SHA2_ROTR(x, 28) ^ SHA2_ROTR(x, 34) ^ SHA2_ROTR(x, 39))
#define SHA512_F2(x) (SHA2_ROTR(x, 14) ^ SHA2_ROTR(x, 18) ^ SHA2_ROTR(x, 41))
#define SHA512_F3(x) (SHA2_ROTR(x,  1) ^ SHA2_ROTR(x,  8) ^ SHA2_SHFR(x,  7))
#define SHA512_F4(x) (SHA2_ROTR(x, 19) ^ SHA2_ROTR(x, 61) ^ SHA2_SHFR(x,  6))
#define SHA2_UNPACK32(x, str){*((str) + 3) = (uint8) ((x));*((str) + 2) = (uint8) ((x) >>  8);*((str) + 1) = (uint8) ((x) >> 16);*((str) + 0) = (uint8) ((x) >> 24);}
#define SHA2_UNPACK64(x, str){*((str) + 7) = (uint8) ((x));*((str) + 6) = (uint8) ((x) >>  8);*((str) + 5) = (uint8) ((x) >> 16);*((str) + 4) = (uint8) ((x) >> 24);*((str) + 3) = (uint8) ((x) >> 32);*((str) + 2) = (uint8) ((x) >> 40);*((str) + 1) = (uint8) ((x) >> 48);*((str) + 0) = (uint8) ((x) >> 56);}
#define SHA2_PACK64(str, x){*(x) = ((uint64) *((str) + 7))|((uint64) *((str) + 6)<<8)|((uint64) *((str) + 5) << 16)|((uint64) *((str) + 4) << 24)|((uint64) *((str) + 3) << 32)|((uint64) *((str) + 2) << 40)|((uint64) *((str) + 1) << 48)|((uint64) *((str) + 0) << 56);}

#endif // !defined(AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_)
