#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"

#include "Titlelogo.h"
//#include "Game.h"
#include "Title.h"
#include "input.h"
#include "Game.h"

void Title::Init()
{
	AddGameObject<Titlelogo>(2);
}

void Title::Uninit()
{
	Scene::Uninit();
}

void Title::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Game>();
	}
}
