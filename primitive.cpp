#include "primitive.h"


//// 点と直線の最短距離
//// p : 点
//// l : 直線
//// h : 点から下ろした垂線の足（戻り値）
//// t :ベクトル係数（戻り値）
//// 戻り値: 最短距離
//float calcPointLineDist(const Point& p, const Line& l, Point& h, float& t) {
//
//	float lenSqV = l.v.lengthSq();
//	t = 0.0f;
//	if (lenSqV > 0.0f)
//		t = l.v.dot(p - l.p) / lenSqV;
//
//	h = l.p + t * l.v;
//	return (h - p).length();
//}
//
//// ∠p1p2p3は鋭角？
//bool isSharpAngle(const Point& p1, const Point& p2, const Point& p3) {
//	Vec3 a(p1 - p2);
//	a.isSharpAngle(p3 - p2);
//	return a.isSharpAngle(p3 - p2);
//	//return (p1 - p2).isSharpAngle(p3 - p2);
//}
//
//// 点と線分の最短距離
//// p : 点
//// seg : 線分
//// h : 最短距離となる端点（戻り値）
//// t : 端点位置（ t < 0: 始点の外, 0 <= t <= 1: 線分内, t > 1: 終点の外 ）
//// 戻り値: 最短距離
//float calcPointSegmentDist(const Point& p, const Segment& seg, Point& h, float& t) {
//
//	const Point e = seg.getEndPoint();
//
//	// 垂線の長さ、垂線の足の座標及びtを算出
//	float len = calcPointLineDist(p, Line(seg.p, e - seg.p), h, t);
//
//	if (isSharpAngle(p, seg.p, e) == false) {
//		// 始点側の外側
//		h = seg.p;
//		return (seg.p - p).length();
//	}
//	else if (isSharpAngle(p, e, seg.p) == false) {
//		// 終点側の外側
//		h = e;
//		return (e - p).length();
//	}
//
//	return len;
//}
//
//// 2直線の最短距離
//// l1 : L1
//// l2 : L2
//// p1 : L1側の垂線の足（戻り値）
//// p2 : L2側の垂線の足（戻り値）
//// t1 : L1側のベクトル係数（戻り値）
//// t2 : L2側のベクトル係数（戻り値）
//// 戻り値: 最短距離
//float calcLineLineDist(const Line& l1, const Line& l2, Point& p1, Point& p2, float& t1, float& t2) {
//
//	// 2直線が平行？
//	if (l1.v.isParallel(l2.v) == true) {
//
//		// 点P11と直線L2の最短距離の問題に帰着
//		float len = calcPointLineDist(l1.p, l2, p2, t2);
//		p1 = l1.p;
//		t1 = 0.0f;
//
//		return len;
//	}
//
//	// 2直線はねじれ関係
//	float DV1V2 = l1.v.dot(l2.v);
//	float DV1V1 = l1.v.lengthSq();
//	float DV2V2 = l2.v.lengthSq();
//	Vec3 P21P11 = l1.p - l2.p;
//	t1 = (DV1V2 * l2.v.dot(P21P11) - DV2V2 * l1.v.dot(P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
//	p1 = l1.getPoint(t1);
//	t2 = l2.v.dot(p1 - l2.p) / DV2V2;
//	p2 = l2.getPoint(t2);
//
//	return (p2 - p1).length();
//}
//
//// 2線分の最短距離
//// s1 : S1(線分1)
//// s2 : S2(線分2)
//// p1 : S1側の垂線の足（戻り値）
//// p2 : S2側の垂線の足（戻り値）
//// t1 : S1側のベクトル係数（戻り値）
//// t2 : S2側のベクトル係数（戻り値）
//// 戻り値: 最短距離
//float calcSegmentSegmentDist(const Segment& s1, const Segment& s2, Point& p1, Point& p2, float& t1, float& t2) {
//
//	// S1が縮退している？
//	if (s1.v.lengthSq() < _OX_EPSILON_) {
//		// S2も縮退？
//		if (s2.v.lengthSq() < _OX_EPSILON_) {
//			// 点と点の距離の問題に帰着
//			float len = (s2.p - s1.p).length();
//			p1 = s1.p;
//			p2 = s2.p;
//			t1 = t2 = 0.0f;
//			return len;
//		}
//		else {
//			// S1の始点とS2の最短問題に帰着
//			float len = calcPointSegmentDist(s1.p, s2, p2, t2);
//			p1 = s1.p;
//			t1 = 0.0f;
//			clamp01(t2);
//			return len;
//		}
//	}
//
//	// S2が縮退している？
//	else if (s2.v.lengthSq() < _OX_EPSILON_) {
//		// S2の始点とS1の最短問題に帰着
//		float len = calcPointSegmentDist(s2.p, s1, p1, t1);
//		p2 = s2.p;
//		clamp01(t1);
//		t2 = 0.0f;
//		return len;
//	}
//
//	/* 線分同士 */
//
//	// 2線分が平行だったら垂線の端点の一つをP1に仮決定
//	if (s1.v.isParallel(s2.v) == true) {
//		t1 = 0.0f;
//		p1 = s1.p;
//		float len = calcPointSegmentDist(s1.p, s2, p2, t2);
//		if (0.0f <= t2 && t2 <= 1.0f)
//			return len;
//	}
//	else {
//		// 線分はねじれの関係
//		// 2直線間の最短距離を求めて仮のt1,t2を求める
//		float len = calcLineLineDist(s1, s2, p1, p2, t1, t2);
//		if (
//			0.0f <= t1 && t1 <= 1.0f &&
//			0.0f <= t2 && t2 <= 1.0f
//			) {
//			return len;
//		}
//	}
//
//	// 垂線の足が外にある事が判明
//	// S1側のt1を0〜1の間にクランプして垂線を降ろす
//	clamp01(t1);
//	p1 = s1.getPoint(t1);
//	float len = calcPointSegmentDist(p1, s2, p2, t2);
//	if (0.0f <= t2 && t2 <= 1.0f)
//		return len;
//
//	// S2側が外だったのでS2側をクランプ、S1に垂線を降ろす
//	clamp01(t2);
//	p2 = s2.getPoint(t2);
//	len = calcPointSegmentDist(p2, s1, p1, t1);
//	if (0.0f <= t1 && t1 <= 1.0f)
//		return len;
//
//	// 双方の端点が最短と判明
//	clamp01(t1);
//	p1 = s1.getPoint(t1);
//	return (p2 - p1).length();
//}
//
//// カプセル同士の衝突判定
//// c1 : S1(線分1)
//// c2 : S2(線分2)
//// 戻り値: 衝突していたらtrue
//bool colCapsuleCapsule(const Capsule& c1, const Capsule& c2, Point *p1, Point *p2, float *vd) {
//	//Point p1, p2;		// 使ってなくて捨ててたけどもらえるようにした
//	float t1, t2;
//	float d = calcSegmentSegmentDist(c1.s, c2.s, *p1, *p2, t1, t2);
//
//	if (d <= c1.r + c2.r)
//	{
//		*vd = c1.r + c2.r - d;
//	}
//	else
//	{
//		*vd = 0.0f;
//	}
//	return (d <= c1.r + c2.r);
//}
//
//// 0〜1の間にクランプ
//void clamp01(float& v) {
//	if (v < 0.0f)
//		v = 0.0f;
//	else if (v > 1.0f)
//		v = 1.0f;
//}
//
//
//
////// OBB に対する点の最接近点
////void ClosestPtPointOBB(Point* p, OBB* b, Point* q)
////{
////	Point d = *p - b->c;
////	*q = b->c;
////	float dist;
////	for (int i = 0; i < 3; i++)
////	{
////		//dist = D3DXVec3Dot(&d, &b->u[i]);
////		dist = d.dot(b->u[i]);
////
////		if (dist > b->e.x)
////		{
////			dist = b->e[i];
////		}
////		if (dist < -b->e[i])
////		{
////			dist = -b->e[i];
////		}
////		*q += dist * b->u[i];
////	}
////}


