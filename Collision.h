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
static const int	BOX_COLLISION = 0;
static const int	CAPSULE_COLLISION = 1;
static const int	SPHERE_COLLISION = 2;
static const int	RAY_COLLISION = 3;			// 線分の点Aから点Bの衝突している最接近点を求めるためのもの

// レスポンスタイプの値の設定
static const int	RESPONSE_IGNORE	 = 0;
static const int	RESPONSE_OVERLAP = 1;
static const int	RESPONSE_BLOCK   = 2;

class Collision : public CComponent
{
private:
	int			m_Collision_type = BOX_COLLISION;		// カプセル子リジョンなのかボックスなのかとかを設定する
	bool		m_Movable = false;
	float		m_OnTheGroundCheckNormal = 0.7f;	// 0.0f~1.0f 接地判定の時、衝突した時の方向ベクトルyがこれ以上のときに接地判定をtrueにする
	int			m_ResponseType = RESPONSE_BLOCK;


	// すべてのコリジョンタイプで使うやつ。Radius = ColScale.xで考える
	D3DXVECTOR3 m_ColRotation = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 m_ColScale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 m_ColInitScale = { 1.0f, 1.0f, 1.0f };


	OBB			m_obb;
	CAPSULE		m_capsule;
	SPHERE		m_sphere;
	//SEGMENT		m_raysegment;
	RAY			m_ray;


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
		if (m_Collision_type == SPHERE_COLLISION)
		{
			m_ModelA = new Model();
			m_ModelA->Load("asset\\model\\ColSphere.obj");
			m_ModelB = new Model();
			m_ModelB->Load("asset\\model\\ColSphere.obj");
			m_ModelC = new Model();
			m_ModelC->Load("asset\\model\\ColSphere.obj");
		}
		if (m_Collision_type == RAY_COLLISION)
		{
			m_ModelA = new Model();
			m_ModelA->Load("asset\\model\\ColSphere.obj");
			m_ModelB = new Model();
			m_ModelB->Load("asset\\model\\ColSphere.obj");
			m_ModelC = new Model();
			m_ModelC->Load("asset\\model\\ColCube.obj");
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
	CAPSULE GetCapsule()
	{
		return m_capsule;
	}
	SPHERE GetSphere()
	{
		return m_sphere;
	}
	RAY GetRay()
	{
		return m_ray;
	}

	void Update() override
	{
		// シーンをゲットしておく
		Scene* scene = Manager::GetScene();

		// 接地判定を毎回falseにリセットしておく。でも他のコリジョンでtrueになったものをリセットしないようにtempで確認
		if (!m_ParentGameObject->GetTempOnTheGround())
			m_ParentGameObject->SetOnTheGround(false);			// リセット


		// Scaleの更新
		D3DXVECTOR3 ScaleRate = m_ParentGameObject->GetScaleRate();
		m_ColScale.x = m_ColInitScale.x * ScaleRate.x;
		m_ColScale.y = m_ColInitScale.y * ScaleRate.y;
		m_ColScale.z = m_ColInitScale.z * ScaleRate.z;

		// コリジョンタイプによる更新
		if (m_Collision_type == BOX_COLLISION)
		{
			m_ColRotation = m_ParentGameObject->GetRotation();

		}
		else if (m_Collision_type == CAPSULE_COLLISION)
		{
			// カプセルは縦固定
			//m_ColRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_ColRotation = m_ParentGameObject->GetRotation();		// 一応これでカプセルも回転するけど球それぞれで回転
		}
		else if (m_Collision_type == SPHERE_COLLISION)
		{
			m_ColRotation = m_ParentGameObject->GetRotation();
		}
		else if (m_Collision_type == RAY_COLLISION)
		{
			SetRayNoHit();	// 毎回初めにリセットしてあげる
		}



		// 計算用に使う変数の準備
		D3DXVECTOR3 t_Position;
		D3DXVECTOR3 t_Velocity;
		t_Position = m_ParentGameObject->GetPosition();		// ゲット
		t_Velocity = m_ParentGameObject->GetVelocity();		// ゲット

		// WorldMatrixを取得しておく(Scale,Rotation,Posのデータ)
		//D3DXMATRIX selfMatrix = m_ParentGameObject->GetWorldMatrix();
		//D3DXMATRIX selfMatrix = CreateWorldMatrix(m_ColScale, m_ColRotation, t_Position);
		D3DXMATRIX selfMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_ColRotation, t_Position);

		// OBBの更新
		m_obb.CreateOBB(m_ColScale, selfMatrix);

		// カプセルの更新
		m_capsule.SetCapsule(m_ColScale.x, m_ColScale.y, t_Position, m_ColRotation);

		// スフィアの更新。半径はどれも同じなのでてきとーにxを使っている
		m_sphere.SetSpehre(t_Position, m_ColScale.x);

		int AAA = 111;
		//_RPTN(_CRT_WARN, "deleting %f\n", selfMatrix._11);

		//// 平面作る計算。ここでやらないけど今はテストでやってる
		//m_obb.CreatePlaneState(m_ColRotation);
		//// m_obb.m_Normal[1].x
		//
		//int asdff = 4;
		//_RPTN(_CRT_WARN, "AAA %f, %f\n", m_obb.m_Normal[1].x, m_obb.m_Normal[1].z);


		// 自分が無視だったら終了(座標等の更新は必要なのでここに書いている)
		if (m_ResponseType == RESPONSE_IGNORE)
			return;

		// 全てのゲームオブジェクトを取得
		std::vector<GameObject*> allObj = scene->GetAllGameObjects(1);
		for (GameObject* other : allObj)
		{
			// otherに衝突相手の親オブジェクト(ゲームオブジェクト)のポインターが入っている

			// 自分だったらコンテニュー
			if (other == m_ParentGameObject)
				continue;

			// 全てのオブジェクトの中からCollisionコンポーネントを持っているものだけを取得
			if (other->HasComponent<Collision>())
			{
				Collision* other_collision = other->GetComponent<Collision>();

				// 衝突相手が無視だったらコンテニュー
				if (other_collision->m_ResponseType == RESPONSE_IGNORE)
					continue;

				//// コンポーネントのリストを取得して、複数個コリジョンを持っていった場合に対応する
				//std::list<CComponent*> other_ComponentList = other->GetComponentList();
				//for (auto other_Component : other_ComponentList)
				//{
				//	// コンポーネントリストのそれがコリジョンクラスだったら
				//	if (typeid(*other_Component) == typeid(Collision))
				//	{
				//		// CComponentクラスをダウンをキャストしてCollisionクラスにする
				//		Collision* other_collision = dynamic_cast <Collision*>(other_Component);


						//static const int	BOX_COLLISION = 0;
						//static const int	CAPSULE_COLLISION = 1;

						// WorldMatrixを取得しておく
				D3DXMATRIX otherMatrix = other->GetWorldMatrix();

				// カプセル(自分)とOBB(相手)の衝突だったら
				if (m_Collision_type == CAPSULE_COLLISION && other_collision->GetCollisionType() == BOX_COLLISION)
				{
					CapsuleOBB(this, other_collision);
				}
				// OBB(自分)とカプセル(相手)の衝突だったら
				else if (m_Collision_type == BOX_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
				{
					CapsuleOBB(other_collision, this);
				}
				// カプセル(自分)とカプセル(相手)の衝突だったら
				else if (m_Collision_type == CAPSULE_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
				{
					CapsuleCapsule(this, other_collision);
				}
				// スフィア(自分)とカプセル(相手)の衝突だったら
				else if (m_Collision_type == SPHERE_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
				{
					SphereCapsule(this, other_collision);
				}
				// カプセル(自分)とスフィア(相手)の衝突だったら
				else if (m_Collision_type == CAPSULE_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
				{
					SphereCapsule(other_collision, this);
				}
				// スフィア(自分)とOBB(相手)の衝突だったら
				else if (m_Collision_type == SPHERE_COLLISION && other_collision->GetCollisionType() == BOX_COLLISION)
				{
					SphereOBB(this, other_collision);
				}
				// OBB(自分)とスフィア(相手)の衝突だったら
				else if (m_Collision_type == BOX_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
				{
					SphereOBB(other_collision, this);
				}
				// スフィア(自分)とスフィア(相手)の衝突だったら
				else if (m_Collision_type == SPHERE_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
				{
					SphereSphere(this, other_collision);
				}
				// レイ(自分)とスフィア(相手)の衝突だったら
				else if (m_Collision_type == RAY_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
				{
					if (RaySegmentSphere(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision))
						RaySegmentSphereDivCalc(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision);	// この中で一番近い距離を出してセットしている
				}
				// レイ(自分)とカプセル(相手)の衝突だったら
				else if (m_Collision_type == RAY_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
				{
					if (RaySegmentCapsule(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision))
						RaySegmentCapsuleDivCalc(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision);	// この中で一番近い距離を出してセットしている
				}
				// レイ(自分)とOBB(相手)の衝突だったら
				else if (m_Collision_type == RAY_COLLISION && other_collision->GetCollisionType() == BOX_COLLISION)
				{
					if (RaySegmentOBB(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision))
						RaySegmentOBBDivCalc(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision);
				}
				// OBB(自分)とOBB(相手)の衝突だったら。ここはまだ未完成。overlapだけ判断できる。
				else if (m_Collision_type == BOX_COLLISION && other_collision->GetCollisionType() == BOX_COLLISION)
				{
					// 省略用に相手のobbのポインタを用意。自分はm_obb
					OBB* other_obb = &other_collision->m_obb;	//other_collision
					/*D3DXMATRIX other_Matrix = CreateWorldMatrix(other_collision->m_ColScale, other_collision->m_ColRotation, other_collision->m_obb.m_Center);
					other_obb->CreateOBB(other_collision->m_ColScale, other_Matrix);*/

					//bool bbb = OBBOBB(this, other_collision);

					//int ccc = 10;
					//if (bbb == false)
					//	ccc = 0;
					//else
					//	ccc = 1;
					////_RPTN(_CRT_WARN, "FPS %d\n", ccc);


					// 値の取得・準備
					// 分割数 DivisionNum
					int DivNum = 100;

					// Postion
					D3DXVECTOR3 self_oldPos = m_ParentGameObject->GetOldPosition();		// 前回のフレームでの座標
					D3DXVECTOR3 other_oldPos = other->GetOldPosition();
					D3DXVECTOR3 self_AfterPos = m_ParentGameObject->GetPosition();		// 衝突しなかった場合の最終移動座標
					D3DXVECTOR3 other_AfterPos = other->GetPosition();
					D3DXVECTOR3 self_movementFrame = self_AfterPos - self_oldPos;		// このフレームでの移動ベクトル
					D3DXVECTOR3 other_movementFrame = other_AfterPos - other_oldPos;
					float self_VecLength = D3DXVec3Length(&self_movementFrame);			// ベクトルの長さ
					float other_VecLength = D3DXVec3Length(&other_movementFrame);
					D3DXVECTOR3 self_Normal;
					D3DXVECTOR3 other_Normal;
					D3DXVec3Normalize(&self_Normal, &self_movementFrame);				// このフレームでの方向ベクトル(単位ベクトル)を作っておく
					D3DXVec3Normalize(&other_Normal, &other_movementFrame);

					D3DXVECTOR3 self_movementOne = self_movementFrame / DivNum;		// 分割したときの１回での移動距離
					D3DXVECTOR3 other_movementOne = other_movementFrame / DivNum;


					// Rotation
					D3DXVECTOR3 self_oldRotation = m_ParentGameObject->GetOldRotation();		// 前回のフレームでの回転
					D3DXVECTOR3 other_oldRotation = other->GetOldRotation();
					D3DXVECTOR3 self_AfterRotation = m_ParentGameObject->GetRotation();		// 衝突しなかった場合の最終回転
					D3DXVECTOR3 other_AfterRotation = other->GetRotation();
					D3DXVECTOR3 self_RotationFrame = self_AfterRotation - self_oldRotation;		// 衝突しない場合のこのフレームでの回転量
					D3DXVECTOR3 other_RotationFrame = other_AfterRotation - other_oldRotation;

					D3DXVECTOR3 self_RotationOne = self_RotationFrame / DivNum;		// 分割したときの１回での回転量
					D3DXVECTOR3 other_RotationOne = other_RotationFrame / DivNum;


					// Scale
					D3DXVECTOR3 self_oldScale = m_ParentGameObject->GetOldScaleRate();		// 前回のフレームでのScaleRate
					self_oldScale.x *= m_ColInitScale.x;
					self_oldScale.y *= m_ColInitScale.y;
					self_oldScale.z *= m_ColInitScale.z;
					D3DXVECTOR3 other_oldScale = other->GetOldScaleRate();
					other_oldScale.x *= m_ColInitScale.x;
					other_oldScale.y *= m_ColInitScale.y;
					other_oldScale.z *= m_ColInitScale.z;
					D3DXVECTOR3 self_AfterScale = m_ParentGameObject->GetScaleRate();		// 衝突しなかった場合の最終ScaleRate
					self_AfterScale.x *= m_ColInitScale.x;
					self_AfterScale.y *= m_ColInitScale.y;
					self_AfterScale.z *= m_ColInitScale.z;
					D3DXVECTOR3 other_AfterScale = other->GetScaleRate();
					other_AfterScale.x *= m_ColInitScale.x;
					other_AfterScale.y *= m_ColInitScale.y;
					other_AfterScale.z *= m_ColInitScale.z;
					D3DXVECTOR3 self_ScaleFrame = self_AfterScale - self_oldScale;		// 衝突しない場合のこのフレームでのScaleRateの変化量
					D3DXVECTOR3 other_ScaleFrame = other_AfterScale - other_oldScale;

					D3DXVECTOR3 self_ScaleOne = self_ScaleFrame / DivNum;		// 分割したときの１回でのScaleRateの変化量
					D3DXVECTOR3 other_ScaleOne = other_ScaleFrame / DivNum;

					//// WorldMatrix Afterバージョン
					//D3DXMATRIX self_AfterMatrix = CreateWorldMatrix(self_AfterScale, self_AfterRotation, self_AfterPos);
					D3DXMATRIX self_AfterMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), self_AfterRotation, self_AfterPos);
					// OBB  Afterバージョン
					OBB self_AfterOBB;
					self_AfterOBB.CreateOBB(self_AfterScale, self_AfterMatrix);

					// 衝突していたら
					if (OBBOBB(this, other_collision))
					{
						// 衝突相手のOBBの更新いらないかも
						/*D3DXMATRIX other_Matrix = CreateWorldMatrix(other_collision->m_ColScale, other_collision->m_ColRotation, other_collision->m_obb.m_Center);
						other_obb->CreateOBB(other_collision->m_ColScale, other_Matrix);*/


						// 相手が動かないなら自分だけ動かす処理
						if (other_collision->m_Movable == false)
						{
							//// WorldMatrixをOldで作り直すold
							//D3DXMATRIX self_oldMatrix = CreateWorldMatrix(self_oldScale, self_oldRotation, self_oldPos);
							D3DXMATRIX self_oldMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), self_oldRotation, self_oldPos);

							// OBBを作り直すold
							OBB self_oldOBB;
							self_oldOBB.CreateOBB(self_oldScale, self_oldMatrix);

							D3DXVECTOR3 self_bcPos;	//before collision 衝突する直前
							D3DXVECTOR3 self_bcRot;
							D3DXVECTOR3 self_bcSca;
							int MaxTrial = 50;
							int low = 0;
							int high = DivNum;
							//int mid = (low + high) / 2;
							int mid;
							int oldmid = -1;
							for (int i = 0; i < MaxTrial; i++)
							{
								mid = (low + high) / 2;

								// 2回連続でmidが同じ値だったら
								if (oldmid == mid)
								{
									break;
								}

								self_bcPos = self_oldPos + (self_movementOne * mid);
								self_bcRot = self_oldRotation + (self_RotationOne * mid);
								self_bcSca = self_oldScale + (self_ScaleOne * mid);

								//// WorldMatrixを作り直すnew
								//D3DXMATRIX self_bcMatrix = CreateWorldMatrix(self_bcSca, self_bcRot, self_bcPos);
								D3DXMATRIX self_bcMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), self_bcRot, self_bcPos);

								// OBBを作り直すnew
								OBB self_bcOBB;
								self_bcOBB.CreateOBB(self_bcSca, self_bcMatrix);

								// OBBの上書き
								m_obb = self_bcOBB;

								if (OBBOBB(this, other_collision))
								{
									// 衝突しているなら、分割数を更に戻す
									high = mid;
								}
								else
								{
									// 衝突していないなら、分割数を更に進める
									low = mid;
								}

								//if (low >= high)
								//{
								//	_RPTN(_CRT_WARN, "GGG %d\n", ggg);
								//	break;
								//}

								oldmid = mid;	// midの値の保存
							}
							//// 分割した後衝突ギリギリだった数がmidなので、mid以降の値分滑らせる必要がある
							//// 方向ベクトルを、衝突相手のOBBの面に対して平行になるように曲げる。self_movementFrame
							//float RemainingRatio = (float)mid / (float)DivNum;			// 残りの割合
							//float RemainingLenght = self_VecLength * RemainingRatio;	// 残りの長さ
							//D3DXVECTOR3 self_RemainingVec = self_movementFrame * RemainingRatio;		// 残りの移動ベクトル

							// midには衝突直前(衝突していない)(mid+1は衝突している)
							// 衝突直前の情報が求められたので次に
							// 片方のOBBの8つの頂点のうちどの頂点がもう片方のOBBに１番近いかを求める
							// まずは平面に使う情報を更新させる(m_obbにはself_bcOBBを上書き済みなのでここでは書かなくてよい)
							m_obb.CreatePlaneState(self_bcRot);						// メンバ変数の平面に使うものが更新される
							other_obb->CreatePlaneState(other_AfterRotation);		// 相手側はこの場合動かない(こちら側の影響を受けない)のでこれで。

							// AのOBBに対して一番近いBのOBBの頂点の添え字が返ってくる
							// この後戻す処理をしたい方をBに設定してあげるといい。
							int NearVer = GetOBBOBBNearestVertex(*other_obb, m_obb);

							float x = m_obb.m_VertexPos[NearVer].x;
							float y = m_obb.m_VertexPos[NearVer].y;
							float z = m_obb.m_VertexPos[NearVer].z;

							// 次にその頂点がどの面と一番近いのかを計算する
							D3DXVECTOR3 distancevec;					// 戻り値用。一番近い面から垂直な距離が返る
							int NearPlane = GetPointOBBNearestPlane(&distancevec, m_obb.m_VertexPos[NearVer], *other_obb);

							_RPTN(_CRT_WARN, "NearVer %d\n", NearVer);
							_RPTN(_CRT_WARN, "NearPlane %d\n", NearPlane);

							// 衝突した頂点と面が求まったので、衝突後の座標で、その頂点がその面に
							// どれだけめり込んでるかを計算しなおす。めり込んでる分面の法線ベクトルの方向に戻す

							// OBBをAfterバージョンで上書き
							m_obb = self_AfterOBB;
							m_obb.CreatePlaneState(self_AfterRotation);
							distancevec = CalcPlaneToPoint(m_obb, NearVer, *other_obb, NearPlane);

							//D3DXVECTOR3 self_newPos = self_AfterPos - distancevec;
							//selfMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), self_AfterRotation, self_newPos);
							//// OBBの更新
							//m_obb.CreateOBB(m_ColScale, selfMatrix);

							OverlapToBackPosition(this, other_collision, -distancevec);		// この中でセットしている

							// 頂点が１つも衝突していない場合は線分同士が衝突しているので
							// ClosestPtSegmentSegment を使う

							//// 計算で求めた衝突しないぎりぎりの時間に戻してあげる
							//m_ParentGameObject->SetPosition(self_bcPos);
							//m_ParentGameObject->SetRotation(self_bcRot);
							//m_ParentGameObject->SetScaleRate(self_bcSca);

							//m_ParentGameObject->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
							//m_ParentGameObject->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

							//_RPTN(_CRT_WARN, "RRR %d\n", rrr);



						}

					}



				}



				//if (m_ParentGameObject->Gettagnum() == 2)
				//{
				//	// cp_Capsule にはカプセル側の最接近点の座標が取得できる(Closest point 最接近点)
				//	int flag;
				//	D3DXVECTOR3 cp_Capsule;
				//	cp_Capsule = ClosestPtCapsuleOBB(m_capsule, other_collision->m_obb, flag);

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
				//	ClosestPtPointOBB(cp_CapsuleCenterLine, other_collision->m_obb, cp_OBB);

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

		//	}
		//}


			}
		}

		// 全ての消灯判定の計算が終わった後に、RAYの衝突したものからの法線を計算で求める
		if (m_Collision_type == RAY_COLLISION)
		{
			if (m_ray.HitCollision != nullptr)
			{
				m_ray.CalcCollisionPointwithLengthNormal();		// レイの衝突座標を線分の長さと法線から計算する
				D3DXVECTOR3 HitPos = m_ray.m_CollisionPoint;
				if (m_ray.HitCollision->m_Collision_type == BOX_COLLISION)
				{
					D3DXVECTOR3 cp_OBB;		// cp_OBB にはOBB側の最接近点の座標が取得できる
					ClosestPtPointOBB(HitPos, m_ray.HitCollision->m_obb, cp_OBB);
					D3DXVECTOR3 distance = HitPos - cp_OBB;
					D3DXVECTOR3 normal;
					D3DXVec3Normalize(&normal, &distance);
					m_ray.m_normal = normal;
					m_ray.m_CollisionPoint = cp_OBB;		// 衝突点をobbの最接近点に変更する
				}
				else if (m_ray.HitCollision->m_Collision_type == CAPSULE_COLLISION)
				{
					float t;
					D3DXVECTOR3 cp_segment;		// cp_segmentには線分側の最短座標が求まる
					ClosetPtPointSegment(HitPos, m_ray.HitCollision->m_capsule.GetLowerSpherePos(), m_ray.HitCollision->m_capsule.GetUpperSpherePos(), t, cp_segment);
					D3DXVECTOR3 distance = HitPos - cp_segment;
					D3DXVECTOR3 normal;
					D3DXVec3Normalize(&normal, &distance);
					m_ray.m_normal = normal;
					// 衝突点をカプセルの最接近点に変更する
					m_ray.m_CollisionPoint = cp_segment + (m_ray.HitCollision->m_capsule.m_Radius * normal);
				}
				else if (m_ray.HitCollision->m_Collision_type == SPHERE_COLLISION)
				{
					D3DXVECTOR3 distance = HitPos - m_ray.HitCollision->m_sphere.m_Center;
					D3DXVECTOR3 normal;
					D3DXVec3Normalize(&normal, &distance);
					m_ray.m_normal = normal;
					// 衝突点をスフィアの最接近点に変更する
					m_ray.m_CollisionPoint = m_ray.HitCollision->m_sphere.m_Center + (m_ray.HitCollision->m_sphere.m_Radius * normal);
				}
				_RPTN(_CRT_WARN, "normal %f, %f, %f\n", m_ray.m_normal.x, m_ray.m_normal.y, m_ray.m_normal.z);
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
			//world = CreateWorldMatrix(m_obb.m_Size, m_ColRotation, m_obb.m_Center);	// 関数にまとめた
			world = CreateWorldMatrix(m_ColScale, m_ColRotation, m_obb.m_Center);	// 関数にまとめた
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
		else if (m_Collision_type == SPHERE_COLLISION)
		{
			D3DXMATRIX world;

			// ワールドマトリクス設定
			world = CreateWorldMatrix(m_ColScale, m_ColRotation, m_sphere.m_Center);	// 関数にまとめた
			Renderer::SetWorldMatrix(&world);
			m_ModelA->Draw();
		}
		else if (m_Collision_type == RAY_COLLISION)
		{
			D3DXMATRIX world;
			D3DXVECTOR3 RayScale = D3DXVECTOR3(0.06f, 0.06f, 0.06f);
			D3DXVECTOR3 RayRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// ワールドマトリクス設定
			world = CreateWorldMatrix(RayScale, RayRotation, m_ray.m_Segment.m_pos1);
			Renderer::SetWorldMatrix(&world);
			m_ModelA->Draw(); 

			world = CreateWorldMatrix(RayScale, RayRotation, m_ray.m_Segment.m_pos2);
			Renderer::SetWorldMatrix(&world);
			m_ModelB->Draw();

			// デフォ値(999)とかだったらp2の場所に表示する
			RayScale = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
			if (m_ray.m_length < 997.0f)
			{
				//m_ray.CalcCollisionPointwithLengthNormal();
				D3DXVECTOR3 collisionpoint = m_ray.m_CollisionPoint;				// 衝突点は衝突相手の最接近点が入っている
				world = CreateWorldMatrix(RayScale, RayRotation, collisionpoint);	// 衝突点
				Renderer::SetWorldMatrix(&world);
				m_ModelC->Draw();
			}
			else
			{
				world = CreateWorldMatrix(RayScale, RayRotation, m_ray.m_Segment.m_pos2);	// 衝突点
				Renderer::SetWorldMatrix(&world);
				m_ModelC->Draw();
			}
		}





	}


	void SetColInitScale(D3DXVECTOR3 setcolscale)
	{
		m_ColInitScale = setcolscale;
		m_ColScale = m_ColInitScale;
	}


	void SetCollisionType(int type)
	{
		m_Collision_type = type;
	}
	int GetCollisionType()
	{
		return m_Collision_type;
	}

	// RESPONSE_IGNORE,RESPONSE_OVERLAP,RESPONSE_BLOCK
	void SetResponseType(int response_type)
	{
		m_ResponseType = response_type;
	}
	int GetResponseType()
	{
		return m_ResponseType;
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
	void SetBoxScale(D3DXVECTOR3 initscale)
	{
		//m_ColScale = scale;
		SetColInitScale(initscale);
	}

	void SetCapsuleScale(float radius, float halfheight)
	{
		//m_ColScale.x = radius;
		//m_ColScale.y = halfheight;
		//m_ColScale.z = radius;
		D3DXVECTOR3 initscale;
		initscale.x = radius;
		initscale.y = halfheight;
		initscale.z = radius;
		SetColInitScale(initscale);
	}

	void SetSphereScale(float radius)
	{
		//m_ColScale = scale;
		SetColInitScale(D3DXVECTOR3(radius, radius, radius));
	}

	void SetRaySegment(D3DXVECTOR3 p1, D3DXVECTOR3 p2)
	{
		m_ray.m_Segment.m_pos1 = p1;
		m_ray.m_Segment.m_pos2 = p2;
	}


	// 衝突時の押し戻す処理
	//void OverlapToBackPosition(GameObject* self, GameObject* other, D3DXVECTOR3 backvector)
	void OverlapToBackPosition(Collision* self_collision, Collision* other_collision, D3DXVECTOR3 backvector)
	{
		GameObject* self_parent = self_collision->m_ParentGameObject;
		GameObject* other_parent = other_collision->m_ParentGameObject;

		float se = self_parent->GetPosition().x;
		float ot = other_parent->GetPosition().x;

		if (self_parent == other_parent)
			int gg = 5;

		D3DXVECTOR3 self_Pos = self_parent->GetPosition();		// ゲット
		D3DXVECTOR3 other_Pos = other_parent->GetPosition();					// ゲット

		// 自分が動くものなら
		if (self_collision->m_Movable)
		{
			// 衝突した相手が動かないなら
			if (!other_collision->m_Movable)
			{
				// 自分だけポジションを変える
				self_parent->SetPosition(self_Pos - backvector);		// セット
			}
			else if (other_collision->m_Movable)
			{
				// 衝突した相手が動くなら
				// ベクトルを半分にする
				backvector *= 0.5f;
				// 自分と相手のポジションをベクトルの半分ずつ変える。(本当はその時の勢いとかもいれたがいいかも)
				self_parent->SetPosition(self_Pos - backvector);		// セット
				other_parent->SetPosition(other_Pos + backvector);		// セット
			}
		}
		else
		{
			// 衝突した相手が動かないなら
			if (!other_collision->m_Movable)
			{
				// 何も起きない
			}
			else if (other_collision->m_Movable)
			{
				// 衝突した相手が動くなら
				// 相手だけ戻す
				other_parent->SetPosition(other_Pos + backvector);		// セット
			}
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



	//// カプセルとボックスの当たり判定
	//void CapsuleOBB(Collision* capsule, Collision* obb)
	//{
	//	// cp_Capsule にはカプセル側の最接近点の座標が取得できる(Closest point 最接近点)
	//	int flag;
	//	D3DXVECTOR3 cp_Capsule;
	//	cp_Capsule = ClosestPtCapsuleOBB(capsule->m_capsule, obb->m_obb, flag);

	//	// 今のままだとカプセル側の最接近点がカプセルの外周なので、中心のラインで計算するように戻す(y座標は維持or上下の球体の場合戻す)
	//	D3DXVECTOR3 cp_CapsuleCenterLine = cp_Capsule;
	//	cp_CapsuleCenterLine.x = capsule->m_capsule.GetCenterPos().x;
	//	cp_CapsuleCenterLine.z = capsule->m_capsule.GetCenterPos().z;
	//	if (cp_Capsule.y > capsule->m_capsule.GetCenterPos().y)
	//		cp_CapsuleCenterLine.y = capsule->m_capsule.GetUpperSpherePos().y;
	//	if (cp_Capsule.y < capsule->m_capsule.GetCenterPos().y)
	//		cp_CapsuleCenterLine.y = capsule->m_capsule.GetLowerSpherePos().y;

	//	// cp_OBB にはOBB側の最接近点の座標が取得できる
	//	D3DXVECTOR3 cp_OBB;
	//	ClosestPtPointOBB(cp_CapsuleCenterLine, obb->m_obb, cp_OBB);

	//	// distance カプセルからみたOBBへの最短距離(ベクトル)
	//	D3DXVECTOR3 distance;
	//	distance = cp_OBB - cp_CapsuleCenterLine;

	//	// length カプセルからOBBへの最短距離
	//	float length;
	//	length = D3DXVec3Length(&distance);

	//	// overlaplength カプセルとOBBの重なっている距離
	//	float overlaplength;
	//	overlaplength = capsule->m_capsule.m_Radius - length;
	//	// 方向ベクトルの準備
	//	D3DXVECTOR3 normal;
	//	D3DXVec3Normalize(&normal, &distance);

	//	// 衝突してたらの処理
	//	if (length <= capsule->m_capsule.m_Radius)
	//	{
	//		D3DXVECTOR3 cap_Pos = capsule->m_ParentGameObject->GetPosition();		// ゲット
	//		D3DXVECTOR3 cap_Vel = capsule->m_ParentGameObject->GetVelocity();		// ゲット
	//		D3DXVECTOR3 obb_Pos = obb->m_ParentGameObject->GetPosition();			// ゲット
	//		D3DXVECTOR3 obb_Vel = obb->m_ParentGameObject->GetVelocity();			// ゲット

	//		// 重なっていたら戻す処理
	//		D3DXVECTOR3 backVector;
	//		backVector = normal * overlaplength * 0.8;	// 方向ベクトルのむきに重なっている距離分戻す

	//		OverlapToBackPosition(capsule->m_ParentGameObject, obb->m_ParentGameObject, backVector);		// この中でセットしている


	//		// カプセル側接地判定処理。flag == 1で上にいる。-1で下にいる。0で横
	//		//if (flag == 1 && cap_Vel.y < 0.0f && cp_Capsule.y < OnTheGroundHeight_cap)
	//		if (flag == 1 && cap_Vel.y < 0.0f && -normal.y > capsule->m_OnTheGroundCheckNormal)
	//		{
	//			capsule->m_ParentGameObject->SetOnTheGround(true);
	//			capsule->m_ParentGameObject->SetTempOnTheGround(true);
	//			capsule->m_ParentGameObject->SetVelocity_y(0.0f);
	//		}

	//		// OBB側接地判定処理。
	//		if (flag == -1 && obb_Vel.y < 0.0f && normal.y > obb->m_OnTheGroundCheckNormal)
	//		{
	//			obb->m_ParentGameObject->SetOnTheGround(true);
	//			obb->m_ParentGameObject->SetTempOnTheGround(true);
	//			obb->m_ParentGameObject->SetVelocity_y(0.0f);
	//		}




	//	}
	//}

	// カプセルとボックスの当たり判定
	void CapsuleOBB(Collision* capsule, Collision* obb)
	{
		// cp_Capsule にはカプセル側の最接近点の座標が取得できる(Closest point 最接近点)
		int flag;
		D3DXVECTOR3 cp_Capsule;
		cp_Capsule = ClosestPtCapsuleOBB(capsule->m_capsule, obb->m_obb, flag);

		//  求めたカプセルの表面の最接近点と、カプセルの軸の線分との最接近点を求める
		float t;
		D3DXVECTOR3 cp_Segment;		// cp_Segmentには線分側の最短座標が求まる
		ClosetPtPointSegment(cp_Capsule, capsule->m_capsule.GetLowerSpherePos(), capsule->m_capsule.GetUpperSpherePos(), t, cp_Segment);

		// 線分の最接近点でカプセル側でスフィアを作る
		capsule->m_sphere.SetSpehre(cp_Segment, capsule->m_capsule.m_Radius);

		// スフィアとOBBの衝突判定を使う
		SphereOBB(capsule, obb);
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
			c_Normal = c_UnitNormal * c_vd * 0.8;				// 戻すベクトル		

			OverlapToBackPosition(self_capsule, other_capsule, -c_Normal);


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




	// スフィアとOBBの当たり判定
	void SphereOBB(Collision* sphere, Collision* obb)
	{
		// cp_OBB にはOBB側の最接近点の座標が取得できる(Closest point 最接近点)
		int flag;
		D3DXVECTOR3 cp_OBB;
		ClosestPtPointOBB(sphere->m_sphere.m_Center, obb->m_obb, cp_OBB);

		// distance スフィアからみたOBBへの最短距離(ベクトル)
		D3DXVECTOR3 distance;
		distance = cp_OBB - sphere->m_sphere.m_Center;

		// length スフィアからOBBへの最短距離
		float length;
		length = D3DXVec3Length(&distance);

		// overlaplength スフィアとOBBの重なっている距離
		float overlaplength;
		overlaplength = sphere->m_sphere.m_Radius - length;

		// スフィアからみたOBBへの方向ベクトル
		D3DXVECTOR3 normal;
		D3DXVec3Normalize(&normal, &distance);

		// 衝突してたらの処理
		if (length <= sphere->m_sphere.m_Radius)
		{
			// 重なっていたら戻す処理。戻しすぎのぶるぶるを解消するためにすくなめに戻している。これはヒットしてる時間で制御してない利点
			D3DXVECTOR3 backVector;
			backVector = normal * overlaplength * 0.8;	// 方向ベクトルのむきに重なっている距離分戻す

			OverlapToBackPosition(sphere, obb, backVector);		// この中でセットしている

			D3DXVECTOR3 sphere_Vel = sphere->m_ParentGameObject->GetVelocity();
			D3DXVECTOR3 obb_Vel = obb->m_ParentGameObject->GetVelocity();

			// スフィア側接地判定処理。flag == 1で上にいる。-1で下にいる。0で横
			if (sphere_Vel.y < 0.0f && -normal.y > sphere->m_OnTheGroundCheckNormal)
			{
				sphere->m_ParentGameObject->SetOnTheGround(true);
				sphere->m_ParentGameObject->SetTempOnTheGround(true);
				sphere->m_ParentGameObject->SetVelocity_y(0.0f);
			}

			// OBB側接地判定処理。
			if (obb_Vel.y < 0.0f && normal.y > obb->m_OnTheGroundCheckNormal)
			{
				obb->m_ParentGameObject->SetOnTheGround(true);
				obb->m_ParentGameObject->SetTempOnTheGround(true);
				obb->m_ParentGameObject->SetVelocity_y(0.0f);
			}
		}
	}


	//static bool SPHERECAPSULE(const SPHERE& sp, const CAPSULE& cap, D3DXVECTOR3& cp_capsule)
	void SphereCapsule(Collision* sphere, Collision* capsule)
	{
		// 衝突しているかどうかの初期設定
		bool hit = false;

		//まず最近接点を設定
		float t;
		D3DXVECTOR3 cp_segment;
		// カプセルの線分とスフィアの中心での最短座標を求める。cp_segmentには線分側の最短座標が求まる
		ClosetPtPointSegment(sphere->m_sphere.m_Center, capsule->m_capsule.GetLowerSpherePos(), capsule->m_capsule.GetUpperSpherePos(), t, cp_segment);
		//ClosetPointは、カプセル線分上の点である
		D3DXVECTOR3 distance = sphere->m_sphere.m_Center - cp_segment;		// 2つの点の距離を求める
		D3DXVECTOR3 nNormal;
		D3DXVec3Normalize(&nNormal, &distance);
		//  カプセル側の最近接点が取得できる
		D3DXVECTOR3 cp_capsule = cp_segment + (distance * capsule->m_capsule.m_Radius);
		//衝突しているか判別。なぜ距離の平方(2乗)なのかというと、計算で求まるものは平方で出てきて
		// それを1乗のじょうたいにする計算の処理を省くため。別に多少重くていいなら普通に距離出していい。その場合下のとこ半径.
		float length = D3DXVec3Length(&distance);

		float radius = sphere->m_sphere.m_Radius + capsule->m_capsule.m_Radius;
		if (length <= radius)
			hit = true;

		// 違反距離を求める
		float overlength = length - radius;
		D3DXVECTOR3 backVector = overlength * nNormal * 0.8;

		if (hit)
		{

			OverlapToBackPosition(sphere, capsule, backVector);		// この中でセットしている

			D3DXVECTOR3 sphere_Vel = sphere->m_ParentGameObject->GetVelocity();
			D3DXVECTOR3 capsule_Vel = capsule->m_ParentGameObject->GetVelocity();

			// スフィア側接地判定処理。flag == 1で上にいる。-1で下にいる。0で横
			if (sphere_Vel.y < 0.0f && nNormal.y > sphere->m_OnTheGroundCheckNormal)
			{
				sphere->m_ParentGameObject->SetOnTheGround(true);
				sphere->m_ParentGameObject->SetTempOnTheGround(true);
				sphere->m_ParentGameObject->SetVelocity_y(0.0f);
			}

			// カプセル側接地判定処理。
			if (capsule_Vel.y < 0.0f && -nNormal.y > capsule->m_OnTheGroundCheckNormal)
			{
				capsule->m_ParentGameObject->SetOnTheGround(true);
				capsule->m_ParentGameObject->SetTempOnTheGround(true);
				capsule->m_ParentGameObject->SetVelocity_y(0.0f);
			}
		}

	}


	void SphereSphere(Collision* self_sp, Collision* other_sp)
	{
		// 衝突しているかどうかの初期設定
		bool hit = false;

		//まず最近接点を設定
		// selfから見たotherへのベクトル
		D3DXVECTOR3 distance = other_sp->m_sphere.m_Center - self_sp->m_sphere.m_Center;
		D3DXVECTOR3 Normal;
		D3DXVec3Normalize(&Normal, &distance);	// selfから見たotherへの方向ベクトル

		//  self側の最近接点が取得できる。今のプログラムだと求めるだけで使ってない
		D3DXVECTOR3 cp_self_sp = self_sp->m_sphere.m_Center + (Normal * self_sp->m_sphere.m_Radius);

		float length = D3DXVec3Length(&distance);		// 距離を長さにする

		float radius = self_sp->m_sphere.m_Radius + other_sp->m_capsule.m_Radius;	// それぞれの半径を足す
		if (length <= radius)
			hit = true;

		// 違反距離を求める
		float overlength = length - radius;
		D3DXVECTOR3 backVector = overlength * Normal * 0.7;

		if (hit)
		{

			OverlapToBackPosition(self_sp, other_sp, -backVector);		// この中でセットしている

			D3DXVECTOR3 self_sp_Vel = self_sp->m_ParentGameObject->GetVelocity();
			D3DXVECTOR3 other_sp_Vel = other_sp->m_ParentGameObject->GetVelocity();

			// スフィア側接地判定処理。flag == 1で上にいる。-1で下にいる。0で横
			if (self_sp_Vel.y < 0.0f && -Normal.y > self_sp->m_OnTheGroundCheckNormal)
			{
				self_sp->m_ParentGameObject->SetOnTheGround(true);
				self_sp->m_ParentGameObject->SetTempOnTheGround(true);
				self_sp->m_ParentGameObject->SetVelocity_y(0.0f);
			}

			// OBB側接地判定処理。
			if (other_sp_Vel.y < 0.0f && Normal.y > other_sp->m_OnTheGroundCheckNormal)
			{
				other_sp->m_ParentGameObject->SetOnTheGround(true);
				other_sp->m_ParentGameObject->SetTempOnTheGround(true);
				other_sp->m_ParentGameObject->SetVelocity_y(0.0f);
			}
		}

	}

	bool RaySegmentSphere(D3DXVECTOR3 p1, D3DXVECTOR3 p2, Collision* sphere_collision)
	{
		D3DXVECTOR3 cp_segment;
		float t;
		// 線分とスフィアの中心での最短座標を求める。cp_segmentには線分側の最短座標が求まる
		ClosetPtPointSegment(sphere_collision->m_sphere.m_Center, p1, p2, t, cp_segment);
		// スフィアから
		D3DXVECTOR3 distance = sphere_collision->m_sphere.m_Center - cp_segment;
		float length = D3DXVec3Length(&distance);
		if (sphere_collision->m_sphere.m_Radius >= length)
			return true;

		return false;
	}

	void RaySegmentSphereDivCalc(D3DXVECTOR3 p1, D3DXVECTOR3 p2, Collision* sphere_collision)
	{
		int div = 100;	// 分割数
		D3DXVECTOR3 distance = p2 - p1;	// 1から2への距離ベクトル
		D3DXVECTOR3 divVec = distance / (float)div;			// 分割したベクトル

		int MaxTrial = 50;
		int low = 0;
		int mid;
		int high = div;
		int oldmid = -1;
		for (int i = 0; i < MaxTrial; i++)
		{
			mid = (low + high) / 2;

			// 2回連続でmidが同じ値だったら
			if (oldmid == mid)
			{
				break;
			}

			p2 = p1 + (divVec * mid);

			if (RaySegmentSphere(p1, p2, sphere_collision))
			{
				// 衝突しているなら、分割数を更に戻す
				high = mid;
			}
			else
			{
				// 衝突していないなら、分割数を更に進める
				low = mid;
			}
			oldmid = mid;	// midの値の保存
		}
		// 
		distance = p2 - p1;
		float length = D3DXVec3Length(&distance);
		SetRayHit(length, sphere_collision);			// セットする
	}

	bool RaySegmentCapsule(D3DXVECTOR3 p1, D3DXVECTOR3 p2, Collision* capsule_collision)
	{
		D3DXVECTOR3 point1, point2;
		float t1, t2;
		SEGMENT seg1;
		seg1.m_pos1 = p1;
		seg1.m_pos2 = p2;
		SEGMENT seg2;
		seg2.m_pos1 = capsule_collision->m_capsule.GetUpperSpherePos();
		seg2.m_pos2 = capsule_collision->m_capsule.GetLowerSpherePos();
		float length = calcSegmentSegmentDist(seg1, seg2, point1, point2, t1, t2);

		if (capsule_collision->m_capsule.m_Radius >= length)
			return true;

		return false;
	}
	void RaySegmentCapsuleDivCalc(D3DXVECTOR3 p1, D3DXVECTOR3 p2, Collision* capsule_collision)
	{
		int div = 100;	// 分割数
		D3DXVECTOR3 distance = p2 - p1;	// 1から2への距離ベクトル
		D3DXVECTOR3 divVec = distance / (float)div;			// 分割したベクトル

		int MaxTrial = 50;
		int low = 0;
		int mid;
		int high = div;
		int oldmid = -1;
		for (int i = 0; i < MaxTrial; i++)
		{
			mid = (low + high) / 2;

			// 2回連続でmidが同じ値だったら
			if (oldmid == mid)
			{
				break;
			}

			p2 = p1 + (divVec * mid);

			if (RaySegmentCapsule(p1, p2, capsule_collision))
			{
				// 衝突しているなら、分割数を更に戻す
				high = mid;
			}
			else
			{
				// 衝突していないなら、分割数を更に進める
				low = mid;
			}
			oldmid = mid;	// midの値の保存
		}
		// 
		distance = p2 - p1;
		float length = D3DXVec3Length(&distance);
		SetRayHit(length, capsule_collision);			// セットする
	}

	bool RaySegmentOBB(D3DXVECTOR3 p1, D3DXVECTOR3 p2, Collision* obb_collision)
	{
		const float EPSILON = 1.175494e-37f;

		OBB* obb = &obb_collision->m_obb;

		D3DXVECTOR3 m = (p1 + p2) * 0.5f;
		D3DXVECTOR3 d = p2 - m;
		m = m - obb->m_Center;
		D3DXVECTOR3 rot[3];
		rot[0] = obb->GetRotNormalVector(0);
		rot[1] = obb->GetRotNormalVector(1);
		rot[2] = obb->GetRotNormalVector(2);
		float mDot0 = D3DXVec3Dot(&rot[0], &m);
		float mDot1 = D3DXVec3Dot(&rot[1], &m);
		float mDot2 = D3DXVec3Dot(&rot[2], &m);
		m = D3DXVECTOR3(mDot0, mDot1, mDot2);
		float dDot0 = D3DXVec3Dot(&rot[0], &d);
		float dDot1 = D3DXVec3Dot(&rot[1], &d);
		float dDot2 = D3DXVec3Dot(&rot[2], &d);
		d = D3DXVECTOR3(dDot0, dDot1, dDot2);

		float adx = fabsf(d.x);
		if (fabsf(m.x) > obb->m_Size.x + adx) return false;
		float ady = fabsf(d.y);
		if (fabsf(m.y) > obb->m_Size.y + ady) return false;
		float adz = fabsf(d.z);
		if (fabsf(m.z) > obb->m_Size.z + adz) return false;
		adx += EPSILON;
		ady += EPSILON;
		adz += EPSILON;

		if (fabsf(m.y * d.z - m.z * d.y) > obb->m_Size.y * adz + obb->m_Size.z * ady) return false;
		if (fabsf(m.z * d.x - m.x * d.z) > obb->m_Size.x * adz + obb->m_Size.z * adx) return false;
		if (fabsf(m.x * d.y - m.y * d.x) > obb->m_Size.x * ady + obb->m_Size.y * adx) return false;

		return true;
	}

	// 最後にRay構造体のセットもしている。上のと組み合わせて使う。これを使うってことは衝突はしてる
	void RaySegmentOBBDivCalc(D3DXVECTOR3 p1, D3DXVECTOR3 p2, Collision* obb_collision)
	{
		int div = 100;	// 分割数
		D3DXVECTOR3 distance = p2 - p1;	// 1から2への距離ベクトル
		D3DXVECTOR3 divVec = distance / (float)div;			// 分割したベクトル

		int MaxTrial = 50;
		int low = 0;
		int mid;
		int high = div;
		int oldmid = -1;
		for (int i = 0; i < MaxTrial; i++)
		{
			mid = (low + high) / 2;

			// 2回連続でmidが同じ値だったら
			if (oldmid == mid)
			{
				break;
			}

			p2 = p1 + (divVec * mid);

			if (RaySegmentOBB(p1, p2, obb_collision))
			{
				// 衝突しているなら、分割数を更に戻す
				high = mid;
			}
			else
			{
				// 衝突していないなら、分割数を更に進める
				low = mid;
			}
			oldmid = mid;	// midの値の保存
		}
		// 
		distance = p2 - p1;
		float length = D3DXVec3Length(&distance);
		SetRayHit(length, obb_collision);			// セットする
	}

	// ヒットした場合セット
	void SetRayHit(float length, Collision* HitCollision)
	{
		// 今の最短より短かったら更新する
		if (m_ray.m_length > length)
		{
			m_ray.m_length = length;
			m_ray.HitCollision = HitCollision;
		}
	}
	// ヒットしなかった場合にデフォ値をセット
	void SetRayNoHit()
	{
		m_ray.m_length = 999;
		m_ray.HitCollision = nullptr;
	}




	// OBB v.s. OBB
	//bool OBBOBB(OBB& obb1, OBB& obb2)
	bool OBBOBB(Collision* self_obb, Collision* other_obb)
	{
		OBB obb1 = self_obb->m_obb;
		OBB obb2 = other_obb->m_obb;

		// 各方向ベクトルの確保
		// （N***:標準化方向ベクトル）
		D3DXVECTOR3 NAe1 = obb1.GetRotNormalVector(0), Ae1 = NAe1 * obb1.GetRotNormalLength(0, self_obb->m_ColScale);
		D3DXVECTOR3 NAe2 = obb1.GetRotNormalVector(1), Ae2 = NAe2 * obb1.GetRotNormalLength(1, self_obb->m_ColScale);
		D3DXVECTOR3 NAe3 = obb1.GetRotNormalVector(2), Ae3 = NAe3 * obb1.GetRotNormalLength(2, self_obb->m_ColScale);
		D3DXVECTOR3 NBe1 = obb2.GetRotNormalVector(0), Be1 = NBe1 * obb2.GetRotNormalLength(0, other_obb->m_ColScale);
		D3DXVECTOR3 NBe2 = obb2.GetRotNormalVector(1), Be2 = NBe2 * obb2.GetRotNormalLength(1, other_obb->m_ColScale);
		D3DXVECTOR3 NBe3 = obb2.GetRotNormalVector(2), Be3 = NBe3 * obb2.GetRotNormalLength(2, other_obb->m_ColScale);
		D3DXVECTOR3 Interval = obb1.m_Center - obb2.m_Center;

		// 分離軸 : Ae1
		FLOAT rA = D3DXVec3Length(&Ae1);
		FLOAT rB = Collision::LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
		FLOAT L = fabs(D3DXVec3Dot(&Interval, &NAe1));
		if (L > rA + rB)
			return false; // 衝突していない

		float L1 = L;
		float rA1 = rA;
		float rB1 = rB;
		float Dist1 = L - (rA + rB);

		// 分離軸 : Ae2
		rA = D3DXVec3Length(&Ae2);
		rB = Collision::LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &NAe2));
		if (L > rA + rB)
			return false;

		float L2 = L;
		float rA2 = rA;
		float rB2 = rB;
		float Dist2 = L - (rA + rB);

		// 分離軸 : Ae3
		rA = D3DXVec3Length(&Ae3);
		rB = Collision::LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &NAe3));
		if (L > rA + rB)
			return false;

		float L3 = L;
		float rA3 = rA;
		float rB3 = rB;
		float Dist3 = L - (rA + rB);

		// 分離軸 : Be1
		rA = Collision::LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
		rB = D3DXVec3Length(&Be1);
		L = fabs(D3DXVec3Dot(&Interval, &NBe1));
		if (L > rA + rB)
			return false;

		float L4 = L;
		float rA4 = rA;
		float rB4 = rB;
		float Dist4 = L - (rA + rB);

		// 分離軸 : Be2
		rA = Collision::LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
		rB = D3DXVec3Length(&Be2);
		L = fabs(D3DXVec3Dot(&Interval, &NBe2));
		if (L > rA + rB)
			return false;

		float L5 = L;
		float rA5 = rA;
		float rB5 = rB;
		float Dist5 = L - (rA + rB);

		// 分離軸 : Be3
		rA = Collision::LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
		rB = D3DXVec3Length(&Be3);
		L = fabs(D3DXVec3Dot(&Interval, &NBe3));
		if (L > rA + rB)
			return false;

		float L6 = L;
		float rA6 = rA;
		float rB6 = rB;
		float Dist6 = L - (rA + rB);

		// 分離軸 : C11
		D3DXVECTOR3 Cross;
		D3DXVec3Cross(&Cross, &NAe1, &NBe1);
		rA = Collision::LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
		rB = Collision::LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &Cross));
		if (L > rA + rB)
			return false;

		float L7 = L;
		float rA7 = rA;
		float rB7 = rB;
		float Dist7 = L - (rA + rB);

		// 分離軸 : C12
		D3DXVec3Cross(&Cross, &NAe1, &NBe2);
		rA = Collision::LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
		rB = Collision::LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &Cross));
		if (L > rA + rB)
			return false;

		float L8 = L;
		float rA8 = rA;
		float rB8 = rB;
		float Dist8 = L - (rA + rB);

		// 分離軸 : C13
		D3DXVec3Cross(&Cross, &NAe1, &NBe3);
		rA = Collision::LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
		rB = Collision::LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
		L = fabs(D3DXVec3Dot(&Interval, &Cross));
		if (L > rA + rB)
			return false;

		float L9 = L;
		float rA9 = rA;
		float rB9 = rB;
		float Dist9 = L - (rA + rB);

		// 分離軸 : C21
		D3DXVec3Cross(&Cross, &NAe2, &NBe1);
		rA = Collision::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
		rB = Collision::LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &Cross));
		if (L > rA + rB)
			return false;

		float L10 = L;
		float rA10 = rA;
		float rB10 = rB;
		float Dist10 = L - (rA + rB);

		// 分離軸 : C22
		D3DXVec3Cross(&Cross, &NAe2, &NBe2);
		rA = Collision::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
		rB = Collision::LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &Cross));
		if (L > rA + rB)
			return false;

		float L11 = L;
		float rA11 = rA;
		float rB11 = rB;
		float Dist11 = L - (rA + rB);

		// 分離軸 : C23
		D3DXVec3Cross(&Cross, &NAe2, &NBe3);
		rA = Collision::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
		rB = Collision::LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
		L = fabs(D3DXVec3Dot(&Interval, &Cross));
		if (L > rA + rB)
			return false;

		float L12 = L;
		float rA12 = rA;
		float rB12 = rB;
		float Dist12 = L - (rA + rB);

		// 分離軸 : C31
		D3DXVec3Cross(&Cross, &NAe3, &NBe1);
		rA = Collision::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
		rB = Collision::LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &Cross));
		if (L > rA + rB)
			return false;

		float L13 = L;
		float rA13 = rA;
		float rB13 = rB;
		float Dist13 = L - (rA + rB);

		// 分離軸 : C32
		D3DXVec3Cross(&Cross, &NAe3, &NBe2);
		rA = Collision::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
		rB = Collision::LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &Cross));
		if (L > rA + rB)
			return false;

		float L14 = L;
		float rA14 = rA;
		float rB14 = rB;
		float Dist14 = L - (rA + rB);

		// 分離軸 : C33
		D3DXVec3Cross(&Cross, &NAe3, &NBe3);
		rA = Collision::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
		rB = Collision::LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
		L = fabs(D3DXVec3Dot(&Interval, &Cross));
		if (L > rA + rB)
			return false;

		float L15 = L;
		float rA15 = rA;
		float rB15 = rB;
		float Dist15 = L - (rA + rB);

		// 分離平面が存在しないので「衝突している」
		return true;
	};


	// 分離軸に投影された軸成分から投影線分長を算出
	FLOAT LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0)
	{
		// 3つの内積の絶対値の和で投影線分長を計算
		// 分離軸Sepは標準化されていること
		FLOAT r1 = fabs(D3DXVec3Dot(Sep, e1));
		FLOAT r2 = fabs(D3DXVec3Dot(Sep, e2));
		FLOAT r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
		return r1 + r2 + r3;
	};







};






