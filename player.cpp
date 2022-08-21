#include "main.h"
#include "renderer.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "gameObject.h"
#include "camera.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Shadow.h"
#include "audio.h"
#include "Rigidbody.h"
#include "HandGameObject.h"
#include "Collision.h"
#include "Platform.h"

#include "gui.h"

//#include "gui.h"

#include "imgui.h"
//#include "imgui_impl_win32.h"
//#include "imgui_impl_dx11.h"
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
	// Componentの追加と同時にアドレスをもらっておく
	HandGameObject* p_HandGameObject_0 = AddComponent<HandGameObject>("HandGameObject");
	Rigidbody* p_Rigidbody_0 = AddComponent<Rigidbody>();
	Collision* p_Collision_Ray = AddComponent<Collision>("Ray");		// Collisionは最期がいい
	Collision* p_Collision_0 = AddComponent<Collision>();
	Collision* p_Collision_1 = AddComponent<Collision>();

	Player::SetBulletNumMax();
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale	   = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	SetInitScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	m_TagNum = 2;



	// Componentの設定
	// <Collision>
	//　カプセルコリジョンの設定
	//Collision* p_Collision = GetComponent<Collision>();
	//Point offset = { 0.0f,0.0f + m_Scale.y,0.0f };
	//GetComponent<Collision>()->SetCollisionOffset(offset);
	//GetComponent<Collision>()->SetCapsule(Point(m_Position.x + offset.x, m_Position.y + offset.y, m_Position.z + offset.z),50.0f, 100.0f);
	
	//GetComponent<Collision>()->SetMovable(true);
	//GetComponent<Collision>()->SetCollisionType(BOX_COLLISION);
	//GetComponent<Collision>()->LoadCollisionModel();
	//GetComponent<Collision>()->SetBoxScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	
	//p_Collision_0->SetMovable(true);
	//p_Collision_0->SetCollisionType(BOX_COLLISION);
	//p_Collision_0->LoadCollisionModel();
	//p_Collision_0->SetBoxScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	p_Collision_0->SetMovable(true);
	p_Collision_0->SetCollisionType(CAPSULE_COLLISION);
	p_Collision_0->LoadCollisionModel();
	p_Collision_0->SetCapsuleScale(1.0f, 2.0f);	// カプセルはスフィアみたいに(1.0f,1.0f)にしたら0となってうまく処理が動かない。

	//p_Collision_0->SetMovable(true);
	//p_Collision_0->SetCollisionType(SPHERE_COLLISION);
	//p_Collision_0->LoadCollisionModel();
	//p_Collision_0->SetSphereScale(1.0f);



	p_Collision_1->SetMovable(true);
	p_Collision_1->SetCollisionType(SPHERE_COLLISION);
	p_Collision_1->LoadCollisionModel();
	p_Collision_1->SetSphereScale(3.0f);

	p_Collision_Ray->SetMovable(true);
	p_Collision_Ray->SetCollisionType(RAY_COLLISION);
	p_Collision_Ray->LoadCollisionModel();
	




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

	// カメラのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	Camera* p_camera = scene->GetGameObject<Camera>(0);
	D3DXVECTOR3 CameraPosition = p_camera->GetPosition();		// カメラのポジション
	D3DXVECTOR3 CameraForward = p_camera->GetCameraForward();	// カメラの向いてる方向ベクトル
	float ray_length = 10.0f;
	D3DXVECTOR3 RayStartPoint = CameraPosition;
	D3DXVECTOR3 RayEndPoint = CameraPosition + (CameraForward * ray_length);

	auto p_Collision_Ray = GetComponentWithName<Collision>("Ray");
	p_Collision_Ray->SetRaySegment(RayStartPoint, RayEndPoint);			// レイの更新


	D3DXVECTOR3 ColPoint   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Normal	   = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 SpawnSize  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 SpawnPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 衝突地点にブロックを配置する時の座標の更新
	if (p_Collision_Ray->GetRay().HitCollision != nullptr)
	{
		ColPoint = p_Collision_Ray->GetRay().m_CollisionPoint;	// レイの衝突点の取得
		Normal = p_Collision_Ray->GetRay().m_normal;				// 衝突点からの法線の取得

		SpawnSize = D3DXVECTOR3(2.0f, 0.5f, 2.0f);					// 配置するブロックのサイズの取得(回転を考慮する)												// 配置する座標
		SpawnPoint.x = ColPoint.x + (SpawnSize.x * Normal.x);
		SpawnPoint.y = ColPoint.y + (SpawnSize.y * Normal.y);
		SpawnPoint.z = ColPoint.z + (SpawnSize.z * Normal.z);
	}


	// レイで衝突したところに、現在手に持っている(指定中の)ブロックを配置場所に表示する	
	auto p_HandGameObjectComponent = GetComponentWithName<HandGameObject>("HandGameObject");
	auto p_HandGameObject = p_HandGameObjectComponent->GetHandGameObject();

	// 現在手に持ってるブロックの更新(今はとりあえずEnemyでやってる)
	if (p_HandGameObject == nullptr)
	{
		// 手に持っているものが設定されてない場合
		Scene* scene = Manager::GetScene();
		//Enemy* enemy = scene->AddGameObject<Enemy>(1);
		auto tempObject = scene->AddGameObject<Enemy>(1);
		tempObject->SetPosition(SpawnPoint);
		tempObject->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		p_HandGameObjectComponent->SetHandGameObject(tempObject);

	}
	else
	{
		// 手に持っているものが設定されてる場合
		p_HandGameObject->SetPosition(SpawnPoint);
		p_HandGameObject->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));


		// 手に持っているものを他のものにする(現在はデバック用にぷらっとフォームにしている)
		if (Input::GetKeyTrigger(DIK_SPACE))
		{
			p_HandGameObject->SetDestroy();
			auto tempObject = scene->AddGameObject<Platform>(1);

			p_HandGameObjectComponent->SetHandGameObject(tempObject);
		}

	}
	

	// モデルを移動したりするときはここに書いたりする

	// インプットの時アルファベットならシングルクォーテーションでそのアルファベットになる。
	//プレスは押している間
	float PlayerMoveSpeed = 0.1f;

	D3DXVECTOR3 forward = GetForward();
	D3DXVECTOR3 right = GetRight();

	//// デバッグ用回転
	//if (Input::GetKeyTrigger(DIK_P))
	//{
	//	m_Rotation.x += 0.1f;
	//	m_Rotation.z += 0.1f;
	//}



	if (Input::GetKeyPress(DIK_A))
	{
		m_Velocity -= right * 0.01;
		//m_temp_Velocity -= right * 0.01;
		//m_Position -= GetComponent<Rigidbody>()->GetVelocity() * 0.01f;
	}
	if (Input::GetKeyPress(DIK_D))
	{
		m_Velocity += right * 0.01;
		//m_temp_Velocity += right * 0.01;
	}

	if (Input::GetKeyPress(DIK_W))
	{
		m_Velocity += forward * 0.01;
		//m_temp_Velocity += forward * 0.01;
	}
	if (Input::GetKeyPress(DIK_S))
	{
		m_Velocity -= forward * 0.01;
		//m_temp_Velocity -= forward * 0.01;
	}

	if (Input::GetKeyPress(DIK_Q))
	{
		m_Rotation.y -= 0.1f;
	}
	if (Input::GetKeyPress(DIK_E))
	{
		m_Rotation.y += 0.1f;
	}
	if (Input::GetKeyPress(DIK_R))
	{
		m_Rotation.x += 0.1f;
	}

	// Fキーでリロードみたいな
	if (Input::GetKeyPress(DIK_F))
	{
		Player::SetBulletNumMax();
	}

	// ジャンプ。接地している場合のみ可能
	if (m_OnTheGround)
	{
		if (Input::GetKeyTrigger(DIK_J))
		{
			m_Velocity.y = 0.25f;
			//m_temp_Velocity.y = 0.25f;
		}
	}


	float aaa = gui::DebugA();
	SetScaleRate(D3DXVECTOR3(aaa, aaa, aaa));
	//Collision* p_Collision = GetComponent<Collision>();
	//p_Collision->SetBoxScale(D3DXVECTOR3(aaa, aaa, aaa));
	

	ImGui::Begin("Debug");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();


	D3DXVECTOR3 shadowPos = m_Position;
	shadowPos.y = 0.0f;

	m_Shadow->SetPosition(shadowPos);


	// アルファベット以外はVK_なんたらでだいたいいける。右クリックでマクロに飛ぶと他のも見れるよ
	// トリガーは押した瞬間だけ
	if (Input::GetKeyTrigger(DIK_SPACE))
	{
		// 衝突地点にブロックを配置する。衝突していない場合は何もしない。
		if (p_Collision_Ray->GetRay().HitCollision != nullptr)
		{
			Scene* scene = Manager::GetScene();
			Enemy* enemy = scene->AddGameObject<Enemy>(1);
			enemy->SetPosition(SpawnPoint);
		}

		// 弾が1発以上あるなら
		if (m_BulletNum >= 1)
		{

			//m_BulletNum--;
			//Scene* scene = Manager::GetScene();
			//Bullet* bullet = scene->AddGameObject<Bullet>(1);
			//bullet->SetPosition(m_Position);
			//bullet->SetVelocity(forward * 0.3);

			//m_ShotSE->Play();
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