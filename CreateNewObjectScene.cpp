
#include "stdafx.h"

Audio* CreateNewObjectScene_bgm;

void CreateNewObjectScene::Init()
{
	Scene::Init();

	// 地面の高さを0.0fにセット
	SetGroundMinHeight(0.0f);


	SetSceneName("CreateNewObjectScene");

	Savedata* save = Manager::GetSavedata();
	Scene* scene = Manager::GetScene();

	// 順番大事～ かめら、３ｄおぶじぇ、２ｄおぶじぇe
	m_Camera = AddGameObject<Camera>(0);
	m_Camera->SetCameraMode(5);			// 視点を右クリ長押しフリーモードに

	m_object = AddGameObject<OriginalBlock>(1);

	AddGameObject<Field>(1);

	CreateNewObjectScene_bgm = AddGameObject<Audio>(2);
	CreateNewObjectScene_bgm->Load("asset\\audio\\bgm.wav");
	CreateNewObjectScene_bgm->Play(true);

	// コリジョンをすべて見えるようにする
	scene->SetCollisionVisibility_GameObjectList(true);		// 配置済みのものに。
	save->SetCollisionVisibility_LoadedObjectList(true);	// 読み込まれてるものに。

	// 全てのオブジェクトの簡易バウンディングボックス2Dを表示オフにしておく
	SetALLSimpleBoundingBox3DUseDraw(false);


}

void CreateNewObjectScene::Uninit()
{
	Scene::Uninit();		// 継承元のクラスのメソッド呼び出し

}


void CreateNewObjectScene::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();
	Savedata* save = Manager::GetSavedata();

	bool backtitle = false;

	// コリジョンレスポンスのGUI
	Renderer::Gui()->SaveData_CollisionResponseGUI();

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


	// GUIを使ってオブジェクトの中身を変更している
	if (Renderer::Gui()->CreateNewObjectGUI(m_object))
	{
		// タイトル画面を押したなら、全てのコリジョンをみえないように戻しておく
		scene->SetCollisionVisibility_GameObjectList(false);		// 配置済みのものに。
		save->SetCollisionVisibility_LoadedObjectList(false);	// 読み込まれてるものに。

		Manager::SetScene<Title>();
		return;
	}

	// 選択しているオブジェクトのバウンディングボックスを表示させておく
	if (m_object != nullptr)
	{
		assert(m_object);
		m_object->SetSimpleBoundingBox3DUseDraw(true);
	}


	// コリジョンの可視性を切り替える。
	if (Input::GetKeyTrigger(DIK_F1))
	{
		bool visility = !scene->GetCollision_Visility();
		scene->SetCollisionVisibility_GameObjectList(visility);		// 配置済みのものに。
		save->SetCollisionVisibility_LoadedObjectList(visility);	// 読み込まれてるものに。
	}
}