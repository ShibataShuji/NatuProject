//#include "main.h"
//#include "renderer.h"
//#include "input.h"
//#include "model.h"
//#include "player.h"
//#include "scene.h"
//#include "manager.h"
//#include "bullet.h"
//#include "audio.h"
//#include "shadow.h"
//#include "platform.h"
//
//
//void Player::Init()
//{
//	m_Model = new Model();
//	m_Model->Load("asset\\model\\cylinder.obj");
//
//	m_BulletNum = 5;
//
//	m_Position = D3DXVECTOR3(0.0f, 1.0f, -5.0f);
//	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//
//	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
//
//	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
//
//
//
//	Scene* scene = Manager::GetScene();
//
//	m_ShotSE = scene->AddGameObject<Audio>(2);
//	m_ShotSE->Load("asset\\audio\\star.wav");
//
//	m_Shadow = scene->AddGameObject<Shadow>(1);
//	m_Shadow->SetPosition(m_Position);
//	m_Shadow->SetScale(D3DXVECTOR3(2.0f, 1.0f, 2.0f));
//
//	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//}
//
//void Player::Uninit()
//{
//	m_Model->Unload();
//	delete m_Model;
//
//	m_VertexLayout->Release();
//	m_VertexShader->Release();
//	m_PixelShader->Release();
//
//}
//
//void Player::Update()
//{
//	//回転
//	if (Input::GetKeyPress('Q'))
//	{
//		m_Rotation.x -= 0.1f;
//	}
//	if (Input::GetKeyPress('E'))
//	{
//		m_Rotation.x += 0.1f;
//	}
//
//    //左右
//	D3DXVECTOR3 right = GetRight();
//
//	if (Input::GetKeyPress('A'))
//	{
//		m_Velocity -= right* 0.01f;
//	}
//	if (Input::GetKeyPress('D'))
//	{
//		m_Velocity += right * 0.01f;
//	}
//
//	//上下
//	D3DXVECTOR3 forward = GetForward();
//
//	if (Input::GetKeyPress('W'))
//	{
//		m_Velocity += forward * 0.01f;
//	}
//	if (Input::GetKeyPress('S'))
//	{
//		m_Velocity -= forward * 0.01f;
//	}
//
//	//ジャンプ
//	if (Input::GetKeyTrigger('J'))
//	{
//		m_Velocity.y = 0.25f;
//	}
//
//	//重力
//	m_Velocity.y -= 0.01;
//
//
//	//抵抗
//	m_Velocity.x -= m_Velocity.x * 0.1f;
//	m_Velocity.y -= m_Velocity.y * 0.01f;
//	m_Velocity.z -= m_Velocity.z * 0.1f;
//
//	D3DXVECTOR3 oldPosition = m_Position;
//
//	//移動
//	m_Position += m_Velocity;
//
//	//衝突判定
//	float groundHeight = 0.0f;
//
//	Scene* scene = Manager::GetScene();
//	std::vector<Platform*>platformList = scene->GetGameObjects<Platform>(1);
//
//	for (Platform* platform : platformList)
//	{
//		D3DXVECTOR3 platformPosition = platform->GetPosition();
//		D3DXVECTOR3 platformScale = platform->GetScale();
//
//
//		D3DXVECTOR3 direction = m_Position - platformPosition;
//		direction.y = 0.0f;
//
//		float length = D3DXVec3Length(&direction);
//
//		if (length < platformScale.x + m_Scale.x)
//		{
//		
//			if (m_Position.y < platformPosition.y + platformScale.y - 0.5f)
//			{
//				m_Position.x = oldPosition.x;
//				m_Position.z = oldPosition.z;
//
//				D3DXVECTOR3 normalize = direction / length;  //方向
//
//				normalize = normalize * platformScale.x; //交点座標
//
//				normalize.x += m_Scale.x;
//				normalize.z += m_Scale.z;
//
//				m_Position = normalize;
//			}
//			else
//			{
//				groundHeight = platformPosition.y + platformScale.y;
//			}
//			break;
//		}
//
//
//	}
//
//
//	//地面との当たり判定
//	/*if (m_Position.y < 1.0f)
//	{
//		m_Position.y = 1.0f;
//		m_Velocity.y = 0.0f;
//
//	}*/
//
//	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
//	{
//		m_Position.y = groundHeight;
//		m_Velocity.y = 0.0f;
//	}
//
//	
//
//
//	//弾を出す
//	// Fキーでリロードみたいな
//		if (Input::GetKeyPress('F'))
//		{
//			Player::SetBulletNumMax();
//		}
//
//	if (Input::GetKeyTrigger('L'))
//	{
//		// 弾が1発以上あるなら
//		if (m_BulletNum >= 1)
//		{
//			m_BulletNum--;
//			Scene* scene = Manager::GetScene();
//			Bullet* bullet = scene->AddGameObject<Bullet>(1);
//			bullet->SetPosition(m_Position);
//			bullet->SetVelocity(forward * 0.3);
//
//			m_ShotSE->Play();
//		}
//	}
//
//	D3DXVECTOR3 shadowPos = m_Position;
//	shadowPos.y = 0.0f;
//
//	m_Shadow->SetPosition(shadowPos);
//	m_Shadow->SetRotation(m_Rotation);
//}
//
//void Player::Draw()
//{
//	// 入力レイアウト設定
//	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
//
//	//シェーダ設定
//	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
//	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
//
//	//ワールドマトリックス設定
//	D3DXMATRIX world, scale, rot, trans;
//	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
//	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
//	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
//	world = scale * rot * trans;
//
//
//	Renderer::SetWorldMatrix(&world);
//
//
//	m_Model->Draw();
//}
//
//void Player::SetBulletNumMax()
//{
//	m_BulletNum = MaxBulletNum;
//
//}