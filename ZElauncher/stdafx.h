#pragma once 
#include <windows.h>
#include <stdio.h>
#include <future>
#include <iostream>
#include <list>
#include <vector>
#include <UIlib.h>

#include "resource.h"
#include "json/json.h"
#include "ModuleSDK/lib_http.h"
#include "ModuleSDK/lib_Process.h"
#include "ModuleSDK/lib_Socket.h"
#include "ModuleSDK/ChineseCode.h"
#include "ModuleSDK/Path.h"
#include "Duilib/include/Menu/UIMenu.h"

#import <progid:VBScript.RegExp>named_guids//no_namespace

#ifdef _DEBUG
#include "Debug/3.tlh"
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"Duilib_d.lib")
#else
#pragma comment(lib,"Duilib.lib")
#endif // _DEBUG
#pragma comment(lib,"ComCtl32.lib") 

#pragma warning(disable:4996)
#pragma warning(disable:4091)
