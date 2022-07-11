#include "primitive.h"
// �_�ƒ����̍ŒZ����
// p : �_
// l : ����
// h : �_���牺�낵�������̑��i�߂�l�j
// t :�x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcPointLineDist(const Point& p, const Line& l, Point& h, float& t) {

	float lenSqV = l.v.lengthSq();
	t = 0.0f;
	if (lenSqV > 0.0f)
		t = l.v.dot(p - l.p) / lenSqV;

	h = l.p + t * l.v;
	return (h - p).length();
}

// ��p1p2p3�͉s�p�H
bool isSharpAngle(const Point& p1, const Point& p2, const Point& p3) {
	Vec3 a(p1 - p2);
	a.isSharpAngle(p3 - p2);
	return a.isSharpAngle(p3 - p2);
	//return (p1 - p2).isSharpAngle(p3 - p2);
}

// �_�Ɛ����̍ŒZ����
// p : �_
// seg : ����
// h : �ŒZ�����ƂȂ�[�_�i�߂�l�j
// t : �[�_�ʒu�i t < 0: �n�_�̊O, 0 <= t <= 1: ������, t > 1: �I�_�̊O �j
// �߂�l: �ŒZ����
float calcPointSegmentDist(const Point& p, const Segment& seg, Point& h, float& t) {

	const Point e = seg.getEndPoint();

	// �����̒����A�����̑��̍��W�y��t���Z�o
	float len = calcPointLineDist(p, Line(seg.p, e - seg.p), h, t);

	if (isSharpAngle(p, seg.p, e) == false) {
		// �n�_���̊O��
		h = seg.p;
		return (seg.p - p).length();
	}
	else if (isSharpAngle(p, e, seg.p) == false) {
		// �I�_���̊O��
		h = e;
		return (e - p).length();
	}

	return len;
}

// 2�����̍ŒZ����
// l1 : L1
// l2 : L2
// p1 : L1���̐����̑��i�߂�l�j
// p2 : L2���̐����̑��i�߂�l�j
// t1 : L1���̃x�N�g���W���i�߂�l�j
// t2 : L2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcLineLineDist(const Line& l1, const Line& l2, Point& p1, Point& p2, float& t1, float& t2) {

	// 2���������s�H
	if (l1.v.isParallel(l2.v) == true) {

		// �_P11�ƒ���L2�̍ŒZ�����̖��ɋA��
		float len = calcPointLineDist(l1.p, l2, p2, t2);
		p1 = l1.p;
		t1 = 0.0f;

		return len;
	}

	// 2�����͂˂���֌W
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

// 2�����̍ŒZ����
// s1 : S1(����1)
// s2 : S2(����2)
// p1 : S1���̐����̑��i�߂�l�j
// p2 : S2���̐����̑��i�߂�l�j
// t1 : S1���̃x�N�g���W���i�߂�l�j
// t2 : S2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcSegmentSegmentDist(const Segment& s1, const Segment& s2, Point& p1, Point& p2, float& t1, float& t2) {

	// S1���k�ނ��Ă���H
	if (s1.v.lengthSq() < _OX_EPSILON_) {
		// S2���k�ށH
		if (s2.v.lengthSq() < _OX_EPSILON_) {
			// �_�Ɠ_�̋����̖��ɋA��
			float len = (s2.p - s1.p).length();
			p1 = s1.p;
			p2 = s2.p;
			t1 = t2 = 0.0f;
			return len;
		}
		else {
			// S1�̎n�_��S2�̍ŒZ���ɋA��
			float len = calcPointSegmentDist(s1.p, s2, p2, t2);
			p1 = s1.p;
			t1 = 0.0f;
			clamp01(t2);
			return len;
		}
	}

	// S2���k�ނ��Ă���H
	else if (s2.v.lengthSq() < _OX_EPSILON_) {
		// S2�̎n�_��S1�̍ŒZ���ɋA��
		float len = calcPointSegmentDist(s2.p, s1, p1, t1);
		p2 = s2.p;
		clamp01(t1);
		t2 = 0.0f;
		return len;
	}

	/* �������m */

	// 2���������s�������琂���̒[�_�̈��P1�ɉ�����
	if (s1.v.isParallel(s2.v) == true) {
		t1 = 0.0f;
		p1 = s1.p;
		float len = calcPointSegmentDist(s1.p, s2, p2, t2);
		if (0.0f <= t2 && t2 <= 1.0f)
			return len;
	}
	else {
		// �����͂˂���̊֌W
		// 2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
		float len = calcLineLineDist(s1, s2, p1, p2, t1, t2);
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
	p1 = s1.getPoint(t1);
	float len = calcPointSegmentDist(p1, s2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f)
		return len;

	// S2�����O�������̂�S2�����N�����v�AS1�ɐ������~�낷
	clamp01(t2);
	p2 = s2.getPoint(t2);
	len = calcPointSegmentDist(p2, s1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f)
		return len;

	// �o���̒[�_���ŒZ�Ɣ���
	clamp01(t1);
	p1 = s1.getPoint(t1);
	return (p2 - p1).length();
}

// �J�v�Z�����m�̏Փ˔���
// c1 : S1(����1)
// c2 : S2(����2)
// �߂�l: �Փ˂��Ă�����true
bool colCapsuleCapsule(const Capsule& c1, const Capsule& c2, Point *p1, Point *p2, float *vd) {
	//Point p1, p2;		// �g���ĂȂ��Ď̂ĂĂ����ǂ��炦��悤�ɂ���
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

// 0�`1�̊ԂɃN�����v
void clamp01(float& v) {
	if (v < 0.0f)
		v = 0.0f;
	else if (v > 1.0f)
		v = 1.0f;
}
