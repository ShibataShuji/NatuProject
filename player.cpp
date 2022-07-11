#include "main.h"
#include "renderer.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "gameObject.h"
#include "Bullet.h"
#include "Shadow.h"
#include "audio.h"
#include "Rigidbody.h"
#include "Collision.h"
#include "Platform.h"

class Gameobject;

void Player::Init()
{
	// Componentの追加
	AddComponent<Rigidbody>();
	AddComponent<Collision>();

	Player::SetBulletNumMax();
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");

	m_Position = D3DXVECTOR3(5.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale	   = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,"vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");


	Scene* scene = Manager::GetScene();
	m_ShotSE = scene->AddGameObject<Audio>(2);
	m_ShotSE->Load("asset\\audio\\wan.wav");

	m_Shadow = scene->AddGameObject<Shadow>(1);
	m_Shadow->SetPosition(m_Position);
	//m_Shadow->SetScale(D3DXVECTOR3(2.0f, 1.0f, 2.0f));

}


void Player::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();



}


void Player::Update()
{
	// 親クラスのUpdateを呼んでいる。ここの中でコンポーネントのUpdateも呼ばれている。
	GameObject::Update();


	// モデルを移動したりするときはここに書いたりする

	// インプットの時アルファベットならシングルクォーテーションでそのアルファベットになる。
	//プレスは押している間
	float PlayerMoveSpeed = 0.1f;

	D3DXVECTOR3 forward = GetForward();
	D3DXVECTOR3 right = GetRight();

	if (Input::GetKeyPress('A'))
	{
		m_temp_Velocity -= right * 0.01;
		//m_Position -= GetComponent<Rigidbody>()->GetVelocity() * 0.01f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_temp_Velocity += right * 0.01;
	}

	if (Input::GetKeyPress('W'))
	{
		m_temp_Velocity += forward * 0.01;
	}
	if (Input::GetKeyPress('S'))
	{
		m_temp_Velocity -= forward * 0.01;
	}

	if (Input::GetKeyPress('Q'))
	{
		m_Rotation.y -= 0.1f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Rotation.y += 0.1f;
	}

	// Fキーでリロードみたいな
	if (Input::GetKeyPress('F'))
	{
		Player::SetBulletNumMax();
	}

	// ジャンプ
	if (Input::GetKeyTrigger('J'))
	{
		m_temp_Velocity.y = 0.25f;
	}

	//// Platformとの当たり判定
	//Scene* scene = Manager::GetScene();
	//std::vector<Platform*> platformList = scene->GetGameObjects<Platform>(1);

	//for (Platform* platform : platformList)
	//{
	//	D3DXVECTOR3 platformPosition = platform->GetPosition();
	//	D3DXVECTOR3 platformScale = platform->GetScale();

	//	D3DXVECTOR3 direction = m_Position - platformPosition;		// 方向ベクトルを出している
	//	direction.y = 0.0f;											// 方向ベクトルのy方向のベクトルをなくしている
	//	float length = D3DXVec3Length(&direction);					// これで円柱の中心からの距離が求まっている

	//	if (length < platformScale.x)
	//	{
	//		if (m_Position.y < platformPosition.y + platformScale.y - 0.5f)		// 移動距離が大きくてめりこみを少しでも防ぐため-0.5fしてゆるくしている
	//		{
	//			m_Position.x = oldPosition.x;
	//			m_Position.z = oldPosition.z;
	//		}
	//		else
	//		{
	//			groundHeight = platformPosition.y + platformScale.y;
	//		}
	//		break;
	//	}
	//}



	D3DXVECTOR3 shadowPos = m_Position;
	shadowPos.y = 0.0f;

	m_Shadow->SetPosition(shadowPos);


	// アルファベット以外はVK_なんたらでだいたいいける。右クリックでマクロに飛ぶと他のも見れるよ
	// トリガーは押した瞬間だけ
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		// 弾が1発以上あるなら
		if (m_BulletNum >= 1)
		{
			m_BulletNum--;
			Scene* scene = Manager::GetScene();
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position);
			bullet->SetVelocity(forward * 0.3);

			m_ShotSE->Play();
		}
	}


	// プレイヤーの移動の入力処理が終わった後にコンポーネント(物理や子リジョン)のUpdateを行う
	GameObject::ComponentUpdate();
	// 本物にTemporaryを更新
	GameObject::TemporarySetUpdate();
}


void Player::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ワールドマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

	GameObject::Draw();
}

// プレイヤーの座標を返す
D3DXVECTOR3 Player::GetPlayerPosition()
{
	return m_Position;
}



void Player::SetBulletNumMax()
{
	m_BulletNum = MaxBulletNum;

}