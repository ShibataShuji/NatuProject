
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
	Scene::Init();	// �e�N���X�̏�����

	SetSceneName("Game");

	Savedata* savedata = Manager::GetSavedata();
	Scene* scene = Manager::GetScene();

	m_OpenGameSettingGUI = false;

	Bullet::Load();

	Enemy::Load();
	Platform::Load();

	// ���ݓǂݍ���ł���X�e�[�W�f�[�^�̒ʂ�Ƀu���b�N��z�u����
	savedata->AddObjectForLoadedStage();
	

	// ���ԑ厖�` ���߂�A�R�����Ԃ����A�Q�����Ԃ����@->�������Ő����n�����Ƃɂ������ς�
	// ���ԑ厖�` ���߂�A�R�����Ԃ����A�Q�����Ԃ���e
	m_Camera = AddGameObject<Camera>(0);
	//m_Camera->SetFollowObject(player);
	//m_Camera->SetCameraMode(5);			// ���_���E�N���������t���[���[�h��

	//AddGameObject<Field>(1);
	AddGameObject<Sky>(1);

	//AddGameObject<Player>(1);

	// "Player"�̃I�u�W�F�N�g��T���Ĕz�u����
	GameObject* player = savedata->AddObjectForLoadedObjectWithName("Player");
	SetPlayerObject(player);

	// �X�^�[�g�n�_��T���B�������炻���Ƀv���C���[���Z�b�g����B
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
		//(float)rand() / RAND_MAX;		// 0~1���łĂ���

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

	//// �Â����̃��b�V���t�B�[���h
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
	// 2D�̌�ɂ�3D�͕\���ł��Ȃ��G�G

	//AddGameObject<Polygon2D>(2);


	bgm = AddGameObject<Audio>(2);
	bgm->Load("asset\\audio\\bgm.wav");
	bgm->Play(true);		// ��������true�̓��[�v���邩���Ȃ����B��������Ȃ��ƃf�t�H���g��false

	m_Camera->SetFollowObject(player);
	m_Camera->SetCameraMode(4);

	// �S�ẴI�u�W�F�N�g�̊ȈՃo�E���f�B���O�{�b�N�X2D��\���I�t�ɂ��Ă���
	SetALLSimpleBoundingBox3DUseDraw(false);


}

void Game::Uninit()
{
	Scene::Uninit();		// �p�����̃N���X�̃��\�b�h�Ăяo���B�����ŃQ�[���I�u�W�F�N�g��Uninit

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
	// GameSettingGUI���J���Ă��邩�ǂ���
	{
		bool close = false;
		if (m_OpenGameSettingGUI)
		{
			Renderer::Gui()->GameSettingGUI(true, m_Camera, &close, &backtitle);
			if (close)
			{
				m_Camera->SetCameraMode(4);	// �J������߂�
				m_OpenGameSettingGUI = false;
			}
		}
		// P�L�[�ŃQ�[���Z�b�e�B���O���J�ł���B
		if (Input::GetKeyTrigger(DIK_P))
		{
			if (!m_OpenGameSettingGUI)
			{
				m_Camera->SetCameraMode(6);	// �J�������Œ肵�Ă���
				m_OpenGameSettingGUI = true;
			}
			else
			{
				m_Camera->SetCameraMode(4);	// �J������߂�
				m_OpenGameSettingGUI = false;
			}
		}
	}
	if (backtitle)
	{
		// �^�C�g���ɖ߂�{�^�������ꂽ��
		// �S�ẴR���W�������݂��Ȃ��悤�ɖ߂��Ă���
		scene->SetCollisionVisibility_GameObjectList(false);		// �z�u�ς݂̂��̂ɁB
		save->SetCollisionVisibility_LoadedObjectList(false);	// �ǂݍ��܂�Ă���̂ɁB

		Manager::SetScene<Title>();
		return;
	}

	// �R���W�����̉�����؂�ւ���B
	if (Input::GetKeyTrigger(DIK_F1))
	{
		bool visility = !scene->GetCollision_Visility();
		scene->SetCollisionVisibility_GameObjectList(visility);		// �z�u�ς݂̂��̂ɁB
		save->SetCollisionVisibility_LoadedObjectList(visility);	// �ǂݍ��܂�Ă���̂ɁB
	}

	// fps����悤
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

	// �����ŃV�[���̑@�ۂ�����
	// ��������ƃV�[���̃A�b�v�f�[�g���I����Ă���
	// �V�[���̑J�ڂ��s����

	// �v���C���[�����Ȃ��Ȃ����`


	//// �G�L���������Ȃ��Ȃ�����`
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