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


//// 3成分float
//struct Float3 {
//	float x, y, z;
//
//	Float3() {}
//	Float3(float x, float y, float z) : x(x), y(y), z(z) {}
//	~Float3() {}
//
//	Float3 operator +(const Float3& r) const {
//		return Float3(x + r.x, y + r.y, z + r.z);
//	}
//
//	Float3 operator -(const Float3& r) const {
//		return Float3(x - r.x, y - r.y, z - r.z);
//	}
//
//	Float3 operator -() const {
//		return Float3(x * -1.0f, y * -1.0f, z * -1.0f);
//	}
//
//	Float3 operator *(const Float3& r) const {
//		return Float3(x * r.x, y * r.y, z * r.z);
//	}
//
//	Float3 operator /(const Float3& r) const {
//		return Float3(x / r.x, y / r.y, z / r.z);
//	}
//
//	Float3 operator *(float r) const {
//		return Float3(x * r, y * r, z * r);
//	}
//
//	Float3 operator /(float r) const {
//		return Float3(x / r, y / r, z / r);
//	}
//
//	friend Float3 operator *(float l, const Float3& r) {
//		return Float3(r.x * l, r.y * l, r.z * l);
//	}
//
//	friend Float3 operator /(float l, const Float3& r) {
//		return Float3(r.x / l, r.y / l, r.z / l);
//	}
//
//	// 自分自身と引数のdot
//	float dot(const Float3& r) const {
//		return x * r.x + y * r.y + z * r.z;
//	}
//
//	Float3 cross(const Float3& r) const {
//		return Float3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
//	}
//
//	float length() const {
//		return sqrtf(lengthSq());
//	}
//
//	float lengthSq() const {
//		return x * x + y * y + z * z;
//	}
//
//	void norm() {
//		const float len = length();
//		if (len > 0.0f) {
//			x /= len;
//			y /= len;
//			z /= len;
//		}
//	}
//
//	Float3 getNorm() const {
//		const float len = length();
//		if (len > 0.0f) {
//			return Float3(x / len, y / len, z / len);
//		}
//		return Float3(0.0f, 0.0f, 0.0f);
//	}
//
//
//	//// 鋭角関係？
//	//bool isSharpAngle(const Vec3& r) const {
//	//	return (dot(r) >= 0.0f);
//	//}
//
//};
//
//
//// 点
//typedef Float3 Point;
//typedef Float3 D3DXVEXTOR3;
//
//
//// ベクトル
//struct Vec3 : public Float3 {
//	Vec3() {}
//	Vec3(float x, float y, float z) : Float3(x, y, z) {}
//	Vec3(const Float3& r) : Float3(r) {}
//	~Vec3() {}
//
//	Vec3& operator =(const Float3& r) {
//		x = r.x;
//		y = r.y;
//		z = r.z;
//		return *this;
//	}
//
//	// 標準化
//	void norm() {
//		const float len = length();
//		if (len > 0.0f) {
//			x /= len;
//			y /= len;
//			z /= len;
//		}
//	}
//
//	// 垂直関係にある？
//	bool isVertical(const Vec3& r) const {
//		float d = dot(r);
//		return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// 誤差範囲内なら垂直と判定
//	}
//
//	// 平行関係にある？
//	bool isParallel(const Vec3& r) const {
//		float d = cross(r).lengthSq();
//		return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// 誤差範囲内なら平行と判定
//	}
//
//	// 鋭角関係？
//	bool isSharpAngle(const Vec3& r) const {
//		return (dot(r) >= 0.0f);
//	}
//
//
//};
//
//
//// 直線
//struct Line {
//	Point p;
//	Vec3 v;		// 方向ベクトル(単位ベクトルではないのでyが30離れてたら30が入ってる)
//	Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
//	Line(const Point& p, const Vec3& v) : p(p), v(v) {}
//	~Line() {}
//
//	// 点上の座標を取得
//	//  ベクトルに掛け算する係数
//	Point getPoint(float t) const {
//		return p + t * v;
//	}
//
//
//	// 1つ目の上の点をゲットする
//	Point GetPoint1() const {
//		return p;
//	}
//	// 2つ目の上の点をゲットする
//	Point GetPoint2() const {
//		return p + v;
//	}
//
//	// ベクターをゲットする
//	Vec3 GetNormalVec() const {
//		return v;
//	}
//};
//
//
//// 線分
//struct Segment : public Line {
//
//	Segment() {}
//	Segment(const Point& p, const Vec3& v) : Line(p, v) {}
//	Segment(const Point& p1, const Point& p2) : Line(p1, p2 - p1) {}
//
//	// 終点を取得
//	Float3 getEndPoint() const {
//		return p + v;
//	}
//};
//
//
//// 球
//struct Sphere {
//	Point p;
//	float r;	// 半径
//	Sphere() : p(0.0f, 0.0f, 0.0f), r(0.5f) {}
//	Sphere(const Point& p, float r) : p(p), r(r) {}
//	~Sphere() {}
//};
//
//// カプセル
//// 半径rとP1P2を設定してあげる。もしくは
//// 半径rとP1(上の方)とそっからの方向ベクトル(長さ持ってるよ)
//struct Capsule {
//	Segment s;
//	float r;		// 球の半径の割合(100.0fが基準 = 1.0f)
//	float r_num;	// 球の半径
//	float h;		// Harf height。(100.0fが基準 = 1.0f)
//	float h_num;	// Harf height。割合。球を含まない高さ。
//	Point pos;		// カプセルの中心座標
//	//Point pos1;	// カプセルの上の球の座標
//	//Point pos2;	// カプセルの上の球の座標
//	Capsule() : r(0.5f), h(0.5f) {}
//	Capsule(const Segment& s, float r, float h) : s(s), r(r), h(h) {}
//	Capsule(const Point& p1, const Point& p2, float r, float h) : s(p1, p2), r(r), h(h) {}
//	~Capsule() {}
//
//	// (座標、半径、半高)pが中心となる高さheightのカプセルを作成
//	void SetCapsule(Point centerpos, float radius, float harfheight)
//	{
//		r_num = radius;
//		h_num = harfheight;
//		if (h_num <= r_num )
//		{
//			h_num = r_num;		// 半高さが半径を上回ることはできない。
//		}
//
//		r = r_num / 100.0f;
//		if (h_num == r_num)
//		{
//			h = 0.0f;
//		}
//		else
//		{
//			h = h_num / 100.0f;
//		}
//
//		pos = centerpos;
//		Point pos1 = pos;
//		Point pos2 = pos;
//		pos1.y += h * 0.5;		// p1を中心とするためそこから高さを半分ずつしてる
//		pos2.y -= h * 0.5;
//
//		s.p = pos1;
//		s.v = pos2 - pos1;
//	}
//
//	//// 1つ目の上の点をゲットする
//	//Point GetPoint1() const {
//	//	return pos1;
//	//}
//	//// 1つ目の上の点をゲットする
//	//Point GetPoint2() const {
//	//	return pos2;
//	//}
//
//};
//
//// AABB 
//struct AABB {
//	Point p;	// 中心点
//	Float3 hl;	// 各軸の辺の長さの半分
//	AABB() {}
//	AABB(const Point& p, const Float3& hl) : p(p), hl(hl) {}
//
//	// 辺の長さを取得
//	float lenX() const { return hl.x * 2.0f; };
//	float lenY() const { return hl.y * 2.0f; };
//	float lenZ() const { return hl.z * 2.0f; };
//	float len(int i) {
//		return *((&hl.x) + i) * 2.0f;
//	}
//};
//
//// OBB Oriented Bounding Box 有向境界ボックス
//struct OBB {
//	Point c;     //中心点の座標
//	Point u[3];  //XYZ の各座標軸の傾きを表す方向ベクトル
//	Point e;     //OBB の各座標軸に沿った長さの半分（中心点から面までの長さ）
//};
//
//
//// 関数の宣言
//// classやstruct以外の関数とかは、cppに書かないと、１つ以上の複数回定義されてるシンボルがあります
//// になってしまう。気を付けて。
//
//float calcPointLineDist(const Point& p, const Line& l, Point& h, float& t);
//bool isSharpAngle(const Point& p1, const Point& p2, const Point& p3);
//float calcPointSegmentDist(const Point& p, const Segment& seg, Point& h, float& t);
//float calcLineLineDist(const Line& l1, const Line& l2, Point& p1, Point& p2, float& t1, float& t2);
//void clamp01(float& v);
//float calcSegmentSegmentDist(const Segment& s1, const Segment& s2, Point& p1, Point& p2, float& t1, float& t2);
//bool colCapsuleCapsule(const Capsule& c1, const Capsule& c2, Point* p1, Point* p2, float* vd);
////void ClosestPtPointOBB(Point* p, OBB* b, Point* q);
////
////int TestSphereOBB(Sphere* s, OBB* b, Point* p)
////{
////	ClosestPtPointOBB(&s->p, b, p);
////	D3DXVECTOR3 v = *p - s->c;
////	return D3DXVec3Dot(&v, &v) <= s->r * s->r;
////}
//
//
//
//


