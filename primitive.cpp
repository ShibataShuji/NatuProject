
#include "stdafx.h"

//#include "primitive.h"


//// �_�ƒ����̍ŒZ����
//// p : �_
//// l : ����
//// h : �_���牺�낵�������̑��i�߂�l�j
//// t :�x�N�g���W���i�߂�l�j
//// �߂�l: �ŒZ����
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
//// ��p1p2p3�͉s�p�H
//bool isSharpAngle(const Point& p1, const Point& p2, const Point& p3) {
//	Vec3 a(p1 - p2);
//	a.isSharpAngle(p3 - p2);
//	return a.isSharpAngle(p3 - p2);
//	//return (p1 - p2).isSharpAngle(p3 - p2);
//}
//
//// �_�Ɛ����̍ŒZ����
//// p : �_
//// seg : ����
//// h : �ŒZ�����ƂȂ�[�_�i�߂�l�j
//// t : �[�_�ʒu�i t < 0: �n�_�̊O, 0 <= t <= 1: ������, t > 1: �I�_�̊O �j
//// �߂�l: �ŒZ����
//float calcPointSegmentDist(const Point& p, const Segment& seg, Point& h, float& t) {
//
//	const Point e = seg.getEndPoint();
//
//	// �����̒����A�����̑��̍��W�y��t���Z�o
//	float len = calcPointLineDist(p, Line(seg.p, e - seg.p), h, t);
//
//	if (isSharpAngle(p, seg.p, e) == false) {
//		// �n�_���̊O��
//		h = seg.p;
//		return (seg.p - p).length();
//	}
//	else if (isSharpAngle(p, e, seg.p) == false) {
//		// �I�_���̊O��
//		h = e;
//		return (e - p).length();
//	}
//
//	return len;
//}
//
//// 2�����̍ŒZ����
//// l1 : L1
//// l2 : L2
//// p1 : L1���̐����̑��i�߂�l�j
//// p2 : L2���̐����̑��i�߂�l�j
//// t1 : L1���̃x�N�g���W���i�߂�l�j
//// t2 : L2���̃x�N�g���W���i�߂�l�j
//// �߂�l: �ŒZ����
//float calcLineLineDist(const Line& l1, const Line& l2, Point& p1, Point& p2, float& t1, float& t2) {
//
//	// 2���������s�H
//	if (l1.v.isParallel(l2.v) == true) {
//
//		// �_P11�ƒ���L2�̍ŒZ�����̖��ɋA��
//		float len = calcPointLineDist(l1.p, l2, p2, t2);
//		p1 = l1.p;
//		t1 = 0.0f;
//
//		return len;
//	}
//
//	// 2�����͂˂���֌W
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
//// 2�����̍ŒZ����
//// s1 : S1(����1)
//// s2 : S2(����2)
//// p1 : S1���̐����̑��i�߂�l�j
//// p2 : S2���̐����̑��i�߂�l�j
//// t1 : S1���̃x�N�g���W���i�߂�l�j
//// t2 : S2���̃x�N�g���W���i�߂�l�j
//// �߂�l: �ŒZ����
//float calcSegmentSegmentDist(const Segment& s1, const Segment& s2, Point& p1, Point& p2, float& t1, float& t2) {
//
//	// S1���k�ނ��Ă���H
//	if (s1.v.lengthSq() < _OX_EPSILON_) {
//		// S2���k�ށH
//		if (s2.v.lengthSq() < _OX_EPSILON_) {
//			// �_�Ɠ_�̋����̖��ɋA��
//			float len = (s2.p - s1.p).length();
//			p1 = s1.p;
//			p2 = s2.p;
//			t1 = t2 = 0.0f;
//			return len;
//		}
//		else {
//			// S1�̎n�_��S2�̍ŒZ���ɋA��
//			float len = calcPointSegmentDist(s1.p, s2, p2, t2);
//			p1 = s1.p;
//			t1 = 0.0f;
//			clamp01(t2);
//			return len;
//		}
//	}
//
//	// S2���k�ނ��Ă���H
//	else if (s2.v.lengthSq() < _OX_EPSILON_) {
//		// S2�̎n�_��S1�̍ŒZ���ɋA��
//		float len = calcPointSegmentDist(s2.p, s1, p1, t1);
//		p2 = s2.p;
//		clamp01(t1);
//		t2 = 0.0f;
//		return len;
//	}
//
//	/* �������m */
//
//	// 2���������s�������琂���̒[�_�̈��P1�ɉ�����
//	if (s1.v.isParallel(s2.v) == true) {
//		t1 = 0.0f;
//		p1 = s1.p;
//		float len = calcPointSegmentDist(s1.p, s2, p2, t2);
//		if (0.0f <= t2 && t2 <= 1.0f)
//			return len;
//	}
//	else {
//		// �����͂˂���̊֌W
//		// 2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
//		float len = calcLineLineDist(s1, s2, p1, p2, t1, t2);
//		if (
//			0.0f <= t1 && t1 <= 1.0f &&
//			0.0f <= t2 && t2 <= 1.0f
//			) {
//			return len;
//		}
//	}
//
//	// �����̑����O�ɂ��鎖������
//	// S1����t1��0�`1�̊ԂɃN�����v���Đ������~�낷
//	clamp01(t1);
//	p1 = s1.getPoint(t1);
//	float len = calcPointSegmentDist(p1, s2, p2, t2);
//	if (0.0f <= t2 && t2 <= 1.0f)
//		return len;
//
//	// S2�����O�������̂�S2�����N�����v�AS1�ɐ������~�낷
//	clamp01(t2);
//	p2 = s2.getPoint(t2);
//	len = calcPointSegmentDist(p2, s1, p1, t1);
//	if (0.0f <= t1 && t1 <= 1.0f)
//		return len;
//
//	// �o���̒[�_���ŒZ�Ɣ���
//	clamp01(t1);
//	p1 = s1.getPoint(t1);
//	return (p2 - p1).length();
//}
//
//// �J�v�Z�����m�̏Փ˔���
//// c1 : S1(����1)
//// c2 : S2(����2)
//// �߂�l: �Փ˂��Ă�����true
//bool colCapsuleCapsule(const Capsule& c1, const Capsule& c2, Point *p1, Point *p2, float *vd) {
//	//Point p1, p2;		// �g���ĂȂ��Ď̂ĂĂ����ǂ��炦��悤�ɂ���
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
//// 0�`1�̊ԂɃN�����v
//void clamp01(float& v) {
//	if (v < 0.0f)
//		v = 0.0f;
//	else if (v > 1.0f)
//		v = 1.0f;
//}
//
//
//
////// OBB �ɑ΂���_�̍Őڋߓ_
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


