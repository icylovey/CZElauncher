#pragma once
#ifndef _LIB_PRCOESS_H
#define _LIB_PROCESS_H
#include <windows.h>
#include <iostream>
#include <sstream>
#include <tchar.h>
#include <TlHelp32.h>

namespace lib_Process
{
	class Process
	{
	public:
		Process();
		~Process();
		//PID获取窗口句柄
		//@parameter PID
		HWND _fastcall PidToHwnd(DWORD pid);
		//进程名获取窗口句柄
		//@parameter 进程名
		HWND _fastcall ProcessNameToHwnd(const TCHAR* ProcessName);
		//进程名获取PID
		//@parameter 进程名
		DWORD _fastcall ProcessNameToPid(const TCHAR* ProcessName);
		//进程名是否存在
		//@parameter 进程名
		bool _fastcall IsProcesNameExist(const TCHAR* ProcessName);
		//进程ID是否存在
		//@parameter 进程名
		bool _fastcall IsPidExist(DWORD pid);
		//获取模块地址
		//@parameter 进程名
		//@parameter 模块名
		DWORD _fastcall GetModuleAddress(const TCHAR* ProcessName, const TCHAR* ModuleName);
		//进程创建
		//@parameter 文件名
		//@parameter 运行参数
		void _fastcall ProcessRun(const TCHAR* FileName, const TCHAR* Parameter);
	private:

	};

}

#endif

