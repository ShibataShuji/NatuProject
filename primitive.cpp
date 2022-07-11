#include "primitive.h"
// 点と直線の最短距離
// p : 点
// l : 直線
// h : 点から下ろした垂線の足（戻り値）
// t :ベクトル係数（戻り値）
// 戻り値: 最短距離
float calcPointLineDist(const Point& p, const Line& l, Point& h, float& t) {

	float lenSqV = l.v.lengthSq();
	t = 0.0f;
	if (lenSqV > 0.0f)
		t = l.v.dot(p - l.p) / lenSqV;

	h = l.p + t * l.v;
	return (h - p).length();
}

// ∠p1p2p3は鋭角？
bool isSharpAngle(const Point& p1, const Point& p2, const Point& p3) {
	Vec3 a(p1 - p2);
	a.isSharpAngle(p3 - p2);
	return a.isSharpAngle(p3 - p2);
	//return (p1 - p2).isSharpAngle(p3 - p2);
}

// 点と線分の最短距離
// p : 点
// seg : 線分
// h : 最短距離となる端点（戻り値）
// t : 端点位置（ t < 0: 始点の外, 0 <= t <= 1: 線分内, t > 1: 終点の外 ）
// 戻り値: 最短距離
float calcPointSegmentDist(const Point& p, const Segment& seg, Point& h, float& t) {

	const Point e = seg.getEndPoint();

	// 垂線の長さ、垂線の足の座標及びtを算出
	float len = calcPointLineDist(p, Line(seg.p, e - seg.p), h, t);

	if (isSharpAngle(p, seg.p, e) == false) {
		// 始点側の外側
		h = seg.p;
		return (seg.p - p).length();
	}
	else if (isSharpAngle(p, e, seg.p) == false) {
		// 終点側の外側
		h = e;
		return (e - p).length();
	}

	return len;
}

// 2直線の最短距離
// l1 : L1
// l2 : L2
// p1 : L1側の垂線の足（戻り値）
// p2 : L2側の垂線の足（戻り値）
// t1 : L1側のベクトル係数（戻り値）
// t2 : L2側のベクトル係数（戻り値）
// 戻り値: 最短距離
float calcLineLineDist(const Line& l1, const Line& l2, Point& p1, Point& p2, float& t1, float& t2) {

	// 2直線が平行？
	if (l1.v.isParallel(l2.v) == true) {

		// 点P11と直線L2の最短距離の問題に帰着
		float len = calcPointLineDist(l1.p, l2, p2, t2);
		p1 = l1.p;
		t1 = 0.0f;

		return len;
	}

	// 2直線はねじれ関係
	float DV1V2 = l1.v.dot(l2.v);
	float DV1V1 = l1.v.lengthSq();
	float DV2V2 = l2.v.lengthSq();
	Vec3 P21P11 = l1.p - l2.p;
	t1 = (DV1V2 * l2.v.dot(P21P11) - DV2V2 * l1.v.dot(P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
	p1 = l1.getPoint(t1);
	t2 = l2.v.dot(p1 - l2.p) / DV2V2;
	p2 = l2.getPoint(t2);

	return (p2 - p1).length();
}

// 2線分の最短距離
// s1 : S1(線分1)
// s2 : S2(線分2)
// p1 : S1側の垂線の足（戻り値）
// p2 : S2側の垂線の足（戻り値）
// t1 : S1側のベクトル係数（戻り値）
// t2 : S2側のベクトル係数（戻り値）
// 戻り値: 最短距離
float calcSegmentSegmentDist(const Segment& s1, const Segment& s2, Point& p1, Point& p2, float& t1, float& t2) {

	// S1が縮退している？
	if (s1.v.lengthSq() < _OX_EPSILON_) {
		// S2も縮退？
		if (s2.v.lengthSq() < _OX_EPSILON_) {
			// 点と点の距離の問題に帰着
			float len = (s2.p - s1.p).length();
			p1 = s1.p;
			p2 = s2.p;
			t1 = t2 = 0.0f;
			return len;
		}
		else {
			// S1の始点とS2の最短問題に帰着
			float len = calcPointSegmentDist(s1.p, s2, p2, t2);
			p1 = s1.p;
			t1 = 0.0f;
			clamp01(t2);
			return len;
		}
	}

	// S2が縮退している？
	else if (s2.v.lengthSq() < _OX_EPSILON_) {
		// S2の始点とS1の最短問題に帰着
		float len = calcPointSegmentDist(s2.p, s1, p1, t1);
		p2 = s2.p;
		clamp01(t1);
		t2 = 0.0f;
		return len;
	}

	/* 線分同士 */

	// 2線分が平行だったら垂線の端点の一つをP1に仮決定
	if (s1.v.isParallel(s2.v) == true) {
		t1 = 0.0f;
		p1 = s1.p;
		float len = calcPointSegmentDist(s1.p, s2, p2, t2);
		if (0.0f <= t2 && t2 <= 1.0f)
			return len;
	}
	else {
		// 線分はねじれの関係
		// 2直線間の最短距離を求めて仮のt1,t2を求める
		float len = calcLineLineDist(s1, s2, p1, p2, t1, t2);
		if (
			0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f
			) {
			return len;
		}
	}

	// 垂線の足が外にある事が判明
	// S1側のt1を0～1の間にクランプして垂線を降ろす
	clamp01(t1);
	p1 = s1.getPoint(t1);
	float len = calcPointSegmentDist(p1, s2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f)
		return len;

	// S2側が外だったのでS2側をクランプ、S1に垂線を降ろす
	clamp01(t2);
	p2 = s2.getPoint(t2);
	len = calcPointSegmentDist(p2, s1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f)
		return len;

	// 双方の端点が最短と判明
	clamp01(t1);
	p1 = s1.getPoint(t1);
	return (p2 - p1).length();
}

// カプセル同士の衝突判定
// c1 : S1(線分1)
// c2 : S2(線分2)
// 戻り値: 衝突していたらtrue
bool colCapsuleCapsule(const Capsule& c1, const Capsule& c2, Point *p1, Point *p2, float *vd) {
	//Point p1, p2;		// 使ってなくて捨ててたけどもらえるようにした
	float t1, t2;
	float d = calcSegmentSegmentDist(c1.s, c2.s, *p1, *p2, t1, t2);

	if (d <= c1.r + c2.r)
	{
		*vd = c1.r + c2.r - d;
	}
	else
	{
		*vd = 0.0f;
	}
	return (d <= c1.r + c2.r);
}

// 0～1の間にクランプ
void clamp01(float& v) {
	if (v < 0.0f)
		v = 0.0f;
	else if (v > 1.0f)
		v = 1.0f;
}
