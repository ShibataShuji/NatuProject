#pragma once

#include "main.h"
#include "model.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "Player.h"
#include "gameObject.h"
#include "CComponent.h"
#include "primitive.h"

//#include "testhghdfvs.h"
//#include "gameObject.h"

// デフォルトのコリジョンのモデルのサイズ
static const float def_Size = 50.0f;

class Collision : public CComponent
{
private:
	int			m_Collision_type = 0;		// カプセル子リジョンなのかボックスなのかとかを設定する
	bool		m_Movable = false;


	// カプセルコリジョン
	D3DXVECTOR2 m_Capusule_Size = D3DXVECTOR2(50.0f, 40.0f);		// カプセルの上下のスフィアの半径と、真ん中のボックスの高さ

	// スフィアコリジョン
	float		m_Sphere_Radius = 50.0f;			// スフィアの半径

	// ボックスコリジョン
	D3DXVECTOR2 m_Box_Size = D3DXVECTOR2(20.0f, 20.0f);				// ボックスコリジョンの横幅と縦幅

	Capsule		m_Capsule;

	Model* m_ModelA;
	Model* m_ModelB;
	Model* m_ModelC;

	D3DXVECTOR3 m_ColScale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 m_ColRotation = { 0.0f, 0.0f, 0.0f };

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;




public:

	// 親クラスのコンストラクタをやってくれる。
	using CComponent::CComponent;


	void Init() override
	{
		m_ModelA = new Model();
		m_ModelA->Load("asset\\model\\ColCylinder.obj");
		m_ModelB = new Model();
		m_ModelB->Load("asset\\model\\ColSphere.obj");
		m_ModelC = new Model();
		m_ModelC->Load("asset\\model\\ColSphere.obj");


		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

		Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
	}

	void Uninit() override
	{
		m_ModelA->Unload();
		delete m_ModelA;
		m_ModelB->Unload();
		delete m_ModelB;
		m_ModelC->Unload();
		delete m_ModelC;

		m_VertexLayout->Release();
		m_VertexShader->Release();
		m_PixelShader->Release();
	}

	void Update() override
	{
		// シーンをゲットしておく
		Scene* scene = Manager::GetScene();

		// 計算用の変数をゲット
		D3DXVECTOR3 t_Position = m_ParentGameObject->GetTempPosition();
		D3DXVECTOR3 t_Velocity = m_ParentGameObject->GetTempVelocity();

		
		// Scaleは1.0fで50cmのサイズなので、cmを50.0fで割って割合を出しておく
		m_Capsule.r = m_Capusule_Size.x / def_Size;

		// カプセルの情報をセットする。
		m_Capsule.SetCapsule(Point(t_Position.x, t_Position.y + 1.0f, t_Position.z), Point(t_Position.x, t_Position.y - 1.0f, t_Position.z), 1.0f, 1.0f);

		//
		m_ColScale.x = m_Capsule.r;
		m_ColScale.y = m_Capsule.r;
		m_ColScale.z = m_Capsule.r;


		// 全てのゲームオブジェクトを取得
		std::vector<GameObject*> allObj = scene->GetAllGameObjects(1);
		for (GameObject* obj : allObj)
		{
			// 自分だったらコンテニュー
			if (obj == m_ParentGameObject)
				continue;

			// 全てのオブジェクトの中からCollisionコンポーネントを持っているものだけを取得
			if (obj->HasComponent<Collision>())
			{

				Point p1(obj->GetPosition().x, obj->GetPosition().y + 1.0f, obj->GetPosition().z);
				Point p2(obj->GetPosition().x, obj->GetPosition().y - 1.0f, obj->GetPosition().z);

				obj->GetComponent<Collision>()->m_Capsule.SetCapsule(p1, p2, 1.0f, 1.0f);

				// 衝突したときのカプセルの中心、コアになっている線分の中での最短距離になるとこの座標が返される
				Point c_p1;
				Point c_p2;
				// c_p1からc_p2への単位方向ベクトル
				Vec3 c_UnitNormal;
				// 違反している距離
				float c_vd;		// violatedistance
				// 単位方向ベクトルと違反距離をかけた分移動させれば重なりを防ぐことができる。
				Vec3 c_Normal;
				// 衝突しているときの最短座標をここに保存する

				if (colCapsuleCapsule(m_Capsule, obj->GetComponent<Collision>()->m_Capsule, &c_p1, &c_p2, &c_vd))
				{
					// 衝突しているなら

					// 重なった分座標を戻すt_Position
					c_UnitNormal = (c_p1 - c_p2).getNorm();
					c_Normal = c_UnitNormal * c_vd;

					t_Position.x += c_Normal.x;
					t_Position.y += c_Normal.y;
					t_Position.z += c_Normal.z;

					//t_Velocity.x = 0.0f;
					//t_Velocity.y = 0.0f;
					//t_Velocity.z = 0.0f;
					
				}
				else
				{
					// 衝突していないなら
					int aaasd = 4;
				}
			}
		}














		// とりあえず地面は0.0fとしておく
		float groundHeight = 0.0f;

		// 地面との当たり判定。接地
		if (t_Position.y < groundHeight && t_Velocity.y < 0.0f)
		{
			t_Position.y = groundHeight;
			t_Velocity.y = 0.0f;
		}




		// m_TempVelocityの更新
		m_ParentGameObject->SetTempVelocity(t_Velocity);

		// m_Positionの更新
		m_ParentGameObject->SetTempPosition(t_Position);

		// 本物にTemporaryを更新
		m_ParentGameObject->TemporarySetUpdate();
	}

	void Draw() override
	{
		// 入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		// シェーダ設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


		// ワールドマトリクス設定
		D3DXMATRIX world, scale, rot, trans;
		//D3DXMatrixScaling(&scale, m_ColScale.x, m_ColScale.y, m_ColScale.z);
		//D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
		//D3DXMatrixTranslation(&trans, m_ParentGameObject->GetPosition().x, m_ParentGameObject->GetPosition().y, m_ParentGameObject->GetPosition().z);
		//world = scale * rot * trans;
		//Renderer::SetWorldMatrix(&world);

		//m_ModelA->Draw();

		// 上の球の描写
		D3DXMatrixScaling(&scale, m_ColScale.x, m_ColScale.y, m_ColScale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
		D3DXMatrixTranslation(&trans, m_Capsule.s.GetPoint1().x, m_Capsule.s.GetPoint1().y, m_Capsule.s.GetPoint1().z);
		//D3DXMatrixTranslation(&trans, 0.0f, 0.0f, 0.0f);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(&world);
		m_ModelB->Draw();

		// 下の球の描写
		D3DXMatrixScaling(&scale, m_ColScale.x, m_ColScale.y, m_ColScale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
		D3DXMatrixTranslation(&trans, m_Capsule.s.GetPoint2().x, m_Capsule.s.GetPoint2().y, m_Capsule.s.GetPoint2().z);
		//D3DXMatrixTranslation(&trans, 3.0f, 0.0f, 0.0f);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(&world);
		m_ModelC->Draw();
	}

	void SetMovable(bool movable)
	{
		m_Movable = movable;
	}


};

