
#include "stdafx.h"

//#include "Collision.h"
//#include "main.h"
//#include "scene.h"
//#include "manager.h"
//#include "gameObject.h"
//#include <d3d11.h>
//
//#include "CComponent.h"
//#include "primitive.h"
//
//#include "model.h"
//#include "ModelComponent.h"
//#include "renderer.h"

void Collision::Update()
{
	// シーンをゲットしておく
	Scene* scene = Manager::GetScene();

	// 接地判定を毎回falseにリセットしておく。でも他のコリジョンでtrueになったものをリセットしないようにtempで確認
	if (!m_ParentGameObject->GetTempOnTheGround())
		m_ParentGameObject->SetOnTheGround(false);			// リセット

	// ワールドマトリクスの作成
	D3DXMATRIX worldmatrix = GetWorldMatrixCollision();

	
	if (m_Collision_type == RAY_COLLISION)
	{
		SetRayNoHit();	// 毎回初めにリセットしてあげる
	}





	// 計算用に使う変数の準備
	//D3DXVECTOR3 t_Position;
	D3DXVECTOR3 t_Velocity;
	//t_Position = m_ParentGameObject->GetPosition() + m_ColOffset;		// ゲット
	t_Velocity = m_ParentGameObject->GetVelocity();		// ゲット

														// WorldMatrixを取得しておく(Scale,Rotation,Posのデータ)
														//D3DXMATRIX selfMatrix = m_ParentGameObject->GetWorldMatrix();
														//D3DXMATRIX selfMatrix = CreateWorldMatrix(m_ColScale, m_ColRotation, t_Position);
	// Scaleが1.0でワールドマトリクスを作る。
	D3DXMATRIX selfMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_ColRotation, m_ColPosition);

	// OBBの更新
	m_obb.CreateOBB(m_ColScale, selfMatrix);

	// カプセルの更新
	m_capsule.SetCapsule(m_ColScale.x, m_ColScale.y, m_ColPosition, m_ColRotation);

	// スフィアの更新。半径はどれも同じなのでてきとーにxを使っている
	m_sphere.SetSpehre(m_ColPosition, m_ColScale.x);

	// ここまでは当たり判定なくても必須。





	// 全てに対してIgnoreだった場合終了
	for (int response : m_ResponseObjectList)
	{
		// Ignore以外があったらその時点で先に進む
		if (response != RESPONSE_IGNORE)
			goto not_allIgnore;
	}
	return;
