#pragma once

// �R���W�����^�C�v�̒l�̐ݒ�
static const int	BOX_COLLISION = 0;
static const int	CAPSULE_COLLISION = 1;
static const int	SPHERE_COLLISION = 2;
static const int	RAY_COLLISION = 3;			// �����̓_A����_B�̏Փ˂��Ă���Őڋߓ_�����߂邽�߂̂���

// ���X�|���X�^�C�v�̒l�̐ݒ�
static const int	RESPONSE_IGNORE	 = 0;
static const int	RESPONSE_OVERLAP = 1;
static const int	RESPONSE_BLOCK   = 2;



struct StructCollisionData
{
	int			sm_Collision_type = BOX_COLLISION;		// �J�v�Z���q���W�����Ȃ̂��{�b�N�X�Ȃ̂��Ƃ���ݒ肷��
	bool		sm_BeMoved = false;
	float		sm_OnTheGroundCheckNormal = 0.7f;	// 0.0f~1.0f �ڒn����̎��A�Փ˂������̕����x�N�g��y������ȏ�̂Ƃ��ɐڒn�����true�ɂ���

	D3DXVECTOR3 sm_ColInitOffset = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 sm_ColInitRotation = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 sm_ColInitScale = { 1.0f, 1.0f, 1.0f };

};

class Collision : public CComponent
{
private:
	int			m_Collision_type = BOX_COLLISION;		// �J�v�Z���q���W�����Ȃ̂��{�b�N�X�Ȃ̂��Ƃ���ݒ肷��
	bool		m_BeMoved = false;
	float		m_OnTheGroundCheckNormal = 0.7f;	// 0.0f~1.0f �ڒn����̎��A�Փ˂������̕����x�N�g��y������ȏ�̂Ƃ��ɐڒn�����true�ɂ���
	int			m_ResponseType = RESPONSE_BLOCK;

	// �V�����́��@���m_ResponseType���͍��͂����g���ĂȂ�
	int					m_ObjectType = 0;
	std::vector<int>	m_ResponseObjectList;		// Init�Ŕz��̑傫����ݒ肷��K�v����

	bool		m_Visibility = false;

	// ���ׂẴR���W�����^�C�v�Ŏg����BRadius = ColScale.x�ōl����
	D3DXVECTOR3 m_ColOffset = { 0.0f, 0.0f, 0.0f };	// ���ݎg���ĂȂ�
	D3DXVECTOR3 m_ColInitOffset = { 0.0f, 0.0f, 0.0f };

	D3DXVECTOR3 m_ColPosition = { 0.0f, 0.0f, 0.0f };		// ��]���l���������W�B�e+Offset

	D3DXVECTOR3 m_ColRotation = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 m_ColInitRotation = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 m_ColScale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 m_ColInitScale = { 1.0f, 1.0f, 1.0f };


	OBB			m_obb;
	CAPSULE		m_capsule;
	SPHERE		m_sphere;
	//SEGMENT		m_raysegment;
	RAY			m_ray;


	Model* m_ModelColCube;
	Model* m_ModelColCylinder;
	Model* m_ModelColSphereA;
	Model* m_ModelColSphereB;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	int		m_collisionModelCount = 0;


public:

	// �e�N���X�̃R���X�g���N�^������Ă����B
	using CComponent::CComponent;

	// model�̃��[�h�����BInit�̒��Ŏg���Ă�
	void LoadCollisionModel()
	{
		m_ModelColCube = new Model();
		m_ModelColCube->Load("asset\\model\\CollisionModel\\ColCube.obj");
		m_ModelColCylinder = new Model();
		m_ModelColCylinder->Load("asset\\model\\CollisionModel\\ColCylinder.obj");
		m_ModelColSphereA = new Model();
		m_ModelColSphereA->Load("asset\\model\\CollisionModel\\ColSphere.obj");
		m_ModelColSphereB = new Model();
		m_ModelColSphereB->Load("asset\\model\\CollisionModel\\ColSphere.obj");
		
	}


	void Init() override
	{
		m_CompName = "Collision";
		m_CompId = CollisionId;

		m_UpdatePriority = 1;		// ��ɌĂ΂�Ăق����̂ŁB

		m_Visibility = false;		// �R���W�����͌����Ȃ��ݒ�B

		//// �R���W�������X�|���X��vector�̃T�C�Y�̐��������Ƃ�B�������B
		//std::vector<std::string> ObjectTypeNameList;
		//std::vector<int> DefaultResponseList;
		//int ObjectTypeNum;

		//Manager::GetSavedata()->Get_Collision_ObjectTypeSaveListData(&ObjectTypeNameList, &DefaultResponseList);
		//ObjectTypeNum = ObjectTypeNameList.size();	// �I�u�W�F�N�g�^�C�v�̎�ސ�
		//if (m_ResponseObjectList.size() != ObjectTypeNum)
		//{
		//	m_ResponseObjectList.clear();
		//	for (int n = 0; n < ObjectTypeNum; n++)
		//	{
		//		m_ResponseObjectList.push_back(DefaultResponseList[n]);
		//	}
		//}

		LoadCollisionModel();

		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

		Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");


	}

