#pragma once

#ifndef __IKD_OX_PRIMITIVE_H__
#define __IKD_OX_PRIMITIVE_H__


// プリミティブ定義

// v1.00
//  初出

// v1.01
// ベクトルの平行判定を外積に置き換え


#include <math.h>
#include <d3dx9math.h>

#define _OX_EPSILON_	0.000001f	// 誤差


D3DXMATRIX CreateWorldMatrix(D3DXVECTOR3 Scale, D3DXVECTOR3 Rotation, D3DXVECTOR3 Position);


struct SEGMENT;
struct SPHERE;
struct CAPSULE;
struct OBB;
struct RAY;
class Collision;

struct Int2;
struct Float2;

// int型をx,yで2つもつ
struct Int2
{
	int x;
	int	y;

	Int2() : x(0), y(0) {}
	Int2(int ax, int ay){ x = ax; y = ay; }

	// 比較の追加
	bool operator==(const Int2& int2) const
	{
		return (x == int2.x && y == int2.y);
	}


};

struct Float2
{
	float x;
	float y;

	Float2() : x(0.0f), y(0.0f) {}
	Float2(float ax, float ay) { x = ax; y = ay; }

	// 比較の追加
	bool operator==(const Float2& float2) const
	{
		return (x == float2.x && y == float2.y);
	}
};


struct SEGMENT
{
	D3DXVECTOR3 m_pos1;		// 線分の始点
	D3DXVECTOR3	m_pos2;		// 線分の終点

	SEGMENT() : m_pos1(0, 0, 0), m_pos2(0, 0, 0) {}

	// pos1からpos2へのベクトルを返す
	D3DXVECTOR3 GetVector()
	{
		return m_pos2 - m_pos1;
	}

	// pos1からpos2への方向ベクトルを返す
	D3DXVECTOR3 GetNormalVector()
	{
		D3DXVECTOR3 vec = m_pos2 - m_pos1;
		D3DXVECTOR3 retvec;
		D3DXVec3Normalize(&retvec, &vec);
		return retvec;
	}

};

struct RAY
{
	SEGMENT		m_Segment;	
	float		m_length;				// スタート位置から一番近い衝突点への長さ
	D3DXVECTOR3 m_normal;				// 衝突してるコリジョンからの法線
	D3DXVECTOR3 m_CollisionPoint;		// 衝突してる座標
	Collision*	HitCollision;			// 衝突したうちの一番近いもの

	RAY() : m_length(999), m_normal(0.0f, 0.0f, 0.0f), m_CollisionPoint(0.0f, 0.0f, 0.0f), HitCollision(nullptr){ }

	// 長さと法線から衝突点を計算する。
	void CalcCollisionPointwithLengthNormal()
	{
		D3DXVECTOR3 normal = m_Segment.GetNormalVector();
		D3DXVECTOR3 CollisionPoint = m_Segment.m_pos1 + (normal * m_length);
		m_CollisionPoint = CollisionPoint;
	}


};

struct SPHERE 
{
	D3DXVECTOR3 m_Center;	// 中心点の座標
	float		m_Radius;	// 半径

	SPHERE() : m_Center(0, 0, 0), m_Radius(0.5f) {}

	void SetSpehre(D3DXVECTOR3 center, float radius)
	{
		m_Center = center;
		m_Radius = radius;
	}

};


struct CAPSULE
{
	float m_Radius;				// 半径
	float m_HalfHeight;				// 半径
	D3DXVECTOR3 m_Center;		// 中心点の座標
	D3DXVECTOR3 m_Rotation;		// カプセルの回転
	//D3DXVECTOR3 m_PointUpper;	// カプセルの上の球体部分の球の中心座標
	//D3DXVECTOR3 m_PointLower;	// カプセルの下の球体部分の球の中心座標

	// コンストラクタ
	CAPSULE() : m_Radius(0.5f), m_HalfHeight(0.5f), m_Center(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f) {}
	//CAPSULE() : m_Radius(0.5f), m_PointUpper(0, -0.5f, 0), m_PointLower(0, 0.5f, 0) {}

	// カプセルをセット
	void SetCapsule(float radius, float halfheight, D3DXVECTOR3 center, D3DXVECTOR3 rotation)
	{
		m_Radius = radius;
		m_HalfHeight = halfheight;
		m_Center = center;
		m_Rotation = rotation;
	}

	// カプセルの中心座標を取得
	D3DXVECTOR3 GetCenterPos() const
	{
		return m_Center;
		//return m_PointUpper + ((m_PointLower - m_PointUpper) * 0.5f);
	}

