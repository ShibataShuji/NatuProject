
#include "stdafx.h"

//#include "CreateNewStageScene.h"
//#include "audio.h"
//#include "manager.cpp"
//#include "renderer.h"
//#include <dinput.h>
//#include "OriginalBlock.h"


Audio* CreateNewStageScene_bgm;

void CreateNewStageScene::Init()
{
	Scene::Init();

	

	// 変数の初期化
	{
		m_Mode = E_CreateStageScene_Mode::Object;

		m_PickupObject = nullptr;
		m_Brush = nullptr;
	}

	SetSceneName("CreateNewStageScene");

	// 順番大事～ かめら、３ｄおぶじぇ、２ｄおぶじぇe
	m_Camera = AddGameObject<Camera>(0);
	m_Camera->SetCameraMode(5);			// 視点を右クリ長押しフリーモードに

	AddGameObject<Field>(1);

	// ブラシのセット
	m_Brush = AddGameObject<Brush>(1);
	// とりあえず初期でブラシをリストに登録しておく						とりあえず初期でブラシをリストに登録しておく  
	m_Brush->SetPaintBrushSet(E_PaintBrushType::Ground001, 0, 0); 
	m_Brush->SetPaintBrushSet(E_PaintBrushType::Grass001, 0, 1);

	// 全てのオブジェクトの簡易バウンディングボックス2Dを表示オフにしておく
	SetALLSimpleBoundingBox3DUseDraw(false);

	//MeshField* meshfield = AddGameObject<MeshField>(1);
	//meshfield->SetAreaBlock(Int2(0, 0));
	//meshfield = AddGameObject<MeshField>(1);
	//meshfield->SetAreaBlock(Int2(0, 1));

	CreateNewStageScene_bgm = AddGameObject<Audio>(2);
	CreateNewStageScene_bgm->Load("asset\\audio\\bgm.wav");
	CreateNewStageScene_bgm->Play(true);

}

void CreateNewStageScene::Uninit()
{
	m_PickupObject = nullptr;
	m_Brush = nullptr;
	Scene::Uninit();		// 継承元のクラスのメソッド呼び出し

}