// 引き数を使ってワールドマトリクスを0から作る
// GetWorldMatrixとの違いはこっちは引き数でもらうのでどんなのにも対応できる
D3DXMATRIX CreateWorldMatrix(D3DXVECTOR3 Scale, D3DXVECTOR3 Rotation, D3DXVECTOR3 Position)
{
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
	D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
	world = scale * rot * trans;
	return world;
}


//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルとObbの衝突判定
		@param[in]	cp	カプセル
		@param[in]	obb	OBB
		@param[out]	retvec	最近接点
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
bool CAPSULE_OBB(CAPSULE cp, OBB obb, D3DXVECTOR3 retvec) {
	//スィープさせる球
	SPHERE StartSp, EndSp;
	StartSp.m_Center = cp.GetLowerSpherePos();
	StartSp.m_Radius = cp.m_Radius;
	EndSp.m_Center = cp.GetUpperSpherePos();
	EndSp.m_Radius = cp.m_Radius;
	//各点とobbの最近接点を得る
	//カプセルとOBBの最近接点を得る（衝突してるかどうかは関係ない）
	int flg;
	retvec = ClosestPtCapsuleOBB(cp, obb, flg);
	float HitTime;
	D3DXVECTOR3 Velocity = EndSp.m_Center - StartSp.m_Center;
	if (CollisionTestSphereObb(StartSp, Velocity, obb, 0, 1.0f, HitTime)) {
		return true;
	}
	return false;
}


