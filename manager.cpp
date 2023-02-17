#include "stdafx.h"


Savedata* Manager::m_Savedata = nullptr;

Scene* Manager::m_Scene = nullptr;			// これは.hの方のやつで書いたようにもっかい呼ばなきゃいけないから、
									// SceneクラスのポインタのManagerクラスのメンバ変数m_Sceneをセットしている。

// てすとぽりごん
testPolygon* g_testPolygon;

void StringToChar64(std::string str, char* ch)
{
	std::memcpy(ch, str.c_str(), strlen(str.c_str()) + 1);	// string型->char[64]へ変換
}
void CharToString(char* ch, std::string* str)
{
	*str = std::string(ch);
}

void Manager::Init()
{
	Renderer::Init();
	Audio::InitMaster();		// staticメソッドなので::この呼び方

	SetScene<Title>();
	InitSavedata();

	// てすとぽりごん
	g_testPolygon = new testPolygon();
	g_testPolygon->Init();

}


void Manager::Uninit()
{
	// てすとぽりごん
	g_testPolygon->Uninit();
	delete g_testPolygon;

	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster();
	Renderer::Uninit();
	Renderer::Gui_Shutdown();
}

void Manager::Update()
{
	Input::Update();
	Renderer::Gui_NewFrame();

	// てすとぽりごん
	g_testPolygon->Update();

	m_Scene->Update();
}

void Manager::Draw()
{
	// ライトカメラのターゲットから離れている距離。離れているほど、プレイヤーが動いてもほかの影が動かなくなる。
	float SunLength = 200.0f;
	float PerspectiveFovMinZ = 180.0f;
	float PerspectiveFovMaxZ = 240.0f;
	D3DXVECTOR3 SunPosition;
	static float radian = M_PI_4;
	// 現在はとりあえずLキーを押すと時間が進むようになっている。
	if (Input::GetKeyPress(DIK_L))
	{
		radian += 0.01f;
	}

	// とりあえずz軸は固定で動く感じの太陽にする。
	SunPosition.x = cos(radian) * SunLength;
	SunPosition.y = sin(radian) * SunLength;
	SunPosition.z = 50.0f;

	// ライトカメラのビュー行列用変数(デフォルト。プレイヤーがいない場合ここになる)
	D3DXVECTOR3 Eye = D3DXVECTOR3(30.0f, 10.0f, -10.0f);
	D3DXVECTOR3 At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// Upは常に上方向でOK

	// プレイヤーの位置を中心にSunLength離れた場所にライトカメラを設置する
	GameObject* player = GetScene()->GetPlayerObject();
	Camera* camera = GetScene()->GetCameraObject();
	if (player != nullptr)
	{
		At = player->GetPosition();

		Eye = At + SunPosition;
	}
	else if (camera != nullptr)
	{
		At = camera->GetCameraForcusPoint();

		Eye = At + SunPosition;
	}



	//ライトカメラ構造体の初期化 最初はライト関連の数値をこの値にしておく
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//ライトカメラのビュー行列を作成
	D3DXMatrixLookAtLH(&light.ViewMatrix, &Eye, &At, &Up);
	//ライトカメラのプロジェクション行列を作成
	// 引き数の説明：pOut, 縦方向の視野角(基本1.0fでいい,変えると影がつぶれて伸びたりする)、縦横比(ウィンドウのアスペクト比)、一番近くのＺ値、一番遠くのＺ値
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, PerspectiveFovMinZ, PerspectiveFovMaxZ);

	//////////ライトカメラ構造体の初期化 最初はライト関連の数値をこの値にしておく
	////////LIGHT light;
	////////light.Enable = true;
	////////light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	////////D3DXVec4Normalize(&light.Direction, &light.Direction);
	////////light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	////////light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//////////ライトカメラのビュー行列を作成
	////////D3DXVECTOR3 Eye = D3DXVECTOR3(30.0f, 10.0f, -10.0f);
	////////D3DXVECTOR3 At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////////D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	////////D3DXMatrixLookAtLH(&light.ViewMatrix, &Eye, &At, &Up);
	//////////ライトカメラのプロジェクション行列を作成
	////////// 引き数の説明：pOut, 縦方向の視野角(基本1.0fでいい)、縦横比(ウィンドウサイズ)、一番近くのＺ値、一番遠くのＺ値
	////////D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 30.0f);

	//ライト情報をセット
	Renderer::SetLight(light);


	//*******１パス目 シャドウバッファの作成*******
	Renderer::BeginDepth();
	//ライトカメラの行列をセット
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
	//影を落としたいオブジェクトを描画
	m_Scene->ShadowDraw();	// UseShadowがtrueのオブジェクトのみ描写
	//m_Scene->Draw();

	//********2パス目 通常の描画********
	Renderer::Begin();

	m_Scene->Draw();

	light.Enable = false;
	Renderer::SetLight(light);

	Renderer::Gui_Draw();	// 3Dシーンの後にImGuiを描写

	//// てすとぽりごん スプライト描画（深度バッファの内容を可視化）
	//g_testPolygon->Draw();

	Renderer::End();// カメラは先！呼び出し順大時// 3D空間での表示// UI系は最後に。
	
}