// ���������g���ă��[���h�}�g���N�X��0������
// GetWorldMatrix�Ƃ̈Ⴂ�͂������͈������ł��炤�̂łǂ�Ȃ̂ɂ��Ή��ł���
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
		@brief	�J�v�Z����Obb�̏Փ˔���
		@param[in]	cp	�J�v�Z��
		@param[in]	obb	OBB
		@param[out]	retvec	�ŋߐړ_
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
bool CAPSULE_OBB(CAPSULE cp, OBB obb, D3DXVECTOR3 retvec) {
	//�X�B�[�v�����鋅
	SPHERE StartSp, EndSp;
	StartSp.m_Center = cp.GetLowerSpherePos();
	StartSp.m_Radius = cp.m_Radius;
	EndSp.m_Center = cp.GetUpperSpherePos();
	EndSp.m_Radius = cp.m_Radius;
	//�e�_��obb�̍ŋߐړ_�𓾂�
	//�J�v�Z����OBB�̍ŋߐړ_�𓾂�i�Փ˂��Ă邩�ǂ����͊֌W�Ȃ��j
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
		@brief	Sphere�Ɠ����Ȃ�Obb�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
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
//	D3DXVECTOR3 Vec(0, 0, 0);   // �ŏI�I�ɒ��������߂�x�N�g��
//
//	// �e���ɂ��Ă͂ݏo�������̃x�N�g�����Z�o
//	for (int i = 0; i < 3; i++)
//	{
//		FLOAT L = obb.GetLen(i);
//		if (L <= 0) continue;  // L=0�͌v�Z�ł��Ȃ�
//		FLOAT s = D3DXVec3Dot(&(p.GetPos() - obb.GetPos()), obb.GetDirect(i)) / L;
//
//		// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
//		s = fabs(s);
//		if (s > 1)
//			Vec += (1 - s) * L * obb.GetDirect(i);   // �͂ݏo���������̃x�N�g���Z�o
//	}
//
//	return D3DXVec3Length(&Vec);   // �������o��
//}



bool colCapsuleCapsule(CAPSULE& c1, CAPSULE& c2, D3DXVECTOR3* p1, D3DXVECTOR3* p2, float* vd) 
{
	//D3DXVECTOR3 p1, p2;		// �g���ĂȂ��Ď̂ĂĂ����ǂ��炦��悤�ɂ���
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

// 2�����̍ŒZ����
// s1 : S1(����1)
// s2 : S2(����2)
// p1 : S1���̐����̑��i�߂�l�j
// p2 : S2���̐����̑��i�߂�l�j
// t1 : S1���̃x�N�g���W���i�߂�l�j
// t2 : S2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcSegmentSegmentDist(SEGMENT seg1, SEGMENT seg2, D3DXVECTOR3& p1, D3DXVECTOR3& p2, float& t1, float& t2)
{
	// pos1����pos2�ւ̃x�N�g��
	D3DXVECTOR3 seg1vec = seg1.GetVector();
	D3DXVECTOR3 seg2vec = seg2.GetVector();
	// pos1�̍��f�B�X�^���X
	D3DXVECTOR3 dist1 = seg2.m_pos1 - seg1.m_pos1;

	//D3DXVec3Length(&seg1vec);
	//D3DXVec3LengthSq(&seg1vec);

	// S1���k�ނ��Ă���H(0���Z)���W1�ƍ��W2�������ʒu(�_�ɂȂ��Ă���)
	if (D3DXVec3LengthSq(&seg1vec) < _OX_EPSILON_)
	{
		// S2���k�ށH
		if (D3DXVec3LengthSq(&seg2vec) < _OX_EPSILON_)
		{
			// �_�Ɠ_�̋����̖��ɋA��(�ǂ�������ł͂Ȃ��_�������ꍇ)
			float len = D3DXVec3Length(&dist1);
			p1 = seg1.m_pos1;
			p2 = seg2.m_pos1;
			t1 = t2 = 0.0f;
			return len;
		}
		else {
			// S1�̎n�_��S2�̍ŒZ���ɋA��(��1���_�ŁA��2�����������ꍇ)
			float len = calcPointSegmentDist(seg1.m_pos1, seg2, p2, t2);	// �|�C���g�Ɛ����̍ŒZ���������߂�
			p1 = seg1.m_pos1;
			t1 = 0.0f;
			clamp01(t2);
			return len;
		}
	}

	// S2���k�ނ��Ă���H
	else if (D3DXVec3LengthSq(&seg2vec) < _OX_EPSILON_) {
		// S2�̎n�_��S1�̍ŒZ���ɋA��
		float len = calcPointSegmentDist(seg2.m_pos1, seg1, p1, t1);
		p2 = seg2.m_pos1;
		clamp01(t1);
		t2 = 0.0f;
		return len;
	}

	/* �������m */

	// 2���������s�������琂���̒[�_�̈��P1�ɉ�����
	if (isParallel(seg1vec, seg2vec) == true) {
		t1 = 0.0f;
		p1 = seg1.m_pos1;
		float len = calcPointSegmentDist(seg1.m_pos1, seg2, p2, t2);
		if (0.0f <= t2 && t2 <= 1.0f)
			return len;
	}
	else {
		// �����͂˂���̊֌W
		// 2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
		float len = calcLineLineDist(seg1, seg2, p1, p2, t1, t2);
		if (
			0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f
			) {
			return len;
		}
	}

	// �����̑����O�ɂ��鎖������
	// S1����t1��0�`1�̊ԂɃN�����v���Đ������~�낷
	clamp01(t1);
	p1 = seg1.m_pos1 + seg1vec * t1;
	float len = calcPointSegmentDist(p1, seg2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f)
		return len;

	// S2�����O�������̂�S2�����N�����v�AS1�ɐ������~�낷
	clamp01(t2);
	p2 = seg2.m_pos1 + seg2vec * t2;
	len = calcPointSegmentDist(p2, seg1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f)
		return len;

	// �o���̒[�_���ŒZ�Ɣ���
	clamp01(t1);
	p1 = seg1.m_pos1 + seg1vec * t1;
	D3DXVECTOR3 distP1toP2 = p2 - p1;
	return D3DXVec3Length(&distP1toP2);
}


// �_�Ɛ����̍ŒZ����
// p : �_
// seg : ����
// h : �ŒZ�����ƂȂ�[�_�i�߂�l�j
// t : �[�_�ʒu�i t < 0: �n�_�̊O, 0 <= t <= 1: ������, t > 1: �I�_�̊O �j
// �߂�l: �ŒZ����
float calcPointSegmentDist(D3DXVECTOR3& point, SEGMENT& seg, D3DXVECTOR3& h, float& t) 
{
	// �����̒����A�����̑��̍��W�y��t���Z�o
	float len = calcPointLineDist(point, seg, h, t);

	if (isSharpAngleCalc3Point(point, seg.m_pos1, seg.m_pos2) == false) 
	{
		// �n�_���̊O��
		h = seg.m_pos1;
		D3DXVECTOR3 distPointtoS = seg.m_pos1 - point;
		return D3DXVec3Length(&distPointtoS);
	}
	else if (isSharpAngleCalc3Point(point, seg.m_pos2, seg.m_pos1) == false) 
	{
		// �I�_���̊O��
		h = seg.m_pos2;
		D3DXVECTOR3 distStoE = seg.m_pos2 - seg.m_pos1;

		return D3DXVec3Length(&distStoE);
	}

	return len;
}

// �_�ƒ����̍ŒZ����
// p : �_
// l : ����
// h : �_���牺�낵�������̑��i�߂�l�j
// t :�x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcPointLineDist(D3DXVECTOR3& point, SEGMENT& segment, D3DXVECTOR3& h, float& t) 
{
	// pos1����pos2�ւ̃x�N�g��
	D3DXVECTOR3 segvec = segment.GetVector();

	float lenSqV = D3DXVec3LengthSq(&segvec);
	t = 0.0f;
	// ����������Ȃ��ē_�������ꍇ
	if (lenSqV > 0.0f)
	{
		// �_�Ɠ_�̍������߂�
		D3DXVECTOR3 dist = point - segment.m_pos1;
		// �����
		float dotvalue = D3DXVec3Dot(&segvec, &dist);
		t = dotvalue / lenSqV;
	}

	h = segment.m_pos1 + t * segvec;
	D3DXVECTOR3 hpoint = h - point;
	D3DXVec3Length(&hpoint);
	return D3DXVec3Length(&hpoint);
}


// ��p1p2p3�͉s�p�H
bool isSharpAngleCalc3Point(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3)
{
	D3DXVECTOR3 a = p1 - p2;
	D3DXVECTOR3 b = p3 - p2;
	return isSharpAngleCalcDot(a, b);
}


// �s�p�֌W�H
bool isSharpAngleCalcDot(D3DXVECTOR3 a, D3DXVECTOR3& b)
{
	float returndot = D3DXVec3Dot(&a, &b);
	return returndot >= 0.0f;
}

// 0�`1�̊ԂɃN�����v
void clamp01(float& v) 
{
	if (v < 0.0f)
		v = 0.0f;
	else if (v > 1.0f)
		v = 1.0f;
}


// 2�����̍ŒZ����
// l1 : L1
// l2 : L2
// p1 : L1���̐����̑��i�߂�l�j
// p2 : L2���̐����̑��i�߂�l�j
// t1 : L1���̃x�N�g���W���i�߂�l�j
// t2 : L2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcLineLineDist(SEGMENT& l1, SEGMENT& l2, D3DXVECTOR3& p1, D3DXVECTOR3& p2, float& t1, float& t2) 
{
	D3DXVECTOR3 l1vec = l1.GetVector();
	D3DXVECTOR3 l2vec = l2.GetVector();
	// 2���������s�H
	if (isParallel(l1vec, l2vec) == true) {

		// �_P11�ƒ���L2�̍ŒZ�����̖��ɋA��
		float len = calcPointLineDist(l1.m_pos1, l2, p2, t2);
		p1 = l1.m_pos1;
		t1 = 0.0f;

		return len;
	}

	// 2�����͂˂���֌W
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


// ���s�֌W�ɂ���H
bool isParallel(D3DXVECTOR3& a, D3DXVECTOR3& b)
{
	
	D3DXVECTOR3 crossd = cross(a, b);
	float d = D3DXVec3LengthSq(&crossd);
	return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// �덷�͈͓��Ȃ畽�s�Ɣ���
}


D3DXVECTOR3 cross(D3DXVECTOR3& a, D3DXVECTOR3& b)
{
	return D3DXVECTOR3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}


// ��]��̍��W���v�Z�BOffset�̍��W���g���Č��_����ǂꂾ�����ꂽ�ʒu�ɂ��邩�����܂�
D3DXVECTOR3 CalcAfterRoatedPos(D3DXVECTOR3 beforeOffset, D3DXVECTOR3 rotation)
{
	//	���̉�]�����]�s����쐬����
	D3DXMATRIX rotmat;
	D3DXMatrixRotationYawPitchRoll(&rotmat, rotation.y, rotation.x, rotation.z);

	// ���_���W����]�s��ŉ�]�����č��W���擾����
	D3DXVECTOR3 AfterOffset;
	D3DXVec3TransformCoord(&AfterOffset, &beforeOffset, &rotmat);

	return AfterOffset;

}

D3DXVECTOR3 GetPositionForWorldMatrix(D3DXMATRIX matrix)
{
	D3DXVECTOR3 re;
	re.x = matrix._14;
	re.x = matrix._24;
	re.x = matrix._34;

	return re;
}


// source��divisor(���鐔)�̔����̑O���Ɉʒu���邩�ǂ����Btrue�őO���Afalse�Ō㔼�Ɉʒu���Ă���B
bool FindoutBeforethehalf(const float source, const float divisor)
{
	float half = divisor * 0.5f;			// ���鐔�̔���
	float surplus = fmod(source, divisor);	// �]��̐�

	// �����ŕς��̂ŁB
	if (surplus < 0)
	{
		if (fabsf(surplus) < half)
			return false;
		else
			return true;
	}
	else
	{
		if (surplus < half)
			return true;
		else
			return false;
	}

	return true;
}


D3DXVECTOR3* CalcWallScratchVector(D3DXVECTOR3* pOut_vec, const D3DXVECTOR3& front, const D3DXVECTOR3& normal)
{
	D3DXVECTOR3 normal_n;
	D3DXVec3Normalize(&normal_n, &normal);

	float a = D3DXVec3Dot(&front, &normal_n);
	D3DXVECTOR3 b = front - a * normal_n;
	D3DXVec3Normalize(pOut_vec, &b);
	return pOut_vec;
}


// raystart : ���C�̎n�_, raydirect : ���C�̕����x�N�g��, spherepos : ���̒��S�_�̍��W, spherer : ���̔��a
// q1: �ՓˊJ�n�_�i�߂�l�j, q2: �ՓˏI���_�i�߂�l�j
// ���C�Ƌ��̏Փ˔�������߂�
bool calcRaySphere(D3DXVECTOR3 raystart, D3DXVECTOR3 raydirect, D3DXVECTOR3 spherepos,
	float spherer, D3DXVECTOR3* q1, D3DXVECTOR3* q2)
{
	spherepos.x = spherepos.x - raystart.x;
	spherepos.y = spherepos.y - raystart.y;
	spherepos.z = spherepos.z - raystart.z;

	float A = raydirect.x * raydirect.x + raydirect.y * raydirect.y + raydirect.z * raydirect.z;
	float B = raydirect.x * spherepos.x + raydirect.y * spherepos.y + raydirect.z * spherepos.z;
	float C = spherepos.x * spherepos.x + spherepos.y * spherepos.y + spherepos.z * spherepos.z - spherer * spherer;

	if (A == 0.0f)
		return false; // ���C�̒�����0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // �Փ˂��Ă��Ȃ�

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // ���C�̔��΂ŏՓ�

	q1->x = raystart.x + a1 * raydirect.x;
	q1->y = raystart.y + a1 * raydirect.y;
	q1->z = raystart.z + a1 * raydirect.z;
	q2->x = raystart.x + a2 * raydirect.x;
	q2->y = raystart.y + a2 * raydirect.y;
	q2->z = raystart.z + a2 * raydirect.z;

	return true;
}



// �Q�����ł̊O�ς����߂�
double Cross2D(const D3DXVECTOR2& a, const D3DXVECTOR2& b) 
{
	return a.x * b.y - a.y * b.x;
}

// �Q�����ł̂Q�̐����ł̌�_�����߂�
bool CalcIntersectionPoint(const D3DXVECTOR2& pointA,
	const D3DXVECTOR2& pointB,
	const D3DXVECTOR2& pointC,
	const D3DXVECTOR2& pointD,
	D3DXVECTOR2& pointIntersection,
	double& dR,
	double& dS)
{
	double dBunbo = (pointB.x - pointA.x)
		* (pointD.y - pointC.y)
		- (pointB.y - pointA.y)
		* (pointD.x - pointC.x);
	if (0 == dBunbo)
	{	// ���s
		return false;
	}

	D3DXVECTOR2 vectorAC = pointC - pointA;

	dR = ((pointD.y - pointC.y) * vectorAC.x
		- (pointD.x - pointC.x) * vectorAC.y) / dBunbo;
	dS = ((pointB.y - pointA.y) * vectorAC.x
		- (pointB.x - pointA.x) * vectorAC.y) / dBunbo;

	pointIntersection = pointA + dR * (pointB - pointA);

	return true;
}

// �Q�����ł̂Q�̃x�N�g���Ԃł̊p�x�����߂�B�p�x�́A0�`�΂ŋ��܂�B
bool CalcAngle(const D3DXVECTOR2& vectorA,
	const D3DXVECTOR2& vectorB,
	double& dAngle_rad)
{
	float VecALen = D3DXVec2Length(&vectorA);
	float VecBLen = D3DXVec2Length(&vectorB);
	double dLengthAxLengthB = VecALen * VecBLen;
	if (0 == dLengthAxLengthB)
	{
		assert(!"�[���x�N�g���G���[");
		return false;
	}

	float VecDot = D3DXVec2Dot(&vectorA, &vectorB);

	double dCosAlpha = VecDot / dLengthAxLengthB;
	if (dCosAlpha > 1.0)
	{
		dCosAlpha = 1.0;
	}
	if (dCosAlpha < -1.0)
	{
		dCosAlpha = -1.0;
	}
	dAngle_rad = acos(dCosAlpha);

	return true;


	//float VecALenSq = D3DXVec2LengthSq(&vectorA);
	//float VecBLenSq = D3DXVec2LengthSq(&vectorB);
	//double dLengthAxLengthSqB = VecALenSq * VecBLenSq;
	//if (0 == dLengthAxLengthSqB)
	//{
	//	assert(!"�[���x�N�g���G���[");
	//	return false;
	//}

	//float VecDot = D3DXVec2Dot(&vectorA, &vectorB);

	//double dCosAlpha = pow(VecDot, 2) / dLengthAxLengthSqB;
	//if (dCosAlpha > 1.0)
	//{
	//	dCosAlpha = 1.0;
	//}
	//if (dCosAlpha < -1.0)
	//{
	//	dCosAlpha = -1.0;
	//}
	//dAngle_rad = acos(dCosAlpha);

	//return true;
}


// -�΁`�΁F�x�N�g���a���x�N�g���`�ɑ΂��Ď��v�������������Ă���Ƃ����̒l���Ԃ�܂��B
// -PI/2 ~ PI/2 ���Ԃ�
bool CalcAngle2(const D3DXVECTOR2& vectorA,
	const D3DXVECTOR2& vectorB,
	double& dAngle_rad)
{
	if (!CalcAngle(vectorA, vectorB, dAngle_rad))
	{
		return false;
	}

	double dOuterProduct = Cross2D(vectorA, vectorB);
	if (dOuterProduct < 0.0)
	{
		dAngle_rad = -dAngle_rad;
	}

	return true;
}

// 0�`2�΁F�x�N�g���a���x�N�g���`�ɑ΂��Ď��v�������������Ă���Ƃ��΁`2�΂̒l���Ԃ�܂��B
bool CalcAngle3(const D3DXVECTOR2& vectorA,
	const D3DXVECTOR2& vectorB,
	double& dAngle_rad)
{
	if (!CalcAngle(vectorA, vectorB, dAngle_rad))
	{
		return false;
	}

	double dOuterProduct = Cross2D(vectorA, vectorB);
	if (dOuterProduct < 0.0)
	{
		dAngle_rad = 2 * M_PI - dAngle_rad;
	}

	return true;
}


// NowValue��Correct�ɋ߂Â���BRoopEndValue��0�ȏォ�������Ő��̌v�Z�����̌v�Z���ύX�ł���B�������܂�������̂ɂ͑Ή����Ă��Ȃ�
// // �������l�B����̓��[�v�𒴂��ĂȂ����̂Ƃ���// �ǉ����Ă����l// ���݂̒l// ���[�v�̃G���h
float BringingValuesCloserFloat(const float& Correct, const float& Addition, const float& NowValue, const float& RoopEndValue)
{
	float re = NowValue;

	// RoopEndValue��0���傫�������琳�̒l�ł̌v�Z�Ƃ���
	if (RoopEndValue >= 0.0)
	{
		// ���[�v�𒴂��Ă���߂�
		if (re > RoopEndValue)
		{
			int div = NowValue / RoopEndValue;
			re -= RoopEndValue * div;
		}
		if (re < 0.0)
		{
			int div = fabsf(NowValue) / fabsf(RoopEndValue);
			re += RoopEndValue * div;
		}

		// �E��肩����肩�߂��������߂�
		if (re - Correct > 0.0f)
		{
			// �E���Ȃ̂�Addition�������Ă���
			re -= Addition;

			// 0�����ɂȂ��Ă�������Z���Ė߂�
			if (re < 0.0f)
			{
				re += RoopEndValue;
			}
		}
		else
		{
			// �����Ȃ̂�Addition�������Ă���
			re += Addition;

			// ���[�v�𒴂��Ă���߂�
			if (re > RoopEndValue)
			{
				re -= RoopEndValue;
			}
		}
	}
	else
	{
		// ���̏ꍇ�̌v�Z

		// ���[�v����菬����������߂�
		if (re < RoopEndValue)
		{
			int div = fabsf(NowValue) / fabsf(RoopEndValue);
			re -= RoopEndValue * div;
		}
		if (re > 0.0)
		{
			int div = fabsf(NowValue) / fabsf(RoopEndValue);
			re += RoopEndValue * div;
		}

		// �E��肩����肩�߂��������߂�
		if (fabsf(Correct) - fabsf(re) > 0.0 && fabsf(Correct) - fabsf(re) <= fabsf(RoopEndValue))	// fabsf(RoopEndValue) * 0.5 re - Correct > 0.0f
		{
			// �E���Ȃ̂�Addition�������Ă���
			re += Addition;

			// 0�ȏ�ɂȂ��Ă�������Z���Ė߂�
			if (re >= 0.0f)
			{
				re += RoopEndValue;
			}
		}
		else
		{
			// �����Ȃ̂�Addition�������Ă���
			re -= Addition;

			// ���[�v��������Ă�����߂�
			if (re < RoopEndValue)
			{
				re -= RoopEndValue;
			}
		}
	}


	return re;
}