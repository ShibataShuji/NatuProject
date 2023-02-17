
#include "stdafx.h"


void Result::Init()
{
	SetSceneName("Result");
	AddImageDisplayFull("asset/texture/Result_Tapioka.png");

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
