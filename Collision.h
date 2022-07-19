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

//// デフォルトのコリジョンのモデルのサイズ
//static const float def_Size = 100.0f;

// コリジョンタイプの値の設定
static const int	BOX_COLLISION	   = 0;
static const int	CAPSULE_COLLISION = 1;

class Collision : public CComponent
{
private:
	int			m_Collision_type = 0;		// カプセル子リジョンなのかボックスなのかとかを設定する
	bool		m_Movable = false;
	float		m_OnTheGroundCheckNormal = 0.7f;	// 0.0f~1.0f 接地判定の時、衝突した時の方向ベクトルyがこれ以上のときに接地判定をtrueにする
	

	// すべてのコリジョンタイプで使うやつ。Radius = Scale.xで考える
	D3DXVECTOR3 m_ColScale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 m_ColRotation = { 0.0f, 0.0f, 0.0f };

	OBB			m_obb;
	CAPSULE		m_capsule;


	Model* m_ModelA;
	Model* m_ModelB;
	Model* m_ModelC;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;




public:

	// 親クラスのコンストラクタをやってくれる。
	using CComponent::CComponent;

	// modelのロードをやる。InitだとPlayerのInitより早く行われるのでコリジョンタイプがセットされる前になってしまうので分けてる
	void LoadCollisionModel()
	{
		if (m_Collision_type == BOX_COLLISION)
		{
			m_ModelA = new Model();
			m_ModelA->Load("asset\\model\\ColCube.obj");
			m_ModelB = new Model();
			m_ModelB->Load("asset\\model\\ColCube.obj");
			m_ModelC = new Model();
			m_ModelC->Load("asset\\model\\ColCube.obj");
		}
		if (m_Collision_type == CAPSULE_COLLISION)
		{
			m_ModelA = new Model();
			m_ModelA->Load("asset\\model\\ColCylinder.obj");
			m_ModelB = new Model();
			m_ModelB->Load("asset\\model\\ColSphere.obj");
			m_ModelC = new Model();
			m_ModelC->Load("asset\\model\\ColSphere.obj");
		}
	}