//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphereと動かないObbの衝突判定
		@param[in]	SrcSp	Srcの球
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestObb	DestのOBB
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
static bool CollisionTestSphereObb(const SPHERE& SrcSp, const D3DXVECTOR3& SrcVelocity,
	const OBB& DestObb,
	float StartTime, float EndTime, float& HitTime) {
	const float m_EPSILON = 0.005f;
	SPHERE SrcSp2;
	float mid = (StartTime + EndTime) * 0.5f;
	SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
	SrcSp2.m_Radius = (mid - StartTime) * D3DXVec3Length(&SrcVelocity) + SrcSp.m_Radius;
	D3DXVECTOR3 RetVec;
	if (!SPHERE_OBB(SrcSp2, DestObb, RetVec)) {
		return false;
	}
	if (EndTime - StartTime < m_EPSILON) {
		HitTime = StartTime;
		return true;
	}
	if (CollisionTestSphereObb(SrcSp, SrcVelocity, DestObb, StartTime, mid, HitTime)) {
		return true;
	}
	return CollisionTestSphereObb(SrcSp, SrcVelocity, DestObb, mid, EndTime, HitTime);
}








//FLOAT LenOBBToPoint(OBB& obb, Point& p)
//{
//	D3DXVECTOR3 Vec(0, 0, 0);   // 最終的に長さを求めるベクトル
//
//	// 各軸についてはみ出た部分のベクトルを算出
//	for (int i = 0; i < 3; i++)
//	{
//		FLOAT L = obb.GetLen(i);
//		if (L <= 0) continue;  // L=0は計算できない
//		FLOAT s = D3DXVec3Dot(&(p.GetPos() - obb.GetPos()), obb.GetDirect(i)) / L;
//
//		// sの値から、はみ出した部分があればそのベクトルを加算
//		s = fabs(s);
//		if (s > 1)
//			Vec += (1 - s) * L * obb.GetDirect(i);   // はみ出した部分のベクトル算出
//	}
//
//	return D3DXVec3Length(&Vec);   // 長さを出力
//}



bool colCapsuleCapsule(CAPSULE& c1, CAPSULE& c2, D3DXVECTOR3* p1, D3DXVECTOR3* p2, float* vd) 
{
	//D3DXVECTOR3 p1, p2;		// 使ってなくて捨ててたけどもらえるようにした
	float t1, t2;
	SEGMENT cap1seg;
	cap1seg.m_pos1 = c1.GetUpperSpherePos();
	cap1seg.m_pos2 = c1.GetLowerSpherePos();
	SEGMENT cap2seg;
	cap2seg.m_pos1 = c2.GetUpperSpherePos();
	cap2seg.m_pos2 = c2.GetLowerSpherePos();
	float d = calcSegmentSegmentDist(cap1seg, cap2seg, *p1, *p2, t1, t2);

	if (d <= c1.m_Radius + c2.m_Radius)
	{
		*vd = c1.m_Radius + c2.m_Radius - d;
	}
	else
	{
		*vd = 0.0f;
	}
	return (d <= c1.m_Radius + c2.m_Radius);
}

