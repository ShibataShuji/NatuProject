
#include "stdafx.h"

#define	VALUE_X_ROTATE_CAMERA_MOUSE	(D3DX_PI * 0.0005f)							// マウスを使用したカメラの回転量
#define	VALUE_Y_ROTATE_CAMERA_MOUSE	(D3DX_PI * 0.0003f)							// マウスを使用したカメラの回転量

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
	// プレイヤー追従モードの場合
	if (m_CameraMode == 0)
	{
		// プレイヤーのビューマトリクス取得
		Scene* scene = Manager::GetScene();
		GameObject* player =  scene->GetPlayerObject();
		
		D3DXVECTOR3 PlayerPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 PlayerForward = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (player != nullptr)
		{
			PlayerPosition = player->GetPosition();
			PlayerForward = player->GetForward();
		}

		//// トップビュー
		//m_Target = PlayerPosition;
		//m_Position = PlayerPosition + D3DXVECTOR3(0.0f, 2.0f, -5.0f);

		// サードパーソンビュー
		m_Target = PlayerPosition;
		m_Position = PlayerPosition - PlayerForward * m_radius + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

		//// ファーストパーソンビュー
		//m_Target = PlayerPosition + PlayerForward;
		//m_Position = PlayerPosition;

	}
	// 座標指定モードの場合
	else if (m_CameraMode == 1)
	{
		// サードパーソンビュー
		m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Position.x = sin(m_radian) * m_radius;
		m_Position.z = cos(m_radian) * m_radius;
		m_Position.y = 2.5f;

	}
	// 追従指定モード
	else if (m_CameraMode == 2)
	{
		// 追従するものがnullptrだったら何もしない。
		if (m_FollowObject != nullptr)
		{
			D3DXVECTOR3 FollowObjPosition = m_FollowObject->GetPosition();
			D3DXVECTOR3 FollowObjForward = m_FollowObject->GetForward();

			// サードパーソンビュー
			m_Target = FollowObjPosition;
			m_Position = FollowObjPosition - FollowObjForward * m_radius + D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		}


	}
	// 一人称視点
	else if (m_CameraMode == 3)
	{
		float deltax = Input::GetMouseX();
		float deltay = Input::GetMouseY();
		m_FPSCamera.AddDirectionX(-1.0f * deltax);
		m_FPSCamera.AddDirectionY(-1.0f * deltay);
		// マウスカーソルを中心に戻す処理
		//Cursor::SetPos(Scene::Center());
		//Cursor::RequestStyle(CursorStyle::Hidden);


		// 追従するものがnullptrだったら何もしない。
		if (m_FollowObject != nullptr)
		{
			D3DXVECTOR3 FollowObjPosition = m_FollowObject->GetPosition();
			D3DXVECTOR3 eye = FollowObjPosition;
			eye.y += 0.2f;

			m_Target = m_FPSCamera.GetFPSTarget(eye);
			m_Position = eye;
		}
	}
	// 三人称視点
	else if (m_CameraMode == 4)
	{
		float deltax = Input::GetMouseX();
		float deltay = Input::GetMouseY();
		m_FPSCamera.AddDirectionX(-1.0f * deltax);
		m_FPSCamera.AddDirectionY(-1.0f * deltay);
		// マウスカーソルを中心に戻す処理
		//Cursor::SetPos(Scene::Center());
		//Cursor::RequestStyle(CursorStyle::Hidden);

		//m_Target = m_FPSCamera.GetTargetPosition(m_Position);



		// 追従するものがnullptrだったら何もしない。
		if (m_FollowObject != nullptr)
		{
			D3DXVECTOR3 FollowObjPosition = m_FollowObject->GetPosition();
			D3DXVECTOR3 FollowObjForward = m_FollowObject->GetForward();

			// サードパーソンビュー
			m_Target = FollowObjPosition;
			m_Position = m_FPSCamera.GetTPSPosition(FollowObjPosition, m_CameraDistance);
		}
	}
	// 一人称視点のカメラフリー移動モード
	else if (m_CameraMode == 5)
	{
		if (Input::IsMouseRightPressed())
		{
			float deltax = Input::GetMouseX();
			float deltay = Input::GetMouseY();
			m_FPSCamera.AddDirectionX(-1.0f * deltax);
			m_FPSCamera.AddDirectionY(-1.0f * deltay);
			// マウスカーソルを中心に戻す処理
			//Cursor::SetPos(Scene::Center());
			//Cursor::RequestStyle(CursorStyle::Hidden);
			FreeMovement();

			m_Target = m_FPSCamera.GetFPSTarget(m_Position);
		}
		else
		{
			// 右クリック押されてないときは速度0にしておく
			m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
	else if (m_CameraMode == 6)
	{
		// 何もしないモード(マウス動かしても支店変わんない。4の動かないバージョン)
		// 追従するものがnullptrだったら何もしない。
		if (m_FollowObject != nullptr)
		{
			D3DXVECTOR3 FollowObjPosition = m_FollowObject->GetPosition();
			D3DXVECTOR3 FollowObjForward = m_FollowObject->GetForward();

			// サードパーソンビュー
			m_Target = FollowObjPosition;
			m_Position = m_FPSCamera.GetTPSPosition(FollowObjPosition, m_CameraDistance);
		}
	}
}


void Camera::Draw()
{

	// ビューマトリクス設定
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);


	// プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);	// アスペクト比を横/縦でやってる。どちらも整数型なのでフロートにする。

	Renderer::SetProjectionMatrix(&projectionMatrix);

	//カメラの座標をシェーダーへ渡す
	Renderer::SetCameraPosition(m_Position);
}



// スクリーン座標をワールド座標に変換し、その原点とそこからの方向ベクトルが求まる。
void Camera::ScreenToWorld(D3DXVECTOR3* poutOrigin, D3DXVECTOR3* poutDirection, float ScreenX, float ScreenY)
{
	//ビューマトリックス設定
	D3DXMATRIX viewMatrix;
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&viewMatrix, &m_Position, &m_Target, &up);

	//プロジェクションマトリックス設定(射影行列)
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);



	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, &viewMatrix);
	D3DXMatrixInverse(&InvPrj, NULL, &projectionMatrix);
	D3DXMatrixIdentity(&VP);
	VP._11 = SCREEN_WIDTH / 2.0f; VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f; VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
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
			// +zが前
			re = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}else
			// -zが前
			re = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	else
	{
		if (CameraForward.x > 0.0f)
		{
			// +xが前
			re = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else
			// -xが前
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

	// 摩擦など抵抗の計算
	m_Velocity.x -= m_Velocity.x * 0.1;
	m_Velocity.y -= m_Velocity.y * 0.1;
	m_Velocity.z -= m_Velocity.z * 0.1;

}

// カメラの注目点を出す
D3DXVECTOR3 Camera::GetCameraForcusPoint(const float& length)
{
	D3DXVECTOR3 pos = m_Position;
	D3DXVECTOR3 forward = GetCameraForward();
	pos += forward * length;
	return pos;
}