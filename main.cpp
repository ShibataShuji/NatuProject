

#include "stdafx.h"


const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";


//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


HWND g_hwnd;
POINT g_ScreenPoint;


HWND Getg_hwnd()
{
	return g_hwnd;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//WNDCLASSEX wcex =
	//{
	//	sizeof(WNDCLASSEX),
	//	CS_CLASSDC,
	//	WndProc,
	//	0,
	//	0,
	//	hInstance,
	//	NULL,
	//	LoadCursor(NULL, IDC_ARROW),
	//	(HBRUSH)(COLOR_WINDOW + 1),
	//	NULL,
	//	CLASS_NAME,
	//	NULL
	//};

	//RegisterClassEx(&wcex);



	//g_hwnd = CreateWindowEx(0,
	//	CLASS_NAME,
	//	WINDOW_NAME,
	//	WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
	//	(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
	//	NULL,
	//	NULL,
	//	hInstance,
	//	NULL);

		// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	WNDCLASSEXW wcex = { sizeof(wcex), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
	::RegisterClassExW(&wcex);
	//g_hwnd = ::CreateWindowW(wcex.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wcex.hInstance, NULL);
	g_hwnd = ::CreateWindowW(wcex.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1920, 1080, NULL, NULL, wcex.hInstance, NULL);

	// Initialize Direct3D
	if (!Renderer::CreateDeviceD3D(g_hwnd))
	{
		Renderer::CleanupDeviceD3D();
		::UnregisterClassW(wcex.lpszClassName, wcex.hInstance);
		return 1;
	}



	// レンダラーの初期化も行っている
	Manager::Init();

	// DirectX入力処理の初期化(ウィンドウを作成してから行う)。Manager::Init()より前で初期化しちゃダメ。
	Input::Init(hInstance, g_hwnd);

	ShowWindow(g_hwnd, nCmdShow);
	UpdateWindow(g_hwnd);

	// ShowWindow, UpdateWindow の後に書く
	Renderer::Gui_Setup(g_hwnd);

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;





	bool endapp = false;

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT || msg.message == DIK_ESCAPE)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				if (Input::GetKeyPress(DIK_ESCAPE))
					endapp = true;



				Manager::Update();
				//Renderer::Gui()->testGui();	// てすとでGUI出してる

				Manager::Draw();

				{
					// このウィンドウでのマウスのスクリーン座標(デスクトップ上の座標)を取得する
					POINT ScreenPoint;
					GetCursorPos(&ScreenPoint);

					// スクリーン座標をクライアント座標に変換する
					ScreenToClient(g_hwnd, &ScreenPoint);	//第一変数を FindWindowA(WINDOW_CLASS_NAME, nullptr)でもOK

					// グローバル変数の方に保存しておく
					g_ScreenPoint = ScreenPoint;

					std::string strMousePos;

					strMousePos += std::to_string(ScreenPoint.x);
					strMousePos += " : ";
					strMousePos += std::to_string(ScreenPoint.y);

					SetWindowText(g_hwnd, strMousePos.c_str());
				}

			}

			if (endapp)
				break;

		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::Uninit();

	return (int)msg.wParam;
}




//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//
//	switch(uMsg)
//	{
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//
//	case WM_KEYDOWN:
//		switch(wParam)
//		{
//		case VK_ESCAPE:
//			DestroyWindow(hWnd);
//			break;
//		}
//		break;
//
//	default:
//		break;
//	}
//
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}



// WINAPI このウィンドウでのマウスの座標を取得する。(今回はウィンドウが一つなので差別化なし)
POINT GetScreenMousePosition()
{
	return g_ScreenPoint;
}






// ここから下のImGui_ImplWin32_WndProcHandlerをWINAPIのWndProcでコールしないとWINAPIの機能がImGuiで使えない。クリックとか。
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (Renderer::GetDevice() != NULL && wParam != SIZE_MINIMIZED)
		{
			Renderer::CleanupRenderTarget();
			Renderer::GetSwapChain()->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			Renderer::CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
