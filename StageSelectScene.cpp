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
		// Close�{�^���������ꂽ��^�C�g���ɖ߂�
		if (stagename == std::string("BackTitle"))
		{
			// �V�[�����^�C�g���ɃZ�b�g����return����Breturn���Ȃ��Ƃ���Update���ŕʂ̂��Ă΂ꂽ�炻�����ɂȂ����Ⴄ
			Manager::SetScene<Title>();
			return;
		}

		// �����ł̓��[�h�������Ă����B�z�u�͐�̃V�[����Init�Ƃ���
		// save->AddObjectForLoadedStage();�����邱�Ƃɂ���ďo����B
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
