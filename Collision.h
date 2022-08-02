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
	

	// ���ׂẴR���W�����^�C�v�Ŏg����BRadius = ColScale.x�ōl����
	D3DXVECTOR3 m_ColRotation = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 m_ColScale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 m_ColInitScale = { 1.0f, 1.0f, 1.0f };


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


		// Scale�̍X�V
		D3DXVECTOR3 ScaleRate = m_ParentGameObject->GetScaleRate();
		m_ColScale.x = m_ColInitScale.x * ScaleRate.x;
		m_ColScale.y = m_ColInitScale.y * ScaleRate.y;
		m_ColScale.z = m_ColInitScale.z * ScaleRate.z;

		// �R���W�����^�C�v�ɂ��X�V
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
		//D3DXMATRIX selfMatrix = m_ParentGameObject->GetWorldMatrix();
		//D3DXMATRIX selfMatrix = CreateWorldMatrix(m_ColScale, m_ColRotation, t_Position);
		D3DXMATRIX selfMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_ColRotation, t_Position);

		// OBB�̍X�V
		m_obb.CreateOBB(m_ColScale, selfMatrix);

		// �J�v�Z���̍X�V
		m_capsule.SetCapsule(m_ColScale.x, m_ColScale.y, t_Position);

		int AAA = 111;
		//_RPTN(_CRT_WARN, "deleting %f\n", selfMatrix._11);

		//// ���ʍ��v�Z�B�����ł��Ȃ����Ǎ��̓e�X�g�ł���Ă�
		//m_obb.CreatePlaneState(m_ColRotation);
		//// m_obb.m_Normal[1].x
		//
		//int asdff = 4;
		//_RPTN(_CRT_WARN, "AAA %f, %f\n", m_obb.m_Normal[1].x, m_obb.m_Normal[1].z);




		// �S�ẴQ�[���I�u�W�F�N�g���擾
		std::vector<GameObject*> allObj = scene->GetAllGameObjects(1);
		for (GameObject* other : allObj)
		{
			// other�ɏՓˑ���̐e�I�u�W�F�N�g(�Q�[���I�u�W�F�N�g)�̃|�C���^�[�������Ă���

			// ������������R���e�j���[
			if (other == m_ParentGameObject)
				continue;

			// �S�ẴI�u�W�F�N�g�̒�����Collision�R���|�[�l���g�������Ă�����̂������擾
			if (other->HasComponent<Collision>())
			{
				// �ȗ��p��other�̃R���W�����̃|�C���^��p��
				Collision* other_collision = other->GetComponent<Collision>();

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
				// OBB(����)��OBB(����)�̏Փ˂�������
				else if (m_Collision_type == BOX_COLLISION && other->GetComponent<Collision>()->GetCollisionType() == BOX_COLLISION)
				{
					// �ȗ��p�ɑ����obb�̃|�C���^��p�ӁB������m_obb
					OBB* other_obb = &other_collision->m_obb;	//other_collision
					/*D3DXMATRIX other_Matrix = CreateWorldMatrix(other_collision->m_ColScale, other_collision->m_ColRotation, other_collision->m_obb.m_Center);
					other_obb->CreateOBB(other_collision->m_ColScale, other_Matrix);*/

					//bool bbb = OBBOBB(this, other->GetComponent<Collision>());

					//int ccc = 10;
					//if (bbb == false)
					//	ccc = 0;
					//else
					//	ccc = 1;
					////_RPTN(_CRT_WARN, "FPS %d\n", ccc);


					// �l�̎擾�E����
					// ������ DivisionNum
					int DivNum = 100;

					// Postion
					D3DXVECTOR3 self_oldPos = m_ParentGameObject->GetOldPosition();		// �O��̃t���[���ł̍��W
					D3DXVECTOR3 other_oldPos = other->GetOldPosition();
					D3DXVECTOR3 self_AfterPos = m_ParentGameObject->GetPosition();		// �Փ˂��Ȃ������ꍇ�̍ŏI�ړ����W
					D3DXVECTOR3 other_AfterPos = other->GetPosition();
					D3DXVECTOR3 self_movementFrame = self_AfterPos - self_oldPos;		// ���̃t���[���ł̈ړ��x�N�g��
					D3DXVECTOR3 other_movementFrame = other_AfterPos - other_oldPos;
					float self_VecLength = D3DXVec3Length(&self_movementFrame);			// �x�N�g���̒���
					float other_VecLength = D3DXVec3Length(&other_movementFrame);
					D3DXVECTOR3 self_Normal;
					D3DXVECTOR3 other_Normal;
					D3DXVec3Normalize(&self_Normal, &self_movementFrame);				// ���̃t���[���ł̕����x�N�g��(�P�ʃx�N�g��)������Ă���
					D3DXVec3Normalize(&other_Normal, &other_movementFrame);

					D3DXVECTOR3 self_movementOne = self_movementFrame / DivNum;		// ���������Ƃ��̂P��ł̈ړ�����
					D3DXVECTOR3 other_movementOne = other_movementFrame / DivNum;


					// Rotation
					D3DXVECTOR3 self_oldRotation = m_ParentGameObject->GetOldRotation();		// �O��̃t���[���ł̉�]
					D3DXVECTOR3 other_oldRotation = other->GetOldRotation();
					D3DXVECTOR3 self_AfterRotation = m_ParentGameObject->GetRotation();		// �Փ˂��Ȃ������ꍇ�̍ŏI��]
					D3DXVECTOR3 other_AfterRotation = other->GetRotation();
					D3DXVECTOR3 self_RotationFrame = self_AfterRotation - self_oldRotation;		// �Փ˂��Ȃ��ꍇ�̂��̃t���[���ł̉�]��
					D3DXVECTOR3 other_RotationFrame = other_AfterRotation - other_oldRotation;

					D3DXVECTOR3 self_RotationOne = self_RotationFrame / DivNum;		// ���������Ƃ��̂P��ł̉�]��
					D3DXVECTOR3 other_RotationOne = other_RotationFrame / DivNum;


					// Scale
					D3DXVECTOR3 self_oldScale = m_ParentGameObject->GetOldScaleRate();		// �O��̃t���[���ł�ScaleRate
					self_oldScale.x *= m_ColInitScale.x;
					self_oldScale.y *= m_ColInitScale.y;
					self_oldScale.z *= m_ColInitScale.z;
					D3DXVECTOR3 other_oldScale = other->GetOldScaleRate();
					other_oldScale.x *= m_ColInitScale.x;
					other_oldScale.y *= m_ColInitScale.y;
					other_oldScale.z *= m_ColInitScale.z;
					D3DXVECTOR3 self_AfterScale = m_ParentGameObject->GetScaleRate();		// �Փ˂��Ȃ������ꍇ�̍ŏIScaleRate
					self_AfterScale.x *= m_ColInitScale.x;
					self_AfterScale.y *= m_ColInitScale.y;
					self_AfterScale.z *= m_ColInitScale.z;
					D3DXVECTOR3 other_AfterScale = other->GetScaleRate();
					other_AfterScale.x *= m_ColInitScale.x;
					other_AfterScale.y *= m_ColInitScale.y;
					other_AfterScale.z *= m_ColInitScale.z;
					D3DXVECTOR3 self_ScaleFrame = self_AfterScale - self_oldScale;		// �Փ˂��Ȃ��ꍇ�̂��̃t���[���ł�ScaleRate�̕ω���
					D3DXVECTOR3 other_ScaleFrame = other_AfterScale - other_oldScale;

					D3DXVECTOR3 self_ScaleOne = self_ScaleFrame / DivNum;		// ���������Ƃ��̂P��ł�ScaleRate�̕ω���
					D3DXVECTOR3 other_ScaleOne = other_ScaleFrame / DivNum;

					//// WorldMatrix After�o�[�W����
					//D3DXMATRIX self_AfterMatrix = CreateWorldMatrix(self_AfterScale, self_AfterRotation, self_AfterPos);
					D3DXMATRIX self_AfterMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), self_AfterRotation, self_AfterPos);
					// OBB  After�o�[�W����
					OBB self_AfterOBB;
					self_AfterOBB.CreateOBB(self_AfterScale, self_AfterMatrix);

					// �Փ˂��Ă�����
					if (OBBOBB(this, other->GetComponent<Collision>()))
					{
						// �Փˑ����OBB�̍X�V����Ȃ�����
						/*D3DXMATRIX other_Matrix = CreateWorldMatrix(other_collision->m_ColScale, other_collision->m_ColRotation, other_collision->m_obb.m_Center);
						other_obb->CreateOBB(other_collision->m_ColScale, other_Matrix);*/


						// ���肪�����Ȃ��Ȃ玩����������������
						if (other->GetComponent<Collision>()->m_Movable == false)
						{
							//// WorldMatrix��Old�ō�蒼��old
							//D3DXMATRIX self_oldMatrix = CreateWorldMatrix(self_oldScale, self_oldRotation, self_oldPos);
							D3DXMATRIX self_oldMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), self_oldRotation, self_oldPos);

							// OBB����蒼��old
							OBB self_oldOBB;
							self_oldOBB.CreateOBB(self_oldScale, self_oldMatrix);

							D3DXVECTOR3 self_bcPos;	//before collision �Փ˂��钼�O
							D3DXVECTOR3 self_bcRot;
							D3DXVECTOR3 self_bcSca;
							int MaxTrial = 50;
							int low = 0;
							int high = DivNum;
							//int mid = (low + high) / 2;
							int mid;
							int oldmid = -1;
							for (int i = 0; i < MaxTrial; i++)
							{
								mid = (low + high) / 2;

								// 2��A����mid�������l��������
								if (oldmid == mid)
								{
									break;
								}

								self_bcPos = self_oldPos + (self_movementOne * mid);
								self_bcRot = self_oldRotation + (self_RotationOne * mid);
								self_bcSca = self_oldScale + (self_ScaleOne * mid);

								//// WorldMatrix����蒼��new
								//D3DXMATRIX self_bcMatrix = CreateWorldMatrix(self_bcSca, self_bcRot, self_bcPos);
								D3DXMATRIX self_bcMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), self_bcRot, self_bcPos);

								// OBB����蒼��new
								OBB self_bcOBB;
								self_bcOBB.CreateOBB(self_bcSca, self_bcMatrix);

								// OBB�̏㏑��
								m_obb = self_bcOBB;
								
								if (OBBOBB(this, other->GetComponent<Collision>()))
								{
									// �Փ˂��Ă���Ȃ�A���������X�ɖ߂�
									high = mid;
								}
								else
								{
									// �Փ˂��Ă��Ȃ��Ȃ�A���������X�ɐi�߂�
									low = mid;
								}
								
								//if (low >= high)
								//{
								//	_RPTN(_CRT_WARN, "GGG %d\n", ggg);
								//	break;
								//}

								oldmid = mid;	// mid�̒l�̕ۑ�
							}
							//// ����������Փ˃M���M������������mid�Ȃ̂ŁAmid�ȍ~�̒l�����点��K�v������
							//// �����x�N�g�����A�Փˑ����OBB�̖ʂɑ΂��ĕ��s�ɂȂ�悤�ɋȂ���Bself_movementFrame
							//float RemainingRatio = (float)mid / (float)DivNum;			// �c��̊���
							//float RemainingLenght = self_VecLength * RemainingRatio;	// �c��̒���
							//D3DXVECTOR3 self_RemainingVec = self_movementFrame * RemainingRatio;		// �c��̈ړ��x�N�g��
							
							// mid�ɂ͏Փ˒��O(�Փ˂��Ă��Ȃ�)(mid+1�͏Փ˂��Ă���)
							// �Փ˒��O�̏�񂪋��߂�ꂽ�̂Ŏ���
							// �Е���OBB��8�̒��_�̂����ǂ̒��_�������Е���OBB�ɂP�ԋ߂��������߂�
							// �܂��͕��ʂɎg�������X�V������(m_obb�ɂ�self_bcOBB���㏑���ς݂Ȃ̂ł����ł͏����Ȃ��Ă悢)
							m_obb.CreatePlaneState(self_bcRot);						// �����o�ϐ��̕��ʂɎg�����̂��X�V�����
							other_obb->CreatePlaneState(other_AfterRotation);		// ���葤�͂��̏ꍇ�����Ȃ�(�����瑤�̉e�����󂯂Ȃ�)�̂ł���ŁB

							// A��OBB�ɑ΂��Ĉ�ԋ߂�B��OBB�̒��_�̓Y�������Ԃ��Ă���
							// ���̌�߂�����������������B�ɐݒ肵�Ă�����Ƃ����B
							int NearVer = GetOBBOBBNearestVertex(*other_obb, m_obb);

							float x = m_obb.m_VertexPos[NearVer].x;
							float y = m_obb.m_VertexPos[NearVer].y;
							float z = m_obb.m_VertexPos[NearVer].z;

							// ���ɂ��̒��_���ǂ̖ʂƈ�ԋ߂��̂����v�Z����
							D3DXVECTOR3 distancevec;					// �߂�l�p�B��ԋ߂��ʂ��琂���ȋ������Ԃ�
							int NearPlane = GetPointOBBNearestPlane(&distancevec, m_obb.m_VertexPos[NearVer], *other_obb);

							_RPTN(_CRT_WARN, "NearVer %d\n", NearVer);
							_RPTN(_CRT_WARN, "NearPlane %d\n", NearPlane);

							// �Փ˂������_�Ɩʂ����܂����̂ŁA�Փˌ�̍��W�ŁA���̒��_�����̖ʂ�
							// �ǂꂾ���߂荞��ł邩���v�Z���Ȃ����B�߂荞��ł镪�ʂ̖@���x�N�g���̕����ɖ߂�

							// OBB��After�o�[�W�����ŏ㏑��
							m_obb = self_AfterOBB;
							m_obb.CreatePlaneState(self_AfterRotation);	
							distancevec = CalcPlaneToPoint(m_obb, NearVer, *other_obb, NearPlane);

							//D3DXVECTOR3 self_newPos = self_AfterPos - distancevec;
							//selfMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), self_AfterRotation, self_newPos);
							//// OBB�̍X�V
							//m_obb.CreateOBB(m_ColScale, selfMatrix);
							
							OverlapToBackPosition(this->m_ParentGameObject, other, -distancevec);		// ���̒��ŃZ�b�g���Ă���
							
							// ���_���P���Փ˂��Ă��Ȃ��ꍇ�͐������m���Փ˂��Ă���̂�
							// ClosestPtSegmentSegment ���g��

							//// �v�Z�ŋ��߂��Փ˂��Ȃ����肬��̎��Ԃɖ߂��Ă�����
							//m_ParentGameObject->SetPosition(self_bcPos);
							//m_ParentGameObject->SetRotation(self_bcRot);
							//m_ParentGameObject->SetScaleRate(self_bcSca);
							
							//m_ParentGameObject->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
							//m_ParentGameObject->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

							//_RPTN(_CRT_WARN, "RRR %d\n", rrr);

							

						}

					}



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
			//world = CreateWorldMatrix(m_obb.m_Size, m_ColRotation, m_obb.m_Center);	// �֐��ɂ܂Ƃ߂�
			world = CreateWorldMatrix(m_ColScale, m_ColRotation, m_obb.m_Center);	// �֐��ɂ܂Ƃ߂�
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


	void SetColInitScale(D3DXVECTOR3 setcolscale)
	{
		m_ColInitScale = setcolscale;
		m_ColScale = m_ColInitScale;
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








	// OBB v.s. OBB
	//bool OBBOBB(OBB& obb1, OBB& obb2)
	bool OBBOBB(Collision* self_obb, Collision* other_obb)
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







};






