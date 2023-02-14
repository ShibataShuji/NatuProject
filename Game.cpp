
#include "stdafx.h"

//#include "main.h"
//#include "manager.h"
//#include "renderer.h"
//#include "game.h"
//#include "camera.h"
//#include "scene.h"
//#include <dinput.h>
//#include "main.h"
//#include "input.h"
//#include <D3D11.h>
//#include <D3DX11.h>
//#include <cassert>
//#include "audio.h"
//#include "Bullet.h"
//#include "Platform.h"
//#include "Enemy.h"
//#include "Savedata.h"
//#include "field.h"
//#include "Sky.h"
//#include "OriginalBlock.h"
//#include "Score.h"
//#include <d3dx9.h>

Audio* bgm;

void Game::Init()
{
	Scene::Init();	// 親クラスの初期化

	SetSceneName("Game");

	Savedata* savedata = Manager::GetSavedata();
	Scene* scene = Manager::GetScene();

	m_OpenGameSettingGUI = false;

	Bullet::Load();

	Enemy::Load();
	Platform::Load();

	// 現在読み込んでいるステージデータの通りにブロックを配置する
	savedata->AddObjectForLoadedStage();
	

	// 順番大事～ かめら、３ｄおぶじぇ、２ｄおぶじぇ　->引き数で数字渡すことにより解決済み
	// 順番大事～ かめら、３ｄおぶじぇ、２ｄおぶじぇe
	m_Camera = AddGameObject<Camera>(0);
	//m_Camera->SetFollowObject(player);
	//m_Camera->SetCameraMode(5);			// 視点を右クリ長押しフリーモードに

	//AddGameObject<Field>(1);
	AddGameObject<Sky>(1);

	//AddGameObject<Player>(1);

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
	}

	//std::string BlockName = "Block";
	//savedata->AddObjectForLoadedObjectWithName(BlockName);


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

	//// 古い方のメッシュフィールド
	//MeshField* meshfield = AddGameObject<MeshField>(1);
	//meshfield->SetAreaBlock(Int2(0, 0));
	//meshfield = AddGameObject<MeshField>(1);
	//meshfield->SetAreaBlock(Int2(0, 1));
	//AddGameObject<Brush>(1);

	/*AddMeshFieldObject(Int2(-1, 0));
	AddMeshFieldObject(Int2(0, 0));
	AddMeshFieldObject(Int2(-1, -1));
	AddMeshFieldObject(Int2(-1, -1));*/

	//AddGameObject<Brush>(1);

	AddGameObject<Score>(2);
	// 2Dの後には3Dは表示できない；；

	//AddGameObject<Polygon2D>(2);


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

	Bullet::Unload();
	Enemy::Unload();
	Platform::Unload();
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


	/*if (Input::GetKeyPress('E'))
	{
		bgm->Stop();
	}*/

	if (Input::GetKeyPress('E'))
	{
		bgm->SetFadeout(120);
	}

	if (Input::GetKeyPress('Q'))
	{
		bgm->SetFadein(120);
	}

	if (Input::GetKeyPress('R'))
	{
		bgm->SetVolume(2.0f);
	}

	// ここでシーンの繊維を書く
	// そうするとシーンのアップデートが終わってから
	// シーンの遷移が行われる

	// プレイヤーがいなくなった～


	//// 敵キャラがいなくなったら～
	//std::vector<Enemy*> enemys = scene->GetGameObjects<Enemy>(1);
	//int AliveEnemyNum = 0;
	//for (auto& i : enemys)
	//{
	//	AliveEnemyNum++;
	//}
	//if (AliveEnemyNum == 0)
	//{
	//	Manager::SetScene<Result>();
	//}

	//if (Input::GetKeyTrigger(VK_RETURN))
	//{
	//	Manager::SetScene<Result>();
	//}
}