not_allIgnore:

	// 全てのゲームオブジェクトを取得
	std::vector<GameObject*> allObj = scene->GetAllGameObjects(1);
	for (GameObject* other : allObj)
	{
		// otherに衝突相手の親オブジェクト(ゲームオブジェクト)のポインターが入っている

		// 自分だったらコンテニュー。同じオブジェクトが複数持っててもここで無視される
		if (other == m_ParentGameObject)
			continue;

		// 全てのオブジェクトの中からCollisionコンポーネントを持っているものだけを取得
		if (other->HasComponent<Collision>())
		{
			Collision* other_collision = other->GetComponent<Collision>();

			// response 今回の相手との反応が入る
			int response = CheckResponseOther(other_collision);

			// 無視だったら終了
			if (response == RESPONSE_IGNORE)
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
				CapsuleOBB(this, other_collision, response);
			}
			// OBB(自分)とカプセル(相手)の衝突だったら
			else if (m_Collision_type == BOX_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
			{
				CapsuleOBB(other_collision, this, response);
			}
			// カプセル(自分)とカプセル(相手)の衝突だったら
			else if (m_Collision_type == CAPSULE_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
			{
				CapsuleCapsule(this, other_collision, response);
			}
			// スフィア(自分)とカプセル(相手)の衝突だったら
			else if (m_Collision_type == SPHERE_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
			{
				SphereCapsule(this, other_collision, response);
			}
			// カプセル(自分)とスフィア(相手)の衝突だったら
			else if (m_Collision_type == CAPSULE_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
			{
				SphereCapsule(other_collision, this, response);
			}
			// スフィア(自分)とOBB(相手)の衝突だったら
			else if (m_Collision_type == SPHERE_COLLISION && other_collision->GetCollisionType() == BOX_COLLISION)
			{
				SphereOBB(this, other_collision, response);
			}
			// OBB(自分)とスフィア(相手)の衝突だったら
			else if (m_Collision_type == BOX_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
			{
				SphereOBB(other_collision, this, response);
			}
			// スフィア(自分)とスフィア(相手)の衝突だったら
			else if (m_Collision_type == SPHERE_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
			{
				SphereSphere(this, other_collision, response);
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
				if (OBBOBB(this, other_collision, response))
				{
					// 衝突相手のOBBの更新いらないかも
					/*D3DXMATRIX other_Matrix = CreateWorldMatrix(other_collision->m_ColScale, other_collision->m_ColRotation, other_collision->m_obb.m_Center);
					other_obb->CreateOBB(other_collision->m_ColScale, other_Matrix);*/


					// 相手が動かないなら自分だけ動かす処理
					if (other_collision->m_BeMoved == false)
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

							if (OBBOBB(this, other_collision, response))
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






	

	// とりあえず地面は-100.0fとしておく
	float groundHeight = scene->GetGroundMinHeight();


	//t_Position = m_ParentGameObject->GetPosition();		// ゲット
	t_Velocity = m_ParentGameObject->GetVelocity();		// ゲット
														// 地面との当たり判定。接地。コリジョンの中心点しか見てないからダメだけど取り急ぎ
	if (m_ColPosition.y < groundHeight && t_Velocity.y < 0.0f)
	{
		D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, groundHeight - m_ColPosition.y, 0.0f);
		m_ParentGameObject->AddPosition(upvector);
		//m_ParentGameObject->SetPosition_y(groundHeight);
		m_ParentGameObject->SetOnTheGround(true);
		m_ParentGameObject->SetVelocity_y(0.0f);
	}

	// メッシュフィールドとの衝突判定を持っているものであれば
	// メッシュフィールドでの接地判定
	{
		// m_ResponseObjectList の　4　がメッシュフィールドで固定としておく。
		if (m_ResponseObjectList[4] == RESPONSE_BLOCK)
		{
			// まず、このコリジョンの半高を求めておく
			float halfheight{};
			if (m_Collision_type == CAPSULE_COLLISION)
				halfheight = GetCapsule().GetHeightRadius();
			else if (m_Collision_type == SPHERE_COLLISION)
				halfheight = GetSphere().m_Radius;
			else if (m_Collision_type == BOX_COLLISION)
				halfheight = GetOBB().GetRotNormalLength(1, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			Int2 block;
			//BackAndSetPositionForMeshFieldObject(&block, halfheight);
			CalcCollisionMeshFieldObject(halfheight);

			//// メッシュフィールド高さ取得。
			//float meshHeight;
			//D3DXVECTOR3 meshNormal;
			//Int2 block;
			//scene->GetMeshFieldObjectHeightandNormal(&block, &meshHeight, &meshNormal, m_ColPosition);
			//if (m_ColPosition.y - halfheight < meshHeight)
			//{
			//	// 地面よりも下にいる。埋まっている。
			//	if (meshNormal.y < 0.6)
			//	{
			//		// 崖とか坂にいるから法線の方向に押し戻してあげる
					//D3DXVECTOR3 moveNormal = meshNormal;
					//moveNormal.y = 0.0f;
					//D3DXVec3Normalize(&moveNormal, &moveNormal);

			//		D3DXVECTOR3 movevector = moveNormal * PanelSize;

			//		m_ParentGameObject->AddPosition(movevector);
			//		m_ParentGameObject->SetOnTheGround(false);
			//		m_ParentGameObject->SetVelocity_y(0.0f);
			//	}
			//	else
			//	{
			//		D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
			//		m_ParentGameObject->AddPosition(upvector);
			//		m_ParentGameObject->SetOnTheGround(true);
			//		m_ParentGameObject->SetVelocity_y(0.0f);
			//	}
			//}
			//else
			//{
			//	// 地面よりも上にいる。浮いている。
			//	// もしメッシュフィールドを考えない場合で地面についているなら、何もしなくていい。
			//	//if (GetOnTheGround())
			//	//{
			//	//	//SetOnTheGround(false);
			//	//	//SetTempOnTheGround(false);
			//	//}

			//}


			//// メッシュフィールド高さ取得。
			//float meshHeight = scene->GetMeshFieldObjectHeight(m_ColPosition);
			//if (m_ColPosition.y - halfheight < meshHeight)
			//{
			//	// 地面よりも下にいる。埋まっている。
			//	D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
			//	m_ParentGameObject->AddPosition(upvector);
			//	m_ParentGameObject->SetOnTheGround(true);
			//	m_ParentGameObject->SetVelocity_y(0.0f);
			//}
			//else
			//{
			//	// 地面よりも上にいる。浮いている。
			//	// もしメッシュフィールドを考えない場合で地面についているなら、何もしなくていい。
			//	//if (GetOnTheGround())
			//	//{
			//	//	//SetOnTheGround(false);
			//	//	//SetTempOnTheGround(false);
			//	//}

			//}
		}
	}

}

//D3DXMATRIX Collision::GetWorldMatrixCollision()
//{
//	D3DXVECTOR3 Scale = m_ParentGameObject->GetScaleRate();
//	Scale.x *= m_ColInitScale.x;
//	Scale.y *= m_ColInitScale.y;
//	Scale.z *= m_ColInitScale.z;
//	D3DXVECTOR3 Rotation = m_ParentGameObject->GetRotation();
//	Rotation.x;
//	Rotation.y;
//	Rotation.z;
//	Rotation += m_ColInitRotation;
//	Rotation.x;
//	Rotation.y;
//	Rotation.z;
//	D3DXVECTOR3 Position = m_ParentGameObject->GetPosition();
//	Position += m_ColInitOffset;
//	//D3DXVECTOR3 Position = m_ColInitOffset;
//
//	D3DXMATRIX world, scale, rot, trans;
//	D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
//	D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
//	D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
//	world = scale * rot * trans;		// 通常はこう
//	//world = scale * trans * rot;		// 親を回転の中心とするとこう
//
//
//	D3DXVECTOR3 ParentPos = m_ParentGameObject->GetPosition();
//	D3DXVECTOR3 ParentScaleRate = m_ParentGameObject->GetOldScaleRate();
//
//	m_ColScale = Scale;
//	m_ColRotation = Rotation;
//
//	// 親の回転分座標を移動させる。超大事。
//	D3DXVECTOR3 offset = CalcAfterRoatedPos(m_ColInitOffset, m_ParentGameObject->GetRotation());
//	offset.x *= ParentScaleRate.x;
//	offset.y *= ParentScaleRate.y;
//	offset.z *= ParentScaleRate.z;
//
//	// 最後に親のオブジェクトに、計算で求めたオフセットを足してあげる
//	m_ColPosition = m_ParentGameObject->GetPosition() + offset;
//
//	return world;
//}


D3DXMATRIX Collision::GetWorldMatrixCollision()
{
	// 子(自分)の情報を取得
	D3DXVECTOR3 CScale = m_ColInitScale;
	D3DXVECTOR3 CRotation = m_ColInitRotation;
	D3DXVECTOR3 CPosition = m_ColInitOffset;

	// 子(自分)の行列を作成
	D3DXMATRIX CworldM, CscaleM, CrotM, CtransM;
	D3DXMatrixScaling(&CscaleM, CScale.x, CScale.y, CScale.z);
	D3DXMatrixRotationYawPitchRoll(&CrotM, CRotation.y, CRotation.x, CRotation.z);
	D3DXMatrixTranslation(&CtransM, CPosition.x, CPosition.y, CPosition.z);
	CworldM = CscaleM * CrotM * CtransM;


	// 親の情報を取得
	D3DXVECTOR3 PScale = m_ParentGameObject->GetScaleRate();
	D3DXVECTOR3 PRotation = m_ParentGameObject->GetRotation();
	D3DXVECTOR3 PPosition = m_ParentGameObject->GetPosition();

	// 親の行列を作成
	D3DXMATRIX PworldM, PscaleM, ProtM, PtransM;
	D3DXMatrixScaling(&PscaleM, PScale.x, PScale.y, PScale.z);
	D3DXMatrixRotationYawPitchRoll(&ProtM, PRotation.y, PRotation.x, PRotation.z);
	D3DXMatrixTranslation(&PtransM, PPosition.x, PPosition.y, PPosition.z);
	PworldM = PscaleM * ProtM * PtransM;

	// 行列の掛け算。ワールド以外いらないが、値が欲しい場合必要。
	D3DXMATRIX RworldM, RscaleM, RrotM, RtransM;
	D3DXMatrixMultiply(&RworldM, &CworldM, &PworldM);	// ワールド

	D3DXMatrixMultiply(&RscaleM, &CscaleM, &PscaleM);	// スケール
	D3DXMatrixMultiply(&RrotM, &CrotM, &ProtM);			// 回転
	D3DXMatrixMultiply(&RtransM, &CtransM, &PtransM);	// 座標

	// 行列計算後の
	// スケールを取得。RscaleM
	D3DXVECTOR3 AfterScale = D3DXVECTOR3(RscaleM._11, RscaleM._22, RscaleM._33);

	// 回転を取得。RrotMから取得できるのは最終的な回転のやつなので、VecのRotにはできない。
	// ので普通に足し算する
	D3DXVECTOR3 AfterRotation = CRotation + PRotation;

	// 座標を取得。RworldM。worldからでもtransからでもどっちでもOK
	D3DXVECTOR3 AfterPosition = D3DXVECTOR3(RworldM._41, RworldM._42, RworldM._43);

	// Afterを使ったコリジョンの変数の更新等
	m_ColScale = AfterScale;
	m_ColRotation = AfterRotation;
	m_ColPosition = AfterPosition;

	return RworldM;
}

// 衝突する寸前に戻し、最初に当たったブロック番号を求める。そもそも当たってなかったらfalseを返す
bool Collision::BackAndSetPositionForMeshFieldObject(Int2* pOut_block, const float& halfheight)
{
	Scene* scene = Manager::GetScene();

	// 今回のフレームでこれだけ動いたよ
	D3DXVECTOR3 moved = m_ParentGameObject->GetOldPosition() - m_ParentGameObject->GetPosition();
	// コリジョンの最初の場所を求めておく
	D3DXVECTOR3 start = m_ColPosition - moved;

	// 分割数を求める。パネルサイズの大きさで割れば最低限OK
	float lengthsq = D3DXVec3LengthSq(&moved);
	float minlength = 0.2f;
	float minlengthsq = pow(minlength, 2);
	if (lengthsq <= minlengthsq)
	{
		// メッシュフィールド高さ取得。
		float meshHeight;
		D3DXVECTOR3 meshNormal;
		Int2 block;
		scene->GetMeshFieldObjectHeightandNormal(&block, &meshHeight, &meshNormal, m_ColPosition);
		if (m_ColPosition.y - halfheight < meshHeight)
		{
			// 地面よりも下にいる。埋まっている。
			if (meshNormal.y < 0.6)
			{
				// 崖とか坂にいるから法線の方向に押し戻してあげる
				D3DXVECTOR3 moveNormal = meshNormal;
				moveNormal.y = 0.0f;
				D3DXVec3Normalize(&moveNormal, &moveNormal);

				D3DXVECTOR3 movevector = moveNormal * minlength;

				m_ParentGameObject->AddPosition(movevector);
				m_ParentGameObject->SetOnTheGround(false);
				//m_ParentGameObject->SetVelocity_y(0.0f);
							// 床すり抜け防止程度に速度を下げる
				if (m_ParentGameObject->GetVelocity().y < -0.15f)
					m_ParentGameObject->SetVelocity_y(-0.15f);
			}
			else
			{
				D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
				m_ParentGameObject->AddPosition(upvector);
				m_ParentGameObject->SetOnTheGround(true);
				m_ParentGameObject->SetVelocity_y(0.0f);
			}
		}
		else
		{
			// 地面よりも上にいる。浮いている。
			// もしメッシュフィールドを考えない場合で地面についているなら、何もしなくていい。
			//if (GetOnTheGround())
			//{
			//	//SetOnTheGround(false);
			//	//SetTempOnTheGround(false);
			//}

		}
		return false;
	}
	int div = (pow(PanelSize, 2) / lengthsq) + 1;
	//if (div > 50)
	//	div = 50;

	// 1回で動くベクトルを求める
	D3DXVECTOR3 onevec = moved / div;

	bool fin = false;
	int min = 0;
	int max = div;
	int mid = (max + min) / 2;
	Int2 block;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 nowpos;
	float meshHeight;

	while (!fin)
	{
		// 今回使う座標とか
		nowpos = start + (mid * onevec);

		// ここから衝突判定を取る
		meshHeight = scene->GetMeshFieldObjectHeight(&block, nowpos);
		if (nowpos.y - halfheight < meshHeight)
		{
			// 衝突している

			min = mid;
		}
		else
		{
			// 衝突していない
			max = mid;
		}

		mid = (max + min) / 2;

		// 終了処理
		if (min + 1 >= max)
		{
			//if (min == 0)

			nowpos = start + (min * onevec);
			scene->GetMeshFieldObjectHeightandNormal(&block, &meshHeight, &normal, nowpos);
			if (nowpos.y - halfheight < meshHeight)
			{
				// 衝突している
				mid = min - 1;
				nowpos = start + (mid * onevec);
				scene->GetMeshFieldObjectHeightandNormal(&block, &meshHeight, &normal, nowpos);
				fin = true;
			}
			else
			{
				// 衝突していない
				mid = min;
				fin = true;
			}
		}
	}
	// mid,nowposがぶつかる寸前の場所,blockにこれからぶつかる
	// 戻す処理↓

	// どれだけ押してあげるかの割合を出す
	int nokori = div - mid;
	float wariai;
	if (nokori == 0)
	{
		D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
		m_ParentGameObject->AddPosition(upvector);
		m_ParentGameObject->SetOnTheGround(true);
		m_ParentGameObject->SetVelocity_y(0.0f);
		return false;
	}
	else
		wariai = (float)nokori / (float)div;

	D3DXVECTOR3 t_normal = normal;
	t_normal.y = 0.0f;
	D3DXVec3Normalize(&t_normal, &normal);

	float backlength = D3DXVec3Length(&moved);
	D3DXVECTOR3 backvec = t_normal * backlength;

	//nowpos += backvec;



	// 地面よりも下にいる。埋まっている。
	if (m_ColPosition.y - halfheight < meshHeight)
	{
		if (normal.y < 0.7)
		{
			// 崖とか坂にいるから法線の方向に押し戻してあげる
			//m_ParentGameObject->AddPosition(backvec);
			m_ParentGameObject->SetPosition(nowpos);
			m_ParentGameObject->SetOnTheGround(false);
			//// 床すり抜け防止程度に速度を下げる
			if(m_ParentGameObject->GetVelocity().y < -0.15f)
				m_ParentGameObject->SetVelocity_y(-0.15f);
			return true;
		}
		else
		{
			D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
			m_ParentGameObject->AddPosition(upvector);
			m_ParentGameObject->SetOnTheGround(true);
			m_ParentGameObject->SetVelocity_y(0.0f);
			return false;
		}
	}
	else
		return false;
}



// 今回のフレームでの移動で、衝突したメッシュフィールドの
void Collision::CalcCollisionMeshFieldObject(const float& halfheight)
{
	Scene* scene = Manager::GetScene();

	// 今回のフレームでこれだけ動いたよ
	D3DXVECTOR3 moved = m_ParentGameObject->GetOldPosition() - m_ParentGameObject->GetPosition();
	D3DXVECTOR3 movedxz = moved;
	movedxz.y = 0.0f;
	D3DXVECTOR3 movedxzNormal;
	D3DXVec3Normalize(&movedxzNormal, &movedxz);
	// コリジョンの最初の場所を求めておく
	D3DXVECTOR3 start = m_ColPosition - moved;

	float meshHeight;
	D3DXVECTOR3 meshNormal;
	D3DXVECTOR3 befor;
	Int2 block;
	int8_t col = 0;

	// 分割数を求める。パネルサイズの大きさで割れば最低限OK
	float lengthsq = D3DXVec3LengthSq(&movedxz);
	float minlength = PanelSize;
	float minlengthsq = pow(minlength, 2);

	int div = lengthsq / minlengthsq;
	D3DXVECTOR3 onemove = movedxzNormal * PanelSize;
	D3DXVECTOR3 nowpos = start;
	for (int i = 0; i < div; i++)
	{
		nowpos += onemove;
		// 当たり判定をとる
		// ぶつかっていたらそこで終了
		D3DXVECTOR3 nowposy = nowpos;
		nowposy.y -= halfheight;
		col = scene->CheckCollisionMeshFieldObjects(&block, &meshHeight, &meshNormal, &befor, nowposy);

		if (col != 0)
			break;
	}
	// for分が終わった後に、終了座標で当たり判定を取る
	if (col == 0)
	{
		nowpos = m_ColPosition;
		D3DXVECTOR3 nowposy = nowpos;
		nowposy.y -= halfheight;
		col = scene->CheckCollisionMeshFieldObjects(&block, &meshHeight, &meshNormal, &befor, nowposy);
	}


	// 衝突していたら
	if (col == 1)
	{
		D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - nowpos.y + halfheight, 0.0f);
		m_ParentGameObject->AddPosition(upvector);
		m_ParentGameObject->SetOnTheGround(true);
		m_ParentGameObject->SetVelocity_y(0.0f);
		return;
	}
	else if (col == 2)
	{
		D3DXVECTOR3 newpos = befor - onemove;
		if (div == 0)
			newpos = m_ParentGameObject->GetOldPosition();
		//newpos.y = meshHeight + halfheight;

		// 残りの長さ分、壁に沿ったベクトルで移動させてあげる
		D3DXVECTOR3 flontvec = moved;
		D3DXVECTOR3 aftervec;
		CalcWallScratchVector(&aftervec, flontvec, meshNormal);
		D3DXVECTOR3 aftervecN = aftervec;
		aftervecN.y = 0.0f;
		//D3DXVec3Normalize(&aftervecN, &aftervec);

		D3DXVECTOR3 trash;
		col = scene->CheckCollisionMeshFieldObjects(&block, &meshHeight, &meshNormal, &trash, befor);

		m_ParentGameObject->SetPosition(newpos);
		m_ParentGameObject->SetOnTheGround(false);

		D3DXVECTOR3 vel = m_ParentGameObject->GetVelocity();
		float veclength = D3DXVec3Length(&vel);	// 現在のベクトルの長さを保存
		D3DXVECTOR3 velN;
		D3DXVec3Normalize(&velN, &vel);			// ベクトルの正規化
		D3DXVECTOR3 addvelN = velN + meshNormal;	// 正規化した同士で足し合わせる



		D3DXVECTOR3 setvel = addvelN * veclength;	// 足し合わせたものを長さ分かけたものを新しいベクトルとする
		m_ParentGameObject->SetVelocity(setvel);
		m_ParentGameObject->AddPosition(-aftervecN * 0.02f);	// 0.02fはとりあえずで戻している。

		return;
	}



}