
#include "stdafx.h"

//#include "CreateNewStageScene.h"
//#include "audio.h"
//#include "manager.cpp"
//#include "renderer.h"
//#include <dinput.h>
//#include "OriginalBlock.h"


Audio* CreateNewStageScene_bgm;

void CreateNewStageScene::Init()
{
	Scene::Init();

	

	// �ϐ��̏�����
	{
		m_Mode = E_CreateStageScene_Mode::Object;

		m_PickupObject = nullptr;
		m_Brush = nullptr;
	}

	SetSceneName("CreateNewStageScene");

	// ���ԑ厖�` ���߂�A�R�����Ԃ����A�Q�����Ԃ���e
	m_Camera = AddGameObject<Camera>(0);
	m_Camera->SetCameraMode(5);			// ���_���E�N���������t���[���[�h��

	AddGameObject<Field>(1);

	// �u���V�̃Z�b�g
	m_Brush = AddGameObject<Brush>(1);
	// �Ƃ肠���������Ńu���V�����X�g�ɓo�^���Ă���						�Ƃ肠���������Ńu���V�����X�g�ɓo�^���Ă���  
	m_Brush->SetPaintBrushSet(E_PaintBrushType::Ground001, 0, 0); 
	m_Brush->SetPaintBrushSet(E_PaintBrushType::Grass001, 0, 1);

	// �S�ẴI�u�W�F�N�g�̊ȈՃo�E���f�B���O�{�b�N�X2D��\���I�t�ɂ��Ă���
	SetALLSimpleBoundingBox3DUseDraw(false);

	//MeshField* meshfield = AddGameObject<MeshField>(1);
	//meshfield->SetAreaBlock(Int2(0, 0));
	//meshfield = AddGameObject<MeshField>(1);
	//meshfield->SetAreaBlock(Int2(0, 1));

	CreateNewStageScene_bgm = AddGameObject<Audio>(2);
	CreateNewStageScene_bgm->Load("asset\\audio\\bgm.wav");
	CreateNewStageScene_bgm->Play(true);

}

void CreateNewStageScene::Uninit()
{
	m_PickupObject = nullptr;
	m_Brush = nullptr;
	Scene::Uninit();		// �p�����̃N���X�̃��\�b�h�Ăяo��

}


