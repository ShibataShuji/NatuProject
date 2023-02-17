#include "stdafx.h"

Audio* bgm;

void Game::Init()
{
	Scene::Init();	// 親クラスの初期化

	SetSceneName("Game");

	Savedata* savedata = Manager::GetSavedata();
	Scene* scene = Manager::GetScene();

	m_OpenGameSettingGUI = false;

	// 現在読み込んでいるステージデータの通りにブロックを配置する
	savedata->AddObjectForLoadedStage();
	
	m_Camera = AddGameObject<Camera>(0);

	AddGameObject<Sky>(1);

	// "Player"のオブジェクトを探して配置する
	GameObject* player = savedata->AddObjectForLoadedObjectWithName("Player");
	SetPlayerObject(player);

	// スタート地点を探す。あったらそこにプレイヤーをセットする。
	GameObject* playerstart = scene->GetGameObjectWithName(1, "Player_Start");
	if (playerstart != nullptr)
	{
		D3DXVECTOR3 StartPos = playerstart->GetPosition();
		StartPos.y += 1.2f;
		player->SetPosition(StartPos);
		player->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	for (int i = 0; i < 20; i++)
	{
		D3DXVECTOR3 randpos;
		//(float)rand() / RAND_MAX;		// 0~1がでてくる

		randpos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		randpos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		randpos.y = 0.0f;

		if (randpos.x < 9.0f && randpos.x > -9.0f)
			randpos.x = 100;
		if (randpos.z < 9.0f && randpos.z > -9.0f)
			randpos.z = 100;

		//AddGameObject<Tree>(1)->SetPosition(randpos);
		AddGameObject<OriginalBlock>(1)->SetPosition(randpos);

	}


	bgm = AddGameObject<Audio>(2);
	bgm->Load("asset\\audio\\bgm.wav");
	bgm->Play(true);		// 引き数のtrueはループするかしないか。何も入れないとデフォルトでfalse

	m_Camera->SetFollowObject(player);
	m_Camera->SetCameraMode(4);

	// 全てのオブジェクトの簡易バウンディングボックス2Dを表示オフにしておく
	SetALLSimpleBoundingBox3DUseDraw(false);


}

void Game::Uninit()
{
	Scene::Uninit();		// 継承元のクラスのメソッド呼び出し。ここでゲームオブジェクトのUninit
}

void Game::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();
	Savedata* save = Manager::GetSavedata();

	bool backtitle = false;
	// GameSettingGUIを開いているかどうか
	{
		bool close = false;
		if (m_OpenGameSettingGUI)
		{
			Renderer::Gui()->GameSettingGUI(true, m_Camera, &close, &backtitle);
			if (close)
			{
				m_Camera->SetCameraMode(4);	// カメラを戻す
				m_OpenGameSettingGUI = false;
			}
		}
		// Pキーでゲームセッティングを開閉できる。
		if (Input::GetKeyTrigger(DIK_P))
		{
			if (!m_OpenGameSettingGUI)
			{
				m_Camera->SetCameraMode(6);	// カメラを固定しておく
				m_OpenGameSettingGUI = true;
			}
			else
			{
				m_Camera->SetCameraMode(4);	// カメラを戻す
				m_OpenGameSettingGUI = false;
			}
		}
	}
	if (backtitle)
	{
		// タイトルに戻るボタン押されたら
		// 全てのコリジョンをみえないように戻しておく
		scene->SetCollisionVisibility_GameObjectList(false);		// 配置済みのものに。
		save->SetCollisionVisibility_LoadedObjectList(false);	// 読み込まれてるものに。

		Manager::SetScene<Title>();
		return;
	}

	// コリジョンの可視性を切り替える。
	if (Input::GetKeyTrigger(DIK_F1))
	{
		bool visility = !scene->GetCollision_Visility();
		scene->SetCollisionVisibility_GameObjectList(visility);		// 配置済みのものに。
		save->SetCollisionVisibility_LoadedObjectList(visility);	// 読み込まれてるものに。
	}

	// fps見るよう
	//Renderer::Gui()->testGui();

	// BGM
		/*if (Input::GetKeyPress('E'))
		{
			bgm->Stop();
		}*/
		//if (Input::GetKeyPress('E'))
		//{
		//	bgm->SetFadeout(120);
		//}

		//if (Input::GetKeyPress('Q'))
		//{
		//	bgm->SetFadein(120);
		//}

		//if (Input::GetKeyPress('R'))
		//{
		//	bgm->SetVolume(2.0f);
		//}
}