//// OBB v.s. OBB
//bool OBBOBB(OBB& obb1, OBB& obb2)
//{
//	// �e�����x�N�g���̊m��
//	// �iN***:�W���������x�N�g���j
//	D3DXVECTOR3 NAe1 = obb1.GetRotNormalVector(0), Ae1 = NAe1 * obb1.GetRotNormalLength(0);
//	D3DXVECTOR3 NAe2 = obb1.GetRotNormalVector(1), Ae2 = NAe2 * obb1.GetRotNormalLength(1);
//	D3DXVECTOR3 NAe3 = obb1.GetRotNormalVector(2), Ae3 = NAe3 * obb1.GetRotNormalLength(2);
//	D3DXVECTOR3 NBe1 = obb2.GetRotNormalVector(0), Be1 = NBe1 * obb2.GetRotNormalLength(0);
//	D3DXVECTOR3 NBe2 = obb2.GetRotNormalVector(1), Be2 = NBe2 * obb2.GetRotNormalLength(1);
//	D3DXVECTOR3 NBe3 = obb2.GetRotNormalVector(2), Be3 = NBe3 * obb2.GetRotNormalLength(2);
//	D3DXVECTOR3 Interval = obb1.m_Center - obb2.m_Center;
//
//	// ������ : Ae1
//	FLOAT rA = D3DXVec3Length(&Ae1);
//	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
//	FLOAT L = fabs(D3DXVec3Dot(&Interval, &NAe1));
//	if (L > rA + rB)
//		return false; // �Փ˂��Ă��Ȃ�
//
//	 // ������ : Ae2
//	rA = D3DXVec3Length(&Ae2);
//	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NAe2));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : Ae3
//	rA = D3DXVec3Length(&Ae3);
//	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NAe3));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : Be1
//	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be1);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe1));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : Be2
//	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe2));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : Be3
//	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe3));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : C11
//	D3DXVECTOR3 Cross;
//	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : C12
//	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : C13
//	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : C21
//	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : C22
//	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : C23
//	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : C31
//	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : C32
//	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// ������ : C33
//	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
//	return true;
//}
//
//
//// �������ɓ��e���ꂽ���������瓊�e���������Z�o
//FLOAT LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0)
//{
//	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
//	// ������Sep�͕W��������Ă��邱��
//	FLOAT r1 = fabs(D3DXVec3Dot(Sep, e1));
//	FLOAT r2 = fabs(D3DXVec3Dot(Sep, e2));
//	FLOAT r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
//	return r1 + r2 + r3;
//}




//_RPTN(_CRT_WARN, "%f, %f, %f\n", cp_OBB.x, cp_OBB.y, cp_OBB.z);
//_RPTN(_CRT_WARN, "%f, %f, %f\n", cp_Capsule.x, cp_Capsule.y, cp_Capsule.z);
//_RPTN(_CRT_WARN, "%f, %f, %f\n", distance.x, distance.y, distance.z);
//_RPTN(_CRT_WARN, "%f\n", length);
////_RPTN(_CRT_WARN, "%f, %f, %f\n", normal.x, normal.y, normal.z);
//_RPTN(_CRT_WARN, "%d\n", flag);