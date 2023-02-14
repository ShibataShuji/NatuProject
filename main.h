#pragma once

#define _CRT_SECURE_NO_WARNINGS




#pragma warning(push)
#pragma warning(disable:4005)

#pragma warning(pop)



#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")


#define SCREEN_WIDTH	(1920)
#define SCREEN_HEIGHT	(1080)

// mainで保存してあるグローバル変数を持ってこれる
HWND Getg_hwnd();
POINT GetScreenMousePosition();



//
//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <windows.h>
//#include <assert.h>
//
//#pragma warning(push)
//#pragma warning(disable:4005)
//
//#include <d3d11.h>
//#include <d3dx9.h>
//#include <d3dx11.h>
//
//#include <stdio.h>
//
////#include <d3d11.h>
////#include <d3dx11.h>
//#include <d3dcompiler.h>
////#include <d3dx9.h>
//
//#define DIRECTINPUT_VERSION 0x0800		// 警告対処
////#include "dinput.h"
////#include "mmsystem.h"
//
//#pragma warning(pop)
//
//
////*****************************************************************************
//// ライブラリのリンク
////*****************************************************************************
//#pragma comment (lib, "winmm.lib")
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dx9.lib")
//#pragma comment (lib, "d3dx11.lib")
//
////#pragma comment (lib, "d3d11.lib")		
//#pragma comment (lib, "d3dcompiler.lib")
////#pragma comment (lib, "d3dx11.lib")	
////#pragma comment (lib, "d3dx9.lib")	
////#pragma comment (lib, "winmm.lib")
//#pragma comment (lib, "dxerr.lib")
//#pragma comment (lib, "dxguid.lib")
//#pragma comment (lib, "dinput8.lib")
//
//
//#define SCREEN_WIDTH	(960)
//#define SCREEN_HEIGHT	(540)
//
////#define SCREEN_WIDTH	(1920)
////#define SCREEN_HEIGHT	(1080)
//
//
//HWND GetWindow();