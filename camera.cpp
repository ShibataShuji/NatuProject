#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "input.h"
#include "player.h"

#define	VALUE_X_ROTATE_CAMERA_MOUSE	(D3DX_PI * 0.0005f)							// マウスを使用したカメラの回転量
#define	VALUE_Y_ROTATE_CAMERA_MOUSE	(D3DX_PI * 0.0003f)							// マウスを使用したカメラの回転量

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 2.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
void Camera::Uninit()
{

}

void Camera::Update()
{
	// プレイヤーのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 PlayerPosition = player->GetPosition();
	D3DXVECTOR3 PlayerForward = player->GetForward();

	//// トップビュー
	//m_Target = PlayerPosition;
	//m_Position = PlayerPosition + D3DXVECTOR3(0.0f, 2.0f, -5.0f);

	//// サードパーソンビュー
	//m_Target = PlayerPosition;
	//m_Position = PlayerPosition - PlayerForward * 5.0f + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

	// ファーストパーソンビュー
	m_Target = PlayerPosition + PlayerForward;
	m_Position = PlayerPosition;




	//////////m_Position = D3DXVECTOR3(0.0f, m_Position.y + 0.001f, m_Position.z - 0.001f);
	//////////m_Position = D3DXVECTOR3(0.0f, m_Position.y, m_Position.z);

	//////////float MouseMoveAmount_X = Input::GetMouseX();
	//////////float MouseMoveAmount_Y = Input::GetMouseY();

	//////////// ほんの少しの移動はマウス動かしてない判定にする(これないとマジで酔う)
	//////////if (fabsf(MouseMoveAmount_X) < 1.0f)
	//////////	MouseMoveAmount_X = 0.0f;
	//////////if (fabsf(MouseMoveAmount_Y) < 1.0f)
	//////////	MouseMoveAmount_Y = 0.0f;

	//////////// マウス操作での視点移動の計算処理
	//////////m_Rotation.y += MouseMoveAmount_X * VALUE_X_ROTATE_CAMERA_MOUSE;
	//////////if (m_Rotation.y < -D3DX_PI)
	//////////{
	//////////	m_Rotation.y = D3DX_PI;
	//////////}
	//////////if (m_Rotation.y > D3DX_PI)
	//////////{
	//////////	m_Rotation.y = -D3DX_PI;
	//////////}

	//////////m_Rotation.x -= MouseMoveAmount_Y * VALUE_Y_ROTATE_CAMERA_MOUSE;
	//////////if (m_Rotation.x > D3DX_PI_HARF - 0.1f)
	//////////{
	//////////	m_Rotation.x = D3DX_PI_HARF - 0.1f;
	//////////}
	//////////if (m_Rotation.x < -D3DX_PI_HARF + 0.1f)
	//////////{
	//////////	m_Rotation.x = -D3DX_PI_HARF + 0.1f;
	//////////}
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
}