void CreateNewStageScene::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();
	Savedata* save = Manager::GetSavedata();




	bool backtitle = false;

	// �Q�[���Z�b�e�B���O��GUI
	{
		bool close = false;
		Renderer::Gui()->GameSettingGUI(false, m_Camera, &close, &backtitle);	// �����ł�close�͎g���Ă��Ȃ�
		if (backtitle)
		{
			// �^�C�g���ɖ߂�{�^�������ꂽ��
			// �S�ẴR���W�������݂��Ȃ��悤�ɖ߂��Ă���
			scene->SetCollisionVisibility_GameObjectList(false);		// �z�u�ς݂̂��̂ɁB
			save->SetCollisionVisibility_LoadedObjectList(false);	// �ǂݍ��܂�Ă���̂ɁB

			Manager::SetScene<Title>();
			return;
		}
	}


	bool addobject = false;
	int objectnum = 0;

	// �ǉ�����n
	{
		// AddGameobject���邽�߂ɃI�u�W�F�N�g�̃��X�g���{�^���ŕ\������B�ǉ�������s�b�N�A�b�v�ɃZ�b�g����
		GameObject* tempGameObject = nullptr;
		tempGameObject = Renderer::Gui()->AddObjectToStage();
		if (tempGameObject != nullptr)
		{
			addobject = true;
			m_PickupObject = tempGameObject;
			D3DXVECTOR3 newpos;
			// ���̉�ʂ̒��S�̈ʒu�ɍ��W���ړ�������
			{
				// �}�E�X�����[���h���W�ɕϊ�
				D3DXVECTOR3 MouseWorldOrigin;
				D3DXVECTOR3 MouseWorldDirection;
				POINT Sp;
				Sp.x = SCREEN_WIDTH * 0.5f;		// �}�E�X�J�[�\�����X�N���[���̒��S�Ƃ��Čv�Z����
				Sp.y = SCREEN_HEIGHT * 0.5f;
				Scene* scene = Manager::GetScene();
				Camera* camera = scene->GetGameObject<Camera>(0);
				camera->ScreenToWorld(&MouseWorldOrigin, &MouseWorldDirection, Sp.x, Sp.y);

				// �}�E�X�J�[�\���ƃ��b�V���t�B�[���h�I�u�W�F�N�g�Ƃ̏Փ˒n�_������Ă���B
				newpos = m_Brush->CheckRayCollisionMeshFieldObject(60.0f, 10.0f, MouseWorldOrigin, MouseWorldDirection);
				m_PickupObject->SetPosition(newpos);
			}
		}

		// ���b�V���t�B�[���h�̒ǉ�
		Renderer::Gui()->AddMeshFieldObjectToStage();
	}

	static std::string PickUpName = "no select";
	{
		GameObject* tempGameObject = nullptr;
		bool BacktitleButton = false;
		{
			GameObject* tempGameObject = nullptr;
			tempGameObject = Renderer::Gui()->CreateNewStageGUI(&m_Mode, &BacktitleButton, &PickUpName, &objectnum, m_PickupObject);
			if (tempGameObject != nullptr)
				m_PickupObject = tempGameObject;
		}
		if (BacktitleButton)
		{
			// �^�C�g���ɖ߂�{�^�������ꂽ��
			// �S�ẴR���W�������݂��Ȃ��悤�ɖ߂��Ă���
			scene->SetCollisionVisibility_GameObjectList(false);		// �z�u�ς݂̂��̂ɁB
			save->SetCollisionVisibility_LoadedObjectList(false);	// �ǂݍ��܂�Ă���̂ɁB

			Manager::SetScene<Title>();
			return;
		}

		if (tempGameObject != nullptr)
			m_PickupObject = tempGameObject;

		if (addobject)
			PickUpName = "_" + to_string(objectnum - 1);
	}



	if (m_Mode == E_CreateStageScene_Mode::Object)
	{
		// �}�E�X�N���b�N�ŃI�u�W�F�N�g��I������
		{
			if (Input::IsMouseLeftTriggered())
			{
				D3DXVECTOR3 q1, q2;
				GameObject* clickobject = GetCollisionRaySimpleBoundingBox3D(&q1, &q2);
				if (clickobject != nullptr)
					m_PickupObject = clickobject;
			}
		}

		// ���ݑI�����Ă���Q�[���I�u�W�F�N�g�̃I�v�V����GUI�̕\��
		{
			bool destroy = false;
			Renderer::Gui()->CreateNewStage_PickupObjectGUI(m_PickupObject, PickUpName, &destroy);
			if (destroy)
			{
				m_PickupObject = nullptr;
			}
			// �I�����Ă���I�u�W�F�N�g�̃o�E���f�B���O�{�b�N�X��\��������
			if (m_PickupObject != nullptr)
				m_PickupObject->SetSimpleBoundingBox3DUseDraw(true);
		}
	}
	else if (m_Mode == E_CreateStageScene_Mode::Sculpt)
	{
		if (Input::GetKeyPress(DIK_Q) && !Input::IsMouseRightPressed())
		{
			m_Brush->AddStrengthSculptBrushState(-0.01f);
		}
		else if (Input::GetKeyPress(DIK_E) && !Input::IsMouseRightPressed())
		{
			m_Brush->AddStrengthSculptBrushState(0.01f);
		}
		// �X�J���v�g�u���V�̃I�v�V����GUI�̕\��
		Renderer::Gui()->CreateNewStage_SculptBrushOptionGUI(m_Brush);
	}
	else if (m_Mode == E_CreateStageScene_Mode::Paint)
	{
		if (Input::GetKeyTrigger(DIK_E) && !Input::IsMouseRightPressed())
		{
			m_Brush->SetToEraserPaintBrushNow();	// �����S���ɃZ�b�g
		}
		else if (Input::GetKeyTrigger(DIK_Q) && !Input::IsMouseRightPressed())
		{
			m_Brush->SetToTempPaintBrushNow();	// �߂�
		}
		// �y�C���g�u���V�̃I�v�V����GUI�̕\��
		Renderer::Gui()->CreateNewStage_PaintBrushOptionGUI(m_Brush);
	}

	//if (Input::GetKeyTrigger(DIK_P))
	//{
	//	AddGameObject<OriginalBlock>(1);
	//}

	// �R���W�����̉�����؂�ւ���B
	if (Input::GetKeyTrigger(DIK_F1))
	{
		bool visility = !scene->GetCollision_Visility();
		scene->SetCollisionVisibility_GameObjectList(visility);		// �z�u�ς݂̂��̂ɁB
		save->SetCollisionVisibility_LoadedObjectList(visility);	// �ǂݍ��܂�Ă���̂ɁB
	}

	// �L�[�ł��؂�ւ��ł���悤�ɂ���B�E�N���b�N����������1,2,3�L�[
	if (Input::GetKeyTrigger(DIK_1) && Input::IsMouseRightPressed())
		m_Mode = E_CreateStageScene_Mode::Object;
	if (Input::GetKeyTrigger(DIK_2) && Input::IsMouseRightPressed())
		m_Mode = E_CreateStageScene_Mode::Sculpt;
	if (Input::GetKeyTrigger(DIK_3) && Input::IsMouseRightPressed())
		m_Mode = E_CreateStageScene_Mode::Paint;

	// ���[�h��ݒ肵�����Ƃɂ��؂�ւ��B�u���V�̕��ł����[�h��ݒ肵�Ă�����
	if (m_Mode == E_CreateStageScene_Mode::Object)
	{
		if(m_Brush != nullptr)
			m_Brush->SetBrushMode(E_CreateStageScene_Mode::Object);

	}
	else if (m_Mode == E_CreateStageScene_Mode::Sculpt)
	{
		if (m_Brush != nullptr)
			m_Brush->SetBrushMode(E_CreateStageScene_Mode::Sculpt);
	}
	else if (m_Mode == E_CreateStageScene_Mode::Paint)
	{
		if (m_Brush != nullptr)
			m_Brush->SetBrushMode(E_CreateStageScene_Mode::Paint);
	}



}