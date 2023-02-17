
#include "stdafx.h"

#define	VALUE_X_ROTATE_CAMERA_MOUSE	(D3DX_PI * 0.0005f)							// �}�E�X���g�p�����J�����̉�]��
#define	VALUE_Y_ROTATE_CAMERA_MOUSE	(D3DX_PI * 0.0003f)							// �}�E�X���g�p�����J�����̉�]��

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 2.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	m_radian = 3.141592 * 1.0f;

	m_FPSCamera.angleX = M_PI;
	m_FPSCamera.angleYvalue = 0.0f;

}
void Camera::Uninit()
{

}

void Camera::Update()
{
	// �v���C���[�Ǐ]���[�h�̏ꍇ
	if (m_CameraMode == 0)
	{
		// �v���C���[�̃r���[�}�g���N�X�擾
		Scene* scene = Manager::GetScene();
		GameObject* player =  scene->GetPlayerObject();
		
		D3DXVECTOR3 PlayerPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 PlayerForward = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (player != nullptr)
		{
			PlayerPosition = player->GetPosition();
			PlayerForward = player->GetForward();
		}

		//// �g�b�v�r���[
		//m_Target = PlayerPosition;
		//m_Position = PlayerPosition + D3DXVECTOR3(0.0f, 2.0f, -5.0f);

		// �T�[�h�p�[�\���r���[
		m_Target = PlayerPosition;
		m_Position = PlayerPosition - PlayerForward * m_radius + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

		//// �t�@�[�X�g�p�[�\���r���[
		//m_Target = PlayerPosition + PlayerForward;
		//m_Position = PlayerPosition;

	}
	// ���W�w�胂�[�h�̏ꍇ
	else if (m_CameraMode == 1)
	{
		// �T�[�h�p�[�\���r���[
		m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Position.x = sin(m_radian) * m_radius;
		m_Position.z = cos(m_radian) * m_radius;
		m_Position.y = 2.5f;

	}
	// �Ǐ]�w�胂�[�h
	else if (m_CameraMode == 2)
	{
		// �Ǐ]������̂�nullptr�������牽�����Ȃ��B
		if (m_FollowObject != nullptr)
		{
			D3DXVECTOR3 FollowObjPosition = m_FollowObject->GetPosition();
			D3DXVECTOR3 FollowObjForward = m_FollowObject->GetForward();

			// �T�[�h�p�[�\���r���[
			m_Target = FollowObjPosition;
			m_Position = FollowObjPosition - FollowObjForward * m_radius + D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		}


	}
	// ��l�̎��_
	else if (m_CameraMode == 3)
	{
		float deltax = Input::GetMouseX();
		float deltay = Input::GetMouseY();
		m_FPSCamera.AddDirectionX(-1.0f * deltax);
		m_FPSCamera.AddDirectionY(-1.0f * deltay);
		// �}�E�X�J�[�\���𒆐S�ɖ߂�����
		//Cursor::SetPos(Scene::Center());
		//Cursor::RequestStyle(CursorStyle::Hidden);


		// �Ǐ]������̂�nullptr�������牽�����Ȃ��B
		if (m_FollowObject != nullptr)
		{
			D3DXVECTOR3 FollowObjPosition = m_FollowObject->GetPosition();
			D3DXVECTOR3 eye = FollowObjPosition;
			eye.y += 0.2f;

			m_Target = m_FPSCamera.GetFPSTarget(eye);
			m_Position = eye;
		}
	}
	// �O�l�̎��_
	else if (m_CameraMode == 4)
	{
		float deltax = Input::GetMouseX();
		float deltay = Input::GetMouseY();
		m_FPSCamera.AddDirectionX(-1.0f * deltax);
		m_FPSCamera.AddDirectionY(-1.0f * deltay);
		// �}�E�X�J�[�\���𒆐S�ɖ߂�����
		//Cursor::SetPos(Scene::Center());
		//Cursor::RequestStyle(CursorStyle::Hidden);

		//m_Target = m_FPSCamera.GetTargetPosition(m_Position);



		// �Ǐ]������̂�nullptr�������牽�����Ȃ��B
		if (m_FollowObject != nullptr)
		{
			D3DXVECTOR3 FollowObjPosition = m_FollowObject->GetPosition();
			D3DXVECTOR3 FollowObjForward = m_FollowObject->GetForward();

			// �T�[�h�p�[�\���r���[
			m_Target = FollowObjPosition;
			m_Position = m_FPSCamera.GetTPSPosition(FollowObjPosition, m_CameraDistance);
		}
	}
	// ��l�̎��_�̃J�����t���[�ړ����[�h
	else if (m_CameraMode == 5)
	{
		if (Input::IsMouseRightPressed())
		{
			float deltax = Input::GetMouseX();
			float deltay = Input::GetMouseY();
			m_FPSCamera.AddDirectionX(-1.0f * deltax);
			m_FPSCamera.AddDirectionY(-1.0f * deltay);
			// �}�E�X�J�[�\���𒆐S�ɖ߂�����
			//Cursor::SetPos(Scene::Center());
			//Cursor::RequestStyle(CursorStyle::Hidden);
			FreeMovement();

			m_Target = m_FPSCamera.GetFPSTarget(m_Position);
		}
		else
		{
			// �E�N���b�N������ĂȂ��Ƃ��͑��x0�ɂ��Ă���
			m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
	else if (m_CameraMode == 6)
	{
		// �������Ȃ����[�h(�}�E�X�������Ă��x�X�ς��Ȃ��B4�̓����Ȃ��o�[�W����)
		// �Ǐ]������̂�nullptr�������牽�����Ȃ��B
		if (m_FollowObject != nullptr)
		{
			D3DXVECTOR3 FollowObjPosition = m_FollowObject->GetPosition();
			D3DXVECTOR3 FollowObjForward = m_FollowObject->GetForward();

			// �T�[�h�p�[�\���r���[
			m_Target = FollowObjPosition;
			m_Position = m_FPSCamera.GetTPSPosition(FollowObjPosition, m_CameraDistance);
		}
	}
}


void Camera::Draw()
{

	// �r���[�}�g���N�X�ݒ�
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);


	// �v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);	// �A�X�y�N�g�����/�c�ł���Ă�B�ǂ���������^�Ȃ̂Ńt���[�g�ɂ���B

	Renderer::SetProjectionMatrix(&projectionMatrix);

	//�J�����̍��W���V�F�[�_�[�֓n��
	Renderer::SetCameraPosition(m_Position);
}



// �X�N���[�����W�����[���h���W�ɕϊ����A���̌��_�Ƃ�������̕����x�N�g�������܂�B
void Camera::ScreenToWorld(D3DXVECTOR3* poutOrigin, D3DXVECTOR3* poutDirection, float ScreenX, float ScreenY)
{
	//�r���[�}�g���b�N�X�ݒ�
	D3DXMATRIX viewMatrix;
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&viewMatrix, &m_Position, &m_Target, &up);

	//�v���W�F�N�V�����}�g���b�N�X�ݒ�(�ˉe�s��)
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);



	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, &viewMatrix);
	D3DXMatrixInverse(&InvPrj, NULL, &projectionMatrix);
	D3DXMatrixIdentity(&VP);
	VP._11 = SCREEN_WIDTH / 2.0f; VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f; VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 Screen0 = D3DXVECTOR3(ScreenX, ScreenY, 0.0f);
	D3DXVec3TransformCoord(poutOrigin, &Screen0, &tmp);

	D3DXVECTOR3 endP;
	D3DXVECTOR3 Screen1 = D3DXVECTOR3(ScreenX, ScreenY, 1.0f);
	D3DXVec3TransformCoord(&endP, &Screen1, &tmp);
	*poutDirection = endP - *poutOrigin;
	D3DXVec3Normalize(poutDirection, poutDirection);

	return;

}


