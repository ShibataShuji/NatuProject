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
//
//struct CAPSULEB
//{
//	float m_Radius;				// 半径
//	D3DXVECTOR3 m_PointUpper;	// カプセルの上の球体部分の球の中心座標
//	D3DXVECTOR3 m_PointLower;	// カプセルの下の球体部分の球の中心座標
//
//	// コンストラクタ
//	CAPSULEB() : m_Radius(0.5f), m_PointUpper(0, -0.5f, 0), m_PointLower(0, 0.5f, 0) {}
//
//	// カプセルの中心座標を取得
//	D3DXVECTOR3 GetCenter() const
//	{
//		return m_PointUpper + ((m_PointLower - m_PointUpper) * 0.5f);
//	}
//
//	// カプセルの中心座標から上下2つの球の座標をセット
//	void SetCenter(const D3DXVECTOR3& Center) {
//		D3DXVECTOR3 CenterToPointA = ((m_PointUpper - m_PointLower) * 0.5f);
//		D3DXVECTOR3 CenterToPointB = ((m_PointLower - m_PointUpper) * 0.5f);
//		m_PointUpper = Center + CenterToPointA;
//		m_PointLower = Center + CenterToPointB;
//	}
//	// 縦の半径を取得する
//	float GetHeightRadius()const {
//		D3DXVECTOR3 temp_calclength = m_PointLower - m_PointUpper;
//		float PointLen = D3DXVec3Length(&temp_calclength) * 0.5f;
//		PointLen += m_Radius;
//		return PointLen;
//	}
//};




class Gameobject;

void Player::Init()
{
	// Componentの追加
	AddComponent<Rigidbody>();
	AddComponent<Collision>();

	Player::SetBulletNumMax();
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");

	m_Position = D3DXVECTOR3(0.0f, 10.0f, 5.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale	   = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_TagNum = 2;



	// Componentの設定
	// <Collision>
	//　カプセルコリジョンの設定
	//Point offset = { 0.0f,0.0f + m_Scale.y,0.0f };
	//GetComponent<Collision>()->SetCollisionOffset(offset);
	//GetComponent<Collision>()->SetCapsule(Point(m_Position.x + offset.x, m_Position.y + offset.y, m_Position.z + offset.z),50.0f, 100.0f);
	GetComponent<Collision>()->SetMovable(true);
	GetComponent<Collision>()->SetCollisionType(BOX_COLLISION);
	GetComponent<Collision>()->LoadCollisionModel();
	GetComponent<Collision>()->SetBoxScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//GetComponent<Collision>()->SetCollisionType(CAPSULE_COLLISION);
	//GetComponent<Collision>()->LoadCollisionModel();
	//GetComponent<Collision>()->SetCapsuleScale(1.0f, 2.0f);		// カプセルはスフィアみたいに(1.0f,1.0f)にしたら0となってうまく処理が動かない。

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
		m_Velocity -= right * 0.01;
		//m_temp_Velocity -= right * 0.01;
		//m_Position -= GetComponent<Rigidbody>()->GetVelocity() * 0.01f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Velocity += right * 0.01;
		//m_temp_Velocity += right * 0.01;
	}

	if (Input::GetKeyPress('W'))
	{
		m_Velocity += forward * 0.01;
		//m_temp_Velocity += forward * 0.01;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Velocity -= forward * 0.01;
		//m_temp_Velocity -= forward * 0.01;
	}

	if (Input::GetKeyPress('Q'))
	{
		m_Rotation.y -= 0.1f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Rotation.y += 0.1f;
	}
	if (Input::GetKeyPress('R'))
	{
		m_Rotation.x += 0.1f;
	}

	// Fキーでリロードみたいな
	if (Input::GetKeyPress('F'))
	{
		Player::SetBulletNumMax();
	}

	// ジャンプ。接地している場合のみ可能
	if (m_OnTheGround)
	{
		if (Input::GetKeyTrigger('J'))
		{
			m_Velocity.y = 0.25f;
			//m_temp_Velocity.y = 0.25f;
		}
	}



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
	/*GameObject::TemporarySetUpdate();*/
}


void Player::Draw()
{
	GameObject::Draw();

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ワールドマトリクス(4x4の行列)は毎回0から計算しなおす
	// スケール、回転、座標、それらをかけあわせることでちゃんとしたワールドマトリクスとなる。
	// 0から作り直さないといけない理由は、30度回転したものをまた回転させようとしたときに、回転の軸が変わってくるからだ。
	// そんな感じで初期状態が違うだけで最終結果が変わってくるので、毎回0から作る。
	// なので、変数で保存しない方が良い(その場で使うだけのローカルならいいと思う)
	// ワールドマトリクス設定
	/*D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;*/
	D3DXMATRIX world = GetWorldMatrix();	// 関数にまとめた
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

	
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