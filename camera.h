#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

class Camera : public GameObject
{
private:

	//D3DXVECTOR3		m_Position;
	D3DXVECTOR3			m_Target;
	//D3DXVECTOR3			m_Rotation;			// �J�����̉�]

	int	  m_CameraMode = 0;
	float m_radius = 10.0f;
	float m_radian = 0.0f;	// ��]�p�x(0.0f~3.14f)
	float m_CameraDistance = 10.0f;	// �^�[�Q�b�g����J�����ւ̋���


	// �Ǐ]�w�胂�[�h�Ŏg���B���̃Q�[���I�u�W�F�N�g�ɂ��Ă���
	GameObject* m_FollowObject = nullptr;


	D3DXMATRIX m_ViewMatrix;

public:

	FPSCamera m_FPSCamera;		// �\���̃p�u���b�N�ɒu���Ă�

	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR3 GetTarget() { return m_Target; }

	void SetFollowObject(GameObject* object)
	{
		m_FollowObject = object;
		m_CameraMode = 2;
	}

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix;}

	// m_Position����m_Target�ւ̕����x�N�g��
	D3DXVECTOR3 GetCameraForward();
	D3DXVECTOR3 GetCameraForwardIgnoreY();
	D3DXVECTOR3 GetCameraRight();
	D3DXVECTOR3 GetCameraRightIgnoreY();
	D3DXVECTOR3 GetCameraForwardParallelToAxis();
	D3DXVECTOR3 GetCameraRightParallelToAxis();

	void SetCameraDistance(const float& distance) { m_CameraDistance = distance; }
	float GetCameraDistance() { return m_CameraDistance; }
	float* GetpCameraDistance() { return &m_CameraDistance; }


	void SetCameraMode(int modenum)
	{
		m_CameraMode = modenum;
	}

	void AddRadian(float addradian)
	{
		m_radian += addradian;
	}

	void ScreenToWorld(D3DXVECTOR3* poutOrigin, D3DXVECTOR3* poutDirection, float ScreenX, float ScreenY);

	void FreeMovement();

	D3DXVECTOR3 GetCameraForcusPoint(const float& length = 20.0f);



};

