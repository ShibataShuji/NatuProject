#pragma once


class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*			m_CameraBuffer;
	static ID3D11Buffer*			m_ParameterBuffer;
	static ID3D11Buffer*			m_SynthesizeColorBuffer;

	// 深度バッファ
	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	// 影用
	static ID3D11DepthStencilView* m_ShadowDepthStencilView;
	static ID3D11ShaderResourceView* m_ShadowDepthShaderResourceView;

	static Gui* m_Gui;




public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);

	static void SetCameraPosition(D3DXVECTOR3 CameraPosition);
	static void SetParameter(D3DXVECTOR4 Parameter);
	static void SetSynthesizeColorToSharder(D3DXVECTOR4 SynthesizeColor);

	//static ID3D11Device* GetDevice( void ){ return m_Device; }
	//static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	static void CreateVertexShader_Floor(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreateVertexShader_Floor2(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);


	static ID3D11ShaderResourceView* GetShadowDepthTexture()
	{
		return m_ShadowDepthShaderResourceView;
	}

	static void BeginDepth()
	{
		//シャドウバッファを深度バッファに設定し、内容を1で塗りつぶすしておく
		// 引き数の3のm_ShadowDepthStencilViewはレンダリングターゲットを設定できる。
		// 何も書かないとバックバッファがデフォルトで設定されている
		m_DeviceContext->OMSetRenderTargets(0, NULL, m_ShadowDepthStencilView);
		m_DeviceContext->ClearDepthStencilView(m_ShadowDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}


	static D3D_FEATURE_LEVEL GetFeatureLevel() {return m_FeatureLevel;}
	static ID3D11Device* GetDevice() {return m_Device;}
	static ID3D11DeviceContext* GetDeviceContext() {return m_DeviceContext;}
	static IDXGISwapChain* GetSwapChain() {return m_SwapChain;}
	static ID3D11RenderTargetView* GetRenderTargetView() {return m_RenderTargetView;}
	static ID3D11DepthStencilView* GetDepthStencilView() {return m_DepthStencilView;}

	static ID3D11Buffer* GetWorldBuffer() {return m_WorldBuffer;}
	static ID3D11Buffer* GetViewBuffer() {return m_ViewBuffer;}
	static ID3D11Buffer* GetProjectionBuffer() {return m_ProjectionBuffer;}
	static ID3D11Buffer* GetLightBuffer() {return m_LightBuffer;}

	static ID3D11DepthStencilState* GetDepthStateEnable() {return m_DepthStateEnable;}
	static ID3D11DepthStencilState* GetDepthStateDisable() {return m_DepthStateDisable;}



	// ImGui用
	static Gui* Gui(void) { return m_Gui; }
	static void OMSetRenderTargetsForImGui();
	static void ClearRenderTargetViewForImGui(const FLOAT ColorRGBA[4]);

	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();

	static void Gui_Setup(void* hwnd);
	static void Gui_NewFrame();
	static void Gui_Draw();
	static void Gui_Shutdown();

};
