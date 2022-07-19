#pragma once

#ifndef __IKD_OX_PRIMITIVE_H__
#define __IKD_OX_PRIMITIVE_H__


// �v���~�e�B�u��`

// v1.00
//  ���o

// v1.01
// �x�N�g���̕��s������O�ςɒu������


#include <math.h>
#include <d3dx9math.h>

#define _OX_EPSILON_	0.000001f	// �덷


//// 3����float
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
	//// �������g�ƈ�����dot
	//float dot(const Float3& r) const {
	//	return x * r.x + y * r.y + z * r.z;
	//}
//
	//Float3 cross(const Float3& r) const {
	//	return Float3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
	//}
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
//	//// �s�p�֌W�H
//	//bool isSharpAngle(const Vec3& r) const {
//	//	return (dot(r) >= 0.0f);
//	//}
//
//};
//
//
//// �_
//typedef Float3 Point;
//typedef Float3 D3DXVEXTOR3;
//
//
//// �x�N�g��
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
//	// �W����
//	void norm() {
//		const float len = length();
//		if (len > 0.0f) {
//			x /= len;
//			y /= len;
//			z /= len;
//		}
//	}
//
//	// �����֌W�ɂ���H
//	bool isVertical(const Vec3& r) const {
//		float d = dot(r);
//		return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// �덷�͈͓��Ȃ琂���Ɣ���
//	}
//
	//// ���s�֌W�ɂ���H
	//bool isParallel(const Vec3& r) const {
	//	float d = cross(r).lengthSq();
	//	return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// �덷�͈͓��Ȃ畽�s�Ɣ���
	//}
