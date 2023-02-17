

#include "stdafx.h"


const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11�Q�[��";


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



	// �����_���[�̏��������s���Ă���
	Manager::Init();

	// DirectX���͏����̏�����(�E�B���h�E���쐬���Ă���s��)�BManager::Init()���O�ŏ�����������_���B
	Input::Init(hInstance, g_hwnd);

	ShowWindow(g_hwnd, nCmdShow);
	UpdateWindow(g_hwnd);

	// ShowWindow, UpdateWindow �̌�ɏ���
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
				//Renderer::Gui()->testGui();	// �Ă��Ƃ�GUI�o���Ă�

				Manager::Draw();

				{
					// ���̃E�B���h�E�ł̃}�E�X�̃X�N���[�����W(�f�X�N�g�b�v��̍��W)���擾����
					POINT ScreenPoint;
					GetCursorPos(&ScreenPoint);

					// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
					ScreenToClient(g_hwnd, &ScreenPoint);	//���ϐ��� FindWindowA(WINDOW_CLASS_NAME, nullptr)�ł�OK

					// �O���[�o���ϐ��̕��ɕۑ����Ă���
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



// WINAPI ���̃E�B���h�E�ł̃}�E�X�̍��W���擾����B(����̓E�B���h�E����Ȃ̂ō��ʉ��Ȃ�)
POINT GetScreenMousePosition()
{
	return g_ScreenPoint;
}






// �������牺��ImGui_ImplWin32_WndProcHandler��WINAPI��WndProc�ŃR�[�����Ȃ���WINAPI�̋@�\��ImGui�Ŏg���Ȃ��B�N���b�N�Ƃ��B
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
