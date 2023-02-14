
#include "stdafx.h"

//#include "Collision.h"
//#include "main.h"
//#include "scene.h"
//#include "manager.h"
//#include "gameObject.h"
//#include <d3d11.h>
//
//#include "CComponent.h"
//#include "primitive.h"
//
//#include "model.h"
//#include "ModelComponent.h"
//#include "renderer.h"

void Collision::Update()
{
	// �V�[�����Q�b�g���Ă���
	Scene* scene = Manager::GetScene();

	// �ڒn����𖈉�false�Ƀ��Z�b�g���Ă����B�ł����̃R���W������true�ɂȂ������̂����Z�b�g���Ȃ��悤��temp�Ŋm�F
	if (!m_ParentGameObject->GetTempOnTheGround())
		m_ParentGameObject->SetOnTheGround(false);			// ���Z�b�g

	// ���[���h�}�g���N�X�̍쐬
	D3DXMATRIX worldmatrix = GetWorldMatrixCollision();

	
	if (m_Collision_type == RAY_COLLISION)
	{
		SetRayNoHit();	// ���񏉂߂Ƀ��Z�b�g���Ă�����
	}





	// �v�Z�p�Ɏg���ϐ��̏���
	//D3DXVECTOR3 t_Position;
	D3DXVECTOR3 t_Velocity;
	//t_Position = m_ParentGameObject->GetPosition() + m_ColOffset;		// �Q�b�g
	t_Velocity = m_ParentGameObject->GetVelocity();		// �Q�b�g

														// WorldMatrix���擾���Ă���(Scale,Rotation,Pos�̃f�[�^)
														//D3DXMATRIX selfMatrix = m_ParentGameObject->GetWorldMatrix();
														//D3DXMATRIX selfMatrix = CreateWorldMatrix(m_ColScale, m_ColRotation, t_Position);
	// Scale��1.0�Ń��[���h�}�g���N�X�����B
	D3DXMATRIX selfMatrix = CreateWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_ColRotation, m_ColPosition);

	// OBB�̍X�V
	m_obb.CreateOBB(m_ColScale, selfMatrix);

	// �J�v�Z���̍X�V
	m_capsule.SetCapsule(m_ColScale.x, m_ColScale.y, m_ColPosition, m_ColRotation);

	// �X�t�B�A�̍X�V�B���a�͂ǂ�������Ȃ̂łĂ��Ɓ[��x���g���Ă���
	m_sphere.SetSpehre(m_ColPosition, m_ColScale.x);

	// �����܂ł͓����蔻��Ȃ��Ă��K�{�B





	// �S�Ăɑ΂���Ignore�������ꍇ�I��
	for (int response : m_ResponseObjectList)
	{
		// Ignore�ȊO���������炻�̎��_�Ő�ɐi��
		if (response != RESPONSE_IGNORE)
			goto not_allIgnore;
	}
	return;
