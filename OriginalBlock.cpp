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
	GameObject::Update();		// 最初に持ってくる
	Scene* scene = Manager::GetScene();
	std::string SceneName = scene->GetSceneName();

	// シーンがこれらだったらUpdateの最後まで飛ぶ
	if (SceneName == "CreateNewObjectScene" || SceneName == "CreateNewStageScene")
		goto end_point;

	m_UseShadow = true;

	// プレイヤーの場合。Endに触れたらタイトルに戻るのでそしたらreturnする
	if (PlayerObjectUpdate())
		return;

	//// 衝突したもの一覧
	//m_Collision_HitObjectList;
	//m_Collision_HitPositionList;
	//m_Collision_OverlapObjectList;
	//m_Collision_OverlapPositionList;




	// ジャンプ
	if (Input::GetKeyTrigger(DIK_K))
	{
		m_Velocity.y = 0.25f;
	}

	// デバッグ用回転
	if (Input::GetKeyTrigger(DIK_O))
	{
		m_Rotation.x += 0.1f;
	}


	end_point:
	GameObject::ComponentUpdate();		// 最後に持ってくる
}


void OriginalBlock::Draw()
{
	GameObject::Draw();	// 最初に持ってくる



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


		// コリジョンの半高を求める。カプセル固定でやってる
		float PlayerColHalf;
		D3DXVECTOR3 PlayerColHalfa, PlayerColHalfb;
		float PlayerColHalfc;
		Collision* col = GetComponent<Collision>();
		PlayerColHalf = col->GetCapsule().GetHeightRadius();
		PlayerColHalfa = col->GetCapsule().GetLowerSpherePos();
		PlayerColHalfb = col->GetCapsule().GetCenterPos();
		PlayerColHalfc = PlayerColHalfa.y - PlayerColHalfb.y;

		// カメラの向きのオイラー角(yは無視)を求める
		D3DXVECTOR3 BaseAngle = D3DXVECTOR3(0.0f, 0.0f, 1.0f);	// 基準線。正面の向きをセットしておく
		double CameraAngle;	// ２次元で考えた時のカメラが向いてるラジアンを求める -PI ~ PI
		float CameraForwardRot;	// ２次元で考えた時のカメラが向いてるラジアンを求める -PI ~ PI　これが現在のカメラのrotとなる
		CalcAngle3(D3DXVECTOR2(BaseAngle.x, BaseAngle.z), D3DXVECTOR2(Cameraforwardxz.x, Cameraforwardxz.z), CameraAngle);
		CameraForwardRot = -CameraAngle;
		float CameraRightRot = CameraForwardRot + M_PI_2;

		// 移動
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
		// ジャンプ。接地している場合のみ可能
		if (m_OnTheGround)
		{
			if (Input::GetKeyTrigger(DIK_SPACE))
			{
				m_Velocity.y = 0.25f;
			}
		}



		// Hitしたもののリストでいろいろするとこ
		for (auto hitobj : m_Collision_HitObjectList)
		{

		}
		// Overlapしたリストでいろいろするとこ
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
		// メッシュフィールドでの接地判定はCollisionで行われている。

	}

	return false;
}