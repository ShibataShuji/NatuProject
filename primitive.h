#pragma once

#ifndef __IKD_OX_PRIMITIVE_H__
#define __IKD_OX_PRIMITIVE_H__


// �v���~�e�B�u��`

// v1.00
//  ���o

// v1.01
// �x�N�g���̕��s������O�ςɒu������


#include <math.h>

#define _OX_EPSILON_	0.000001f	// �덷


// 3����float
struct Float3 {
	float x, y, z;

	Float3() {}
	Float3(float x, float y, float z) : x(x), y(y), z(z) {}
	~Float3() {}

	Float3 operator +(const Float3& r) const {
		return Float3(x + r.x, y + r.y, z + r.z);
	}

	Float3 operator -(const Float3& r) const {
		return Float3(x - r.x, y - r.y, z - r.z);
	}

	Float3 operator -() const {
		return Float3(x * -1.0f, y * -1.0f, z * -1.0f);
	}

	Float3 operator *(const Float3& r) const {
		return Float3(x * r.x, y * r.y, z * r.z);
	}

	Float3 operator /(const Float3& r) const {
		return Float3(x / r.x, y / r.y, z / r.z);
	}

	Float3 operator *(float r) const {
		return Float3(x * r, y * r, z * r);
	}

	Float3 operator /(float r) const {
		return Float3(x / r, y / r, z / r);
	}

	friend Float3 operator *(float l, const Float3& r) {
		return Float3(r.x * l, r.y * l, r.z * l);
	}

	friend Float3 operator /(float l, const Float3& r) {
		return Float3(r.x / l, r.y / l, r.z / l);
	}

	float dot(const Float3& r) const {
		return x * r.x + y * r.y + z * r.z;
	}

	Float3 cross(const Float3& r) const {
		return Float3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
	}

	float length() const {
		return sqrtf(lengthSq());
	}

	float lengthSq() const {
		return x * x + y * y + z * z;
	}

	void norm() {
		const float len = length();
		if (len > 0.0f) {
			x /= len;
			y /= len;
			z /= len;
		}
	}

	Float3 getNorm() const {
		const float len = length();
		if (len > 0.0f) {
			return Float3(x / len, y / len, z / len);
		}
		return Float3(0.0f, 0.0f, 0.0f);
	}


	//// �s�p�֌W�H
	//bool isSharpAngle(const Vec3& r) const {
	//	return (dot(r) >= 0.0f);
	//}

};


// �_
typedef Float3 Point;


// �x�N�g��
struct Vec3 : public Float3 {
	Vec3() {}
	Vec3(float x, float y, float z) : Float3(x, y, z) {}
	Vec3(const Float3& r) : Float3(r) {}
	~Vec3() {}

	Vec3& operator =(const Float3& r) {
		x = r.x;
		y = r.y;
		z = r.z;
		return *this;
	}

	// �W����
	void norm() {
		const float len = length();
		if (len > 0.0f) {
			x /= len;
			y /= len;
			z /= len;
		}
	}

	// �����֌W�ɂ���H
	bool isVertical(const Vec3& r) const {
		float d = dot(r);
		return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// �덷�͈͓��Ȃ琂���Ɣ���
	}

	// ���s�֌W�ɂ���H
	bool isParallel(const Vec3& r) const {
		float d = cross(r).lengthSq();
		return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// �덷�͈͓��Ȃ畽�s�Ɣ���
	}

	// �s�p�֌W�H
	bool isSharpAngle(const Vec3& r) const {
		return (dot(r) >= 0.0f);
	}


};


// ����
struct Line {
	Point p;
	Vec3 v;		// �����x�N�g��
	Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
	Line(const Point& p, const Vec3& v) : p(p), v(v) {}
	~Line() {}

	// �_��̍��W���擾
	//  �x�N�g���Ɋ|���Z����W��
	Point getPoint(float t) const {
		return p + t * v;
	}


	// 1�ڂ̏�̓_���Q�b�g����
	Point GetPoint1() const {
		return p;
	}
	// 1�ڂ̏�̓_���Q�b�g����
	Point GetPoint2() const {
		return p + v;
	}
};


// ����
struct Segment : public Line {

	Segment() {}
	Segment(const Point& p, const Vec3& v) : Line(p, v) {}
	Segment(const Point& p1, const Point& p2) : Line(p1, p2 - p1) {}

	// �I�_���擾
	Float3 getEndPoint() const {
		return p + v;
	}
};


// ��
struct Sphere {
	Point p;
	float r;	// ���a
	Sphere() : p(0.0f, 0.0f, 0.0f), r(0.5f) {}
	Sphere(const Point& p, float r) : p(p), r(r) {}
	~Sphere() {}
};

// �J�v�Z��
// ���ar��P1P2��ݒ肵�Ă�����B��������
// ���ar��P1(��̕�)�Ƃ�������̕����x�N�g��(���������Ă��)
struct Capsule {
	Segment s;
	float r;	// ���̔��a
	float h;	// �����܂܂Ȃ������B�ォ��ǉ���������v�Z�ł͎g��Ȃ������B
	Capsule() : r(0.5f), h(0.5f) {}
	Capsule(const Segment& s, float r, float h) : s(s), r(r), h(h) {}
	Capsule(const Point& p1, const Point& p2, float r, float h) : s(p1, p2), r(r), h(h) {}
	~Capsule() {}

	void SetCapsule(Point p1, Point p2, float radius, float height)
	{
		r = radius;
		h = height;
		s.p = p1;
		s.v = p2 - p1;
	}

};

// AABB
struct AABB {
	Point p;	// ���S�_
	Float3 hl;	// �e���̕ӂ̒����̔���
	AABB() {}
	AABB(const Point& p, const Float3& hl) : p(p), hl(hl) {}

	// �ӂ̒������擾
	float lenX() const { return hl.x * 2.0f; };
	float lenY() const { return hl.y * 2.0f; };
	float lenZ() const { return hl.z * 2.0f; };
	float len(int i) {
		return *((&hl.x) + i) * 2.0f;
	}
};









// �֐��̐錾
// class��struct�ȊO�̊֐��Ƃ��́Acpp�ɏ����Ȃ��ƁA�P�ȏ�̕������`����Ă�V���{��������܂�
// �ɂȂ��Ă��܂��B�C��t���āB

float calcPointLineDist(const Point& p, const Line& l, Point& h, float& t);
bool isSharpAngle(const Point& p1, const Point& p2, const Point& p3);
float calcPointSegmentDist(const Point& p, const Segment& seg, Point& h, float& t);
float calcLineLineDist(const Line& l1, const Line& l2, Point& p1, Point& p2, float& t1, float& t2);
void clamp01(float& v);
float calcSegmentSegmentDist(const Segment& s1, const Segment& s2, Point& p1, Point& p2, float& t1, float& t2);
bool colCapsuleCapsule(const Capsule& c1, const Capsule& c2, Point* p1, Point* p2, float* vd);



#endif