// 関数の宣言
// classやstruct以外の関数とかは、cppに書かないと、１つ以上の複数回定義されてるシンボルがあります
// になってしまう。気を付けて。

// 引き数を使ってワールドマトリクスを0から作る
// GetWorldMatrixとの違いはこっちは引き数でもらうのでどんなのにも対応できる
D3DXMATRIX CreateWorldMatrix(D3DXVECTOR3 Scale, D3DXVECTOR3 Rotation, D3DXVECTOR3 Position);



// ここから新しいの


struct SPHERE;
struct CAPSULE;
struct OBB;

struct SPHERE 
{
	D3DXVECTOR3 m_Center;	// 中心点の座標
	float		m_Radius;	// 半径

	SPHERE() : m_Center(0, 0, 0), m_Radius(0.5f) {}

};


struct CAPSULE
{
	float m_Radius;				// 半径
	D3DXVECTOR3 m_PointUpper;	// カプセルの上の球体部分の球の中心座標
	D3DXVECTOR3 m_PointLower;	// カプセルの下の球体部分の球の中心座標

	// コンストラクタ
	CAPSULE() : m_Radius(0.5f), m_PointUpper(0, -0.5f, 0), m_PointLower(0, 0.5f, 0) {}

	// カプセルの中心座標を取得
	D3DXVECTOR3 GetCenter() const
	{
		return m_PointUpper + ((m_PointLower - m_PointUpper) * 0.5f);
	}