//
//	// �s�p�֌W�H
//	bool isSharpAngle(const Vec3& r) const {
//		return (dot(r) >= 0.0f);
//	}
//
//
//};
//
//
//// ����
//struct Line {
//	Point p;
//	Vec3 v;		// �����x�N�g��(�P�ʃx�N�g���ł͂Ȃ��̂�y��30����Ă���30�������Ă�)
//	Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
//	Line(const Point& p, const Vec3& v) : p(p), v(v) {}
//	~Line() {}
//
//	// �_��̍��W���擾
//	//  �x�N�g���Ɋ|���Z����W��
//	Point getPoint(float t) const {
//		return p + t * v;
//	}
//
//
//	// 1�ڂ̏�̓_���Q�b�g����
//	Point GetPoint1() const {
//		return p;
//	}
//	// 2�ڂ̏�̓_���Q�b�g����
//	Point GetPoint2() const {
//		return p + v;
//	}
//
//	// �x�N�^�[���Q�b�g����
//	Vec3 GetNormalVec() const {
//		return v;
//	}
//};
//
//
//// ����
//struct Segment : public Line {
//
//	Segment() {}
//	Segment(const Point& p, const Vec3& v) : Line(p, v) {}
//	Segment(const Point& p1, const Point& p2) : Line(p1, p2 - p1) {}
//
//	// �I�_���擾
//	Float3 getEndPoint() const {
//		return p + v;
//	}
//};
//
//
//// ��
//struct Sphere {
//	Point p;
//	float r;	// ���a
//	Sphere() : p(0.0f, 0.0f, 0.0f), r(0.5f) {}
//	Sphere(const Point& p, float r) : p(p), r(r) {}
//	~Sphere() {}
//};
//
//// �J�v�Z��
//// ���ar��P1P2��ݒ肵�Ă�����B��������
//// ���ar��P1(��̕�)�Ƃ�������̕����x�N�g��(���������Ă��)
//struct Capsule {
//	Segment s;
//	float r;		// ���̔��a�̊���(100.0f��� = 1.0f)
//	float r_num;	// ���̔��a
//	float h;		// Harf height�B(100.0f��� = 1.0f)
//	float h_num;	// Harf height�B�����B�����܂܂Ȃ������B
//	Point pos;		// �J�v�Z���̒��S���W
//	//Point pos1;	// �J�v�Z���̏�̋��̍��W
//	//Point pos2;	// �J�v�Z���̏�̋��̍��W
//	Capsule() : r(0.5f), h(0.5f) {}
//	Capsule(const Segment& s, float r, float h) : s(s), r(r), h(h) {}
//	Capsule(const Point& p1, const Point& p2, float r, float h) : s(p1, p2), r(r), h(h) {}
//	~Capsule() {}
//
//	// (���W�A���a�A����)p�����S�ƂȂ鍂��height�̃J�v�Z�����쐬
//	void SetCapsule(Point centerpos, float radius, float harfheight)
//	{
//		r_num = radius;
//		h_num = harfheight;
//		if (h_num <= r_num )
//		{
//			h_num = r_num;		// �����������a�����邱�Ƃ͂ł��Ȃ��B
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
//		pos1.y += h * 0.5;		// p1�𒆐S�Ƃ��邽�߂������獂���𔼕������Ă�
//		pos2.y -= h * 0.5;
//
//		s.p = pos1;
//		s.v = pos2 - pos1;
//	}
//
//	//// 1�ڂ̏�̓_���Q�b�g����
//	//Point GetPoint1() const {
//	//	return pos1;
//	//}
//	//// 1�ڂ̏�̓_���Q�b�g����
//	//Point GetPoint2() const {
//	//	return pos2;
//	//}
//
//};
//
//// AABB 
//struct AABB {
//	Point p;	// ���S�_
//	Float3 hl;	// �e���̕ӂ̒����̔���
//	AABB() {}
//	AABB(const Point& p, const Float3& hl) : p(p), hl(hl) {}
//
//	// �ӂ̒������擾
//	float lenX() const { return hl.x * 2.0f; };
//	float lenY() const { return hl.y * 2.0f; };
//	float lenZ() const { return hl.z * 2.0f; };
//	float len(int i) {
//		return *((&hl.x) + i) * 2.0f;
//	}
//};
//
//// OBB Oriented Bounding Box �L�����E�{�b�N�X
//struct OBB {
//	Point c;     //���S�_�̍��W
//	Point u[3];  //XYZ �̊e���W���̌X����\�������x�N�g��
//	Point e;     //OBB �̊e���W���ɉ����������̔����i���S�_����ʂ܂ł̒����j
//};
//
//
//// �֐��̐錾
//// class��struct�ȊO�̊֐��Ƃ��́Acpp�ɏ����Ȃ��ƁA�P�ȏ�̕������`����Ă�V���{��������܂�
//// �ɂȂ��Ă��܂��B�C��t���āB
//
//float calcPointLineDist(const Point& p, const Line& l, Point& h, float& t);
//bool isSharpAngle(const Point& p1, const Point& p2, const Point& p3);
//float calcPointSegmentDist(const Point& p, const Segment& seg, Point& h, float& t);
//float calcLineLineDist(const Line& l1, const Line& l2, Point& p1, Point& p2, float& t1, float& t2);
//void clamp01(float& v);
//float calcSegmentSegmentDist(const Segment& s1, const Segment& s2, Point& p1, Point& p2, float& t1, float& t2);
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




// ��������V������

struct SEGMENT;
struct SPHERE;
struct CAPSULE;
struct OBB;

struct SEGMENT
{
	D3DXVECTOR3 m_pos1;		// �����̎n�_
	D3DXVECTOR3	m_pos2;		// �����̏I�_

	SEGMENT() : m_pos1(0, 0, 0), m_pos2(0, 0, 0) {}

	// pos1����pos2�ւ̃x�N�g����Ԃ�
	D3DXVECTOR3 GetVector()
	{
		return m_pos2 - m_pos1;
	}

};

struct SPHERE 
{
	D3DXVECTOR3 m_Center;	// ���S�_�̍��W
	float		m_Radius;	// ���a

	SPHERE() : m_Center(0, 0, 0), m_Radius(0.5f) {}

};


struct CAPSULE
{
	float m_Radius;				// ���a
	float m_HalfHeight;				// ���a
	D3DXVECTOR3 m_Center;		// ���S�_�̍��W
	//D3DXVECTOR3 m_PointUpper;	// �J�v�Z���̏�̋��̕����̋��̒��S���W
	//D3DXVECTOR3 m_PointLower;	// �J�v�Z���̉��̋��̕����̋��̒��S���W

