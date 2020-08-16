#pragma once 
#ifndef FD_SETSIZE
#define FD_SETSIZE 1024
#endif
/*
#define _AFXDLL
#include <afx.h>*/
#include <windows.h>
#include <stdio.h>
#include <future>
#include <iostream>
#include <thread> 
#include <list>
#include <vector>

#include "Duilib/StdAfx.h"
#include "Variable.h"
#include "resource.h"
#include "json/json.h"
#include "ModuleSDK/lib_http.h"
#include "ModuleSDK/lib_Process.h"
#include "ModuleSDK/lib_Socket.h"
#include "ModuleSDK/ChineseCode.h"
#include "ModuleSDK/Path.h"

#import <progid:VBScript.RegExp>named_guids//no_namespace

#ifdef _DEBUG
#include "Debug/3.tlh"
#endif // _DEBUG

#ifdef _DEBUG
//#pragma comment(lib,"Duilib_d.lib")
#else
//#pragma comment(lib,"Duilib.lib")
#endif // _DEBUG
#pragma comment(lib,"ComCtl32.lib") 

#pragma warning(disable:4996)
#pragma warning(disable:4091)
