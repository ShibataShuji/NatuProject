#include "stdafx.h"

//#include "scene.h"
//#include "Title.h"
//#include "Titlelogo.h"
//#include <dinput.h>
//#include "input.h"
//#include "manager.h"
//#include "CreateNewObjectScene.h"
//#include "CreateNewStageScene.h"
//#include "StageSelectScene.h"

void Title::Init()
{
	SetSceneName("Title");
	AddGameObject<Titlelogo>(2);
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