void CreateNewStageScene::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();
	Savedata* save = Manager::GetSavedata();




	bool backtitle = false;

	// ゲームセッティングのGUI
	{
		bool close = false;
		Renderer::Gui()->GameSettingGUI(false, m_Camera, &close, &backtitle);	// ここではcloseは使っていない
		if (backtitle)
		{
			// タイトルに戻るボタン押されたら
			// 全てのコリジョンをみえないように戻しておく
			scene->SetCollisionVisibility_GameObjectList(false);		// 配置済みのものに。
			save->SetCollisionVisibility_LoadedObjectList(false);	// 読み込まれてるものに。

			Manager::SetScene<Title>();
			return;
		}
	}


	bool addobject = false;
	int objectnum = 0;

	// 追加する系
	{
		// AddGameobjectするためにオブジェクトのリストをボタンで表示する。追加したらピックアップにセットする
		GameObject* tempGameObject = nullptr;
		tempGameObject = Renderer::Gui()->AddObjectToStage();
		if (tempGameObject != nullptr)
		{
			addobject = true;
			m_PickupObject = tempGameObject;
			D3DXVECTOR3 newpos;
			// 今の画面の中心の位置に座標を移動させる
			{
				// マウスをワールド座標に変換
				D3DXVECTOR3 MouseWorldOrigin;
				D3DXVECTOR3 MouseWorldDirection;
				POINT Sp;
				Sp.x = SCREEN_WIDTH * 0.5f;		// マウスカーソルがスクリーンの中心として計算する
				Sp.y = SCREEN_HEIGHT * 0.5f;
				Scene* scene = Manager::GetScene();
				Camera* camera = scene->GetGameObject<Camera>(0);
				camera->ScreenToWorld(&MouseWorldOrigin, &MouseWorldDirection, Sp.x, Sp.y);

				// マウスカーソルとメッシュフィールドオブジェクトとの衝突地点を取っている。
				newpos = m_Brush->CheckRayCollisionMeshFieldObject(60.0f, 10.0f, MouseWorldOrigin, MouseWorldDirection);
				m_PickupObject->SetPosition(newpos);
			}
		}

		// メッシュフィールドの追加
		Renderer::Gui()->AddMeshFieldObjectToStage();
	}

	static std::string PickUpName = "no select";
	{
		GameObject* tempGameObject = nullptr;
		bool BacktitleButton = false;
		{
			GameObject* tempGameObject = nullptr;
			tempGameObject = Renderer::Gui()->CreateNewStageGUI(&m_Mode, &BacktitleButton, &PickUpName, &objectnum, m_PickupObject);
			if (tempGameObject != nullptr)
				m_PickupObject = tempGameObject;
		}
		if (BacktitleButton)
		{
			// タイトルに戻るボタン押されたら
			// 全てのコリジョンをみえないように戻しておく
			scene->SetCollisionVisibility_GameObjectList(false);		// 配置済みのものに。
			save->SetCollisionVisibility_LoadedObjectList(false);	// 読み込まれてるものに。

			Manager::SetScene<Title>();
			return;
		}

		if (tempGameObject != nullptr)
			m_PickupObject = tempGameObject;

		if (addobject)
			PickUpName = "_" + to_string(objectnum - 1);
	}



	if (m_Mode == E_CreateStageScene_Mode::Object)
	{
		// マウスクリックでオブジェクトを選択する
		{
			if (Input::IsMouseLeftTriggered())
			{
				D3DXVECTOR3 q1, q2;
				GameObject* clickobject = GetCollisionRaySimpleBoundingBox3D(&q1, &q2);
				if (clickobject != nullptr)
					m_PickupObject = clickobject;
			}
		}

		// 現在選択しているゲームオブジェクトのオプションGUIの表示
		{
			bool destroy = false;
			Renderer::Gui()->CreateNewStage_PickupObjectGUI(m_PickupObject, PickUpName, &destroy);
			if (destroy)
			{
				m_PickupObject = nullptr;
			}
			// 選択しているオブジェクトのバウンディングボックスを表示させる
			if (m_PickupObject != nullptr)
				m_PickupObject->SetSimpleBoundingBox3DUseDraw(true);
		}
	}
	else if (m_Mode == E_CreateStageScene_Mode::Sculpt)
	{
		if (Input::GetKeyPress(DIK_Q) && !Input::IsMouseRightPressed())
		{
			m_Brush->AddStrengthSculptBrushState(-0.01f);
		}
		else if (Input::GetKeyPress(DIK_E) && !Input::IsMouseRightPressed())
		{
			m_Brush->AddStrengthSculptBrushState(0.01f);
		}
		// スカルプトブラシのオプションGUIの表示
		Renderer::Gui()->CreateNewStage_SculptBrushOptionGUI(m_Brush);
	}
	else if (m_Mode == E_CreateStageScene_Mode::Paint)
	{
		if (Input::GetKeyTrigger(DIK_E) && !Input::IsMouseRightPressed())
		{
			m_Brush->SetToEraserPaintBrushNow();	// 消しゴムにセット
		}
		else if (Input::GetKeyTrigger(DIK_Q) && !Input::IsMouseRightPressed())
		{
			m_Brush->SetToTempPaintBrushNow();	// 戻す
		}
		// ペイントブラシのオプションGUIの表示
		Renderer::Gui()->CreateNewStage_PaintBrushOptionGUI(m_Brush);
	}

	//if (Input::GetKeyTrigger(DIK_P))
	//{
	//	AddGameObject<OriginalBlock>(1);
	//}

	// コリジョンの可視性を切り替える。
	if (Input::GetKeyTrigger(DIK_F1))
	{
		bool visility = !scene->GetCollision_Visility();
		scene->SetCollisionVisibility_GameObjectList(visility);		// 配置済みのものに。
		save->SetCollisionVisibility_LoadedObjectList(visility);	// 読み込まれてるものに。
	}

	// キーでも切り替えできるようにする。右クリック長押し中に1,2,3キー
	if (Input::GetKeyTrigger(DIK_1) && Input::IsMouseRightPressed())
		m_Mode = E_CreateStageScene_Mode::Object;
	if (Input::GetKeyTrigger(DIK_2) && Input::IsMouseRightPressed())
		m_Mode = E_CreateStageScene_Mode::Sculpt;
	if (Input::GetKeyTrigger(DIK_3) && Input::IsMouseRightPressed())
		m_Mode = E_CreateStageScene_Mode::Paint;

	// モードを設定したことによる切り替え。ブラシの方でもモードを設定してあげる
	if (m_Mode == E_CreateStageScene_Mode::Object)
	{
		if(m_Brush != nullptr)
			m_Brush->SetBrushMode(E_CreateStageScene_Mode::Object);

	}
	else if (m_Mode == E_CreateStageScene_Mode::Sculpt)
	{
		if (m_Brush != nullptr)
			m_Brush->SetBrushMode(E_CreateStageScene_Mode::Sculpt);
	}
	else if (m_Mode == E_CreateStageScene_Mode::Paint)
	{
		if (m_Brush != nullptr)
			m_Brush->SetBrushMode(E_CreateStageScene_Mode::Paint);
	}



}