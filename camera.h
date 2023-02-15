#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

class Camera : public GameObject
{
private:

	//D3DXVECTOR3		m_Position;
	D3DXVECTOR3			m_Target;
	//D3DXVECTOR3			m_Rotation;			// カメラの回転

	int	  m_CameraMode = 0;
	float m_radius = 10.0f;
	float m_radian = 0.0f;	// 回転角度(0.0f~3.14f)
	float m_CameraDistance = 10.0f;	// ターゲットからカメラへの距離


	// 追従指定モードで使う。このゲームオブジェクトについていく
	GameObject* m_FollowObject = nullptr;


	D3DXMATRIX m_ViewMatrix;

public:

	FPSCamera m_FPSCamera;		// 構造体パブリックに置いてる

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

	// m_Positionからm_Targetへの方向ベクトル
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

