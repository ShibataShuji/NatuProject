#pragma once

#include "main.h"
#include "model.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "Player.h"
#include "gameObject.h"
#include "CComponent.h"
#include "primitive.h"

//#include "testhghdfvs.h"
//#include "gameObject.h"

//// �f�t�H���g�̃R���W�����̃��f���̃T�C�Y
//static const float def_Size = 100.0f;

// �R���W�����^�C�v�̒l�̐ݒ�
static const int	BOX_COLLISION	   = 0;
static const int	CAPSULE_COLLISION = 1;

class Collision : public CComponent
{
private:
	int			m_Collision_type = 0;		// �J�v�Z���q���W�����Ȃ̂��{�b�N�X�Ȃ̂��Ƃ���ݒ肷��
	bool		m_Movable = false;
	float		m_OnTheGroundCheckNormal = 0.7f;	// 0.0f~1.0f �ڒn����̎��A�Փ˂������̕����x�N�g��y������ȏ�̂Ƃ��ɐڒn�����true�ɂ���
	

	// ���ׂẴR���W�����^�C�v�Ŏg����BRadius = Scale.x�ōl����
	D3DXVECTOR3 m_ColScale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 m_ColRotation = { 0.0f, 0.0f, 0.0f };

	OBB			m_obb;
	CAPSULE		m_capsule;


	Model* m_ModelA;
	Model* m_ModelB;
	Model* m_ModelC;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;




public:

	// �e�N���X�̃R���X�g���N�^������Ă����B
	using CComponent::CComponent;

	// model�̃��[�h�����BInit����Player��Init��葁���s����̂ŃR���W�����^�C�v���Z�b�g�����O�ɂȂ��Ă��܂��̂ŕ����Ă�
	void LoadCollisionModel()
	{
		if (m_Collision_type == BOX_COLLISION)
		{
			m_ModelA = new Model();
			m_ModelA->Load("asset\\model\\ColCube.obj");
			m_ModelB = new Model();
			m_ModelB->Load("asset\\model\\ColCube.obj");
			m_ModelC = new Model();
			m_ModelC->Load("asset\\model\\ColCube.obj");
		}
		if (m_Collision_type == CAPSULE_COLLISION)
		{
			m_ModelA = new Model();
			m_ModelA->Load("asset\\model\\ColCylinder.obj");
			m_ModelB = new Model();
			m_ModelB->Load("asset\\model\\ColSphere.obj");
			m_ModelC = new Model();
			m_ModelC->Load("asset\\model\\ColSphere.obj");
		}
	}


	void Init() override
	{


		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

		Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");


	}

	void Uninit() override
	{
		m_ModelA->Unload();
		delete m_ModelA;
		m_ModelB->Unload();
		delete m_ModelB;
		m_ModelC->Unload();
		delete m_ModelC;

		m_VertexLayout->Release();
		m_VertexShader->Release();
		m_PixelShader->Release();
	}

	OBB GetOBB()
	{
		return m_obb;
	}