	// カプセルの中心座標から上下2つの球の座標をセット
	void SetCenter(const D3DXVECTOR3& Center) {
		D3DXVECTOR3 CenterToPointA = ((m_PointUpper - m_PointLower) * 0.5f);
		D3DXVECTOR3 CenterToPointB = ((m_PointLower - m_PointUpper) * 0.5f);
		m_PointUpper = Center + CenterToPointA;
		m_PointLower = Center + CenterToPointB;
	}
	// 縦の半径を取得する
	float GetHeightRadius()const {
		D3DXVECTOR3 temp_calclength = m_PointLower - m_PointUpper;
		float PointLen = D3DXVec3Length(&temp_calclength) * 0.5f;
		PointLen += m_Radius;
		return PointLen;
	}
};


struct OBB
{
	D3DXVECTOR3 m_Center;   // 中心点の座標
	D3DXVECTOR3 m_Rot[3];	// XYZ の各座標軸の傾きを表す方向ベクトル
	D3DXVECTOR3 m_Size;     // OBB の各座標軸に沿った長さの半分（中心点から面までの長さ）

	// コンストラクタ
	OBB() 
	{
		//デフォルトは1辺1.0のOBB
		D3DXVECTOR3 SizeVec(1.0f, 1.0f, 1.0f);
		//行列はアイデンティティ
		D3DXMATRIX Matrix;
		CreateOBB(SizeVec, Matrix);
	}

	
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
		m_Size *= 0.5f;
		//回転を得る
		