	void Init() override
	{


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

	OBB GetOBB()
	{
		return m_obb;
	}

	void Update() override
	{
		// シーンをゲットしておく
		Scene* scene = Manager::GetScene();

		// 接地判定を毎回falseにリセットしておく。でも他のコリジョンでtrueになったものをリセットしないようにtempで確認
		if(!m_ParentGameObject->GetTempOnTheGround())
			m_ParentGameObject->SetOnTheGround(false);			// リセット


		// コリジョンタイプによって回転の保存の仕方が違う?
		if (m_Collision_type == BOX_COLLISION)
		{
			m_ColRotation = m_ParentGameObject->GetRotation(); 

		}
		else if (m_Collision_type == CAPSULE_COLLISION)
		{
			// カプセルは縦固定
			m_ColRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//m_ColRotation = m_ParentGameObject->GetRotation();		// 一応これでカプセルも回転するけど球それぞれで回転
		}



		// 計算用に使う変数の準備
		D3DXVECTOR3 t_Position;
		D3DXVECTOR3 t_Velocity;
		t_Position = m_ParentGameObject->GetPosition();		// ゲット
		t_Velocity = m_ParentGameObject->GetVelocity();		// ゲット

		// WorldMatrixを取得しておく(Scale,Rotation,Posのデータ)
		D3DXMATRIX selfMatrix = m_ParentGameObject->GetWorldMatrix();

		// OBBの更新
		m_obb.CreateOBB(m_ColScale, selfMatrix);

		// カプセルの更新
		m_capsule.SetCapsule(m_ColScale.x, m_ColScale.y, t_Position);

		int AAA = 111;
		//_RPTN(_CRT_WARN, "deleting %f\n", selfMatrix._11);


		// 全てのゲームオブジェクトを取得
		std::vector<GameObject*> allObj = scene->GetAllGameObjects(1);
		for (GameObject* other : allObj)
		{
			// otherに衝突相手の親オブジェクトのポインターが入っている

			// 自分だったらコンテニュー
			if (other == m_ParentGameObject)
				continue;

			// 全てのオブジェクトの中からCollisionコンポーネントを持っているものだけを取得
			if (other->HasComponent<Collision>())
			{
				//static const int	BOX_COLLISION = 0;
				//static const int	CAPSULE_COLLISION = 1;

				// WorldMatrixを取得しておく
				D3DXMATRIX otherMatrix = other->GetWorldMatrix(); 

				// カプセル(自分)とOBB(相手)の衝突だったら
				if (m_Collision_type == CAPSULE_COLLISION && other->GetComponent<Collision>()->GetCollisionType() == BOX_COLLISION)
				{
					CapsuleOBB(this, other->GetComponent<Collision>());
				}
				// OBB(自分)とカプセル(相手)の衝突だったら
				else if (m_Collision_type == BOX_COLLISION && other->GetComponent<Collision>()->GetCollisionType() == CAPSULE_COLLISION)
				{
					CapsuleOBB(other->GetComponent<Collision>(), this);
				}
				// カプセル(自分)とカプセル(相手)の衝突だったら
				else if (m_Collision_type == CAPSULE_COLLISION && other->GetComponent<Collision>()->GetCollisionType() == CAPSULE_COLLISION)
				{
					CapsuleCapsule(this, other->GetComponent<Collision>());
				}

				//if (m_ParentGameObject->Gettagnum() == 2)
				//{
				//	// cp_Capsule にはカプセル側の最接近点の座標が取得できる(Closest point 最接近点)
				//	int flag;
				//	D3DXVECTOR3 cp_Capsule;
				//	cp_Capsule = ClosestPtCapsuleOBB(m_capsule, other->GetComponent<Collision>()->m_obb, flag);

				//	// 今のままだとカプセル側の最接近点がカプセルの外周なので、中心のラインで計算するように戻す(y座標は維持or上下の球体の場合戻す)
				//	D3DXVECTOR3 cp_CapsuleCenterLine = cp_Capsule;
				//	cp_CapsuleCenterLine.x = m_capsule.GetCenterPos().x;
				//	cp_CapsuleCenterLine.z = m_capsule.GetCenterPos().z;
				//	if (cp_Capsule.y > m_capsule.GetCenterPos().y)
				//		cp_CapsuleCenterLine.y = m_capsule.GetUpperSpherePos().y;
				//	if (cp_Capsule.y < m_capsule.GetCenterPos().y)
				//		cp_CapsuleCenterLine.y = m_capsule.GetLowerSpherePos().y;

				//	// cp_OBB にはOBB側の最接近点の座標が取得できる
				//	D3DXVECTOR3 cp_OBB;
				//	ClosestPtPointOBB(cp_CapsuleCenterLine, other->GetComponent<Collision>()->m_obb, cp_OBB);

				//	// distance カプセルからみたOBBへの最短距離(ベクトル)
				//	D3DXVECTOR3 distance;
				//	distance = cp_OBB - cp_CapsuleCenterLine;

				//	// length カプセルからOBBへの最短距離
				//	float length;
				//	length = D3DXVec3Length(&distance);

				//	// overlaplength カプセルとOBBの重なっている距離
				//	float overlaplength;
				//	overlaplength = m_capsule.m_Radius - length;
				//	// 方向ベクトルの準備
				//	D3DXVECTOR3 normal;
				//	D3DXVec3Normalize(&normal, &distance);

				//	// 衝突してたらの処理
				//	if (length <= m_capsule.m_Radius)
				//	{
				//		// 重なっていたら戻す処理
				//		t_Position = m_ParentGameObject->GetPosition();		// ゲット
				//		D3DXVECTOR3 backVector;
				//		backVector = normal * overlaplength;	// 方向ベクトルのむきに重なっている距離分戻す

				//		OverlapToBackPosition(m_ParentGameObject, other, backVector);		// この中でセットしている

				//		// これより下で衝突をしているならば接地しているとみなす
				//		float OnTheGroundHeight;
				//		OnTheGroundHeight = m_capsule.GetLowerSpherePos().y - m_capsule.m_Radius * (1.0f - m_OnTheGroundCoefficient);


				//		// 接地判定処理。flag == 1で上にいる。-1で下にいる。0で横
				//		if (flag == 1 && t_Velocity.y < 0.0f && cp_Capsule.y < OnTheGroundHeight)
				//		{
				//			m_ParentGameObject->SetOnTheGround(true);
				//			m_ParentGameObject->SetVelocity_y(0.0f);
				//		}
				//	}
				//}



				
			}
		}










	



		// とりあえず地面は0.0fとしておく
		float groundHeight = 0.0f;


		t_Position = m_ParentGameObject->GetPosition();		// ゲット
		t_Velocity = m_ParentGameObject->GetVelocity();		// ゲット
		// 地面との当たり判定。接地
		if (t_Position.y < groundHeight && t_Velocity.y < 0.0f)
		{
			m_ParentGameObject->SetPosition_y(groundHeight);
			m_ParentGameObject->SetOnTheGround(true);
			m_ParentGameObject->SetVelocity_y(0.0f);
		}

	}

	void Draw() override
	{
		// 入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		// シェーダ設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		if (m_Collision_type == BOX_COLLISION)
		{
			D3DXMATRIX world;

			// ワールドマトリクス設定
			world = CreateWorldMatrix(m_obb.m_Size, m_ColRotation, m_obb.m_Center);	// 関数にまとめた
			//world = CreateWorldMatrix(OBBScale, m_ColRotation, OBBpos);	// 関数にまとめた
			Renderer::SetWorldMatrix(&world);
			m_ModelA->Draw();

		}
		else if (m_Collision_type == CAPSULE_COLLISION)
		{
			D3DXVECTOR3 CylinderScale;
			CylinderScale.x = m_capsule.m_Radius;
			CylinderScale.y = m_capsule.m_HalfHeight;
			CylinderScale.z = m_capsule.m_Radius;

			D3DXMATRIX world;

			// ワールドマトリクス設定
			world = CreateWorldMatrix(CylinderScale, m_ColRotation, m_capsule.GetCenterPos());	// 関数にまとめた
			Renderer::SetWorldMatrix(&world);
			m_ModelA->Draw();

			D3DXVECTOR3 SphereScale;
			SphereScale = D3DXVECTOR3(m_capsule.m_Radius, m_capsule.m_Radius, m_capsule.m_Radius);

			// 上の球の描写
			world = CreateWorldMatrix(SphereScale, m_ColRotation, m_capsule.GetUpperSpherePos());	// 関数にまとめた
			Renderer::SetWorldMatrix(&world);
			m_ModelB->Draw();

			// 下の球の描写
			world = CreateWorldMatrix(SphereScale, m_ColRotation, m_capsule.GetLowerSpherePos());	// 関数にまとめた
			Renderer::SetWorldMatrix(&world);
			m_ModelC->Draw();
		}




	}


	void SetCollisionType(int type)
	{
		m_Collision_type = type;
	}
	int GetCollisionType()
	{
		return m_Collision_type;
	}


	void SetMovable(bool movable)
	{
		m_Movable = movable;
	}
	bool GetMovable()
	{
		return m_Movable;
	}

	// 
	void SetBoxScale(D3DXVECTOR3 scale)
	{
		m_ColScale = scale;
	}

	void SetCapsuleScale(float radius, float halfheight)
	{
		m_ColScale.x = radius;
		m_ColScale.y = halfheight;
		m_ColScale.z = radius;
	}


	// 衝突時の押し戻す処理
	void OverlapToBackPosition(GameObject* self, GameObject* other, D3DXVECTOR3 backvector)
	{

		D3DXVECTOR3 self_Pos = self->GetPosition();		// ゲット
		D3DXVECTOR3 other_Pos = other->GetPosition();					// ゲット

		// 衝突した相手が動かないなら
		if (!other->GetComponent<Collision>()->m_Movable)
		{
			// 自分だけポジションを変える
			self->SetPosition(self_Pos - backvector);		// セット
		}
		else if (other->GetComponent<Collision>()->m_Movable)
		{
			// 衝突した相手が動くなら
			// ベクトルを半分にする
			backvector *= 0.5f;
			// 自分と相手のポジションをベクトルの半分ずつ変える。(本当はその時の勢いとかもいれたがいいかも)
			self->SetPosition(self_Pos - backvector);		// セット
			other->SetPosition(other_Pos + backvector);		// セット
		}
	}


	//// Offsetの設定
	//void SetCollisionOffset(Point offset)
	//{
	//	m_CollisionOffset = offset;
	//}


	//// カプセルのサイズ等の設定
	//void SetCapsule(Point centerpos, float radius, float height)
	//{
	//	m_Capsule.SetCapsule(Point(centerpos.x, centerpos.y, centerpos.z), radius, height);
	//}



	// カプセルとボックスの当たり判定
	void CapsuleOBB(Collision* capsule, Collision* obb)
	{
		// cp_Capsule にはカプセル側の最接近点の座標が取得できる(Closest point 最接近点)
		int flag;
		D3DXVECTOR3 cp_Capsule;
		cp_Capsule = ClosestPtCapsuleOBB(capsule->m_capsule, obb->m_obb, flag);

		// 今のままだとカプセル側の最接近点がカプセルの外周なので、中心のラインで計算するように戻す(y座標は維持or上下の球体の場合戻す)
		D3DXVECTOR3 cp_CapsuleCenterLine = cp_Capsule;
		cp_CapsuleCenterLine.x = capsule->m_capsule.GetCenterPos().x;
		cp_CapsuleCenterLine.z = capsule->m_capsule.GetCenterPos().z;
		if (cp_Capsule.y > capsule->m_capsule.GetCenterPos().y)
			cp_CapsuleCenterLine.y = capsule->m_capsule.GetUpperSpherePos().y;
		if (cp_Capsule.y < capsule->m_capsule.GetCenterPos().y)
			cp_CapsuleCenterLine.y = capsule->m_capsule.GetLowerSpherePos().y;

		// cp_OBB にはOBB側の最接近点の座標が取得できる
		D3DXVECTOR3 cp_OBB;
		ClosestPtPointOBB(cp_CapsuleCenterLine, obb->m_obb, cp_OBB);

		// distance カプセルからみたOBBへの最短距離(ベクトル)
		D3DXVECTOR3 distance;
		distance = cp_OBB - cp_CapsuleCenterLine;

		// length カプセルからOBBへの最短距離
		float length;
		length = D3DXVec3Length(&distance);

		// overlaplength カプセルとOBBの重なっている距離
		float overlaplength;
		overlaplength = capsule->m_capsule.m_Radius - length;
		// 方向ベクトルの準備
		D3DXVECTOR3 normal;
		D3DXVec3Normalize(&normal, &distance);

		// 衝突してたらの処理
		if (length <= capsule->m_capsule.m_Radius)
		{
			D3DXVECTOR3 cap_Pos = capsule->m_ParentGameObject->GetPosition();		// ゲット
			D3DXVECTOR3 cap_Vel = capsule->m_ParentGameObject->GetVelocity();		// ゲット
			D3DXVECTOR3 obb_Pos = obb->m_ParentGameObject->GetPosition();			// ゲット
			D3DXVECTOR3 obb_Vel = obb->m_ParentGameObject->GetVelocity();			// ゲット

			// 重なっていたら戻す処理
			D3DXVECTOR3 backVector;
			backVector = normal * overlaplength;	// 方向ベクトルのむきに重なっている距離分戻す

			OverlapToBackPosition(capsule->m_ParentGameObject, obb->m_ParentGameObject, backVector);		// この中でセットしている


			// カプセル側接地判定処理。flag == 1で上にいる。-1で下にいる。0で横
			//if (flag == 1 && cap_Vel.y < 0.0f && cp_Capsule.y < OnTheGroundHeight_cap)
			if (flag == 1 && cap_Vel.y < 0.0f && -normal.y > capsule->m_OnTheGroundCheckNormal)
			{
				capsule->m_ParentGameObject->SetOnTheGround(true);
				capsule->m_ParentGameObject->SetTempOnTheGround(true);
				capsule->m_ParentGameObject->SetVelocity_y(0.0f);
			}

			// OBB側接地判定処理。
			if (flag == -1 && obb_Vel.y < 0.0f && normal.y > obb->m_OnTheGroundCheckNormal)
			{
				obb->m_ParentGameObject->SetOnTheGround(true);
				obb->m_ParentGameObject->SetTempOnTheGround(true);
				obb->m_ParentGameObject->SetVelocity_y(0.0f);
			}




		}
	}

	// カプセルとカプセルの当たり判定
	void CapsuleCapsule(Collision* self_capsule, Collision* other_capsule)
	{
		// 衝突したときのカプセルの中心、コアになっている線分の中での最短距離になるとこの座標が返される
		D3DXVECTOR3 c_p1;
		D3DXVECTOR3 c_p2;
		// c_p1からc_p2への単位方向ベクトル
		D3DXVECTOR3 c_UnitNormal;
		// 違反している距離
		float c_vd;		// violatedistance
		// 方向ベクトル。単位方向ベクトルと違反距離をかけた分移動させれば重なりを防ぐことができる。
		D3DXVECTOR3 c_Normal;
		// 衝突しているときの最短座標をここに保存する

		if (colCapsuleCapsule(self_capsule->m_capsule, other_capsule->m_capsule, &c_p1, &c_p2, &c_vd))
		{
			// 衝突しているなら

			D3DXVECTOR3 scap_vel = self_capsule->m_ParentGameObject->GetVelocity();
			D3DXVECTOR3 ocap_vel = other_capsule->m_ParentGameObject->GetVelocity();

			// 重なった分座標を戻すt_Position
			D3DXVECTOR3 tvec = c_p1 - c_p2;
			D3DXVec3Normalize(&c_UnitNormal, &tvec);
			c_Normal = c_UnitNormal * c_vd;				// 戻すベクトル		

			OverlapToBackPosition(self_capsule->m_ParentGameObject, other_capsule->m_ParentGameObject, -c_Normal);


			// カプセル側の接地判定処理
			// これより下で衝突をしているならば接地しているとみなす
			//float OnTheGroundHeight_cap;
			//OnTheGroundHeight_cap = capsule->m_capsule.GetLowerSpherePos().y - capsule->m_capsule.m_Radius * (1.0f - capsule->m_OnTheGroundCoefficient);

			// self側接地判定処理
			if (scap_vel.y < 0.0f && c_UnitNormal.y > self_capsule->m_OnTheGroundCheckNormal)
			{
				self_capsule->m_ParentGameObject->SetOnTheGround(true);
				self_capsule->m_ParentGameObject->SetTempOnTheGround(true);
				self_capsule->m_ParentGameObject->SetVelocity_y(0.0f);
			}
			// other側接地判定処理
			if (ocap_vel.y < 0.0f && -c_UnitNormal.y > other_capsule->m_OnTheGroundCheckNormal)
			{
				other_capsule->m_ParentGameObject->SetOnTheGround(true);
				other_capsule->m_ParentGameObject->SetTempOnTheGround(true);
				other_capsule->m_ParentGameObject->SetVelocity_y(0.0f);
			}

		}
		else
		{
			// 衝突していないなら
			int aaasd = 4;
		}
	}



};




//_RPTN(_CRT_WARN, "%f, %f, %f\n", cp_OBB.x, cp_OBB.y, cp_OBB.z);
//_RPTN(_CRT_WARN, "%f, %f, %f\n", cp_Capsule.x, cp_Capsule.y, cp_Capsule.z);
//_RPTN(_CRT_WARN, "%f, %f, %f\n", distance.x, distance.y, distance.z);
//_RPTN(_CRT_WARN, "%f\n", length);
////_RPTN(_CRT_WARN, "%f, %f, %f\n", normal.x, normal.y, normal.z);
//_RPTN(_CRT_WARN, "%d\n", flag);