#include "stdafx.h"

void OriginalBlock::Init()
{


	m_TagNum = 0;
	m_name = "Input ObjectName";
	m_modelname = "Torus";
	m_DoSave = 1;

	m_UseShadow = true;

}


void OriginalBlock::Uninit()
{

}


void OriginalBlock::Update()
{
	GameObject::Update();		// �ŏ��Ɏ����Ă���
	Scene* scene = Manager::GetScene();
	std::string SceneName = scene->GetSceneName();

	// �V�[��������炾������Update�̍Ō�܂Ŕ��
	if (SceneName == "CreateNewObjectScene" || SceneName == "CreateNewStageScene")
		goto end_point;

	m_UseShadow = true;

	// �v���C���[�̏ꍇ�BEnd�ɐG�ꂽ��^�C�g���ɖ߂�̂ł�������return����
	if (PlayerObjectUpdate())
		return;

	//// �Փ˂������̈ꗗ
	//m_Collision_HitObjectList;
	//m_Collision_HitPositionList;
	//m_Collision_OverlapObjectList;
	//m_Collision_OverlapPositionList;




	// �W�����v
	if (Input::GetKeyTrigger(DIK_K))
	{
		m_Velocity.y = 0.25f;
	}

	// �f�o�b�O�p��]
	if (Input::GetKeyTrigger(DIK_O))
	{
		m_Rotation.x += 0.1f;
	}


	end_point:
	GameObject::ComponentUpdate();		// �Ō�Ɏ����Ă���
}


void OriginalBlock::Draw()
{
	GameObject::Draw();	// �ŏ��Ɏ����Ă���



}

bool OriginalBlock::PlayerObjectUpdate()
{

	if (m_name == "Player")
	{
		Scene* scene = Manager::GetScene();

		D3DXVECTOR3 forward = GetForward();
		D3DXVECTOR3 right = GetRight();

		D3DXVECTOR3 Cameraforward = scene->GetCameraObject()->GetCameraForward();
		D3DXVECTOR3 Cameraforwardxz = Cameraforward;
		Cameraforwardxz.y = 0.0f;
		D3DXVec3Normalize(&Cameraforwardxz, &Cameraforwardxz);
		D3DXVECTOR3 Cameraright = scene->GetCameraObject()->GetCameraRight();
		D3DXVECTOR3 Camerarightxz = Cameraright;
		Camerarightxz.y = 0.0f;
		D3DXVec3Normalize(&Camerarightxz, &Camerarightxz);


		// �R���W�����̔��������߂�B�J�v�Z���Œ�ł���Ă�
		float PlayerColHalf;
		D3DXVECTOR3 PlayerColHalfa, PlayerColHalfb;
		float PlayerColHalfc;
		Collision* col = GetComponent<Collision>();
		PlayerColHalf = col->GetCapsule().GetHeightRadius();
		PlayerColHalfa = col->GetCapsule().GetLowerSpherePos();
		PlayerColHalfb = col->GetCapsule().GetCenterPos();
		PlayerColHalfc = PlayerColHalfa.y - PlayerColHalfb.y;

		// �J�����̌����̃I�C���[�p(y�͖���)�����߂�
		D3DXVECTOR3 BaseAngle = D3DXVECTOR3(0.0f, 0.0f, 1.0f);	// ����B���ʂ̌������Z�b�g���Ă���
		double CameraAngle;	// �Q�����ōl�������̃J�����������Ă郉�W�A�������߂� -PI ~ PI
		float CameraForwardRot;	// �Q�����ōl�������̃J�����������Ă郉�W�A�������߂� -PI ~ PI�@���ꂪ���݂̃J������rot�ƂȂ�
		CalcAngle3(D3DXVECTOR2(BaseAngle.x, BaseAngle.z), D3DXVECTOR2(Cameraforwardxz.x, Cameraforwardxz.z), CameraAngle);
		CameraForwardRot = -CameraAngle;
		float CameraRightRot = CameraForwardRot + M_PI_2;

		// �ړ�
		float MoveSpeed = 0.015f;
		if (Input::GetKeyPress(DIK_A))
		{
			m_Velocity -= Camerarightxz * MoveSpeed;

			m_Rotation.y = CameraRightRot + M_PI;
		}
		if (Input::GetKeyPress(DIK_D))
		{
			m_Velocity += Camerarightxz * MoveSpeed;

			m_Rotation.y = CameraRightRot;
		}

		if (Input::GetKeyPress(DIK_W))
		{
			m_Velocity += Cameraforwardxz * MoveSpeed;

			m_Rotation.y = CameraForwardRot;
		}
		if (Input::GetKeyPress(DIK_S))
		{
			m_Velocity -= Cameraforwardxz * MoveSpeed;

			m_Rotation.y = CameraForwardRot + M_PI;
		}

		//if (Input::GetKeyPress(DIK_Q))
		//{
		//	m_Rotation.y -= 0.1f;
		//}
		//if (Input::GetKeyPress(DIK_E))
		//{
		//	m_Rotation.y += 0.1f;
		//}
		//if (Input::GetKeyPress(DIK_R))
		//{
		//	m_Rotation.x += 0.1f;
		//}
		// �W�����v�B�ڒn���Ă���ꍇ�̂݉\
		if (m_OnTheGround)
		{
			if (Input::GetKeyTrigger(DIK_SPACE))
			{
				m_Velocity.y = 0.25f;
			}
		}



		// Hit�������̂̃��X�g�ł��낢�낷��Ƃ�
		for (auto hitobj : m_Collision_HitObjectList)
		{

		}
		// Overlap�������X�g�ł��낢�낷��Ƃ�
		for (auto oberlapobj : m_Collision_OverlapObjectList)
		{
			char overlapname[64];
			oberlapobj->GetObjectNameChar64(overlapname);
			std::string overlapnameS;
			CharToString(overlapname, &overlapnameS);
			if (overlapnameS == "Player_End")
			{
				Manager::SetScene<Result>();
				return true;
			}

			if (overlapnameS == "Coin")
			{
				oberlapobj->SetDestroy();
			}


		}
		// ���b�V���t�B�[���h�ł̐ڒn�����Collision�ōs���Ă���B

	}

	return false;
}