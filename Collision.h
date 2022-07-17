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
static const int	CAPUSULE_COLLISION = 1;

class Collision : public CComponent
{
private:
	//Point		m_CollisionOffset = { 0.0f, 0.0f, 0.0f };			// Offset
	int			m_Collision_type = 0;		// �J�v�Z���q���W�����Ȃ̂��{�b�N�X�Ȃ̂��Ƃ���ݒ肷��
	bool		m_Movable = false;


	// �J�v�Z���R���W����
	D3DXVECTOR2 m_Capusule_Size = D3DXVECTOR2(50.0f, 40.0f);		// �J�v�Z���̏㉺�̃X�t�B�A�̔��a�ƁA�^�񒆂̃{�b�N�X�̍���

	// �X�t�B�A�R���W����
	float		m_Sphere_Radius = 50.0f;			// �X�t�B�A�̔��a

	// �{�b�N�X�R���W����
	D3DXVECTOR2 m_Box_Size = D3DXVECTOR2(20.0f, 20.0f);				// �{�b�N�X�R���W�����̉����Əc��

	//Capsule		m_Capsule;

	Model* m_ModelA;
	Model* m_ModelB;
	Model* m_ModelC;

	D3DXVECTOR3 m_ColScale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 m_ColRotation = { 0.0f, 0.0f, 0.0f };

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;


	OBB			m_obb;
	CAPSULE		m_capsule;




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
		if (m_Collision_type == CAPUSULE_COLLISION)
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

		// �v�Z�p�̕ϐ����Q�b�g
		//D3DXVECTOR3 t_Position = m_ParentGameObject->GetTempPosition();
		//D3DXVECTOR3 t_Velocity = m_ParentGameObject->GetTempVelocity();
		D3DXVECTOR3 t_Position;
		D3DXVECTOR3 t_Velocity;

		// WorldMatrix���擾���Ă���
		D3DXMATRIX selfMatrix = m_ParentGameObject->GetWorldMatrix();

		t_Position = m_ParentGameObject->GetPosition();		// �Q�b�g
		t_Velocity = m_ParentGameObject->GetVelocity();		// �Q�b�g

		m_capsule.m_Radius = 0.5f;
		m_capsule.SetCenter(t_Position);

		m_obb.CreateOBB(D3DXVECTOR3(0.5f, 0.5f, 0.5f), selfMatrix);

		int AAA = 111;
		_RPTN(_CRT_WARN, "deleting %f\n", selfMatrix._11);


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
				// WorldMatrix���擾���Ă���
				D3DXMATRIX otherMatrix = other->GetWorldMatrix(); 
				
				other->GetComponent<Collision>()->m_obb.CreateOBB(D3DXVECTOR3(0.5f, 0.5f, 0.5f), otherMatrix);

				//Mat4x4

				int flagu = 0;
				D3DXVECTOR3 ret  = ClosestPtCapsuleOBB(m_capsule, other->GetComponent<Collision>()->m_obb, flagu);
				//ClosestPtPointOBB(t_Position, other->GetComponent<Collision>()->m_obb, ret);
				int asdd = 0;

				t_Position = m_ParentGameObject->GetPosition();		// �Q�b�g
				D3DXVECTOR3 distance = ret - t_Position;
				float length = D3DXVec3Length(&distance);


				if (m_ParentGameObject->Gettagnum() == 2)
				{
					if (length <= 0.5f)
					{
						float abb = 4.0f;
						other->SetVelocity(D3DXVECTOR3(0, 0.1f, 0));
						
						//_RPTN(_CRT_WARN, "deleting %f\n", selfMatrix[2]);

						asdd = 4;
					}
				}