		D3DXVec3Normalize(&m_Rot[0] , &VecX); 
		D3DXVec3Normalize(&m_Rot[1] , &VecY);
		D3DXVec3Normalize(&m_Rot[2] , &VecZ);
	}

	// 回転行列の取得
	D3DXMATRIX GetRotMatrix()const 
	{
		D3DXMATRIX ret;
		ret._11 = m_Rot[0].x;
		ret._12 = m_Rot[0].y;
		ret._13 = m_Rot[0].z;
		ret._21 = m_Rot[1].x;
		ret._22 = m_Rot[1].y;
		ret._23 = m_Rot[1].z;
		ret._31 = m_Rot[2].x;
		ret._32 = m_Rot[2].y;
		ret._33 = m_Rot[2].z;
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
		if (m_Rot[0] == D3DXVECTOR3(1.0f, 0.0f, 0.0f)
			&& m_Rot[1] == D3DXVECTOR3(0.0f, 1.0f, 0.0f)
			&& m_Rot[2] == D3DXVECTOR3(0.0f, 0.0f, 1.0f)) {
			return true;
		}
		return false;
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
		
		dist = D3DXVec3Dot(&d, &obb.m_Rot[i]);
		if (dist > obb.m_Size[i])
		{
			dist = obb.m_Size[i];
		}
		if (dist < -obb.m_Size[i])
		{
			dist = -obb.m_Size[i];
		}
		retvec += dist * obb.m_Rot[i];
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
		@return	最近接点
		*/
		//--------------------------------------------------------------------------------------
static D3DXVECTOR3 ClosestPtCapsuleOBB(const CAPSULE& cp, const OBB& obb, int& flg) {
	SPHERE Sp;
	Sp.m_Center = cp.m_PointUpper;
	Sp.m_Radius = cp.m_Radius;
	D3DXVECTOR3 retvec;
	//スタート位置で最近接点を得る
	SPHERE_OBB(Sp, obb, retvec);
	//内積を図る
	D3DXVECTOR3 Base = cp.m_PointLower - cp.m_PointUpper;
	//Base.normalize();
	D3DXVec3Normalize(&Base, &Base);
	D3DXVECTOR3 Dest = retvec - cp.m_PointUpper;
	float dot = D3DXVec3Dot(&Base, &Dest);
	//float dot = bsm::dot(Base, Dest);
	if (dot < 0) {
		//スタート位置の球体の外側
		//retvecは有効
		flg = -1;
		return retvec;
	}
	D3DXVECTOR3 LU = cp.m_PointLower - cp.m_PointUpper;
	float  size = D3DXVec3Length(&LU);
	if (dot > size) {
		//終点より先にある
		Sp.m_Center = cp.m_PointLower;
		SPHERE_OBB(Sp, obb, retvec);
		//終点で最近接点をとる
		flg = 1;
		return retvec;
	}
	//中心とobbの最近接点を得る
	ClosestPtPointOBB(cp.GetCenter(), obb, retvec);
	float t;
	D3DXVECTOR3 SegPoint;
	ClosetPtPointSegment(retvec, cp.m_PointUpper, cp.m_PointLower, t, SegPoint);
	D3DXVECTOR3 Span = retvec - SegPoint;
	D3DXVec3Normalize(&Span, &Span);
	/*Span.normalize();*/
	Span *= cp.m_Radius;
	SegPoint += Span;
	retvec = SegPoint;
	flg = 0;
	return retvec;
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












#endif