	void Update() override
	{
		// �V�[�����Q�b�g���Ă���
		Scene* scene = Manager::GetScene();

		// �ڒn����𖈉�false�Ƀ��Z�b�g���Ă����B�ł����̃R���W������true�ɂȂ������̂����Z�b�g���Ȃ��悤��temp�Ŋm�F
		if(!m_ParentGameObject->GetTempOnTheGround())
			m_ParentGameObject->SetOnTheGround(false);			// ���Z�b�g


		// �R���W�����^�C�v�ɂ���ĉ�]�̕ۑ��̎d�����Ⴄ?
		if (m_Collision_type == BOX_COLLISION)
		{
			m_ColRotation = m_ParentGameObject->GetRotation(); 

		}
		else if (m_Collision_type == CAPSULE_COLLISION)
		{
			// �J�v�Z���͏c�Œ�
			m_ColRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//m_ColRotation = m_ParentGameObject->GetRotation();		// �ꉞ����ŃJ�v�Z������]���邯�ǋ����ꂼ��ŉ�]
		}



		// �v�Z�p�Ɏg���ϐ��̏���
		D3DXVECTOR3 t_Position;
		D3DXVECTOR3 t_Velocity;
		t_Position = m_ParentGameObject->GetPosition();		// �Q�b�g
		t_Velocity = m_ParentGameObject->GetVelocity();		// �Q�b�g

		// WorldMatrix���擾���Ă���(Scale,Rotation,Pos�̃f�[�^)
		D3DXMATRIX selfMatrix = m_ParentGameObject->GetWorldMatrix();

		// OBB�̍X�V
		m_obb.CreateOBB(m_ColScale, selfMatrix);

		// �J�v�Z���̍X�V
		m_capsule.SetCapsule(m_ColScale.x, m_ColScale.y, t_Position);

		int AAA = 111;
		//_RPTN(_CRT_WARN, "deleting %f\n", selfMatrix._11);


		// �S�ẴQ�[���I�u�W�F�N�g���擾
		std::vector<GameObject*> allObj = scene->GetAllGameObjects(1);
		for (GameObject* other : allObj)
		{
			// other�ɏՓˑ���̐e�I�u�W�F�N�g�̃|�C���^�[�������Ă���

			// ������������R���e�j���[
			if (other == m_ParentGameObject)
				continue;

			// �S�ẴI�u�W�F�N�g�̒�����Collision�R���|�[�l���g�������Ă�����̂������擾
			if (other->HasComponent<Collision>())
			{
				//static const int	BOX_COLLISION = 0;
				//static const int	CAPSULE_COLLISION = 1;

				// WorldMatrix���擾���Ă���
				D3DXMATRIX otherMatrix = other->GetWorldMatrix(); 

				// �J�v�Z��(����)��OBB(����)�̏Փ˂�������
				if (m_Collision_type == CAPSULE_COLLISION && other->GetComponent<Collision>()->GetCollisionType() == BOX_COLLISION)
				{
					CapsuleOBB(this, other->GetComponent<Collision>());
				}
				// OBB(����)�ƃJ�v�Z��(����)�̏Փ˂�������
				else if (m_Collision_type == BOX_COLLISION && other->GetComponent<Collision>()->GetCollisionType() == CAPSULE_COLLISION)
				{
					CapsuleOBB(other->GetComponent<Collision>(), this);
				}
				// �J�v�Z��(����)�ƃJ�v�Z��(����)�̏Փ˂�������
				else if (m_Collision_type == CAPSULE_COLLISION && other->GetComponent<Collision>()->GetCollisionType() == CAPSULE_COLLISION)
				{
					CapsuleCapsule(this, other->GetComponent<Collision>());
				}

				//if (m_ParentGameObject->Gettagnum() == 2)
				//{
				//	// cp_Capsule �ɂ̓J�v�Z�����̍Őڋߓ_�̍��W���擾�ł���(Closest point �Őڋߓ_)
				//	int flag;
				//	D3DXVECTOR3 cp_Capsule;
				//	cp_Capsule = ClosestPtCapsuleOBB(m_capsule, other->GetComponent<Collision>()->m_obb, flag);

				//	// ���̂܂܂��ƃJ�v�Z�����̍Őڋߓ_���J�v�Z���̊O���Ȃ̂ŁA���S�̃��C���Ōv�Z����悤�ɖ߂�(y���W�͈ێ�or�㉺�̋��̂̏ꍇ�߂�)
				//	D3DXVECTOR3 cp_CapsuleCenterLine = cp_Capsule;
				//	cp_CapsuleCenterLine.x = m_capsule.GetCenterPos().x;
				//	cp_CapsuleCenterLine.z = m_capsule.GetCenterPos().z;
				//	if (cp_Capsule.y > m_capsule.GetCenterPos().y)
				//		cp_CapsuleCenterLine.y = m_capsule.GetUpperSpherePos().y;
				//	if (cp_Capsule.y < m_capsule.GetCenterPos().y)
				//		cp_CapsuleCenterLine.y = m_capsule.GetLowerSpherePos().y;

				//	// cp_OBB �ɂ�OBB���̍Őڋߓ_�̍��W���擾�ł���
				//	D3DXVECTOR3 cp_OBB;
				//	ClosestPtPointOBB(cp_CapsuleCenterLine, other->GetComponent<Collision>()->m_obb, cp_OBB);

				//	// distance �J�v�Z������݂�OBB�ւ̍ŒZ����(�x�N�g��)
				//	D3DXVECTOR3 distance;
				//	distance = cp_OBB - cp_CapsuleCenterLine;

				//	// length �J�v�Z������OBB�ւ̍ŒZ����
				//	float length;
				//	length = D3DXVec3Length(&distance);

				//	// overlaplength �J�v�Z����OBB�̏d�Ȃ��Ă��鋗��
				//	float overlaplength;
				//	overlaplength = m_capsule.m_Radius - length;
				//	// �����x�N�g���̏���
				//	D3DXVECTOR3 normal;
				//	D3DXVec3Normalize(&normal, &distance);

				//	// �Փ˂��Ă���̏���
				//	if (length <= m_capsule.m_Radius)
				//	{
				//		// �d�Ȃ��Ă�����߂�����
				//		t_Position = m_ParentGameObject->GetPosition();		// �Q�b�g
				//		D3DXVECTOR3 backVector;
				//		backVector = normal * overlaplength;	// �����x�N�g���̂ނ��ɏd�Ȃ��Ă��鋗�����߂�

				//		OverlapToBackPosition(m_ParentGameObject, other, backVector);		// ���̒��ŃZ�b�g���Ă���

				//		// �����艺�ŏՓ˂����Ă���Ȃ�ΐڒn���Ă���Ƃ݂Ȃ�
				//		float OnTheGroundHeight;
				//		OnTheGroundHeight = m_capsule.GetLowerSpherePos().y - m_capsule.m_Radius * (1.0f - m_OnTheGroundCoefficient);


				//		// �ڒn���菈���Bflag == 1�ŏ�ɂ���B-1�ŉ��ɂ���B0�ŉ�
				//		if (flag == 1 && t_Velocity.y < 0.0f && cp_Capsule.y < OnTheGroundHeight)
				//		{
				//			m_ParentGameObject->SetOnTheGround(true);
				//			m_ParentGameObject->SetVelocity_y(0.0f);
				//		}
				//	}
				//}



				
			}
		}










	



		// �Ƃ肠�����n�ʂ�0.0f�Ƃ��Ă���
		float groundHeight = 0.0f;


		t_Position = m_ParentGameObject->GetPosition();		// �Q�b�g
		t_Velocity = m_ParentGameObject->GetVelocity();		// �Q�b�g
		// �n�ʂƂ̓����蔻��B�ڒn
		if (t_Position.y < groundHeight && t_Velocity.y < 0.0f)
		{
			m_ParentGameObject->SetPosition_y(groundHeight);
			m_ParentGameObject->SetOnTheGround(true);
			m_ParentGameObject->SetVelocity_y(0.0f);
		}

	}