//// OBB v.s. OBB
//bool OBBOBB(OBB& obb1, OBB& obb2)
//{
//	// 各方向ベクトルの確保
//	// （N***:標準化方向ベクトル）
//	D3DXVECTOR3 NAe1 = obb1.GetRotNormalVector(0), Ae1 = NAe1 * obb1.GetRotNormalLength(0);
//	D3DXVECTOR3 NAe2 = obb1.GetRotNormalVector(1), Ae2 = NAe2 * obb1.GetRotNormalLength(1);
//	D3DXVECTOR3 NAe3 = obb1.GetRotNormalVector(2), Ae3 = NAe3 * obb1.GetRotNormalLength(2);
//	D3DXVECTOR3 NBe1 = obb2.GetRotNormalVector(0), Be1 = NBe1 * obb2.GetRotNormalLength(0);
//	D3DXVECTOR3 NBe2 = obb2.GetRotNormalVector(1), Be2 = NBe2 * obb2.GetRotNormalLength(1);
//	D3DXVECTOR3 NBe3 = obb2.GetRotNormalVector(2), Be3 = NBe3 * obb2.GetRotNormalLength(2);
//	D3DXVECTOR3 Interval = obb1.m_Center - obb2.m_Center;
//
//	// 分離軸 : Ae1
//	FLOAT rA = D3DXVec3Length(&Ae1);
//	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
//	FLOAT L = fabs(D3DXVec3Dot(&Interval, &NAe1));
//	if (L > rA + rB)
//		return false; // 衝突していない
//
//	 // 分離軸 : Ae2
//	rA = D3DXVec3Length(&Ae2);
//	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NAe2));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : Ae3
//	rA = D3DXVec3Length(&Ae3);
//	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NAe3));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : Be1
//	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be1);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe1));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : Be2
//	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe2));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : Be3
//	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe3));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C11
//	D3DXVECTOR3 Cross;
//	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C12
//	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C13
//	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C21
//	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C22
//	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C23
//	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C31
//	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C32
//	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C33
//	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離平面が存在しないので「衝突している」
//	return true;
//}
//
//
//// 分離軸に投影された軸成分から投影線分長を算出
//FLOAT LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0)
//{
//	// 3つの内積の絶対値の和で投影線分長を計算
//	// 分離軸Sepは標準化されていること
//	FLOAT r1 = fabs(D3DXVec3Dot(Sep, e1));
//	FLOAT r2 = fabs(D3DXVec3Dot(Sep, e2));
//	FLOAT r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
//	return r1 + r2 + r3;
//}




//_RPTN(_CRT_WARN, "%f, %f, %f\n", cp_OBB.x, cp_OBB.y, cp_OBB.z);
//_RPTN(_CRT_WARN, "%f, %f, %f\n", cp_Capsule.x, cp_Capsule.y, cp_Capsule.z);
//_RPTN(_CRT_WARN, "%f, %f, %f\n", distance.x, distance.y, distance.z);
//_RPTN(_CRT_WARN, "%f\n", length);
////_RPTN(_CRT_WARN, "%f, %f, %f\n", normal.x, normal.y, normal.z);
//_RPTN(_CRT_WARN, "%d\n", flag);