	void Uninit() override
	{

		m_ModelColCube->Unload();
		delete m_ModelColCube;
		m_ModelColCylinder->Unload();
		delete m_ModelColCylinder;
		m_ModelColSphereA->Unload();
		delete m_ModelColSphereA;
		m_ModelColSphereB->Unload();
		delete m_ModelColSphereB;

		m_VertexLayout->Release();
		m_VertexShader->Release();
		m_PixelShader->Release();

		CComponent::Uninit();
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

	void Update() override;


	void Draw() override
	{
		// ���̓��C�A�E�g�ݒ�
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		// �R���W������������ݒ�Ȃ�΃��f����\������
		if (m_Visibility)
		{
			if (m_Collision_type == BOX_COLLISION)
			{
				D3DXMATRIX world;

				// ���[���h�}�g���N�X�ݒ�
				//world = CreateWorldMatrix(m_obb.m_Size, m_ColRotation, m_obb.m_Center);	// �֐��ɂ܂Ƃ߂�
				world = CreateWorldMatrix(m_ColScale, m_ColRotation, m_obb.m_Center);	// �֐��ɂ܂Ƃ߂�
				//world = CreateWorldMatrix(OBBScale, m_ColRotation, OBBpos);	// �֐��ɂ܂Ƃ߂�
				Renderer::SetWorldMatrix(&world);
				m_ModelColCube->Draw();

			}
			else if (m_Collision_type == CAPSULE_COLLISION)
			{
				D3DXVECTOR3 CylinderScale;
				CylinderScale.x = m_capsule.m_Radius;
				CylinderScale.y = m_capsule.m_HalfHeight;
				CylinderScale.z = m_capsule.m_Radius;

				D3DXMATRIX world;

				// ���[���h�}�g���N�X�ݒ�
				world = CreateWorldMatrix(CylinderScale, m_ColRotation, m_capsule.GetCenterPos());	// �֐��ɂ܂Ƃ߂�
				Renderer::SetWorldMatrix(&world);
				m_ModelColCylinder->Draw();

				D3DXVECTOR3 SphereScale;
				SphereScale = D3DXVECTOR3(m_capsule.m_Radius, m_capsule.m_Radius, m_capsule.m_Radius);

				// ��̋��̕`��
				world = CreateWorldMatrix(SphereScale, m_ColRotation, m_capsule.GetUpperSpherePos());	// �֐��ɂ܂Ƃ߂�
				Renderer::SetWorldMatrix(&world);
				m_ModelColSphereA->Draw();

				// ���̋��̕`��
				world = CreateWorldMatrix(SphereScale, m_ColRotation, m_capsule.GetLowerSpherePos());	// �֐��ɂ܂Ƃ߂�
				Renderer::SetWorldMatrix(&world);
				m_ModelColSphereB->Draw();
			}
			else if (m_Collision_type == SPHERE_COLLISION)
			{
				D3DXMATRIX world;

				// ���[���h�}�g���N�X�ݒ�
				world = CreateWorldMatrix(m_ColScale, m_ColRotation, m_sphere.m_Center);	// �֐��ɂ܂Ƃ߂�
				Renderer::SetWorldMatrix(&world);
				m_ModelColSphereA->Draw();
			}
			else if (m_Collision_type == RAY_COLLISION)
			{
				D3DXMATRIX world;
				D3DXVECTOR3 RayScale = D3DXVECTOR3(0.06f, 0.06f, 0.06f);
				D3DXVECTOR3 RayRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// ���[���h�}�g���N�X�ݒ�
				world = CreateWorldMatrix(RayScale, RayRotation, m_ray.m_Segment.m_pos1);
				Renderer::SetWorldMatrix(&world);
				m_ModelColSphereA->Draw();

				world = CreateWorldMatrix(RayScale, RayRotation, m_ray.m_Segment.m_pos2);
				Renderer::SetWorldMatrix(&world);
				m_ModelColSphereB->Draw();

				// �f�t�H�l(999)�Ƃ���������p2�̏ꏊ�ɕ\������
				RayScale = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
				if (m_ray.m_length < 997.0f)
				{
					//m_ray.CalcCollisionPointwithLengthNormal();
					D3DXVECTOR3 collisionpoint = m_ray.m_CollisionPoint;				// �Փ˓_�͏Փˑ���̍Őڋߓ_�������Ă���
					world = CreateWorldMatrix(RayScale, RayRotation, collisionpoint);	// �Փ˓_
					Renderer::SetWorldMatrix(&world);
					m_ModelColCube->Draw();
				}
				else
				{
					world = CreateWorldMatrix(RayScale, RayRotation, m_ray.m_Segment.m_pos2);	// �Փ˓_
					Renderer::SetWorldMatrix(&world);
					m_ModelColCube->Draw();
				}
			}
		}
	}


	

	// Init Offset
	D3DXVECTOR3 GetColInitOffset()
	{
		return m_ColInitOffset;
	}
	void SetColInitOffset(D3DXVECTOR3 setcoloffset)
	{
		m_ColInitOffset = setcoloffset;
	}

	// Init Scale
	D3DXVECTOR3 GetColInitScale()
	{
		return m_ColInitScale;
	}
	void SetColInitScale(D3DXVECTOR3 setcolscale)
	{
		m_ColInitScale = setcolscale;
	}
	
	// Init Rotation
	D3DXVECTOR3 GetColInitRotation()
	{
		return m_ColInitRotation;
	}
	void SetColInitRotation(D3DXVECTOR3 setcolrotation)
	{
		m_ColInitRotation = setcolrotation;
	}


	void SetCollisionType(int type)
	{
		m_Collision_type = type;
	}
	int GetCollisionType()
	{
		return m_Collision_type;
	}


	void SetBeMoved(bool movable)
	{
		m_BeMoved = movable;
	}
	bool GetBeMoved()
	{
		return m_BeMoved;
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


	// �Փˎ��̉����߂�����
	//void OverlapToBackPosition(GameObject* self, GameObject* other, D3DXVECTOR3 backvector)
	void OverlapToBackPosition(Collision* self_collision, Collision* other_collision, D3DXVECTOR3 backvector)
	{
		GameObject* self_parent = self_collision->m_ParentGameObject;
		GameObject* other_parent = other_collision->m_ParentGameObject;

		float se = self_parent->GetPosition().x;
		float ot = other_parent->GetPosition().x;

		if (self_parent == other_parent)
			int gg = 5;

		D3DXVECTOR3 self_Pos = self_parent->GetPosition();		// �Q�b�g
		D3DXVECTOR3 other_Pos = other_parent->GetPosition();					// �Q�b�g

		// �������������̂Ȃ�
		if (self_collision->m_BeMoved)
		{
			// �Փ˂������肪�����Ȃ��Ȃ�
			if (!other_collision->m_BeMoved)
			{
				// ���������|�W�V������ς���
				self_parent->SetPosition(self_Pos - backvector);		// �Z�b�g
			}
			else if (other_collision->m_BeMoved)
			{
				// �Փ˂������肪�����Ȃ�
				// �x�N�g���𔼕��ɂ���
				backvector *= 0.5f;
				// �����Ƒ���̃|�W�V�������x�N�g���̔������ς���B(�{���͂��̎��̐����Ƃ������ꂽ����������)
				self_parent->SetPosition(self_Pos - backvector);		// �Z�b�g
				other_parent->SetPosition(other_Pos + backvector);		// �Z�b�g
			}
		}
		else
		{
			// �Փ˂������肪�����Ȃ��Ȃ�
			if (!other_collision->m_BeMoved)
			{
				// �����N���Ȃ�
			}
			else if (other_collision->m_BeMoved)
			{
				// �Փ˂������肪�����Ȃ�
				// ���肾���߂�
				other_parent->SetPosition(other_Pos + backvector);		// �Z�b�g
			}
		}
	}

	// �J�v�Z���ƃ{�b�N�X�̓����蔻��
	void CapsuleOBB(Collision* capsule, Collision* obb, int response)
	{
		// cp_Capsule �ɂ̓J�v�Z�����̍Őڋߓ_�̍��W���擾�ł���(Closest point �Őڋߓ_)
		int flag;
		D3DXVECTOR3 cp_Capsule;
		cp_Capsule = ClosestPtCapsuleOBB(capsule->m_capsule, obb->m_obb, flag);

		//  ���߂��J�v�Z���̕\�ʂ̍Őڋߓ_�ƁA�J�v�Z���̎��̐����Ƃ̍Őڋߓ_�����߂�
		float t;
		D3DXVECTOR3 cp_Segment;		// cp_Segment�ɂ͐������̍ŒZ���W�����܂�
		ClosetPtPointSegment(cp_Capsule, capsule->m_capsule.GetLowerSpherePos(), capsule->m_capsule.GetUpperSpherePos(), t, cp_Segment);

		// �����̍Őڋߓ_�ŃJ�v�Z�����ŃX�t�B�A�����
		capsule->m_sphere.SetSpehre(cp_Segment, capsule->m_capsule.m_Radius);

		// �X�t�B�A��OBB�̏Փ˔�����g��
		SphereOBB(capsule, obb, response);
	}

	// �J�v�Z���ƃJ�v�Z���̓����蔻��
	void CapsuleCapsule(Collision* self_capsule, Collision* other_capsule, int response)
	{
		// �Փ˂����Ƃ��̃J�v�Z���̒��S�A�R�A�ɂȂ��Ă�������̒��ł̍ŒZ�����ɂȂ�Ƃ��̍��W���Ԃ����
		D3DXVECTOR3 c_p1;
		D3DXVECTOR3 c_p2;
		// c_p1����c_p2�ւ̒P�ʕ����x�N�g��
		D3DXVECTOR3 c_UnitNormal;
		// �ᔽ���Ă��鋗��
		float c_vd;		// violatedistance
		// �����x�N�g���B�P�ʕ����x�N�g���ƈᔽ���������������ړ�������Ώd�Ȃ��h�����Ƃ��ł���B
		D3DXVECTOR3 c_Normal;
		// �Փ˂��Ă���Ƃ��̍ŒZ���W�������ɕۑ�����

		if (colCapsuleCapsule(self_capsule->m_capsule, other_capsule->m_capsule, &c_p1, &c_p2, &c_vd))
		{
			// �Փ˂��Ă���Ȃ�

			// �Փˏ���e�I�u�W�F�N�g�ɓ`����BOverlap�̏ꍇreturn�I������
			if (AddCollisionStateToParentObject(self_capsule, other_capsule, response, c_p1))
				return;

			D3DXVECTOR3 scap_vel = self_capsule->m_ParentGameObject->GetVelocity();
			D3DXVECTOR3 ocap_vel = other_capsule->m_ParentGameObject->GetVelocity();

			// �d�Ȃ��������W��߂�t_Position
			D3DXVECTOR3 tvec = c_p1 - c_p2;
			D3DXVec3Normalize(&c_UnitNormal, &tvec);
			c_Normal = c_UnitNormal * c_vd * 0.8;				// �߂��x�N�g��		

			OverlapToBackPosition(self_capsule, other_capsule, -c_Normal);


			// �J�v�Z�����̐ڒn���菈��
			// �����艺�ŏՓ˂����Ă���Ȃ�ΐڒn���Ă���Ƃ݂Ȃ�
			//float OnTheGroundHeight_cap;
			//OnTheGroundHeight_cap = capsule->m_capsule.GetLowerSpherePos().y - capsule->m_capsule.m_Radius * (1.0f - capsule->m_OnTheGroundCoefficient);

			// self���ڒn���菈��
			if (scap_vel.y < 0.0f && c_UnitNormal.y > self_capsule->m_OnTheGroundCheckNormal)
			{
				self_capsule->m_ParentGameObject->SetOnTheGround(true);
				self_capsule->m_ParentGameObject->SetTempOnTheGround(true);
				self_capsule->m_ParentGameObject->SetVelocity_y(0.0f);
			}
			// other���ڒn���菈��
			if (ocap_vel.y < 0.0f && -c_UnitNormal.y > other_capsule->m_OnTheGroundCheckNormal)
			{
				other_capsule->m_ParentGameObject->SetOnTheGround(true);
				other_capsule->m_ParentGameObject->SetTempOnTheGround(true);
				other_capsule->m_ParentGameObject->SetVelocity_y(0.0f);
			}

		}
		else
		{
			// �Փ˂��Ă��Ȃ��Ȃ�
			int aaasd = 4;
		}
	}




	// �X�t�B�A��OBB�̓����蔻��
	void SphereOBB(Collision* sphere, Collision* obb, int response)
	{
		// cp_OBB �ɂ�OBB���̍Őڋߓ_�̍��W���擾�ł���(Closest point �Őڋߓ_)
		int flag;
		D3DXVECTOR3 cp_OBB;
		ClosestPtPointOBB(sphere->m_sphere.m_Center, obb->m_obb, cp_OBB);

		// distance �X�t�B�A����݂�OBB�ւ̍ŒZ����(�x�N�g��)
		D3DXVECTOR3 distance;
		distance = cp_OBB - sphere->m_sphere.m_Center;

		// length �X�t�B�A����OBB�ւ̍ŒZ����
		float length;
		length = D3DXVec3Length(&distance);

		// overlaplength �X�t�B�A��OBB�̏d�Ȃ��Ă��鋗��
		float overlaplength;
		overlaplength = sphere->m_sphere.m_Radius - length;

		// �X�t�B�A����݂�OBB�ւ̕����x�N�g��
		D3DXVECTOR3 normal;
		D3DXVec3Normalize(&normal, &distance);

		// �Փ˂��Ă���̏���
		if (length <= sphere->m_sphere.m_Radius)
		{
			// �Փˏ���e�I�u�W�F�N�g�ɓ`����BOverlap�̏ꍇreturn�I������
			if (AddCollisionStateToParentObject(sphere, obb, response, cp_OBB))
				return;

			// �d�Ȃ��Ă�����߂������B�߂������̂Ԃ�Ԃ���������邽�߂ɂ����Ȃ߂ɖ߂��Ă���B����̓q�b�g���Ă鎞�ԂŐ��䂵�ĂȂ����_
			D3DXVECTOR3 backVector;
			backVector = normal * overlaplength * 0.8;	// �����x�N�g���̂ނ��ɏd�Ȃ��Ă��鋗�����߂�

			OverlapToBackPosition(sphere, obb, backVector);		// ���̒��ŃZ�b�g���Ă���

			D3DXVECTOR3 sphere_Vel = sphere->m_ParentGameObject->GetVelocity();
			D3DXVECTOR3 obb_Vel = obb->m_ParentGameObject->GetVelocity();

			// �X�t�B�A���ڒn���菈���Bflag == 1�ŏ�ɂ���B-1�ŉ��ɂ���B0�ŉ�
			if (sphere_Vel.y < 0.0f && -normal.y > sphere->m_OnTheGroundCheckNormal)
			{
				sphere->m_ParentGameObject->SetOnTheGround(true);
				sphere->m_ParentGameObject->SetTempOnTheGround(true);
				sphere->m_ParentGameObject->SetVelocity_y(0.0f);
			}

			// OBB���ڒn���菈���B
			if (obb_Vel.y < 0.0f && normal.y > obb->m_OnTheGroundCheckNormal)
			{
				obb->m_ParentGameObject->SetOnTheGround(true);
				obb->m_ParentGameObject->SetTempOnTheGround(true);
				obb->m_ParentGameObject->SetVelocity_y(0.0f);
			}
		}
	}


	//static bool SPHERECAPSULE(const SPHERE& sp, const CAPSULE& cap, D3DXVECTOR3& cp_capsule)
	void SphereCapsule(Collision* sphere, Collision* capsule, int response)
	{
		// �Փ˂��Ă��邩�ǂ����̏����ݒ�
		bool hit = false;

		//�܂��ŋߐړ_��ݒ�
		float t;
		D3DXVECTOR3 cp_segment;
		// �J�v�Z���̐����ƃX�t�B�A�̒��S�ł̍ŒZ���W�����߂�Bcp_segment�ɂ͐������̍ŒZ���W�����܂�
		ClosetPtPointSegment(sphere->m_sphere.m_Center, capsule->m_capsule.GetLowerSpherePos(), capsule->m_capsule.GetUpperSpherePos(), t, cp_segment);
		//ClosetPoint�́A�J�v�Z��������̓_�ł���
		D3DXVECTOR3 distance = sphere->m_sphere.m_Center - cp_segment;		// 2�̓_�̋��������߂�
		D3DXVECTOR3 nNormal;
		D3DXVec3Normalize(&nNormal, &distance);
		//  �J�v�Z�����̍ŋߐړ_���擾�ł���
		D3DXVECTOR3 cp_capsule = cp_segment + (distance * capsule->m_capsule.m_Radius);
		//�Փ˂��Ă��邩���ʁB�Ȃ������̕���(2��)�Ȃ̂��Ƃ����ƁA�v�Z�ŋ��܂���͕̂����ŏo�Ă���
		// �����1��̂��傤�����ɂ���v�Z�̏������Ȃ����߁B�ʂɑ����d���Ă����Ȃ畁�ʂɋ����o���Ă����B���̏ꍇ���̂Ƃ����a.
		float length = D3DXVec3Length(&distance);

		float radius = sphere->m_sphere.m_Radius + capsule->m_capsule.m_Radius;
		if (length <= radius)
			hit = true;

		// �ᔽ���������߂�
		float overlength = length - radius;
		D3DXVECTOR3 backVector = overlength * nNormal * 0.8;

		if (hit)
		{
			// �Փˏ���e�I�u�W�F�N�g�ɓ`����BOverlap�̏ꍇreturn�I������
			if (AddCollisionStateToParentObject(sphere, capsule, response, cp_capsule))
				return;

			OverlapToBackPosition(sphere, capsule, backVector);		// ���̒��ŃZ�b�g���Ă���

			D3DXVECTOR3 sphere_Vel = sphere->m_ParentGameObject->GetVelocity();
			D3DXVECTOR3 capsule_Vel = capsule->m_ParentGameObject->GetVelocity();

			// �X�t�B�A���ڒn���菈���Bflag == 1�ŏ�ɂ���B-1�ŉ��ɂ���B0�ŉ�
			if (sphere_Vel.y < 0.0f && nNormal.y > sphere->m_OnTheGroundCheckNormal)
			{
				sphere->m_ParentGameObject->SetOnTheGround(true);
				sphere->m_ParentGameObject->SetTempOnTheGround(true);
				sphere->m_ParentGameObject->SetVelocity_y(0.0f);
			}

			// �J�v�Z�����ڒn���菈���B
			if (capsule_Vel.y < 0.0f && -nNormal.y > capsule->m_OnTheGroundCheckNormal)
			{
				capsule->m_ParentGameObject->SetOnTheGround(true);
				capsule->m_ParentGameObject->SetTempOnTheGround(true);
				capsule->m_ParentGameObject->SetVelocity_y(0.0f);
			}
		}

	}


	void SphereSphere(Collision* self_sp, Collision* other_sp, int response)
	{
		// �Փ˂��Ă��邩�ǂ����̏����ݒ�
		bool hit = false;

		//�܂��ŋߐړ_��ݒ�
		// self���猩��other�ւ̃x�N�g��
		D3DXVECTOR3 distance = other_sp->m_sphere.m_Center - self_sp->m_sphere.m_Center;
		D3DXVECTOR3 Normal;
		D3DXVec3Normalize(&Normal, &distance);	// self���猩��other�ւ̕����x�N�g��

		//  self���̍ŋߐړ_���擾�ł���
		D3DXVECTOR3 cp_self_sp = self_sp->m_sphere.m_Center + (Normal * self_sp->m_sphere.m_Radius);

		float length = D3DXVec3Length(&distance);		// �����𒷂��ɂ���

		float radius = self_sp->m_sphere.m_Radius + other_sp->m_capsule.m_Radius;	// ���ꂼ��̔��a�𑫂�
		if (length <= radius)
			hit = true;

		// �ᔽ���������߂�
		float overlength = length - radius;
		D3DXVECTOR3 backVector = overlength * Normal * 0.7;

		if (hit)
		{
			// �Փˏ���e�I�u�W�F�N�g�ɓ`����BOverlap�̏ꍇreturn�I������
			if (AddCollisionStateToParentObject(self_sp, other_sp, response, cp_self_sp))
				return;

			OverlapToBackPosition(self_sp, other_sp, -backVector);		// ���̒��ŃZ�b�g���Ă���

			D3DXVECTOR3 self_sp_Vel = self_sp->m_ParentGameObject->GetVelocity();
			D3DXVECTOR3 other_sp_Vel = other_sp->m_ParentGameObject->GetVelocity();

			// �X�t�B�A���ڒn���菈���Bflag == 1�ŏ�ɂ���B-1�ŉ��ɂ���B0�ŉ�
			if (self_sp_Vel.y < 0.0f && -Normal.y > self_sp->m_OnTheGroundCheckNormal)
			{
				self_sp->m_ParentGameObject->SetOnTheGround(true);
				self_sp->m_ParentGameObject->SetTempOnTheGround(true);
				self_sp->m_ParentGameObject->SetVelocity_y(0.0f);
			}

			// OBB���ڒn���菈���B
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
		// �����ƃX�t�B�A�̒��S�ł̍ŒZ���W�����߂�Bcp_segment�ɂ͐������̍ŒZ���W�����܂�
		ClosetPtPointSegment(sphere_collision->m_sphere.m_Center, p1, p2, t, cp_segment);
		// �X�t�B�A����
		D3DXVECTOR3 distance = sphere_collision->m_sphere.m_Center - cp_segment;
		float length = D3DXVec3Length(&distance);
		if (sphere_collision->m_sphere.m_Radius >= length)
			return true;

		return false;
	}

	void RaySegmentSphereDivCalc(D3DXVECTOR3 p1, D3DXVECTOR3 p2, Collision* sphere_collision)
	{
		int div = 100;	// ������
		D3DXVECTOR3 distance = p2 - p1;	// 1����2�ւ̋����x�N�g��
		D3DXVECTOR3 divVec = distance / (float)div;			// ���������x�N�g��

		int MaxTrial = 50;
		int low = 0;
		int mid;
		int high = div;
		int oldmid = -1;
		for (int i = 0; i < MaxTrial; i++)
		{
			mid = (low + high) / 2;

			// 2��A����mid�������l��������
			if (oldmid == mid)
			{
				break;
			}

			p2 = p1 + (divVec * mid);

			if (RaySegmentSphere(p1, p2, sphere_collision))
			{
				// �Փ˂��Ă���Ȃ�A���������X�ɖ߂�
				high = mid;
			}
			else
			{
				// �Փ˂��Ă��Ȃ��Ȃ�A���������X�ɐi�߂�
				low = mid;
			}
			oldmid = mid;	// mid�̒l�̕ۑ�
		}
		// 
		distance = p2 - p1;
		float length = D3DXVec3Length(&distance);
		SetRayHit(length, sphere_collision);			// �Z�b�g����
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
		int div = 100;	// ������
		D3DXVECTOR3 distance = p2 - p1;	// 1����2�ւ̋����x�N�g��
		D3DXVECTOR3 divVec = distance / (float)div;			// ���������x�N�g��

		int MaxTrial = 50;
		int low = 0;
		int mid;
		int high = div;
		int oldmid = -1;
		for (int i = 0; i < MaxTrial; i++)
		{
			mid = (low + high) / 2;

			// 2��A����mid�������l��������
			if (oldmid == mid)
			{
				break;
			}

			p2 = p1 + (divVec * mid);

			if (RaySegmentCapsule(p1, p2, capsule_collision))
			{
				// �Փ˂��Ă���Ȃ�A���������X�ɖ߂�
				high = mid;
			}
			else
			{
				// �Փ˂��Ă��Ȃ��Ȃ�A���������X�ɐi�߂�
				low = mid;
			}
			oldmid = mid;	// mid�̒l�̕ۑ�
		}
		// 
		distance = p2 - p1;
		float length = D3DXVec3Length(&distance);
		SetRayHit(length, capsule_collision);			// �Z�b�g����
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

	// �Ō��Ray�\���̂̃Z�b�g�����Ă���B��̂Ƒg�ݍ��킹�Ďg���B������g�����Ă��Ƃ͏Փ˂͂��Ă�
	void RaySegmentOBBDivCalc(D3DXVECTOR3 p1, D3DXVECTOR3 p2, Collision* obb_collision)
	{
		int div = 100;	// ������
		D3DXVECTOR3 distance = p2 - p1;	// 1����2�ւ̋����x�N�g��
		D3DXVECTOR3 divVec = distance / (float)div;			// ���������x�N�g��

		int MaxTrial = 50;
		int low = 0;
		int mid;
		int high = div;
		int oldmid = -1;
		for (int i = 0; i < MaxTrial; i++)
		{
			mid = (low + high) / 2;

			// 2��A����mid�������l��������
			if (oldmid == mid)
			{
				break;
			}

			p2 = p1 + (divVec * mid);

			if (RaySegmentOBB(p1, p2, obb_collision))
			{
				// �Փ˂��Ă���Ȃ�A���������X�ɖ߂�
				high = mid;
			}
			else
			{
				// �Փ˂��Ă��Ȃ��Ȃ�A���������X�ɐi�߂�
				low = mid;
			}
			oldmid = mid;	// mid�̒l�̕ۑ�
		}
		// 
		distance = p2 - p1;
		float length = D3DXVec3Length(&distance);
		SetRayHit(length, obb_collision);			// �Z�b�g����
	}

	// �q�b�g�����ꍇ�Z�b�g
	void SetRayHit(float length, Collision* HitCollision)
	{
		// ���̍ŒZ���Z��������X�V����
		if (m_ray.m_length > length)
		{
			m_ray.m_length = length;
			m_ray.HitCollision = HitCollision;
		}
	}
	// �q�b�g���Ȃ������ꍇ�Ƀf�t�H�l���Z�b�g
	void SetRayNoHit()
	{
		m_ray.m_length = 999;
		m_ray.HitCollision = nullptr;
	}




	// OBB v.s. OBB
	//bool OBBOBB(OBB& obb1, OBB& obb2)
	bool OBBOBB(Collision* self_obb, Collision* other_obb, int response)
	{
		OBB obb1 = self_obb->m_obb;
		OBB obb2 = other_obb->m_obb;

		// �e�����x�N�g���̊m��
		// �iN***:�W���������x�N�g���j
		D3DXVECTOR3 NAe1 = obb1.GetRotNormalVector(0), Ae1 = NAe1 * obb1.GetRotNormalLength(0, self_obb->m_ColScale);
		D3DXVECTOR3 NAe2 = obb1.GetRotNormalVector(1), Ae2 = NAe2 * obb1.GetRotNormalLength(1, self_obb->m_ColScale);
		D3DXVECTOR3 NAe3 = obb1.GetRotNormalVector(2), Ae3 = NAe3 * obb1.GetRotNormalLength(2, self_obb->m_ColScale);
		D3DXVECTOR3 NBe1 = obb2.GetRotNormalVector(0), Be1 = NBe1 * obb2.GetRotNormalLength(0, other_obb->m_ColScale);
		D3DXVECTOR3 NBe2 = obb2.GetRotNormalVector(1), Be2 = NBe2 * obb2.GetRotNormalLength(1, other_obb->m_ColScale);
		D3DXVECTOR3 NBe3 = obb2.GetRotNormalVector(2), Be3 = NBe3 * obb2.GetRotNormalLength(2, other_obb->m_ColScale);
		D3DXVECTOR3 Interval = obb1.m_Center - obb2.m_Center;

		// ������ : Ae1
		FLOAT rA = D3DXVec3Length(&Ae1);
		FLOAT rB = Collision::LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
		FLOAT L = fabs(D3DXVec3Dot(&Interval, &NAe1));
		if (L > rA + rB)
			return false; // �Փ˂��Ă��Ȃ�

		float L1 = L;
		float rA1 = rA;
		float rB1 = rB;
		float Dist1 = L - (rA + rB);

		// ������ : Ae2
		rA = D3DXVec3Length(&Ae2);
		rB = Collision::LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &NAe2));
		if (L > rA + rB)
			return false;

		float L2 = L;
		float rA2 = rA;
		float rB2 = rB;
		float Dist2 = L - (rA + rB);

		// ������ : Ae3
		rA = D3DXVec3Length(&Ae3);
		rB = Collision::LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
		L = fabs(D3DXVec3Dot(&Interval, &NAe3));
		if (L > rA + rB)
			return false;

		float L3 = L;
		float rA3 = rA;
		float rB3 = rB;
		float Dist3 = L - (rA + rB);

		// ������ : Be1
		rA = Collision::LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
		rB = D3DXVec3Length(&Be1);
		L = fabs(D3DXVec3Dot(&Interval, &NBe1));
		if (L > rA + rB)
			return false;

		float L4 = L;
		float rA4 = rA;
		float rB4 = rB;
		float Dist4 = L - (rA + rB);

		// ������ : Be2
		rA = Collision::LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
		rB = D3DXVec3Length(&Be2);
		L = fabs(D3DXVec3Dot(&Interval, &NBe2));
		if (L > rA + rB)
			return false;

		float L5 = L;
		float rA5 = rA;
		float rB5 = rB;
		float Dist5 = L - (rA + rB);

		// ������ : Be3
		rA = Collision::LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
		rB = D3DXVec3Length(&Be3);
		L = fabs(D3DXVec3Dot(&Interval, &NBe3));
		if (L > rA + rB)
			return false;

		float L6 = L;
		float rA6 = rA;
		float rB6 = rB;
		float Dist6 = L - (rA + rB);

		// ������ : C11
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

		// ������ : C12
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

		// ������ : C13
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

		// ������ : C21
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

		// ������ : C22
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

		// ������ : C23
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

		// ������ : C31
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

		// ������ : C32
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

		// ������ : C33
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

		// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
		return true;
	};


	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	FLOAT LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0)
	{
		// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
		// ������Sep�͕W��������Ă��邱��
		FLOAT r1 = fabs(D3DXVec3Dot(Sep, e1));
		FLOAT r2 = fabs(D3DXVec3Dot(Sep, e2));
		FLOAT r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
		return r1 + r2 + r3;
	};




	void InitforSaveData(std::fstream* p_file)
	{
		StructCollisionData scd;
		p_file->read((char*)&scd, sizeof(scd));

		m_Collision_type = scd.sm_Collision_type;
		m_BeMoved = scd.sm_BeMoved;
		m_OnTheGroundCheckNormal = scd.sm_OnTheGroundCheckNormal;
		m_ColInitOffset = scd.sm_ColInitOffset;
		m_ColInitRotation = scd.sm_ColInitRotation;
		m_ColInitScale = scd.sm_ColInitScale;

		LoadCollisionModel();
	}

	void SCDtoReal(StructCollisionData scd)
	{
		m_Collision_type = scd.sm_Collision_type;
		m_BeMoved = scd.sm_BeMoved;
		m_OnTheGroundCheckNormal = scd.sm_OnTheGroundCheckNormal;
		m_ColInitOffset = scd.sm_ColInitOffset;
		m_ColInitRotation = scd.sm_ColInitRotation;
		m_ColInitScale = scd.sm_ColInitScale;
	}

	StructCollisionData RealtoSCD()
	{
		StructCollisionData scd;

		scd.sm_Collision_type = m_Collision_type;
		scd.sm_BeMoved = m_BeMoved;
		scd.sm_OnTheGroundCheckNormal = m_OnTheGroundCheckNormal;
		scd.sm_ColInitOffset = m_ColInitOffset;
		scd.sm_ColInitRotation = m_ColInitRotation;
		scd.sm_ColInitScale = m_ColInitScale;

		return scd;
	}


	void Save(std::ofstream* Objfile, std::ofstream* ObjfileB) override;

	void Load(std::ifstream* Objfile) override;

	void CopyThisComponent(CComponent* fromComponent)	override
	{
		Collision* from = dynamic_cast<Collision*>(fromComponent);

		// m_ParentGameObject �|�C���^�[����AddComponent�̎��ɏo���Ă�̂ŃR�s�[���Ȃ�

		// �e�֐�����R�s�[�������
		m_CompId = from->m_CompId;
		m_CompName = from->m_CompName;

		// �R���W�����ŃR�s�[�������
		m_Collision_type = from->m_Collision_type;
		m_BeMoved = from->m_BeMoved;
		m_OnTheGroundCheckNormal = from->m_OnTheGroundCheckNormal;

		m_ObjectType = from->m_ObjectType;
		m_ResponseObjectList = from->m_ResponseObjectList;	// ������������v�b�V���o�b�N�Ƃ��ł��Ȃ��ƃ_������

		m_Visibility = from->GetVisibility();


		// ���ׂẴR���W�����^�C�v�Ŏg����BRadius = ColScale.x�ōl����
		m_ColInitOffset = from->m_ColInitOffset;
		m_ColOffset = from->m_ColOffset;
		m_ColRotation = from->m_ColRotation;
		m_ColInitRotation = from->m_ColInitRotation;
		m_ColScale = from->m_ColScale;
		m_ColInitScale = from->m_ColInitScale;


		m_obb = from->m_obb;
		m_capsule = from->m_capsule;
		m_sphere = from->m_sphere;
		m_ray = from->m_ray;
	}


	int GetObjectType()
	{
		return m_ObjectType;
	}
	void SetObjectType(int objecttype)
	{
		m_ObjectType = objecttype;
	}

	std::vector<int> GetResponseObjectList()
	{
		return m_ResponseObjectList;
	}
	void SetResponseObjectList(std::vector<int> responseobjectlist)
	{
		m_ResponseObjectList = responseobjectlist;
	}

	// �������̔ԍ��̃��X�|���X��Ԃ�
	int GetResponseObject(int listnum)
	{
		return m_ResponseObjectList[listnum];
	}
	void SetResponseObject(int objectnum, int responsenum)
	{
		m_ResponseObjectList[objectnum] = responsenum;
	}

	// �����ɒǉ�����
	void AddResponseObjectList(int response)
	{
		m_ResponseObjectList.push_back(response);
	}
	// �w�����폜���ċl�߂�
	void DeleteResponseObjectList(int num)
	{
		auto itr = m_ResponseObjectList.begin();
		for (int i = 0; i < num; i++)
			itr++;

		m_ResponseObjectList.erase(itr);
	}

	int ResponseObject(int objecttype)
	{
		return m_ResponseObjectList[objecttype];
	}

	// ����������ɑ΂��Ă̔���
	int CheckResponseOther(Collision* other_collision)
	{
		// ����̃I�u�W�F�N�g�^�C�v
		int other_objtype = other_collision->GetObjectType();

		// ����̃I�u�W�F�N�g�^�C�v�ɑ΂��鎩���̔���
		int self_Response = ResponseObject(other_objtype);

		// �����̃I�u�W�F�N�g�^�C�v�ɑ΂��鑊��̔���
		int other_Response = other_collision->ResponseObject(m_ObjectType);

		// ���݂���������甽�����Ȃ��BRESPONSE_IGNORE
		if (self_Response != other_Response)
			return RESPONSE_IGNORE;

		// ���݂�����������Ȃ炻���Ԃ�
		return self_Response;
	}



	bool AddCollisionStateToParentObject(Collision* colA, Collision* colB, int response, 
		D3DXVECTOR3 cpos)
	{

		Collision* self = nullptr;
		Collision* other = nullptr;
		if (colA == this)
		{
			self = colA;
			other = colB;
		}
		else
		{
			self = colB;
			other = colA;
		}

		// Overlap���̒ǉ�
		if (response == RESPONSE_OVERLAP)
		{
			self->m_ParentGameObject->Collision_AddOverlapObject
			(other->m_ParentGameObject, cpos);

			other->m_ParentGameObject->Collision_AddOverlapObject
			(self->m_ParentGameObject, cpos);

			// Overlap�̏ꍇ�������Ȃ��̂ł����ŏI���B
			return true;
		}

		// Hit���̒ǉ�
		if (response == RESPONSE_BLOCK)
		{
			self->m_ParentGameObject->Collision_AddHitObject
			(other->m_ParentGameObject, cpos);

			other->m_ParentGameObject->Collision_AddHitObject
			(self->m_ParentGameObject, cpos);

			// Hit�̏ꍇ�܂�������
			return false;
		}

		return false;
	}

	D3DXMATRIX GetWorldMatrixCollision();

	void SetVisibility(bool visibility)
	{
		m_Visibility = visibility;
	}
	bool GetVisibility()
	{
		return m_Visibility;
	}


	bool BackAndSetPositionForMeshFieldObject(Int2* pOut_block, const float& halfheight);

	void CalcCollisionMeshFieldObject(const float& halfheight);

};