	void Draw() override
	{
		// ���̓��C�A�E�g�ݒ�
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		if (m_Collision_type == BOX_COLLISION)
		{
			D3DXMATRIX world;

			// ���[���h�}�g���N�X�ݒ�
			world = CreateWorldMatrix(m_obb.m_Size, m_ColRotation, m_obb.m_Center);	// �֐��ɂ܂Ƃ߂�
			//world = CreateWorldMatrix(OBBScale, m_ColRotation, OBBpos);	// �֐��ɂ܂Ƃ߂�
			Renderer::SetWorldMatrix(&world);
			m_ModelA->Draw();

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
			m_ModelA->Draw();

			D3DXVECTOR3 SphereScale;
			SphereScale = D3DXVECTOR3(m_capsule.m_Radius, m_capsule.m_Radius, m_capsule.m_Radius);

			// ��̋��̕`��
			world = CreateWorldMatrix(SphereScale, m_ColRotation, m_capsule.GetUpperSpherePos());	// �֐��ɂ܂Ƃ߂�
			Renderer::SetWorldMatrix(&world);
			m_ModelB->Draw();

			// ���̋��̕`��
			world = CreateWorldMatrix(SphereScale, m_ColRotation, m_capsule.GetLowerSpherePos());	// �֐��ɂ܂Ƃ߂�
			Renderer::SetWorldMatrix(&world);
			m_ModelC->Draw();
		}




	}