D3DXVECTOR3 Camera::GetCameraForward()
{
	D3DXVECTOR3 distance = m_Target - m_Position;
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal, &distance);
	return normal;
}

D3DXVECTOR3 Camera::GetCameraForwardIgnoreY()
{
	D3DXVECTOR3 distance = m_Target - m_Position;
	distance.y = 0.0f;
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal, &distance);
	return normal;
}

D3DXVECTOR3 Camera::GetCameraRight()
{
	D3DXVECTOR3 forward = GetCameraForward();
	D3DXVECTOR3 right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// ++ 1
	if (forward.x >= 0 && forward.z >= 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);
	// +- 2
	else 	if (forward.x >= 0 && forward.z < 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);
	// -- 3
	else 	if (forward.x < 0 && forward.z < 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);
	// -+ 4
	else 	if (forward.x < 0 && forward.z >= 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);


	D3DXVec3Normalize(&right, &right);
	return right;
}

D3DXVECTOR3 Camera::GetCameraRightIgnoreY()
{
	D3DXVECTOR3 forward = GetCameraForwardIgnoreY();
	D3DXVECTOR3 right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// ++ 1
	if (forward.x >= 0 && forward.z >= 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);
	// +- 2
	else 	if (forward.x >= 0 && forward.z < 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);
	// -- 3
	else 	if (forward.x < 0 && forward.z < 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);
	// -+ 4
	else 	if (forward.x < 0 && forward.z >= 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);


	D3DXVec3Normalize(&right, &right);
	return right;
}