	// �R���X�g���N�^
	CAPSULE() : m_Radius(0.5f), m_HalfHeight(0.5f), m_Center(0.0f, 0.0f, 0.0f) {}
	//CAPSULE() : m_Radius(0.5f), m_PointUpper(0, -0.5f, 0), m_PointLower(0, 0.5f, 0) {}

	// �J�v�Z�����Z�b�g
	void SetCapsule(float raius, float halfheight, D3DXVECTOR3 center)
	{
		m_Radius = raius;
		m_HalfHeight = halfheight;
		m_Center = center;
	}

	// �J�v�Z���̒��S���W���擾
	D3DXVECTOR3 GetCenterPos() const
	{
		return m_Center;
		//return m_PointUpper + ((m_PointLower - m_PointUpper) * 0.5f);
	}

	// �J�v�Z���̒��S���W����㉺2�̋��̍��W���Z�b�g
	void SetCenter(const D3DXVECTOR3& Center) {
		//D3DXVECTOR3 CenterToPointA = ((m_PointUpper - m_PointLower) * 0.5f);
		//D3DXVECTOR3 CenterToPointB = ((m_PointLower - m_PointUpper) * 0.5f);
		//m_PointUpper = Center + CenterToPointA;
		//m_PointLower = Center + CenterToPointB;
	}
	// �c�̔��a���擾����
	float GetHeightRadius()const 
	{
		return m_HalfHeight;
		//D3DXVECTOR3 temp_calclength = m_PointLower - m_PointUpper;
		//float PointLen = D3DXVec3Length(&temp_calclength) * 0.5f;
		//PointLen += m_Radius;
		//return PointLen;
	}
	// ��̋��̍��W���擾����
	D3DXVECTOR3 GetUpperSpherePos()const
	{
		D3DXVECTOR3 rpos = m_Center;
		rpos.y += m_HalfHeight - m_Radius;
		return rpos;
	}
	// ���̋��̍��W���擾����
	D3DXVECTOR3 GetLowerSpherePos()const
	{
		D3DXVECTOR3 rpos = m_Center;
		rpos.y += -m_HalfHeight + m_Radius;
		return rpos;
	}
};


struct OBB
{
	D3DXVECTOR3 m_Center;   // ���S�_�̍��W
	D3DXVECTOR3 m_Rot[3];	// XYZ �̊e���W���̌X����\�������x�N�g��
	D3DXVECTOR3 m_Size;     // OBB �̊e���W���ɉ����������̔����i���S�_����ʂ܂ł̒����j

	// �R���X�g���N�^
	OBB() 
	{
		//�f�t�H���g��1��1.0��OBB
		D3DXVECTOR3 SizeVec(1.0f, 1.0f, 1.0f);
		//�s��̓A�C�f���e�B�e�B
		D3DXMATRIX Matrix;
		CreateOBB(SizeVec, Matrix);
	}

	// OBB�̑S�Ă�ݒ肵�Ă����
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
		//��]�𓾂�
		