	void SetCollisionType(int type)
	{
		m_Collision_type = type;
	}
	int GetCollisionType()
	{
		return m_Collision_type;
	}


	void SetMovable(bool movable)
	{
		m_Movable = movable;
	}
	bool GetMovable()
	{
		return m_Movable;
	}

	// 
	void SetBoxScale(D3DXVECTOR3 scale)
	{
		m_ColScale = scale;
	}

	void SetCapsuleScale(float radius, float halfheight)
	{
		m_ColScale.x = radius;
		m_ColScale.y = halfheight;
		m_ColScale.z = radius;
	}


	// �Փˎ��̉����߂�����
	void OverlapToBackPosition(GameObject* self, GameObject* other, D3DXVECTOR3 backvector)
	{

		D3DXVECTOR3 self_Pos = self->GetPosition();		// �Q�b�g
		D3DXVECTOR3 other_Pos = other->GetPosition();					// �Q�b�g

		// �Փ˂������肪�����Ȃ��Ȃ�
		if (!other->GetComponent<Collision>()->m_Movable)
		{
			// ���������|�W�V������ς���
			self->SetPosition(self_Pos - backvector);		// �Z�b�g
		}
		else if (other->GetComponent<Collision>()->m_Movable)
		{
			// �Փ˂������肪�����Ȃ�
			// �x�N�g���𔼕��ɂ���
			backvector *= 0.5f;
			// �����Ƒ���̃|�W�V�������x�N�g���̔������ς���B(�{���͂��̎��̐����Ƃ������ꂽ����������)
			self->SetPosition(self_Pos - backvector);		// �Z�b�g
			other->SetPosition(other_Pos + backvector);		// �Z�b�g
		}
	}


	//// Offset�̐ݒ�
	//void SetCollisionOffset(Point offset)
	//{
	//	m_CollisionOffset = offset;
	//}


	//// �J�v�Z���̃T�C�Y���̐ݒ�
	//void SetCapsule(Point centerpos, float radius, float height)
	//{
	//	m_Capsule.SetCapsule(Point(centerpos.x, centerpos.y, centerpos.z), radius, height);
	//}



