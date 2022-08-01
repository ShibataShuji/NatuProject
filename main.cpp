#include "main.h"
#include "manager.h"
#include "input.h"
//#include "fullscreen.h"
#include "gui.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "renderer.h"

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11�Q�[��";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd);
void Uninit(void);
void Update(void);

long g_MouseX = 0;
long g_MouseY = 0;

HWND g_Window;

HWND GetWindow()
{
	return g_Window;
}

int main()
{
	return 0;
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RegisterClassEx(&wcex);

	g_Window = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL);

	Manager::Init();
	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	Init(hInstance, g_Window);
	// Initialize Direct3D
	if (!CreateDeviceD3D(g_Window))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wcex.lpszClassName, wcex.hInstance);
		return 1;
	}

	ShowWindow(g_Window, nCmdShow);
	//ShowWindow(g_Window, SW_SHOWDEFAULT);
	UpdateWindow(g_Window);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	DWORD nowtime = timeGetTime();	// ���ݎ���
	DWORD oldtime = nowtime;		// �O��`�掞��

	// FPS����l
	float g_fFPS;

	bool done = false;
	while (!done)
	{
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;

		dwCurrentTime = timeGetTime();
		nowtime = timeGetTime();			// ���ݎ����l��

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
		{
			dwExecLastTime = dwCurrentTime;

			// FPS�v�Z(���ȗ���)
			g_fFPS = 1000.0f / (nowtime - oldtime);

			/*_RPTN(_CRT_WARN, "FPS %f\n", g_fFPS);*/

			oldtime = nowtime;


			// Imgui�̖��t���[���\���̑O�ɕK�v
			// ����Update����Imgui���g�����߂�Update�̑O�ɌĂяo���Ă���
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			// �v���C���[���̃A�b�v�f�[�g�֐��̌Ăяo��
			Manager::Update();
			Update();

			// ����ȕ���gui�N���X�Ŋ֐��ɂ܂Ƃ߂āAplayer.cpp�Ƃ��ŌĂяo���Ƃ��Ɋ֐���ŌĂяo����Ƃ����Ǝv��
			//gui::DebugStorage();

			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if (show_demo_window)	// �ł��E�B���h�E�̕\���B�e�X�g�p
				ImGui::ShowDemoWindow(&show_demo_window);
			

			Renderer::Begin();
			// �v���C���[�Ȃǂ̃Q�[���I�u�W�F�N�g���p�����Ă���Draw�֐����Ăяo������B
			Manager::Draw();
			// �����_���[��End���ŁAImgui�̕\�����Ăяo���Ă���BGUI��1�ԏ�ɗ��Ăق�������Ō�ɂ��Ă���B
			Renderer::End();// �J�����͐�I�Ăяo�����厞// 3D��Ԃł̕\��// UI�n�͍Ō�ɁB

			
		}


	}

	timeEndPeriod(1);

	//-----ImGui�̍폜
	gui::DestroyImGui();

	CleanupDeviceD3D();
	::DestroyWindow(g_Window);
	::UnregisterClass(wcex.lpszClassName, wcex.hInstance);
	//::UnregisterClass(wc.lpszClassName, wc.hInstance);

	UnregisterClass(CLASS_NAME, wcex.hInstance);		//aaaaaaaaa
	//UnregisterClass(CLASS_NAME, wc.hInstance);

	Manager::Uninit();
	Uninit();

	return 0;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
	switch (uMsg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;

	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);		//�}�E�X��X���W���擾
		g_MouseY = HIWORD(lParam);		//�}�E�X��Y���W���擾
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		g_MouseX = LOWORD(lParam);		//�}�E�X��X���W���擾
		g_MouseY = HIWORD(lParam);		//�}�E�X��Y���W���擾

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HRESULT Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏����̏�����
	Input::Init(hInstance, hWnd);

	return S_OK;
}

void Uninit(void)
{
	// ���͂̏I������
	Input::Uninit();
}

void Update(void)
{
	//���͌n�̍X�V����
	Input::Update();
}

long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}