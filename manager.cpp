#include "stdafx.h"


Savedata* Manager::m_Savedata = nullptr;

Scene* Manager::m_Scene = nullptr;			// �����.h�̕��̂�ŏ������悤�ɂ��������Ă΂Ȃ��Ⴂ���Ȃ�����A
									// Scene�N���X�̃|�C���^��Manager�N���X�̃����o�ϐ�m_Scene���Z�b�g���Ă���B

// �Ă��Ƃۂ育��
testPolygon* g_testPolygon;

void StringToChar64(std::string str, char* ch)
{
	std::memcpy(ch, str.c_str(), strlen(str.c_str()) + 1);	// string�^->char[64]�֕ϊ�
}
void CharToString(char* ch, std::string* str)
{
	*str = std::string(ch);
}

void Manager::Init()
{
	Renderer::Init();
	Audio::InitMaster();		// static���\�b�h�Ȃ̂�::���̌Ăѕ�

	SetScene<Title>();
	InitSavedata();

	// �Ă��Ƃۂ育��
	g_testPolygon = new testPolygon();
	g_testPolygon->Init();

}


void Manager::Uninit()
{
	// �Ă��Ƃۂ育��
	g_testPolygon->Uninit();
	delete g_testPolygon;

	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster();
	Renderer::Uninit();
	Renderer::Gui_Shutdown();
}

void Manager::Update()
{
	Input::Update();
	Renderer::Gui_NewFrame();

	// �Ă��Ƃۂ育��
	g_testPolygon->Update();

	m_Scene->Update();
}

void Manager::Draw()
{
	// ���C�g�J�����̃^�[�Q�b�g���痣��Ă��鋗���B����Ă���قǁA�v���C���[�������Ă��ق��̉e�������Ȃ��Ȃ�B
	float SunLength = 200.0f;
	float PerspectiveFovMinZ = 180.0f;
	float PerspectiveFovMaxZ = 240.0f;
	D3DXVECTOR3 SunPosition;
	static float radian = M_PI_4;
	// ���݂͂Ƃ肠����L�L�[�������Ǝ��Ԃ��i�ނ悤�ɂȂ��Ă���B
	if (Input::GetKeyPress(DIK_L))
	{
		radian += 0.01f;
	}

	// �Ƃ肠����z���͌Œ�œ��������̑��z�ɂ���B
	SunPosition.x = cos(radian) * SunLength;
	SunPosition.y = sin(radian) * SunLength;
	SunPosition.z = 50.0f;

	// ���C�g�J�����̃r���[�s��p�ϐ�(�f�t�H���g�B�v���C���[�����Ȃ��ꍇ�����ɂȂ�)
	D3DXVECTOR3 Eye = D3DXVECTOR3(30.0f, 10.0f, -10.0f);
	D3DXVECTOR3 At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// Up�͏�ɏ������OK

	// �v���C���[�̈ʒu�𒆐S��SunLength���ꂽ�ꏊ�Ƀ��C�g�J������ݒu����
	GameObject* player = GetScene()->GetPlayerObject();
	Camera* camera = GetScene()->GetCameraObject();
	if (player != nullptr)
	{
		At = player->GetPosition();

		Eye = At + SunPosition;
	}
	else if (camera != nullptr)
	{
		At = camera->GetCameraForcusPoint();

		Eye = At + SunPosition;
	}



	//���C�g�J�����\���̂̏����� �ŏ��̓��C�g�֘A�̐��l�����̒l�ɂ��Ă���
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//���C�g�J�����̃r���[�s����쐬
	D3DXMatrixLookAtLH(&light.ViewMatrix, &Eye, &At, &Up);
	//���C�g�J�����̃v���W�F�N�V�����s����쐬
	// �������̐����FpOut, �c�����̎���p(��{1.0f�ł���,�ς���Ɖe���Ԃ�ĐL�т��肷��)�A�c����(�E�B���h�E�̃A�X�y�N�g��)�A��ԋ߂��̂y�l�A��ԉ����̂y�l
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, PerspectiveFovMinZ, PerspectiveFovMaxZ);

	//////////���C�g�J�����\���̂̏����� �ŏ��̓��C�g�֘A�̐��l�����̒l�ɂ��Ă���
	////////LIGHT light;
	////////light.Enable = true;
	////////light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	////////D3DXVec4Normalize(&light.Direction, &light.Direction);
	////////light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	////////light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//////////���C�g�J�����̃r���[�s����쐬
	////////D3DXVECTOR3 Eye = D3DXVECTOR3(30.0f, 10.0f, -10.0f);
	////////D3DXVECTOR3 At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////////D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	////////D3DXMatrixLookAtLH(&light.ViewMatrix, &Eye, &At, &Up);
	//////////���C�g�J�����̃v���W�F�N�V�����s����쐬
	////////// �������̐����FpOut, �c�����̎���p(��{1.0f�ł���)�A�c����(�E�B���h�E�T�C�Y)�A��ԋ߂��̂y�l�A��ԉ����̂y�l
	////////D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 30.0f);

	//���C�g�����Z�b�g
	Renderer::SetLight(light);


	//*******�P�p�X�� �V���h�E�o�b�t�@�̍쐬*******
	Renderer::BeginDepth();
	//���C�g�J�����̍s����Z�b�g
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
	//�e�𗎂Ƃ������I�u�W�F�N�g��`��
	m_Scene->ShadowDraw();	// UseShadow��true�̃I�u�W�F�N�g�̂ݕ`��
	//m_Scene->Draw();

	//********2�p�X�� �ʏ�̕`��********
	Renderer::Begin();

	m_Scene->Draw();

	light.Enable = false;
	Renderer::SetLight(light);

	Renderer::Gui_Draw();	// 3D�V�[���̌��ImGui��`��

	//// �Ă��Ƃۂ育�� �X�v���C�g�`��i�[�x�o�b�t�@�̓��e�������j
	//g_testPolygon->Draw();

	Renderer::End();// �J�����͐�I�Ăяo�����厞// 3D��Ԃł̕\��// UI�n�͍Ō�ɁB
	
}
