#include "stdafx.h"


void Title::Init()
{
	SetSceneName("Title");
	AddImageDisplayFull("asset/texture/SSE_Title.png");

}

void Title::Uninit()
{
	Scene::Uninit();
}

void Title::Update()
{
	Scene::Update();

	int selected = 0;
	selected = Renderer::Gui()->Title_SelectSeaneGUI();

	if (Input::GetKeyTrigger(DIK_Q) || selected == 1)
	{
		Manager::SetScene<CreateNewObjectScene>();
		return;
	}

	if (Input::GetKeyTrigger(DIK_E) || selected == 2)
	{
		Manager::SetScene<CreateNewStageScene>();
		return;
	}

	if (Input::GetKeyTrigger(DIK_RETURN) || selected == 3)
	{
		Manager::SetScene<StageSelectScene>();
		//Manager::SetScene<Game>();
		return;
	}

	if (selected == 4)
	{
		exit(0);
		return;
	}
}
