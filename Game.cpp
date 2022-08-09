#include <random>
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"


#include "camera.h"
#include "field.h"
#include "model.h"
#include "player.h"
#include "field.h"
#include "Enemy.h"
#include "Platform.h"
#include "Bullet.h"
#include "Sky.h"
#include "Explosion.h"
#include "Tree.h"
#include "polygon2D.h"
#include "Game.h"
#include "Title.h"
#include "Result.h"
#include "audio.h"
#include "Score.h"

Audio* bgm;

void Game::Init()
{
	Bullet::Load();

	Enemy::Load();
	Platform::Load();

	// 順番大事～ かめら、３ｄおぶじぇ、２ｄおぶじぇe
	AddGameObject<Camera>(0);

	AddGameObject<Field>(1);
	AddGameObject<Sky>(1);
	// どうにかしてプレイヤーのポインターを持っておきたい、グローバル変数とかで。
	//T p_Player = AddGameObject<Player>(1);
	AddGameObject<Player>(1);
	Enemy* p_Enemy_0 = AddGameObject<Enemy>(1); 
	p_Enemy_0->SetPosition(D3DXVECTOR3(2.0f, 1.0f, 5.0f));
	//p_Enemy_0->SetScaleRate(D3DXVECTOR3(2.0f, 5.0f, 2.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 5.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(3.0f, 1.0f, 5.0f));

	Platform* platform = AddGameObject<Platform>(1);
	platform->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 10.0f));
	platform->SetInitScale(D3DXVECTOR3(3.0f, 1.0f, 3.0f));

	for (int i = 0; i < 20; i++)
	{
		D3DXVECTOR3 randpos;
		//(float)rand() / RAND_MAX;		// 0~1がでてくる

		randpos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		randpos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		randpos.y = 0.0f;
		AddGameObject<Tree>(1)->SetPosition(randpos);
		//randpos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		//randpos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		//AddGameObject<Platform>(1)->SetPosition(randpos);
	}


	AddGameObject<Score>(2);
	// 2Dの後には3Dは表示できない；；



	bgm = AddGameObject<Audio>(2);
	bgm->Load("asset\\audio\\bgm.wav");
	bgm->Play(true);		// 引き数のtrueはループするかしないか。何も入れないとデフォルトでfalse


}

void Game::Uninit()
{
	Scene::Uninit();		// 継承元のクラスのメソッド呼び出し
	Bullet::Unload();
	Enemy::Unload();
	Platform::Unload();
}

void Game::Update()
{
	Scene::Update();

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
	//Scene* scene = Manager::GetScene();
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