D3DXVECTOR3 Camera::GetCameraForwardParallelToAxis()
{
	D3DXVECTOR3 re = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 CameraForward = GetCameraForward();
	CameraForward.y = 0.0f;
	D3DXVec3Normalize(&CameraForward, &CameraForward);

	if (fabsf(CameraForward.z) > fabsf(CameraForward.x))
	{
		if (CameraForward.z > 0.0f)
		{
			// +z���O
			re = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}else
			// -z���O
			re = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	else
	{
		if (CameraForward.x > 0.0f)
		{
			// +x���O
			re = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else
			// -x���O
			re = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}

	return re;
}

D3DXVECTOR3 Camera::GetCameraRightParallelToAxis()
{
	D3DXVECTOR3 forward = GetCameraForwardParallelToAxis();
	D3DXVECTOR3 right = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// ++ 1
	if (forward.x >= 0 && forward.z >= 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);
	// +- 2
	else 	if (forward.x >= 0 && forward.z < 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);
	// -- 3
	else 	if (forward.x < 0 && forward.z < 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);
	// -+ 4
	else 	if (forward.x < 0 && forward.z >= 0)
		right = D3DXVECTOR3(forward.z, 0.0f, -forward.x);


	D3DXVec3Normalize(&right, &right);
	return right;
}

void Camera::FreeMovement()
{
	D3DXVECTOR3 Cameraforward = GetCameraForward();
	D3DXVECTOR3 Camerafright = GetCameraRight();

	D3DXVECTOR3 Cameraforwardxz = Cameraforward;
	Cameraforwardxz.y = 0.0f;
	D3DXVec3Normalize(&Cameraforwardxz, &Cameraforwardxz);
	D3DXVECTOR3 Camerafrightxz = Camerafright;
	Camerafrightxz.y = 0.0f;
	D3DXVec3Normalize(&Camerafrightxz, &Camerafrightxz);

	float cameraspeed = 0.05f;

	if (Input::GetKeyPress(DIK_A))
	{
		m_Velocity -= Camerafrightxz * cameraspeed;
	}
	if (Input::GetKeyPress(DIK_D))
	{
		m_Velocity += Camerafrightxz * cameraspeed;
	}
	if (Input::GetKeyPress(DIK_W))
	{
		m_Velocity += Cameraforwardxz * cameraspeed;
	}
	if (Input::GetKeyPress(DIK_S))
	{
		m_Velocity -= Cameraforwardxz * cameraspeed;
	}

	if (Input::GetKeyPress(DIK_Q))
	{
		m_Velocity.y -= cameraspeed;
	}
	if (Input::GetKeyPress(DIK_E))
	{
		m_Velocity.y += cameraspeed;
	}

	m_Position += m_Velocity;

	// ���C�Ȃǒ�R�̌v�Z
	m_Velocity.x -= m_Velocity.x * 0.1;
	m_Velocity.y -= m_Velocity.y * 0.1;
	m_Velocity.z -= m_Velocity.z * 0.1;

}

// �J�����̒��ړ_���o��
D3DXVECTOR3 Camera::GetCameraForcusPoint(const float& length)
{
	D3DXVECTOR3 pos = m_Position;
	D3DXVECTOR3 forward = GetCameraForward();
	pos += forward * length;
	return pos;
}