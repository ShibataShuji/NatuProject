
#include "stdafx.h"

//#include "main.h"
//#include "manager.h"
//#include "renderer.h"
//#include "scene.h"
//
//#include "Resultlogo.h"
////#include "Game.h"
//#include "Result.h"
//#include "Title.h"
//#include "input.h"
//#include "Game.h"
//#include <dinput.h>
//#include "scene.h"

void Result::Init()
{
	SetSceneName("Result");
	AddGameObject<Resultlogo>(2);
}

void Result::Uninit()
{
	Scene::Uninit();
}

void Result::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(DIK_RETURN))
	{
		Manager::SetScene<Title>();
	}
}