	// �J�v�Z���ƃ{�b�N�X�̓����蔻��
	void CapsuleOBB(Collision* capsule, Collision* obb)
	{
		// cp_Capsule �ɂ̓J�v�Z�����̍Őڋߓ_�̍��W���擾�ł���(Closest point �Őڋߓ_)
		int flag;
		D3DXVECTOR3 cp_Capsule;
		cp_Capsule = ClosestPtCapsuleOBB(capsule->m_capsule, obb->m_obb, flag);

		// ���̂܂܂��ƃJ�v�Z�����̍Őڋߓ_���J�v�Z���̊O���Ȃ̂ŁA���S�̃��C���Ōv�Z����悤�ɖ߂�(y���W�͈ێ�or�㉺�̋��̂̏ꍇ�߂�)
		D3DXVECTOR3 cp_CapsuleCenterLine = cp_Capsule;
		cp_CapsuleCenterLine.x = capsule->m_capsule.GetCenterPos().x;
		cp_CapsuleCenterLine.z = capsule->m_capsule.GetCenterPos().z;
		if (cp_Capsule.y > capsule->m_capsule.GetCenterPos().y)
			cp_CapsuleCenterLine.y = capsule->m_capsule.GetUpperSpherePos().y;
		if (cp_Capsule.y < capsule->m_capsule.GetCenterPos().y)
			cp_CapsuleCenterLine.y = capsule->m_capsule.GetLowerSpherePos().y;

		// cp_OBB �ɂ�OBB���̍Őڋߓ_�̍��W���擾�ł���
		D3DXVECTOR3 cp_OBB;
		ClosestPtPointOBB(cp_CapsuleCenterLine, obb->m_obb, cp_OBB);

		// distance �J�v�Z������݂�OBB�ւ̍ŒZ����(�x�N�g��)
		D3DXVECTOR3 distance;
		distance = cp_OBB - cp_CapsuleCenterLine;

		// length �J�v�Z������OBB�ւ̍ŒZ����
		float length;
		length = D3DXVec3Length(&distance);

		// overlaplength �J�v�Z����OBB�̏d�Ȃ��Ă��鋗��
		float overlaplength;
		overlaplength = capsule->m_capsule.m_Radius - length;
		// �����x�N�g���̏���
		D3DXVECTOR3 normal;
		D3DXVec3Normalize(&normal, &distance);

		// �Փ˂��Ă���̏���
		if (length <= capsule->m_capsule.m_Radius)
		{
			D3DXVECTOR3 cap_Pos = capsule->m_ParentGameObject->GetPosition();		// �Q�b�g
			D3DXVECTOR3 cap_Vel = capsule->m_ParentGameObject->GetVelocity();		// �Q�b�g
			D3DXVECTOR3 obb_Pos = obb->m_ParentGameObject->GetPosition();			// �Q�b�g
			D3DXVECTOR3 obb_Vel = obb->m_ParentGameObject->GetVelocity();			// �Q�b�g

			// �d�Ȃ��Ă�����߂�����
			D3DXVECTOR3 backVector;
			backVector = normal * overlaplength;	// �����x�N�g���̂ނ��ɏd�Ȃ��Ă��鋗�����߂�

			OverlapToBackPosition(capsule->m_ParentGameObject, obb->m_ParentGameObject, backVector);		// ���̒��ŃZ�b�g���Ă���


			// �J�v�Z�����ڒn���菈���Bflag == 1�ŏ�ɂ���B-1�ŉ��ɂ���B0�ŉ�
			//if (flag == 1 && cap_Vel.y < 0.0f && cp_Capsule.y < OnTheGroundHeight_cap)
			if (flag == 1 && cap_Vel.y < 0.0f && -normal.y > capsule->m_OnTheGroundCheckNormal)
			{
				capsule->m_ParentGameObject->SetOnTheGround(true);
				capsule->m_ParentGameObject->SetTempOnTheGround(true);
				capsule->m_ParentGameObject->SetVelocity_y(0.0f);
			}

			// OBB���ڒn���菈���B
			if (flag == -1 && obb_Vel.y < 0.0f && normal.y > obb->m_OnTheGroundCheckNormal)
			{
				obb->m_ParentGameObject->SetOnTheGround(true);
				obb->m_ParentGameObject->SetTempOnTheGround(true);
				obb->m_ParentGameObject->SetVelocity_y(0.0f);
			}




		}
	}

	// �J�v�Z���ƃJ�v�Z���̓����蔻��
	void CapsuleCapsule(Collision* self_capsule, Collision* other_capsule)
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

			D3DXVECTOR3 scap_vel = self_capsule->m_ParentGameObject->GetVelocity();
			D3DXVECTOR3 ocap_vel = other_capsule->m_ParentGameObject->GetVelocity();

			// �d�Ȃ��������W��߂�t_Position
			D3DXVECTOR3 tvec = c_p1 - c_p2;
			D3DXVec3Normalize(&c_UnitNormal, &tvec);
			c_Normal = c_UnitNormal * c_vd;				// �߂��x�N�g��		

			OverlapToBackPosition(self_capsule->m_ParentGameObject, other_capsule->m_ParentGameObject, -c_Normal);


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



};




//_RPTN(_CRT_WARN, "%f, %f, %f\n", cp_OBB.x, cp_OBB.y, cp_OBB.z);
//_RPTN(_CRT_WARN, "%f, %f, %f\n", cp_Capsule.x, cp_Capsule.y, cp_Capsule.z);
//_RPTN(_CRT_WARN, "%f, %f, %f\n", distance.x, distance.y, distance.z);
//_RPTN(_CRT_WARN, "%f\n", length);
////_RPTN(_CRT_WARN, "%f, %f, %f\n", normal.x, normal.y, normal.z);
//_RPTN(_CRT_WARN, "%d\n", flag);