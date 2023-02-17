
#include "stdafx.h"

Audio* CreateNewObjectScene_bgm;

void CreateNewObjectScene::Init()
{
	Scene::Init();

	// �n�ʂ̍�����0.0f�ɃZ�b�g
	SetGroundMinHeight(0.0f);


	SetSceneName("CreateNewObjectScene");

	Savedata* save = Manager::GetSavedata();
	Scene* scene = Manager::GetScene();

	// ���ԑ厖�` ���߂�A�R�����Ԃ����A�Q�����Ԃ���e
	m_Camera = AddGameObject<Camera>(0);
	m_Camera->SetCameraMode(5);			// ���_���E�N���������t���[���[�h��

	m_object = AddGameObject<OriginalBlock>(1);

	AddGameObject<Field>(1);

	CreateNewObjectScene_bgm = AddGameObject<Audio>(2);
	CreateNewObjectScene_bgm->Load("asset\\audio\\bgm.wav");
	CreateNewObjectScene_bgm->Play(true);

	// �R���W���������ׂČ�����悤�ɂ���
	scene->SetCollisionVisibility_GameObjectList(true);		// �z�u�ς݂̂��̂ɁB
	save->SetCollisionVisibility_LoadedObjectList(true);	// �ǂݍ��܂�Ă���̂ɁB

	// �S�ẴI�u�W�F�N�g�̊ȈՃo�E���f�B���O�{�b�N�X2D��\���I�t�ɂ��Ă���
	SetALLSimpleBoundingBox3DUseDraw(false);


}

void CreateNewObjectScene::Uninit()
{
	Scene::Uninit();		// �p�����̃N���X�̃��\�b�h�Ăяo��

}


void CreateNewObjectScene::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();
	Savedata* save = Manager::GetSavedata();

	bool backtitle = false;

	// �R���W�������X�|���X��GUI
	Renderer::Gui()->SaveData_CollisionResponseGUI();

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


	// GUI���g���ăI�u�W�F�N�g�̒��g��ύX���Ă���
	if (Renderer::Gui()->CreateNewObjectGUI(m_object))
	{
		// �^�C�g����ʂ��������Ȃ�A�S�ẴR���W�������݂��Ȃ��悤�ɖ߂��Ă���
		scene->SetCollisionVisibility_GameObjectList(false);		// �z�u�ς݂̂��̂ɁB
		save->SetCollisionVisibility_LoadedObjectList(false);	// �ǂݍ��܂�Ă���̂ɁB

		Manager::SetScene<Title>();
		return;
	}

	// �I�����Ă���I�u�W�F�N�g�̃o�E���f�B���O�{�b�N�X��\�������Ă���
	if (m_object != nullptr)
	{
		assert(m_object);
		m_object->SetSimpleBoundingBox3DUseDraw(true);
	}


	// �R���W�����̉�����؂�ւ���B
	if (Input::GetKeyTrigger(DIK_F1))
	{
		bool visility = !scene->GetCollision_Visility();
		scene->SetCollisionVisibility_GameObjectList(visility);		// �z�u�ς݂̂��̂ɁB
		save->SetCollisionVisibility_LoadedObjectList(visility);	// �ǂݍ��܂�Ă���̂ɁB
	}
}