		D3DXVec3Normalize(&m_Rot[0] , &VecX); 
		D3DXVec3Normalize(&m_Rot[1] , &VecY);
		D3DXVec3Normalize(&m_Rot[2] , &VecZ);
	}

	// ��]�s��̎擾
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

	//// ���[���h�s��̎擾
	//D3DXMATRIX GetWorldMatrix()const {
	//	D3DXMATRIX ret, Pos;
	//	ret = XMMatrixScaling(m_Size.x * 2.0f, m_Size.y * 2.0f, m_Size.z * 2.0f);
	//	ret = XMMatrixMultiply(ret, GetRotMatrix());
	//	Pos = XMMatrixTranslation(m_Center.x, m_Center.y, m_Center.z);
	//	ret = XMMatrixMultiply(ret, Pos);
	//	return ret;
	//}

	// ��ԑ傫���ӂ̔����������擾
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

	// ��ԑ傫���ӂ̒������擾
	float GetMaxSize()const 
	{
		return GetMaxHalfSize() * 2.0f;
	}

	// ��ԏ������ӂ̔����������擾
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

	// ��ԏ������ӂ̒������擾
	float GetMinSize() const 
	{
		return  GetMinHalfSize() * 2.0f;
	}

	//// �x�N�g���Ɉ�ԋ߂���]�𓾂�
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

	// OBB����]���ĂȂ����ǂ����𒲂ׂ�iAABB�Ƃ��Ďg���邩�ǂ����𒲂ׂ���j
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
@brief  point���猩��OBB�̍ŋߐړ_�𓾂�
@param[in]  point   ��_
@param[in]  obb OBB
@param[out] retvec  �ŋߐړ_��Ԃ��Q��
@return �Ȃ�
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
@brief  ����OBB�Ƃ̏Փ˔���
@param[in]  sp  ��
@param[in]  obb OBB
@param[out] retvec  �ŋߐړ_����������Q��
@return �Փ˂��Ă����true
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
		@brief	�_c�Ɛ���ab�̍ŋߐړ_��Ԃ�
		@param[in]	c	���ƂɂȂ�_
		@param[in]	a	�����J�n�_
		@param[in]	b	�����I���_
		@param[out]	t	�ŋߐړ_�̐�����̈ʒu�̊����̖߂�l
		@param[out]	d	�ŋߐړ_�̖߂�l
		@return	�Ȃ�
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
		@brief	�J�v�Z����Obb�̍ŋߐړ_�𓾂�
		@param[in]	cp	�J�v�Z��
		@param[in]	obb	OBB
		@param[out]	flg	2���̂Ƃ̈ʒu�֌W
		@return	�ŋߐړ_(�J�v�Z����)
		*/
		//--------------------------------------------------------------------------------------
static D3DXVECTOR3 ClosestPtCapsuleOBB(const CAPSULE& cp, const OBB& obb, int& flg) 
{
	// �v�Z���₷���悤�Ɏ擾���Ă���
	D3DXVECTOR3 UpperPos = cp.GetUpperSpherePos();
	D3DXVECTOR3 LowerPos = cp.GetLowerSpherePos();
	SPHERE Sp;	// �ϐ��̏���
	Sp.m_Center = UpperPos;
	Sp.m_Radius = cp.m_Radius;

	D3DXVECTOR3 retvec;
	//�X�^�[�g�ʒu�ōŋߐړ_�𓾂�
	SPHERE_OBB(Sp, obb, retvec);
	//���ς�}��
	D3DXVECTOR3 Base = LowerPos - UpperPos;
	//Base.normalize();
	D3DXVec3Normalize(&Base, &Base);
	D3DXVECTOR3 Dest = retvec - UpperPos;
	float dot = D3DXVec3Dot(&Base, &Dest);
	//float dot = bsm::dot(Base, Dest);
	if (dot < 0) {
		//�X�^�[�g�ʒu�̋��̂̊O��
		//retvec�͗L��
		flg = -1;
		return retvec;
	}
	D3DXVECTOR3 LU = LowerPos - UpperPos;
	float  size = D3DXVec3Length(&LU);
	if (dot > size) {
		//�I�_����ɂ���
		Sp.m_Center = LowerPos;
		SPHERE_OBB(Sp, obb, retvec);
		//�I�_�ōŋߐړ_���Ƃ�
		flg = 1;
		return retvec;
	}
	//���S��obb�̍ŋߐړ_�𓾂�
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





// �֐��̐錾
// class��struct�ȊO�̊֐��Ƃ��́Acpp�ɏ����Ȃ��ƁA�P�ȏ�̕������`����Ă�V���{��������܂�
// �ɂȂ��Ă��܂��B�C��t���āB

// ���������g���ă��[���h�}�g���N�X��0������
// GetWorldMatrix�Ƃ̈Ⴂ�͂������͈������ł��炤�̂łǂ�Ȃ̂ɂ��Ή��ł���
D3DXMATRIX CreateWorldMatrix(D3DXVECTOR3 Scale, D3DXVECTOR3 Rotation, D3DXVECTOR3 Position);
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






#endif