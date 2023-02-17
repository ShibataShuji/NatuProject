#include "stdafx.h"

Audio* bgm;

void Game::Init()
{
	Scene::Init();	// �e�N���X�̏�����

	SetSceneName("Game");

	Savedata* savedata = Manager::GetSavedata();
	Scene* scene = Manager::GetScene();

	m_OpenGameSettingGUI = false;

	// ���ݓǂݍ���ł���X�e�[�W�f�[�^�̒ʂ�Ƀu���b�N��z�u����
	savedata->AddObjectForLoadedStage();
	
	m_Camera = AddGameObject<Camera>(0);

	AddGameObject<Sky>(1);

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
		player->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

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

	// BGM
		/*if (Input::GetKeyPress('E'))
		{
			bgm->Stop();
		}*/
		//if (Input::GetKeyPress('E'))
		//{
		//	bgm->SetFadeout(120);
		//}

		//if (Input::GetKeyPress('Q'))
		//{
		//	bgm->SetFadein(120);
		//}

		//if (Input::GetKeyPress('R'))
		//{
		//	bgm->SetVolume(2.0f);
		//}
}