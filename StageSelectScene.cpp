#include "stdafx.h"

//#include "StageSelectScene.h"
//#include "scene.h"
//#include "Savedata.h"
//#include "manager.h"
//#include "renderer.h"
//#include <string>
//#include "manager.h"
//#include "Title.h"
//#include "Game.h"

void StageSelectScene::Init()
{
	SetSceneName("StageSelectScene");
	AddImageDisplayFull("asset/texture/SSE_StageSelect.png");

}

void StageSelectScene::Uninit()
{
	Scene::Uninit();
}

void StageSelectScene::Update()
{
	Scene::Update();

	Savedata* save = Manager::GetSavedata();

	std::string stagename;

	if (Renderer::Gui()->StageSelectScene_StageSelect(&stagename))
	{
		// Closeボタンを押されたらタイトルに戻る
		if (stagename == std::string("BackTitle"))
		{
			// シーンをタイトルにセットしてreturnする。returnしないとこのUpdate内で別のが呼ばれたらそっちになっちゃう
			Manager::SetScene<Title>();
			return;
		}

		// ここではロードだけしておく。配置は先のシーンのInitとかで
		// save->AddObjectForLoadedStage();をすることによって出来る。
		save->LoadStage(stagename);

		Manager::SetScene<Game>();
	}



	//if (Input::GetKeyTrigger(DIK_1))
	//{
	//	Manager::SetScene<CreateNewObjectScene>();
	//}

	//if (Input::GetKeyTrigger(DIK_2))
	//{
	//	Manager::SetScene<CreateNewStageScene>();
	//}

	//if (Input::GetKeyTrigger(DIK_RETURN))
	//{
	//	Manager::SetScene<Game>();
	//}
}