// 2線分の最短距離
// s1 : S1(線分1)
// s2 : S2(線分2)
// p1 : S1側の垂線の足（戻り値）
// p2 : S2側の垂線の足（戻り値）
// t1 : S1側のベクトル係数（戻り値）
// t2 : S2側のベクトル係数（戻り値）
// 戻り値: 最短距離
float calcSegmentSegmentDist(SEGMENT seg1, SEGMENT seg2, D3DXVECTOR3& p1, D3DXVECTOR3& p2, float& t1, float& t2)
{
	// pos1からpos2へのベクトル
	D3DXVECTOR3 seg1vec = seg1.GetVector();
	D3DXVECTOR3 seg2vec = seg2.GetVector();
	// pos1の差ディスタンス
	D3DXVECTOR3 dist1 = seg2.m_pos1 - seg1.m_pos1;

	//D3DXVec3Length(&seg1vec);
	//D3DXVec3LengthSq(&seg1vec);

	// S1が縮退している？(0除算)座標1と座標2が同じ位置(点になっている)
	if (D3DXVec3LengthSq(&seg1vec) < _OX_EPSILON_)
	{
		// S2も縮退？
		if (D3DXVec3LengthSq(&seg2vec) < _OX_EPSILON_)
		{
			// 点と点の距離の問題に帰着(どちらも線ではなく点だった場合)
			float len = D3DXVec3Length(&dist1);
			p1 = seg1.m_pos1;
			p2 = seg2.m_pos1;
			t1 = t2 = 0.0f;
			return len;
		}
		else {
			// S1の始点とS2の最短問題に帰着(線1が点で、線2が線だった場合)
			float len = calcPointSegmentDist(seg1.m_pos1, seg2, p2, t2);	// ポイントと線分の最短距離を求める
			p1 = seg1.m_pos1;
			t1 = 0.0f;
			clamp01(t2);
			return len;
		}
	}

	// S2が縮退している？
	else if (D3DXVec3LengthSq(&seg2vec) < _OX_EPSILON_) {
		// S2の始点とS1の最短問題に帰着
		float len = calcPointSegmentDist(seg2.m_pos1, seg1, p1, t1);
		p2 = seg2.m_pos1;
		clamp01(t1);
		t2 = 0.0f;
		return len;
	}

	/* 線分同士 */

	// 2線分が平行だったら垂線の端点の一つをP1に仮決定
	if (isParallel(seg1vec, seg2vec) == true) {
		t1 = 0.0f;
		p1 = seg1.m_pos1;
		float len = calcPointSegmentDist(seg1.m_pos1, seg2, p2, t2);
		if (0.0f <= t2 && t2 <= 1.0f)
			return len;
	}
	else {
		// 線分はねじれの関係
		// 2直線間の最短距離を求めて仮のt1,t2を求める
		float len = calcLineLineDist(seg1, seg2, p1, p2, t1, t2);
		if (
			0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f
			) {
			return len;
		}
	}

	// 垂線の足が外にある事が判明
	// S1側のt1を0〜1の間にクランプして垂線を降ろす
	clamp01(t1);
	p1 = seg1.m_pos1 + seg1vec * t1;
	float len = calcPointSegmentDist(p1, seg2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f)
		return len;

	// S2側が外だったのでS2側をクランプ、S1に垂線を降ろす
	clamp01(t2);
	p2 = seg2.m_pos1 + seg2vec * t2;
	len = calcPointSegmentDist(p2, seg1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f)
		return len;

	// 双方の端点が最短と判明
	clamp01(t1);
	p1 = seg1.m_pos1 + seg1vec * t1;
	D3DXVECTOR3 distP1toP2 = p2 - p1;
	return D3DXVec3Length(&distP1toP2);
}


// 点と線分の最短距離
// p : 点
// seg : 線分
// h : 最短距離となる端点（戻り値）
// t : 端点位置（ t < 0: 始点の外, 0 <= t <= 1: 線分内, t > 1: 終点の外 ）
// 戻り値: 最短距離
float calcPointSegmentDist(D3DXVECTOR3& point, SEGMENT& seg, D3DXVECTOR3& h, float& t) 
{
	// 垂線の長さ、垂線の足の座標及びtを算出
	float len = calcPointLineDist(point, seg, h, t);

	if (isSharpAngleCalc3Point(point, seg.m_pos1, seg.m_pos2) == false) 
	{
		// 始点側の外側
		h = seg.m_pos1;
		D3DXVECTOR3 distPointtoS = seg.m_pos1 - point;
		return D3DXVec3Length(&distPointtoS);
	}
	else if (isSharpAngleCalc3Point(point, seg.m_pos2, seg.m_pos1) == false) 
	{
		// 終点側の外側
		h = seg.m_pos2;
		D3DXVECTOR3 distStoE = seg.m_pos2 - seg.m_pos1;

		return D3DXVec3Length(&distStoE);
	}

	return len;
}