				// �J�v�Z��(����)�ƃJ�v�Z��(����)�̏Փ˂�������
				if (m_Collision_type == CAPUSULE_COLLISION)
				{
					//CapsuleCapsule(other, &t_Position);
				}
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
			m_ParentGameObject->SetVelocity_y(0.0f);
			//t_Position.y = groundHeight;
			//t_Velocity.y = 0.0f;
		}




		//// m_TempVelocity�̍X�V
		//m_ParentGameObject->SetTempVelocity(t_Velocity);

		//// m_Position�̍X�V
		//m_ParentGameObject->SetTempPosition(t_Position);

		//// �{����Temporary���X�V
		//m_ParentGameObject->TemporarySetUpdate();
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
			D3DXVECTOR3 OBBpos;
			OBBpos.x = m_obb.m_Center.x;
			OBBpos.y = m_obb.m_Center.y;
			OBBpos.z = m_obb.m_Center.z;

			D3DXVECTOR3 OBBScale;
			OBBScale.x = m_obb.m_Size.x;
			OBBScale.y = m_obb.m_Size.y;
			OBBScale.z = m_obb.m_Size.z;

			// ���[���h�}�g���N�X�ݒ�
			D3DXMATRIX world, scale, rot, trans;
			D3DXMatrixScaling(&scale, OBBScale.x, OBBScale.y, OBBScale.z);
			D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
			D3DXMatrixTranslation(&trans, OBBpos.x, OBBpos.y, OBBpos.z);
			world = scale * rot * trans;
			Renderer::SetWorldMatrix(&world);

			m_ModelA->Draw();
		}

		if (m_Collision_type == CAPUSULE_COLLISION)
		{
			D3DXVECTOR3 Cylinder;
			Cylinder.x = m_capsule.GetCenter().x;
			Cylinder.y = m_capsule.GetCenter().y;
			Cylinder.z = m_capsule.GetCenter().z;

			D3DXVECTOR3 CylinderScale;
			CylinderScale.x = m_capsule.m_Radius;
			CylinderScale.y = m_capsule.GetHeightRadius();
			CylinderScale.z = m_capsule.m_Radius;

			// ���[���h�}�g���N�X�ݒ�
			D3DXMATRIX world, scale, rot, trans;
			D3DXMatrixScaling(&scale, CylinderScale.x, CylinderScale.y, CylinderScale.z);
			D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
			D3DXMatrixTranslation(&trans, Cylinder.x, Cylinder.y, Cylinder.z);
			world = scale * rot * trans;
			Renderer::SetWorldMatrix(&world);

			m_ModelA->Draw();

			D3DXVECTOR3 Sphere1;
			Sphere1.x = m_capsule.m_PointUpper.x;
			Sphere1.y = m_capsule.m_PointUpper.y;
			Sphere1.z = m_capsule.m_PointUpper.z;

			D3DXVECTOR3 SphereScale;
			SphereScale.x = m_capsule.m_Radius;
			SphereScale.y = m_capsule.m_Radius;
			SphereScale.z = m_capsule.m_Radius;

			//D3DXVECTOR3 vec;
			//vec = m_Capsule.s.GetNormalVec();

			// ��̋��̕`��
			D3DXMatrixScaling(&scale, SphereScale.x, SphereScale.y, SphereScale.z);
			D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
			//D3DXMatrixTranslation(&trans, m_Capsule.s.GetPoint1().x, m_Capsule.s.GetPoint1().y, m_Capsule.s.GetPoint1().z);
			//D3DXMatrixTranslation(&trans, m_Capsule.GetPoint1().x, m_Capsule.GetPoint1().y, m_Capsule.GetPoint1().z);
			D3DXMatrixTranslation(&trans, Sphere1.x, Sphere1.y, Sphere1.z);
			world = scale * rot * trans;
			Renderer::SetWorldMatrix(&world);
			m_ModelB->Draw();

			D3DXVECTOR3 Sphere2;
			Sphere2.x = m_capsule.m_PointLower.x;
			Sphere2.y = m_capsule.m_PointLower.y;
			Sphere2.z = m_capsule.m_PointLower.z;

			// ���̋��̕`��
			D3DXMatrixScaling(&scale, SphereScale.x, SphereScale.y, SphereScale.z);
			D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
			//D3DXMatrixTranslation(&trans, m_Capsule.s.GetPoint2().x, m_Capsule.s.GetPoint2().y, m_Capsule.s.GetPoint2().z);
			//D3DXMatrixTranslation(&trans, m_Capsule.GetPoint2().x, m_Capsule.GetPoint2().y, m_Capsule.GetPoint2().z);
			D3DXMatrixTranslation(&trans, Sphere2.x, Sphere2.y, Sphere2.z);
			world = scale * rot * trans;
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

	//void CapsuleCapsule(GameObject* other, D3DXVECTOR3* t_Position)
	//{
	//	// �Փ˂����Ƃ��̃J�v�Z���̒��S�A�R�A�ɂȂ��Ă�������̒��ł̍ŒZ�����ɂȂ�Ƃ��̍��W���Ԃ����
	//	Point c_p1;
	//	Point c_p2;
	//	// c_p1����c_p2�ւ̒P�ʕ����x�N�g��
	//	Vec3 c_UnitNormal;
	//	// �ᔽ���Ă��鋗��
	//	float c_vd;		// violatedistance
	//	// �����x�N�g���B�P�ʕ����x�N�g���ƈᔽ���������������ړ�������Ώd�Ȃ��h�����Ƃ��ł���B
	//	Vec3 c_Normal;
	//	// �Փ˂��Ă���Ƃ��̍ŒZ���W�������ɕۑ�����

	//	if (colCapsuleCapsule(m_Capsule, other->GetComponent<Collision>()->m_Capsule, &c_p1, &c_p2, &c_vd))
	//	{
	//		// �Փ˂��Ă���Ȃ�


	//		// �d�Ȃ��������W��߂�t_Position
	//		c_UnitNormal = (c_p1 - c_p2).getNorm();
	//		c_Normal = c_UnitNormal * c_vd;				// �����x�N�g��		

	//		// �������Ă�����̂������Ȃ����̂Ȃ�
	//		if (!other->GetComponent<Collision>()->m_Movable)
	//		{
	//			// �������������̂Ȃ�
	//			if (m_Movable)
	//			{
	//				// �����x�N�g��������������߂��Ă�����
	//				(*t_Position).x += c_Normal.x;
	//				(*t_Position).y += c_Normal.y;
	//				(*t_Position).z += c_Normal.z;
	//			}
	//		}
	//		else
	//		{

	//			// �������Ă�����̂��ړ����������ꍇ�ǂ�����e��0.5�{�œ�����
	//			// �������������̂Ȃ�
	//			if (m_Movable)
	//			{
	//				(*t_Position).x += c_Normal.x * 0.5f;
	//				(*t_Position).y += c_Normal.y * 0.5f;
	//				(*t_Position).z += c_Normal.z * 0.5f;
	//			}

	//			// �������Ă�����̂̃|�W�V������ς���B
	//			// �{���̓R���W�����ɂ���1�����o�ϐ��ǉ����āAD3DXVECTOR3�ő��̃R���W��������̉e���ǂꂭ�炢�󂯂���
	//			// ��ۑ����āA�R���W������Update�̍ŏ��ɂ�������̍��W�ɒǉ����Ă�����v�Z�̂����������B
	//			D3DXVECTOR3 DX_c_Normal = { c_Normal.x, c_Normal.y, c_Normal.z };
	//			DX_c_Normal *= -1;		// �x�N�g���𔽓]������

	//			D3DXVECTOR3 HitObjPos = other->GetPosition();
	//			D3DXVECTOR3 HitObjSetpos = HitObjPos + (DX_c_Normal * 0.5);
	//			other->SetPosition(HitObjSetpos);
	//		}


	//		//t_Velocity.x = 0.0f;
	//		//t_Velocity.y = 0.0f;
	//		//t_Velocity.z = 0.0f;

	//	}
	//	else
	//	{
	//		// �Փ˂��Ă��Ȃ��Ȃ�
	//		int aaasd = 4;
	//	}
	//}

	//void CapsuleBox(GameObject* other, D3DXVECTOR3* t_Position)
	//{
	//	// �Փ˂����Ƃ��̃J�v�Z���̒��S�A�R�A�ɂȂ��Ă�������̒��ł̍ŒZ�����ɂȂ�Ƃ��̍��W���Ԃ����
	//	Point c_p1;
	//	Point c_p2;
	//	// c_p1����c_p2�ւ̒P�ʕ����x�N�g��
	//	Vec3 c_UnitNormal;
	//	// �ᔽ���Ă��鋗��
	//	float c_vd;		// violatedistance
	//	// �����x�N�g���B�P�ʕ����x�N�g���ƈᔽ���������������ړ�������Ώd�Ȃ��h�����Ƃ��ł���B
	//	Vec3 c_Normal;
	//	// �Փ˂��Ă���Ƃ��̍ŒZ���W�������ɕۑ�����

	//	if (colCapsuleCapsule(m_Capsule, other->GetComponent<Collision>()->m_Capsule, &c_p1, &c_p2, &c_vd))
	//	{
	//		// �Փ˂��Ă���Ȃ�


	//		// �d�Ȃ��������W��߂�t_Position
	//		c_UnitNormal = (c_p1 - c_p2).getNorm();
	//		c_Normal = c_UnitNormal * c_vd;				// �����x�N�g��		

	//		// �������Ă�����̂������Ȃ����̂Ȃ�
	//		if (!other->GetComponent<Collision>()->m_Movable)
	//		{
	//			// �������������̂Ȃ�
	//			if (m_Movable)
	//			{
	//				// �����x�N�g��������������߂��Ă�����
	//				(*t_Position).x += c_Normal.x;
	//				(*t_Position).y += c_Normal.y;
	//				(*t_Position).z += c_Normal.z;
	//			}
	//		}
	//		else
	//		{

	//			// �������Ă�����̂��ړ����������ꍇ�ǂ�����e��0.5�{�œ�����
	//			// �������������̂Ȃ�
	//			if (m_Movable)
	//			{
	//				(*t_Position).x += c_Normal.x * 0.5f;
	//				(*t_Position).y += c_Normal.y * 0.5f;
	//				(*t_Position).z += c_Normal.z * 0.5f;
	//			}

	//			// �������Ă�����̂̃|�W�V������ς���B
	//			// �{���̓R���W�����ɂ���1�����o�ϐ��ǉ����āAD3DXVECTOR3�ő��̃R���W��������̉e���ǂꂭ�炢�󂯂���
	//			// ��ۑ����āA�R���W������Update�̍ŏ��ɂ�������̍��W�ɒǉ����Ă�����v�Z�̂����������B
	//			D3DXVECTOR3 DX_c_Normal = { c_Normal.x, c_Normal.y, c_Normal.z };
	//			DX_c_Normal *= -1;		// �x�N�g���𔽓]������

	//			D3DXVECTOR3 HitObjPos = other->GetPosition();
	//			D3DXVECTOR3 HitObjSetpos = HitObjPos + (DX_c_Normal * 0.5);
	//			other->SetPosition(HitObjSetpos);
	//		}


	//		//t_Velocity.x = 0.0f;
	//		//t_Velocity.y = 0.0f;
	//		//t_Velocity.z = 0.0f;

	//	}
	//	else
	//	{
	//		// �Փ˂��Ă��Ȃ��Ȃ�
	//		int aaasd = 4;
	//	}
	//}

};