	// カプセルの中心座標から上下2つの球の座標をセット
	void SetCenter(const D3DXVECTOR3& Center) {
		//D3DXVECTOR3 CenterToPointA = ((m_PointUpper - m_PointLower) * 0.5f);
		//D3DXVECTOR3 CenterToPointB = ((m_PointLower - m_PointUpper) * 0.5f);
		//m_PointUpper = Center + CenterToPointA;
		//m_PointLower = Center + CenterToPointB;
	}
	// 縦の半径を取得する
	float GetHeightRadius()const 
	{
		return m_HalfHeight;
		//D3DXVECTOR3 temp_calclength = m_PointLower - m_PointUpper;
		//float PointLen = D3DXVec3Length(&temp_calclength) * 0.5f;
		//PointLen += m_Radius;
		//return PointLen;
	}

	// 上の球の座標を取得する
	D3DXVECTOR3 GetUpperSpherePos()const
	{

		// 回転を計算するために BeforePos には原点を中心として高さを与えておく
		D3DXVECTOR3 BeforePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		BeforePos.y = m_HalfHeight - m_Radius;

		//	今の回転から回転行列を作成する
		D3DXMATRIX rotmat;
		D3DXMatrixRotationYawPitchRoll(&rotmat, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		// 頂点座標を回転行列で回転させて座標を取得する
		D3DXVECTOR3 AfterPos;
		D3DXVec3TransformCoord(&AfterPos, &BeforePos, &rotmat);	// BeforePosをrotmatで回転させたものがAfterPosに戻り値として入る。
		
		// 中心点座標に原点を中心とした回転を足す
		D3DXVECTOR3 Upeer;
		Upeer.x = m_Center.x + AfterPos.x;
		Upeer.y = m_Center.y + AfterPos.y;
		Upeer.z = m_Center.z + AfterPos.z;
		return Upeer;
	}

	// 下の球の座標を取得する
	D3DXVECTOR3 GetLowerSpherePos()const
	{
		// 回転を計算するために BeforePos には原点を中心として高さを与えておく
		D3DXVECTOR3 BeforePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		BeforePos.y = -m_HalfHeight + m_Radius;

		//	今の回転から回転行列を作成する
		D3DXMATRIX rotmat;
		D3DXMatrixRotationYawPitchRoll(&rotmat, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		// 頂点座標を回転行列で回転させて座標を取得する
		D3DXVECTOR3 AfterPos;
		D3DXVec3TransformCoord(&AfterPos, &BeforePos, &rotmat);	// BeforePosをrotmatで回転させたものがAfterPosに戻り値として入る。

		// 中心点座標に原点を中心とした回転を足す
		D3DXVECTOR3 Upeer;
		Upeer.x = m_Center.x + AfterPos.x;
		Upeer.y = m_Center.y + AfterPos.y;
		Upeer.z = m_Center.z + AfterPos.z;
		return Upeer;
	}
};


struct OBB
{
	D3DXVECTOR3 m_Center;   // 中心点の座標
	D3DXVECTOR3 m_RotNormal[3];	// XYZ の各座標軸の傾きを表す方向ベクトル
	D3DXVECTOR3 m_Size;     // OBB の各座標軸に沿った長さの半分（中心点から面までの長さ）

	D3DXPLANE	m_Plane[6];
	D3DXVECTOR3 m_Normal[6];			// 面から垂直に出てる方向ベクトル。
	D3DXVECTOR3 m_VertexPos[8];
	SEGMENT		m_Segment[12];

	// コンストラクタ
	OBB() 
	{
		//デフォルトは1辺1.0のOBB
		D3DXVECTOR3 SizeVec(1.0f, 1.0f, 1.0f);
		//行列はアイデンティティ
		D3DXMATRIX Matrix;
		CreateOBB(SizeVec, Matrix);
	}

	// OBBの全てを設定してくれる。Sizeは半径。ColScaleも半径なのでそのままでOK
	void CreateOBB(const D3DXVECTOR3& Size, const D3DXMATRIX& Matrix) 
	{
		m_Center.x = Matrix._41;
		m_Center.y = Matrix._42;
		m_Center.z = Matrix._43;
		D3DXVECTOR3 VecX(Matrix._11, Matrix._12, Matrix._13);
		D3DXVECTOR3 VecY(Matrix._21, Matrix._22, Matrix._23);
		D3DXVECTOR3 VecZ(Matrix._31, Matrix._32, Matrix._33);
		m_Size = D3DXVECTOR3(
			Size.x * D3DXVec3Length(&VecX),
			Size.y * D3DXVec3Length(&VecY),
			Size.z * D3DXVec3Length(&VecZ)
		);
		//m_Size *= 0.5f;
		//回転を得る
		
		D3DXVec3Normalize(&m_RotNormal[0] , &VecX);
		D3DXVec3Normalize(&m_RotNormal[1] , &VecY);
		D3DXVec3Normalize(&m_RotNormal[2] , &VecZ);
	}

	// 回転行列の取得
	D3DXMATRIX GetRotNormalMatrix()const 
	{
		D3DXMATRIX ret;
		ret._11 = m_RotNormal[0].x;
		ret._12 = m_RotNormal[0].y;
		ret._13 = m_RotNormal[0].z;
		ret._21 = m_RotNormal[1].x;
		ret._22 = m_RotNormal[1].y;
		ret._23 = m_RotNormal[1].z;
		ret._31 = m_RotNormal[2].x;
		ret._32 = m_RotNormal[2].y;
		ret._33 = m_RotNormal[2].z;
		return ret;
	}

	// 指定軸番号の標準化された(Normalizeされている)方向ベクトルを取得。回転行列の1方向をベクターで取得。引き数に0,1,2を指定。
	D3DXVECTOR3 GetRotNormalVector(int DirectNum)const
	{
		// 既に単位ベクトルになっている
		D3DXVECTOR3 ret;
		ret.x = m_RotNormal[DirectNum].x;
		ret.y = m_RotNormal[DirectNum].y;
		ret.z = m_RotNormal[DirectNum].z;
		return ret;
	}

	// 指定軸番号の長さを取得。回転行列の1方向をベクターで取得。引き数に0,1,2を指定。
	float GetRotNormalLength(int DirectNum, D3DXVECTOR3 colscale)const
	{
		D3DXVECTOR3 RotNormalVec;
		// 指定方向に応じてScale分かけてあげると長さになる。
		float directscale;
		if (DirectNum == 0)
			directscale = colscale.x;
		else if (DirectNum == 1)
			directscale = colscale.y;
		else if (DirectNum == 2)
			directscale = colscale.z;

		//RotNormalVec.x = m_RotNormal[DirectNum].x;
		//RotNormalVec.y = m_RotNormal[DirectNum].y;
		//RotNormalVec.z = m_RotNormal[DirectNum].z;

		RotNormalVec.x = m_RotNormal[DirectNum].x * directscale;
		RotNormalVec.y = m_RotNormal[DirectNum].y * directscale;
		RotNormalVec.z = m_RotNormal[DirectNum].z * directscale;

		float ret = D3DXVec3Length(&RotNormalVec);
		//if (DirectNum == 0)
		//	ret* m_Size.x;
		//else if (DirectNum == 1)
		//	ret* m_Size.y;
		//else if (DirectNum == 2)
		//	ret* m_Size.z;

		return ret;
	}

	//// ワールド行列の取得
	//D3DXMATRIX GetWorldMatrix()const {
	//	D3DXMATRIX ret, Pos;
	//	ret = XMMatrixScaling(m_Size.x * 2.0f, m_Size.y * 2.0f, m_Size.z * 2.0f);
	//	ret = XMMatrixMultiply(ret, GetRotMatrix());
	//	Pos = XMMatrixTranslation(m_Center.x, m_Center.y, m_Center.z);
	//	ret = XMMatrixMultiply(ret, Pos);
	//	return ret;
	//}

	// 一番大きい辺の半分長さを取得
	float GetMaxHalfSize()const 
	{
		float ret = m_Size.x;
		if (ret < m_Size.y) {
			ret = m_Size.y;
		}
		if (ret < m_Size.z) {
			ret = m_Size.z;
		}
		return ret;
	}

	// 一番大きい辺の長さを取得
	float GetMaxSize()const 
	{
		return GetMaxHalfSize() * 2.0f;
	}

	// 一番小さい辺の半分長さを取得
	float GetMinHalfSize() const 
	{
		float ret = m_Size.x;
		if (ret > m_Size.y) {
			ret = m_Size.y;
		}
		if (ret > m_Size.z) {
			ret = m_Size.z;
		}
		return ret;
	}

	// 一番小さい辺の長さを取得
	float GetMinSize() const 
	{
		return  GetMinHalfSize() * 2.0f;
	}

	//// ベクトルに一番近い回転を得る
	//void GetNearNormalRot(const D3DXVECTOR3& Check, float Epsilon, D3DXVECTOR3& Ret)
	//{
	//	for (int i = 0; i < 3; i++) {
	//		D3DXVECTOR4 VecEp(Epsilon);
	//		if (XMVector3NearEqual(Check, m_Rot[i], VecEp)) {
	//			Ret = m_Rot[i];
	//			return;
	//		}
	//		if (XMVector3NearEqual(Check, -m_Rot[i], VecEp)) {
	//			Ret = -m_Rot[i];
	//			return;
	//		}
	//	}
	//}

	// OBBが回転してないかどうかを調べる（AABBとして使えるかどうかを調べられる）
	bool IsRotIdentity() const 
	{
		if (m_RotNormal[0] == D3DXVECTOR3(1.0f, 0.0f, 0.0f)
			&& m_RotNormal[1] == D3DXVECTOR3(0.0f, 1.0f, 0.0f)
			&& m_RotNormal[2] == D3DXVECTOR3(0.0f, 0.0f, 1.0f)) {
			return true;
		}
		return false;
	}

	// 回転後の頂点の座標を取得
	void CalcVertexPos(D3DXVECTOR3 rotation)
	{
		D3DXVECTOR3 VertexPos[8];

		float px = m_Size.x;
		float py = m_Size.y;
		float pz = m_Size.z;
		float mx = -m_Size.x;
		float my = -m_Size.y;
		float mz = -m_Size.z;

		// 上の面m_VertexPos[0]
		VertexPos[0] = D3DXVECTOR3(mx, py, pz);		// 左上
		VertexPos[1] = D3DXVECTOR3(mx, py, mz);		// 左下
		VertexPos[2] = D3DXVECTOR3(px, py, mz);		// 右下
		VertexPos[3] = D3DXVECTOR3(px, py, pz);		// 右上
		// 下の面
		VertexPos[4] = D3DXVECTOR3(mx, my, pz);		// 左上
		VertexPos[5] = D3DXVECTOR3(mx, my, mz);		// 左下
		VertexPos[6] = D3DXVECTOR3(px, my, mz);		// 右下
		VertexPos[7] = D3DXVECTOR3(px, my, pz);		// 右上

		//	今の回転から回転行列を作成する
		D3DXMATRIX rotmat;
		D3DXMatrixRotationYawPitchRoll(&rotmat, rotation.y, rotation.x, rotation.z);

		for (int i = 0; i < 8; i++)
		{
			if (i == 2)
				int ggghhhh = 4;
			// 頂点座標を回転行列で回転させて座標を取得する
			D3DXVECTOR3 BeforePos = VertexPos[i];
			D3DXVECTOR3 AfterPos;
			D3DXVec3TransformCoord(&AfterPos, &BeforePos, &rotmat);	// BeforePosをrotmatで回転させたものがAfterPosに戻り値として入る。

			// 相対座標を絶対座標にする
			m_VertexPos[i] = m_Center + AfterPos;
		}

		int ggrgrg = 55;

		return;

	}

	// 線分12個の設定をする。頂点を先に設定しておかないとダメ
	void CalcSegment()
	{
		// 線分12個の設定をする
		m_Segment[0].m_pos1 = m_VertexPos[0];	// 上面
		m_Segment[0].m_pos2 = m_VertexPos[1];
		m_Segment[1].m_pos1 = m_VertexPos[1];
		m_Segment[1].m_pos2 = m_VertexPos[2];
		m_Segment[2].m_pos1 = m_VertexPos[2];
		m_Segment[2].m_pos2 = m_VertexPos[3];
		m_Segment[3].m_pos1 = m_VertexPos[3];
		m_Segment[3].m_pos2 = m_VertexPos[0];

		m_Segment[4].m_pos1 = m_VertexPos[4];	// 下面
		m_Segment[4].m_pos2 = m_VertexPos[5];
		m_Segment[5].m_pos1 = m_VertexPos[5];
		m_Segment[5].m_pos2 = m_VertexPos[6];
		m_Segment[6].m_pos1 = m_VertexPos[6];
		m_Segment[6].m_pos2 = m_VertexPos[7];
		m_Segment[7].m_pos1 = m_VertexPos[7];
		m_Segment[7].m_pos2 = m_VertexPos[4];

		m_Segment[8].m_pos1 = m_VertexPos[0];	// 縦
		m_Segment[8].m_pos2 = m_VertexPos[4];
		m_Segment[9].m_pos1 = m_VertexPos[1];
		m_Segment[9].m_pos2 = m_VertexPos[5];
		m_Segment[10].m_pos1 = m_VertexPos[2];
		m_Segment[10].m_pos2 = m_VertexPos[6];
		m_Segment[11].m_pos1 = m_VertexPos[3];
		m_Segment[11].m_pos2 = m_VertexPos[7];
	}


	// 平面の法線を作成。先に線分を作っておかないといけない
	void CalcNormal()
	{
		m_Normal[0] = m_Segment[8].GetNormalVector() * -1;	// 上方向ベクトル
		m_Normal[1] = m_Segment[1].GetNormalVector();		// 右方向ベクトル
		m_Normal[2] = m_Segment[2].GetNormalVector();		// 奥方向ベクトル
		m_Normal[3] = m_Normal[0] * -1;				// 下
		m_Normal[4] = m_Normal[1] * -1;				// 左
		m_Normal[5] = m_Normal[2] * -1;				// 前
	}


	// 平面を作成。先に法線を作っておかないといけない
	void CalcPlane()
	{
		
		D3DXPlaneFromPointNormal(&m_Plane[0], &m_VertexPos[0], &m_Normal[0]);		// 上面	
		D3DXPlaneFromPointNormal(&m_Plane[1], &m_VertexPos[6], &m_Normal[3]);		// 下面
		D3DXPlaneFromPointNormal(&m_Plane[2], &m_VertexPos[6], &m_Normal[1]);		// 右面
		D3DXPlaneFromPointNormal(&m_Plane[3], &m_VertexPos[0], &m_Normal[4]);		// 左面
		D3DXPlaneFromPointNormal(&m_Plane[4], &m_VertexPos[0], &m_Normal[2]);		// 奥面
		D3DXPlaneFromPointNormal(&m_Plane[5], &m_VertexPos[6], &m_Normal[5]);		// 前面

		//D3DXPlaneFromPoints(&m_Plane[0], &m_VertexPos[0], &m_VertexPos[1], &m_VertexPos[2]);		// 上面	
		//D3DXPlaneFromPoints(&m_Plane[1], &m_VertexPos[2], &m_VertexPos[6], &m_VertexPos[7]);		// 右面
		//D3DXPlaneFromPoints(&m_Plane[2], &m_VertexPos[0], &m_VertexPos[4], &m_VertexPos[7]);		// 奥面
		//D3DXPlaneFromPoints(&m_Plane[3], &m_VertexPos[4], &m_VertexPos[5], &m_VertexPos[6]);		// 下面
		//D3DXPlaneFromPoints(&m_Plane[4], &m_VertexPos[0], &m_VertexPos[1], &m_VertexPos[5]);		// 左面
		//D3DXPlaneFromPoints(&m_Plane[5], &m_VertexPos[1], &m_VertexPos[5], &m_VertexPos[6]);		// 前面
		


		int a = 4;
	}

	// obbの6つの平面、その12つの線分、6平面の法線、8つの頂点を求める。それぞれメンバ変数に入っている
	void CreatePlaneState(D3DXVECTOR3 rotation)
	{
		CalcVertexPos(rotation);
		CalcSegment();
		CalcNormal();
		CalcPlane();
	}

};











//--------------------------------------------------------------------------------------
/*!
@brief  pointから見たOBBの最近接点を得る
@param[in]  point   基準点
@param[in]  obb OBB
@param[out] retvec  最近接点を返す参照
@return なし
*/
//--------------------------------------------------------------------------------------
static void ClosestPtPointOBB(const D3DXVECTOR3& point, const OBB& obb, D3DXVECTOR3& retvec) 
{
	D3DXVECTOR3 d = point - obb.m_Center;
	retvec = obb.m_Center;
	float dist;
	for (int i = 0; i < 3; i++)
	{
		
		dist = D3DXVec3Dot(&d, &obb.m_RotNormal[i]);
		if (dist > obb.m_Size[i])
		{
			dist = obb.m_Size[i];
		}
		if (dist < -obb.m_Size[i])
		{
			dist = -obb.m_Size[i];
		}
		retvec += dist * obb.m_RotNormal[i];
	}
}
//--------------------------------------------------------------------------------------
/*!
@brief  球とOBBとの衝突判定
@param[in]  sp  球
@param[in]  obb OBB
@param[out] retvec  最近接点が代入される参照
@return 衝突していればtrue
*/
//--------------------------------------------------------------------------------------
static bool SPHERE_OBB(const SPHERE& sp, const OBB& obb, D3DXVECTOR3& retvec) 
{
	ClosestPtPointOBB(sp.m_Center, obb, retvec);
	D3DXVECTOR3 v = retvec - sp.m_Center;
	return D3DXVec3Dot(&v, &v) <= sp.m_Radius * sp.m_Radius;
}
//static bool SPHERE_OBB(const SPHERE& sp, const OBB& obb, bsm::Vec3& retvec) {
//	ClosestPtPointOBB(sp.m_Center, obb, retvec);
//	bsm::Vec3 v = retvec - sp.m_Center;
//	return bsm::dot(v, v) <= sp.m_Radius * sp.m_Radius;
//}


//--------------------------------------------------------------------------------------
		/*!
		@brief	点cと線分abの最近接点を返す
		@param[in]	c	もとになる点
		@param[in]	a	線分開始点
		@param[in]	b	線分終了点
		@param[out]	t	最近接点の線分上の位置の割合の戻り値
		@param[out]	d	最近接点の戻り値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
// 線分側の最近接点が求まる
static void ClosetPtPointSegment(const D3DXVECTOR3& c,
	const D3DXVECTOR3& a, const D3DXVECTOR3& b,
	float& t, D3DXVECTOR3& d) 
{
	D3DXVECTOR3 ab = b - a;
	D3DXVECTOR3 ca = c - a;
	t = D3DXVec3Dot(&ca, &ab) / D3DXVec3Dot(&ab, &ab);
	if (t < 0.0f) {
		t = 0.0f;
	}
	if (t > 1.0f) {
		t = 1.0f;
	}
	d = a + (ab * t);
}



//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルとObbの最近接点を得る
		@param[in]	cp	カプセル
		@param[in]	obb	OBB
		@param[out]	flg	2つ球のとの位置関係
		@return	最近接点(カプセルの)
		*/
		//--------------------------------------------------------------------------------------
static D3DXVECTOR3 ClosestPtCapsuleOBB(const CAPSULE& cp, const OBB& obb, int& flg) 
{
	// 計算しやすいように取得しておく
	D3DXVECTOR3 UpperPos = cp.GetUpperSpherePos();
	D3DXVECTOR3 LowerPos = cp.GetLowerSpherePos();
	SPHERE Sp;	// 変数の準備
	Sp.m_Center = UpperPos;
	Sp.m_Radius = cp.m_Radius;

	D3DXVECTOR3 retvec;
	//スタート位置で最近接点を得る
	SPHERE_OBB(Sp, obb, retvec);
	//内積を図る
	D3DXVECTOR3 Base = LowerPos - UpperPos;
	//Base.normalize();
	D3DXVec3Normalize(&Base, &Base);
	D3DXVECTOR3 Dest = retvec - UpperPos;
	float dot = D3DXVec3Dot(&Base, &Dest);
	//float dot = bsm::dot(Base, Dest);
	if (dot < 0) {
		//スタート位置の球体の外側
		//retvecは有効
		flg = -1;
		return retvec;
	}
	D3DXVECTOR3 LU = LowerPos - UpperPos;
	float  size = D3DXVec3Length(&LU);
	if (dot > size) {
		//終点より先にある
		Sp.m_Center = LowerPos;
		SPHERE_OBB(Sp, obb, retvec);
		//終点で最近接点をとる
		flg = 1;
		return retvec;
	}
	//中心とobbの最近接点を得る
	ClosestPtPointOBB(cp.GetCenterPos(), obb, retvec);
	float t;
	D3DXVECTOR3 SegPoint;
	ClosetPtPointSegment(retvec, UpperPos, LowerPos, t, SegPoint);
	D3DXVECTOR3 Span = retvec - SegPoint;
	D3DXVec3Normalize(&Span, &Span);
	/*Span.normalize();*/
	Span *= cp.m_Radius;
	SegPoint += Span;
	retvec = SegPoint;
	flg = 0;
	return retvec;
}

// AのOBBに対して一番近いBのOBBの頂点の添え字が返ってくる
static int GetOBBOBBNearestVertex(OBB Aobb, OBB Bobb)
{
	int nearest = -1;
	float nearestLength = 999.0f;
	// 2点間の距離が1番短いものを更新していく感じ
	for (int i = 0; i < 4; i++)
	{
		D3DXVECTOR3 Closestpos;
		ClosestPtPointOBB(Bobb.m_VertexPos[i], Aobb, Closestpos);	// AのOBBとBの頂点の最接近点が求まる
		D3DXVECTOR3 distanceVec = Bobb.m_VertexPos[i] - Closestpos;
		float length = D3DXVec3Length(&distanceVec);				// 2点間の距離が求まる
		if (nearestLength > length)
		{
			nearestLength = length;
			nearest = i;
		}
	}

	return nearest;
}




// ある点とOBBで、ある点はOBBのどの面と一番近いのかを計算する。面の添え字が返る。これはOBBと点の衝突寸前ほど近いことが前提の計算。
static int GetPointOBBNearestPlane(D3DXVECTOR3* retDistanceVec, D3DXVECTOR3 point, OBB obb)
{

	int nearest = -1;
	float nearestLength = 999.0f;
	for (int i = 0; i < 6; i++)
	{
		D3DXVECTOR3 PenetratingPoint;	// 平面と直線の貫通座標が入る

		// 直線を作らなければならないが、その平面の内側に入っているかどうかも同時に調べるために、平面の法線の逆に伸ばして線分を作る
		D3DXVECTOR3 pos1 = point;
		D3DXVECTOR3 pos2 = point - (obb.m_Normal[i] * 10.0f);		// もしpos1が外側にあればこっちだけ内側になる

		D3DXVECTOR3 pv = pos2 - pos1;

		D3DXPlaneIntersectLine(&PenetratingPoint, &obb.m_Plane[i], &pos1, &pv);	// 平面(無限に広がっている)との貫通点を出す

		
		float a = obb.m_Plane[i].a;	// -1
		float b = obb.m_Plane[i].b;
		float c = obb.m_Plane[i].c;
		float d = obb.m_Plane[i].d;	// -2

		int kkkk = 10;
		if (PenetratingPoint == NULL)
			kkkk = 6;

		if (i == 5)
			int fjjj = 5;
		D3DXVECTOR3 PeToP1Vec = pos1 - PenetratingPoint;		// 貫通点からそれぞれへのベクトルを出す
		D3DXVECTOR3 PeToP2Vec = pos2 - PenetratingPoint;

		float p1dot = D3DXVec3Dot(&PeToP1Vec, &obb.m_Normal[i]);	// それぞれの内積を出す
		float p2dot = D3DXVec3Dot(&PeToP2Vec, &obb.m_Normal[i]);

		float xxx = obb.m_VertexPos[0].x;


		// 平面を貫通している。違うならば、その点はもともと平面の内側に入っているので無視する
		// 貫通点とある点の距離を求める
		if (p1dot * p2dot <= 0)
		{
			D3DXVECTOR3 distanceVec = PenetratingPoint - point;
			float length = D3DXVec3Length(&distanceVec);				// 2点間の距離が求まる
			if (nearestLength > length)
			{
				*retDistanceVec = distanceVec;
				nearestLength = length;
				nearest = i;
			}
		}
		else
			continue;
	}

	return nearest;

}


//static D3DXVECTOR3 CalcPlaneToPoint(D3DXPLANE plane, D3DXVECTOR3 point, int vertexnumber)
static D3DXVECTOR3 CalcPlaneToPoint(OBB Aobb, int vertexnum, OBB Bobb, int planenum)
{
	D3DXVECTOR3 distancevec;	// 戻り値用
	D3DXVECTOR3 PenetratingPoint;	// 平面と直線の貫通座標が入る

	D3DXPlaneIntersectLine(&PenetratingPoint, &Bobb.m_Plane[planenum], &Aobb.m_VertexPos[vertexnum], &Bobb.m_Normal[planenum]);

	distancevec = Aobb.m_VertexPos[vertexnum] - PenetratingPoint;
	float distanceLength = D3DXVec3Length(&distancevec);
	D3DXVECTOR3 backvec = Bobb.m_Normal[planenum] * distanceLength;

	float DotCoord = D3DXPlaneDotCoord(&Bobb.m_Plane[planenum], &Aobb.m_VertexPos[vertexnum]);
	float len = DotCoord / D3DXVec3Length(&Bobb.m_Normal[planenum]);
	//backvec = Bobb.m_Normal[planenum] * len;
	return backvec * 1.0f;
}








//D3DXVec3Normalize(&m_Rot[0], &VecX); D3DXVec3Length



//
//const D3DXMATRIX& Transform::GetWorldMatrix() const {
//	auto ParPtr = GetParent();
//	if (pImpl->m_Changeed || ParPtr) {
//		pImpl->m_WorldMatrix.affineTransformation(
//			pImpl->m_Scale,
//			pImpl->m_Pivot,
//			pImpl->m_Quaternion,
//			pImpl->m_Position
//		);
//		pImpl->m_Changeed = false;
//		if (ParPtr) {
//			auto ParWorld = ParPtr->GetComponent<Transform>()->GetWorldMatrix();
//			ParWorld.scaleIdentity();
//			pImpl->m_WorldMatrix = pImpl->m_WorldMatrix * ParWorld;
//		}
//	}
//	return pImpl->m_WorldMatrix;
//}





// 関数の宣言
// classやstruct以外の関数とかは、cppに書かないと、１つ以上の複数回定義されてるシンボルがあります
// になってしまう。気を付けて。

// 引き数を使ってワールドマトリクスを0から作る
// GetWorldMatrixとの違いはこっちは引き数でもらうのでどんなのにも対応できる
//D3DXMATRIX CreateWorldMatrix(D3DXVECTOR3 Scale, D3DXVECTOR3 Rotation, D3DXVECTOR3 Position);
bool CAPSULE_OBB(CAPSULE cp, OBB obb, D3DXVECTOR3 retvec);
static bool CollisionTestSphereObb(const SPHERE& SrcSp, const D3DXVECTOR3& SrcVelocity,
	const OBB& DestObb,
	float StartTime, float EndTime, float& HitTime);
bool colCapsuleCapsule(CAPSULE& c1, CAPSULE& c2, D3DXVECTOR3* p1, D3DXVECTOR3* p2, float* vd);
float calcSegmentSegmentDist(SEGMENT seg1, SEGMENT seg2, D3DXVECTOR3& p1, D3DXVECTOR3& p2, float& t1, float& t2);
float calcPointSegmentDist(D3DXVECTOR3& point, SEGMENT& seg, D3DXVECTOR3& h, float& t);
float calcPointLineDist(D3DXVECTOR3& point, SEGMENT& segment, D3DXVECTOR3& h, float& t);
bool isSharpAngleCalc3Point(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3);
bool isSharpAngleCalcDot(D3DXVECTOR3 a, D3DXVECTOR3& b);
void clamp01(float& v);
float calcLineLineDist(SEGMENT& l1, SEGMENT& l2, D3DXVECTOR3& p1, D3DXVECTOR3& p2, float& t1, float& t2);
bool isParallel(D3DXVECTOR3& a, D3DXVECTOR3& b);
D3DXVECTOR3 cross(D3DXVECTOR3& a, D3DXVECTOR3& b);
D3DXVECTOR3 CalcAfterRoatedPos(D3DXVECTOR3 beforeOffset, D3DXVECTOR3 rotation);
D3DXVECTOR3 GetPositionForWorldMatrix(D3DXMATRIX matrix);

// sourceがdivisor(割る数)の半分の前半に位置するかどうか。trueで前半、falseで後半に位置している。
bool FindoutBeforethehalf(const float source, const float divisor = 1.0f);

// 壁ずりベクトル : out : 正規化壁ずりベクトル（戻り値） : front : 進行ベクトル : normal: 衝突点での法線ベクトル
D3DXVECTOR3* CalcWallScratchVector(D3DXVECTOR3* pOut_vec, const D3DXVECTOR3& front, const D3DXVECTOR3& normal);

bool calcRaySphere(D3DXVECTOR3 raystart, D3DXVECTOR3 raydirect, D3DXVECTOR3 spherepos,
	float spherer, D3DXVECTOR3* q1, D3DXVECTOR3* q2);


double Cross2D(const D3DXVECTOR2& a, const D3DXVECTOR2& b);

bool CalcIntersectionPoint(const D3DXVECTOR2& pointA,
	const D3DXVECTOR2& pointB,
	const D3DXVECTOR2& pointC,
	const D3DXVECTOR2& pointD,
	D3DXVECTOR2& pointIntersection,
	double& dR,
	double& dS);

bool CalcAngle(const D3DXVECTOR2& vectorA,
	const D3DXVECTOR2& vectorB,
	double& dAngle_rad);

bool CalcAngle2(const D3DXVECTOR2& vectorA,
	const D3DXVECTOR2& vectorB,
	double& dAngle_rad);

bool CalcAngle3(const D3DXVECTOR2& vectorA,
	const D3DXVECTOR2& vectorB,
	double& dAngle_rad);

float BringingValuesCloserFloat(const float& Correct, const float& Addition, const float& NowValue, const float& RoopEndValue);


#endif