// 点と直線の最短距離
// p : 点
// l : 直線
// h : 点から下ろした垂線の足（戻り値）
// t :ベクトル係数（戻り値）
// 戻り値: 最短距離
float calcPointLineDist(D3DXVECTOR3& point, SEGMENT& segment, D3DXVECTOR3& h, float& t) 
{
	// pos1からpos2へのベクトル
	D3DXVECTOR3 segvec = segment.GetVector();

	float lenSqV = D3DXVec3LengthSq(&segvec);
	t = 0.0f;
	// 線が線じゃなくて点だった場合
	if (lenSqV > 0.0f)
	{
		// 点と点の差を求める
		D3DXVECTOR3 dist = point - segment.m_pos1;
		// それの
		float dotvalue = D3DXVec3Dot(&segvec, &dist);
		t = dotvalue / lenSqV;
	}

	h = segment.m_pos1 + t * segvec;
	D3DXVECTOR3 hpoint = h - point;
	D3DXVec3Length(&hpoint);
	return D3DXVec3Length(&hpoint);
}


// ∠p1p2p3は鋭角？
bool isSharpAngleCalc3Point(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3)
{
	D3DXVECTOR3 a = p1 - p2;
	D3DXVECTOR3 b = p3 - p2;
	return isSharpAngleCalcDot(a, b);
}


// 鋭角関係？
bool isSharpAngleCalcDot(D3DXVECTOR3 a, D3DXVECTOR3& b)
{
	float returndot = D3DXVec3Dot(&a, &b);
	return returndot >= 0.0f;
}

// 0〜1の間にクランプ
void clamp01(float& v) 
{
	if (v < 0.0f)
		v = 0.0f;
	else if (v > 1.0f)
		v = 1.0f;
}


// 2直線の最短距離
// l1 : L1
// l2 : L2
// p1 : L1側の垂線の足（戻り値）
// p2 : L2側の垂線の足（戻り値）
// t1 : L1側のベクトル係数（戻り値）
// t2 : L2側のベクトル係数（戻り値）
// 戻り値: 最短距離
float calcLineLineDist(SEGMENT& l1, SEGMENT& l2, D3DXVECTOR3& p1, D3DXVECTOR3& p2, float& t1, float& t2) 
{
	D3DXVECTOR3 l1vec = l1.GetVector();
	D3DXVECTOR3 l2vec = l2.GetVector();
	// 2直線が平行？
	if (isParallel(l1vec, l2vec) == true) {

		// 点P11と直線L2の最短距離の問題に帰着
		float len = calcPointLineDist(l1.m_pos1, l2, p2, t2);
		p1 = l1.m_pos1;
		t1 = 0.0f;

		return len;
	}

	// 2直線はねじれ関係
	float DV1V2 = D3DXVec3Dot(&l1vec, &l2vec);
	float DV1V1 = D3DXVec3LengthSq(&l1vec);
	float DV2V2 = D3DXVec3LengthSq(&l2vec);
	D3DXVECTOR3 P21P11 = l1.m_pos1 - l2.m_pos1;
	float a = D3DXVec3Dot(&l2vec, &P21P11);
	float b = D3DXVec3Dot(&l1vec, &P21P11);
	t1 = (DV1V2 * a - DV2V2 * b) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
	p1 = l1.m_pos1 + l1vec * t1;
	D3DXVECTOR3 dotb = p1 - l2.m_pos1;
	t2 = D3DXVec3Dot(&l2vec, &dotb) / DV2V2;
	p2 = l2.m_pos1 + l2vec * t2;

	D3DXVECTOR3 tLength = p2 - p1;
	return D3DXVec3Length(&tLength);
}


// 平行関係にある？
bool isParallel(D3DXVECTOR3& a, D3DXVECTOR3& b)
{
	
	D3DXVECTOR3 crossd = cross(a, b);
	float d = D3DXVec3LengthSq(&crossd);
	return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// 誤差範囲内なら平行と判定
}


D3DXVECTOR3 cross(D3DXVECTOR3& a, D3DXVECTOR3& b)
{
	return D3DXVECTOR3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}