not_allIgnore:

	// �S�ẴQ�[���I�u�W�F�N�g���擾
	std::vector<GameObject*> allObj = scene->GetAllGameObjects(1);
	for (GameObject* other : allObj)
	{
		// other�ɏՓˑ���̐e�I�u�W�F�N�g(�Q�[���I�u�W�F�N�g)�̃|�C���^�[�������Ă���

		// ������������R���e�j���[�B�����I�u�W�F�N�g�����������ĂĂ������Ŗ��������
		if (other == m_ParentGameObject)
			continue;

		// �S�ẴI�u�W�F�N�g�̒�����Collision�R���|�[�l���g�������Ă�����̂������擾
		if (other->HasComponent<Collision>())
		{
			Collision* other_collision = other->GetComponent<Collision>();

			// response ����̑���Ƃ̔���������
			int response = CheckResponseOther(other_collision);

			// ������������I��
			if (response == RESPONSE_IGNORE)
				continue;


			//// �R���|�[�l���g�̃��X�g���擾���āA�����R���W�����������Ă������ꍇ�ɑΉ�����
			//std::list<CComponent*> other_ComponentList = other->GetComponentList();
			//for (auto other_Component : other_ComponentList)
			//{
			//	// �R���|�[�l���g���X�g�̂��ꂪ�R���W�����N���X��������
			//	if (typeid(*other_Component) == typeid(Collision))
			//	{
			//		// CComponent�N���X���_�E�����L���X�g����Collision�N���X�ɂ���
			//		Collision* other_collision = dynamic_cast <Collision*>(other_Component);


			//static const int	BOX_COLLISION = 0;
			//static const int	CAPSULE_COLLISION = 1;

			// WorldMatrix���擾���Ă���
			D3DXMATRIX otherMatrix = other->GetWorldMatrix();

			// �J�v�Z��(����)��OBB(����)�̏Փ˂�������
			if (m_Collision_type == CAPSULE_COLLISION && other_collision->GetCollisionType() == BOX_COLLISION)
			{
				CapsuleOBB(this, other_collision, response);
			}
			// OBB(����)�ƃJ�v�Z��(����)�̏Փ˂�������
			else if (m_Collision_type == BOX_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
			{
				CapsuleOBB(other_collision, this, response);
			}
			// �J�v�Z��(����)�ƃJ�v�Z��(����)�̏Փ˂�������
			else if (m_Collision_type == CAPSULE_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
			{
				CapsuleCapsule(this, other_collision, response);
			}
			// �X�t�B�A(����)�ƃJ�v�Z��(����)�̏Փ˂�������
			else if (m_Collision_type == SPHERE_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
			{
				SphereCapsule(this, other_collision, response);
			}
			// �J�v�Z��(����)�ƃX�t�B�A(����)�̏Փ˂�������
			else if (m_Collision_type == CAPSULE_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
			{
				SphereCapsule(other_collision, this, response);
			}
			// �X�t�B�A(����)��OBB(����)�̏Փ˂�������
			else if (m_Collision_type == SPHERE_COLLISION && other_collision->GetCollisionType() == BOX_COLLISION)
			{
				SphereOBB(this, other_collision, response);
			}
			// OBB(����)�ƃX�t�B�A(����)�̏Փ˂�������
			else if (m_Collision_type == BOX_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
			{
				SphereOBB(other_collision, this, response);
			}
			// �X�t�B�A(����)�ƃX�t�B�A(����)�̏Փ˂�������
			else if (m_Collision_type == SPHERE_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
			{
				SphereSphere(this, other_collision, response);
			}
			// ���C(����)�ƃX�t�B�A(����)�̏Փ˂�������
			else if (m_Collision_type == RAY_COLLISION && other_collision->GetCollisionType() == SPHERE_COLLISION)
			{
				if (RaySegmentSphere(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision))
					RaySegmentSphereDivCalc(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision);	// ���̒��ň�ԋ߂��������o���ăZ�b�g���Ă���
			}
			// ���C(����)�ƃJ�v�Z��(����)�̏Փ˂�������
			else if (m_Collision_type == RAY_COLLISION && other_collision->GetCollisionType() == CAPSULE_COLLISION)
			{
				if (RaySegmentCapsule(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision))
					RaySegmentCapsuleDivCalc(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision);	// ���̒��ň�ԋ߂��������o���ăZ�b�g���Ă���
			}
			// ���C(����)��OBB(����)�̏Փ˂�������
			else if (m_Collision_type == RAY_COLLISION && other_collision->GetCollisionType() == BOX_COLLISION)
			{
				if (RaySegmentOBB(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision))
					RaySegmentOBBDivCalc(m_ray.m_Segment.m_pos1, m_ray.m_Segment.m_pos2, other_collision);
			}
			// OBB(����)��OBB(����)�̏Փ˂�������B�����͂܂��������Boverlap�������f�ł���B
			else if (m_Collision_type == BOX_COLLISION && other_collision->GetCollisionType() == BOX_COLLISION)
			{
				// �ȗ��p�ɑ����obb�̃|�C���^��p�ӁB������m_obb
				OBB* other_obb = &other_collision->m_obb;	//other_collision
															/*D3DXMATRIX other_Matrix = CreateWorldMatrix(other_collision->m_ColScale, other_collision->m_ColRotation, other_collision->m_obb.m_Center);
															other_obb->CreateOBB(other_collision->m_ColScale, other_Matrix);*/

															//bool bbb = OBBOBB(this, other_collision);

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
				if (OBBOBB(this, other_collision, response))
				{
					// �Փˑ����OBB�̍X�V����Ȃ�����
					/*D3DXMATRIX other_Matrix = CreateWorldMatrix(other_collision->m_ColScale, other_collision->m_ColRotation, other_collision->m_obb.m_Center);
					other_obb->CreateOBB(other_collision->m_ColScale, other_Matrix);*/


					// ���肪�����Ȃ��Ȃ玩����������������
					if (other_collision->m_BeMoved == false)
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

							if (OBBOBB(this, other_collision, response))
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

						OverlapToBackPosition(this, other_collision, -distancevec);		// ���̒��ŃZ�b�g���Ă���

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
			//	cp_Capsule = ClosestPtCapsuleOBB(m_capsule, other_collision->m_obb, flag);

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
			//	ClosestPtPointOBB(cp_CapsuleCenterLine, other_collision->m_obb, cp_OBB);

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

			//	}
			//}


		}
	}

	// �S�Ă̏�������̌v�Z���I�������ɁARAY�̏Փ˂������̂���̖@�����v�Z�ŋ��߂�
	if (m_Collision_type == RAY_COLLISION)
	{
		if (m_ray.HitCollision != nullptr)
		{
			m_ray.CalcCollisionPointwithLengthNormal();		// ���C�̏Փˍ��W������̒����Ɩ@������v�Z����
			D3DXVECTOR3 HitPos = m_ray.m_CollisionPoint;
			if (m_ray.HitCollision->m_Collision_type == BOX_COLLISION)
			{
				D3DXVECTOR3 cp_OBB;		// cp_OBB �ɂ�OBB���̍Őڋߓ_�̍��W���擾�ł���
				ClosestPtPointOBB(HitPos, m_ray.HitCollision->m_obb, cp_OBB);
				D3DXVECTOR3 distance = HitPos - cp_OBB;
				D3DXVECTOR3 normal;
				D3DXVec3Normalize(&normal, &distance);
				m_ray.m_normal = normal;
				m_ray.m_CollisionPoint = cp_OBB;		// �Փ˓_��obb�̍Őڋߓ_�ɕύX����
			}
			else if (m_ray.HitCollision->m_Collision_type == CAPSULE_COLLISION)
			{
				float t;
				D3DXVECTOR3 cp_segment;		// cp_segment�ɂ͐������̍ŒZ���W�����܂�
				ClosetPtPointSegment(HitPos, m_ray.HitCollision->m_capsule.GetLowerSpherePos(), m_ray.HitCollision->m_capsule.GetUpperSpherePos(), t, cp_segment);
				D3DXVECTOR3 distance = HitPos - cp_segment;
				D3DXVECTOR3 normal;
				D3DXVec3Normalize(&normal, &distance);
				m_ray.m_normal = normal;
				// �Փ˓_���J�v�Z���̍Őڋߓ_�ɕύX����
				m_ray.m_CollisionPoint = cp_segment + (m_ray.HitCollision->m_capsule.m_Radius * normal);
			}
			else if (m_ray.HitCollision->m_Collision_type == SPHERE_COLLISION)
			{
				D3DXVECTOR3 distance = HitPos - m_ray.HitCollision->m_sphere.m_Center;
				D3DXVECTOR3 normal;
				D3DXVec3Normalize(&normal, &distance);
				m_ray.m_normal = normal;
				// �Փ˓_���X�t�B�A�̍Őڋߓ_�ɕύX����
				m_ray.m_CollisionPoint = m_ray.HitCollision->m_sphere.m_Center + (m_ray.HitCollision->m_sphere.m_Radius * normal);
			}
			_RPTN(_CRT_WARN, "normal %f, %f, %f\n", m_ray.m_normal.x, m_ray.m_normal.y, m_ray.m_normal.z);
		}
	}






	

	// �Ƃ肠�����n�ʂ�-100.0f�Ƃ��Ă���
	float groundHeight = scene->GetGroundMinHeight();


	//t_Position = m_ParentGameObject->GetPosition();		// �Q�b�g
	t_Velocity = m_ParentGameObject->GetVelocity();		// �Q�b�g
														// �n�ʂƂ̓����蔻��B�ڒn�B�R���W�����̒��S�_�������ĂȂ�����_�������ǎ��}��
	if (m_ColPosition.y < groundHeight && t_Velocity.y < 0.0f)
	{
		D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, groundHeight - m_ColPosition.y, 0.0f);
		m_ParentGameObject->AddPosition(upvector);
		//m_ParentGameObject->SetPosition_y(groundHeight);
		m_ParentGameObject->SetOnTheGround(true);
		m_ParentGameObject->SetVelocity_y(0.0f);
	}

	// ���b�V���t�B�[���h�Ƃ̏Փ˔���������Ă�����̂ł����
	// ���b�V���t�B�[���h�ł̐ڒn����
	{
		// m_ResponseObjectList �́@4�@�����b�V���t�B�[���h�ŌŒ�Ƃ��Ă����B
		if (m_ResponseObjectList[4] == RESPONSE_BLOCK)
		{
			// �܂��A���̃R���W�����̔��������߂Ă���
			float halfheight{};
			if (m_Collision_type == CAPSULE_COLLISION)
				halfheight = GetCapsule().GetHeightRadius();
			else if (m_Collision_type == SPHERE_COLLISION)
				halfheight = GetSphere().m_Radius;
			else if (m_Collision_type == BOX_COLLISION)
				halfheight = GetOBB().GetRotNormalLength(1, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			Int2 block;
			//BackAndSetPositionForMeshFieldObject(&block, halfheight);
			CalcCollisionMeshFieldObject(halfheight);

			//// ���b�V���t�B�[���h�����擾�B
			//float meshHeight;
			//D3DXVECTOR3 meshNormal;
			//Int2 block;
			//scene->GetMeshFieldObjectHeightandNormal(&block, &meshHeight, &meshNormal, m_ColPosition);
			//if (m_ColPosition.y - halfheight < meshHeight)
			//{
			//	// �n�ʂ������ɂ���B���܂��Ă���B
			//	if (meshNormal.y < 0.6)
			//	{
			//		// �R�Ƃ���ɂ��邩��@���̕����ɉ����߂��Ă�����
					//D3DXVECTOR3 moveNormal = meshNormal;
					//moveNormal.y = 0.0f;
					//D3DXVec3Normalize(&moveNormal, &moveNormal);

			//		D3DXVECTOR3 movevector = moveNormal * PanelSize;

			//		m_ParentGameObject->AddPosition(movevector);
			//		m_ParentGameObject->SetOnTheGround(false);
			//		m_ParentGameObject->SetVelocity_y(0.0f);
			//	}
			//	else
			//	{
			//		D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
			//		m_ParentGameObject->AddPosition(upvector);
			//		m_ParentGameObject->SetOnTheGround(true);
			//		m_ParentGameObject->SetVelocity_y(0.0f);
			//	}
			//}
			//else
			//{
			//	// �n�ʂ�����ɂ���B�����Ă���B
			//	// �������b�V���t�B�[���h���l���Ȃ��ꍇ�Œn�ʂɂ��Ă���Ȃ�A�������Ȃ��Ă����B
			//	//if (GetOnTheGround())
			//	//{
			//	//	//SetOnTheGround(false);
			//	//	//SetTempOnTheGround(false);
			//	//}

			//}


			//// ���b�V���t�B�[���h�����擾�B
			//float meshHeight = scene->GetMeshFieldObjectHeight(m_ColPosition);
			//if (m_ColPosition.y - halfheight < meshHeight)
			//{
			//	// �n�ʂ������ɂ���B���܂��Ă���B
			//	D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
			//	m_ParentGameObject->AddPosition(upvector);
			//	m_ParentGameObject->SetOnTheGround(true);
			//	m_ParentGameObject->SetVelocity_y(0.0f);
			//}
			//else
			//{
			//	// �n�ʂ�����ɂ���B�����Ă���B
			//	// �������b�V���t�B�[���h���l���Ȃ��ꍇ�Œn�ʂɂ��Ă���Ȃ�A�������Ȃ��Ă����B
			//	//if (GetOnTheGround())
			//	//{
			//	//	//SetOnTheGround(false);
			//	//	//SetTempOnTheGround(false);
			//	//}

			//}
		}
	}

}

//D3DXMATRIX Collision::GetWorldMatrixCollision()
//{
//	D3DXVECTOR3 Scale = m_ParentGameObject->GetScaleRate();
//	Scale.x *= m_ColInitScale.x;
//	Scale.y *= m_ColInitScale.y;
//	Scale.z *= m_ColInitScale.z;
//	D3DXVECTOR3 Rotation = m_ParentGameObject->GetRotation();
//	Rotation.x;
//	Rotation.y;
//	Rotation.z;
//	Rotation += m_ColInitRotation;
//	Rotation.x;
//	Rotation.y;
//	Rotation.z;
//	D3DXVECTOR3 Position = m_ParentGameObject->GetPosition();
//	Position += m_ColInitOffset;
//	//D3DXVECTOR3 Position = m_ColInitOffset;
//
//	D3DXMATRIX world, scale, rot, trans;
//	D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
//	D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
//	D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
//	world = scale * rot * trans;		// �ʏ�͂���
//	//world = scale * trans * rot;		// �e����]�̒��S�Ƃ���Ƃ���
//
//
//	D3DXVECTOR3 ParentPos = m_ParentGameObject->GetPosition();
//	D3DXVECTOR3 ParentScaleRate = m_ParentGameObject->GetOldScaleRate();
//
//	m_ColScale = Scale;
//	m_ColRotation = Rotation;
//
//	// �e�̉�]�����W���ړ�������B���厖�B
//	D3DXVECTOR3 offset = CalcAfterRoatedPos(m_ColInitOffset, m_ParentGameObject->GetRotation());
//	offset.x *= ParentScaleRate.x;
//	offset.y *= ParentScaleRate.y;
//	offset.z *= ParentScaleRate.z;
//
//	// �Ō�ɐe�̃I�u�W�F�N�g�ɁA�v�Z�ŋ��߂��I�t�Z�b�g�𑫂��Ă�����
//	m_ColPosition = m_ParentGameObject->GetPosition() + offset;
//
//	return world;
//}


D3DXMATRIX Collision::GetWorldMatrixCollision()
{
	// �q(����)�̏����擾
	D3DXVECTOR3 CScale = m_ColInitScale;
	D3DXVECTOR3 CRotation = m_ColInitRotation;
	D3DXVECTOR3 CPosition = m_ColInitOffset;

	// �q(����)�̍s����쐬
	D3DXMATRIX CworldM, CscaleM, CrotM, CtransM;
	D3DXMatrixScaling(&CscaleM, CScale.x, CScale.y, CScale.z);
	D3DXMatrixRotationYawPitchRoll(&CrotM, CRotation.y, CRotation.x, CRotation.z);
	D3DXMatrixTranslation(&CtransM, CPosition.x, CPosition.y, CPosition.z);
	CworldM = CscaleM * CrotM * CtransM;


	// �e�̏����擾
	D3DXVECTOR3 PScale = m_ParentGameObject->GetScaleRate();
	D3DXVECTOR3 PRotation = m_ParentGameObject->GetRotation();
	D3DXVECTOR3 PPosition = m_ParentGameObject->GetPosition();

	// �e�̍s����쐬
	D3DXMATRIX PworldM, PscaleM, ProtM, PtransM;
	D3DXMatrixScaling(&PscaleM, PScale.x, PScale.y, PScale.z);
	D3DXMatrixRotationYawPitchRoll(&ProtM, PRotation.y, PRotation.x, PRotation.z);
	D3DXMatrixTranslation(&PtransM, PPosition.x, PPosition.y, PPosition.z);
	PworldM = PscaleM * ProtM * PtransM;

	// �s��̊|���Z�B���[���h�ȊO����Ȃ����A�l���~�����ꍇ�K�v�B
	D3DXMATRIX RworldM, RscaleM, RrotM, RtransM;
	D3DXMatrixMultiply(&RworldM, &CworldM, &PworldM);	// ���[���h

	D3DXMatrixMultiply(&RscaleM, &CscaleM, &PscaleM);	// �X�P�[��
	D3DXMatrixMultiply(&RrotM, &CrotM, &ProtM);			// ��]
	D3DXMatrixMultiply(&RtransM, &CtransM, &PtransM);	// ���W

	// �s��v�Z���
	// �X�P�[�����擾�BRscaleM
	D3DXVECTOR3 AfterScale = D3DXVECTOR3(RscaleM._11, RscaleM._22, RscaleM._33);

	// ��]���擾�BRrotM����擾�ł���͍̂ŏI�I�ȉ�]�̂�Ȃ̂ŁAVec��Rot�ɂ͂ł��Ȃ��B
	// �̂ŕ��ʂɑ����Z����
	D3DXVECTOR3 AfterRotation = CRotation + PRotation;

	// ���W���擾�BRworldM�Bworld����ł�trans����ł��ǂ����ł�OK
	D3DXVECTOR3 AfterPosition = D3DXVECTOR3(RworldM._41, RworldM._42, RworldM._43);

	// After���g�����R���W�����̕ϐ��̍X�V��
	m_ColScale = AfterScale;
	m_ColRotation = AfterRotation;
	m_ColPosition = AfterPosition;

	return RworldM;
}

// �Փ˂��鐡�O�ɖ߂��A�ŏ��ɓ��������u���b�N�ԍ������߂�B���������������ĂȂ�������false��Ԃ�
bool Collision::BackAndSetPositionForMeshFieldObject(Int2* pOut_block, const float& halfheight)
{
	Scene* scene = Manager::GetScene();

	// ����̃t���[���ł��ꂾ����������
	D3DXVECTOR3 moved = m_ParentGameObject->GetOldPosition() - m_ParentGameObject->GetPosition();
	// �R���W�����̍ŏ��̏ꏊ�����߂Ă���
	D3DXVECTOR3 start = m_ColPosition - moved;

	// �����������߂�B�p�l���T�C�Y�̑傫���Ŋ���΍Œ��OK
	float lengthsq = D3DXVec3LengthSq(&moved);
	float minlength = 0.2f;
	float minlengthsq = pow(minlength, 2);
	if (lengthsq <= minlengthsq)
	{
		// ���b�V���t�B�[���h�����擾�B
		float meshHeight;
		D3DXVECTOR3 meshNormal;
		Int2 block;
		scene->GetMeshFieldObjectHeightandNormal(&block, &meshHeight, &meshNormal, m_ColPosition);
		if (m_ColPosition.y - halfheight < meshHeight)
		{
			// �n�ʂ������ɂ���B���܂��Ă���B
			if (meshNormal.y < 0.6)
			{
				// �R�Ƃ���ɂ��邩��@���̕����ɉ����߂��Ă�����
				D3DXVECTOR3 moveNormal = meshNormal;
				moveNormal.y = 0.0f;
				D3DXVec3Normalize(&moveNormal, &moveNormal);

				D3DXVECTOR3 movevector = moveNormal * minlength;

				m_ParentGameObject->AddPosition(movevector);
				m_ParentGameObject->SetOnTheGround(false);
				//m_ParentGameObject->SetVelocity_y(0.0f);
							// �����蔲���h�~���x�ɑ��x��������
				if (m_ParentGameObject->GetVelocity().y < -0.15f)
					m_ParentGameObject->SetVelocity_y(-0.15f);
			}
			else
			{
				D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
				m_ParentGameObject->AddPosition(upvector);
				m_ParentGameObject->SetOnTheGround(true);
				m_ParentGameObject->SetVelocity_y(0.0f);
			}
		}
		else
		{
			// �n�ʂ�����ɂ���B�����Ă���B
			// �������b�V���t�B�[���h���l���Ȃ��ꍇ�Œn�ʂɂ��Ă���Ȃ�A�������Ȃ��Ă����B
			//if (GetOnTheGround())
			//{
			//	//SetOnTheGround(false);
			//	//SetTempOnTheGround(false);
			//}

		}
		return false;
	}
	int div = (pow(PanelSize, 2) / lengthsq) + 1;
	//if (div > 50)
	//	div = 50;

	// 1��œ����x�N�g�������߂�
	D3DXVECTOR3 onevec = moved / div;

	bool fin = false;
	int min = 0;
	int max = div;
	int mid = (max + min) / 2;
	Int2 block;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 nowpos;
	float meshHeight;

	while (!fin)
	{
		// ����g�����W�Ƃ�
		nowpos = start + (mid * onevec);

		// ��������Փ˔�������
		meshHeight = scene->GetMeshFieldObjectHeight(&block, nowpos);
		if (nowpos.y - halfheight < meshHeight)
		{
			// �Փ˂��Ă���

			min = mid;
		}
		else
		{
			// �Փ˂��Ă��Ȃ�
			max = mid;
		}

		mid = (max + min) / 2;

		// �I������
		if (min + 1 >= max)
		{
			//if (min == 0)

			nowpos = start + (min * onevec);
			scene->GetMeshFieldObjectHeightandNormal(&block, &meshHeight, &normal, nowpos);
			if (nowpos.y - halfheight < meshHeight)
			{
				// �Փ˂��Ă���
				mid = min - 1;
				nowpos = start + (mid * onevec);
				scene->GetMeshFieldObjectHeightandNormal(&block, &meshHeight, &normal, nowpos);
				fin = true;
			}
			else
			{
				// �Փ˂��Ă��Ȃ�
				mid = min;
				fin = true;
			}
		}
	}
	// mid,nowpos���Ԃ��鐡�O�̏ꏊ,block�ɂ��ꂩ��Ԃ���
	// �߂�������

	// �ǂꂾ�������Ă����邩�̊������o��
	int nokori = div - mid;
	float wariai;
	if (nokori == 0)
	{
		D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
		m_ParentGameObject->AddPosition(upvector);
		m_ParentGameObject->SetOnTheGround(true);
		m_ParentGameObject->SetVelocity_y(0.0f);
		return false;
	}
	else
		wariai = (float)nokori / (float)div;

	D3DXVECTOR3 t_normal = normal;
	t_normal.y = 0.0f;
	D3DXVec3Normalize(&t_normal, &normal);

	float backlength = D3DXVec3Length(&moved);
	D3DXVECTOR3 backvec = t_normal * backlength;

	//nowpos += backvec;



	// �n�ʂ������ɂ���B���܂��Ă���B
	if (m_ColPosition.y - halfheight < meshHeight)
	{
		if (normal.y < 0.7)
		{
			// �R�Ƃ���ɂ��邩��@���̕����ɉ����߂��Ă�����
			//m_ParentGameObject->AddPosition(backvec);
			m_ParentGameObject->SetPosition(nowpos);
			m_ParentGameObject->SetOnTheGround(false);
			//// �����蔲���h�~���x�ɑ��x��������
			if(m_ParentGameObject->GetVelocity().y < -0.15f)
				m_ParentGameObject->SetVelocity_y(-0.15f);
			return true;
		}
		else
		{
			D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - m_ColPosition.y + halfheight, 0.0f);
			m_ParentGameObject->AddPosition(upvector);
			m_ParentGameObject->SetOnTheGround(true);
			m_ParentGameObject->SetVelocity_y(0.0f);
			return false;
		}
	}
	else
		return false;
}



// ����̃t���[���ł̈ړ��ŁA�Փ˂������b�V���t�B�[���h��
void Collision::CalcCollisionMeshFieldObject(const float& halfheight)
{
	Scene* scene = Manager::GetScene();

	// ����̃t���[���ł��ꂾ����������
	D3DXVECTOR3 moved = m_ParentGameObject->GetOldPosition() - m_ParentGameObject->GetPosition();
	D3DXVECTOR3 movedxz = moved;
	movedxz.y = 0.0f;
	D3DXVECTOR3 movedxzNormal;
	D3DXVec3Normalize(&movedxzNormal, &movedxz);
	// �R���W�����̍ŏ��̏ꏊ�����߂Ă���
	D3DXVECTOR3 start = m_ColPosition - moved;

	float meshHeight;
	D3DXVECTOR3 meshNormal;
	D3DXVECTOR3 befor;
	Int2 block;
	int8_t col = 0;

	// �����������߂�B�p�l���T�C�Y�̑傫���Ŋ���΍Œ��OK
	float lengthsq = D3DXVec3LengthSq(&movedxz);
	float minlength = PanelSize;
	float minlengthsq = pow(minlength, 2);

	int div = lengthsq / minlengthsq;
	D3DXVECTOR3 onemove = movedxzNormal * PanelSize;
	D3DXVECTOR3 nowpos = start;
	for (int i = 0; i < div; i++)
	{
		nowpos += onemove;
		// �����蔻����Ƃ�
		// �Ԃ����Ă����炻���ŏI��
		D3DXVECTOR3 nowposy = nowpos;
		nowposy.y -= halfheight;
		col = scene->CheckCollisionMeshFieldObjects(&block, &meshHeight, &meshNormal, &befor, nowposy);

		if (col != 0)
			break;
	}
	// for�����I�������ɁA�I�����W�œ����蔻������
	if (col == 0)
	{
		nowpos = m_ColPosition;
		D3DXVECTOR3 nowposy = nowpos;
		nowposy.y -= halfheight;
		col = scene->CheckCollisionMeshFieldObjects(&block, &meshHeight, &meshNormal, &befor, nowposy);
	}


	// �Փ˂��Ă�����
	if (col == 1)
	{
		D3DXVECTOR3 upvector = D3DXVECTOR3(0.0f, meshHeight - nowpos.y + halfheight, 0.0f);
		m_ParentGameObject->AddPosition(upvector);
		m_ParentGameObject->SetOnTheGround(true);
		m_ParentGameObject->SetVelocity_y(0.0f);
		return;
	}
	else if (col == 2)
	{
		D3DXVECTOR3 newpos = befor - onemove;
		if (div == 0)
			newpos = m_ParentGameObject->GetOldPosition();
		//newpos.y = meshHeight + halfheight;

		// �c��̒������A�ǂɉ������x�N�g���ňړ������Ă�����
		D3DXVECTOR3 flontvec = moved;
		D3DXVECTOR3 aftervec;
		CalcWallScratchVector(&aftervec, flontvec, meshNormal);
		D3DXVECTOR3 aftervecN = aftervec;
		aftervecN.y = 0.0f;
		//D3DXVec3Normalize(&aftervecN, &aftervec);

		D3DXVECTOR3 trash;
		col = scene->CheckCollisionMeshFieldObjects(&block, &meshHeight, &meshNormal, &trash, befor);

		m_ParentGameObject->SetPosition(newpos);
		m_ParentGameObject->SetOnTheGround(false);

		D3DXVECTOR3 vel = m_ParentGameObject->GetVelocity();
		float veclength = D3DXVec3Length(&vel);	// ���݂̃x�N�g���̒�����ۑ�
		D3DXVECTOR3 velN;
		D3DXVec3Normalize(&velN, &vel);			// �x�N�g���̐��K��
		D3DXVECTOR3 addvelN = velN + meshNormal;	// ���K���������m�ő������킹��



		D3DXVECTOR3 setvel = addvelN * veclength;	// �������킹�����̂𒷂������������̂�V�����x�N�g���Ƃ���
		m_ParentGameObject->SetVelocity(setvel);
		m_ParentGameObject->AddPosition(-aftervecN * 0.02f);	// 0.02f�͂Ƃ肠�����Ŗ߂��Ă���B

		return;
	}



}