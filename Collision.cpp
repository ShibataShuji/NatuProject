//#include "Collision.h"
//
//void Collision::CapsuleCapsule(GameObject* obj, D3DXVECTOR3* t_Position)
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
//
//	if (colCapsuleCapsule(m_Capsule, obj->GetComponent<Collision>()->m_Capsule, &c_p1, &c_p2, &c_vd))
//	{
//		// �Փ˂��Ă���Ȃ�
//
//
//		// �d�Ȃ��������W��߂�t_Position
//		c_UnitNormal = (c_p1 - c_p2).getNorm();
//		c_Normal = c_UnitNormal * c_vd;				// �����x�N�g��		
//
//		// �������Ă�����̂������Ȃ����̂Ȃ�
//		if (!obj->GetComponent<Collision>()->m_Movable)
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
//
//			// �������Ă�����̂��ړ����������ꍇ�ǂ�����e��0.5�{�œ�����
//			// �������������̂Ȃ�
//			if (m_Movable)
//			{
//				(*t_Position).x += c_Normal.x * 0.5f;
//				(*t_Position).y += c_Normal.y * 0.5f;
//				(*t_Position).z += c_Normal.z * 0.5f;
//			}
//
//			// �������Ă�����̂̃|�W�V������ς���B
//			// �{���̓R���W�����ɂ���1�����o�ϐ��ǉ����āAD3DXVECTOR3�ő��̃R���W��������̉e���ǂꂭ�炢�󂯂���
//			// ��ۑ����āA�R���W������Update�̍ŏ��ɂ�������̍��W�ɒǉ����Ă�����v�Z�̂����������B
//			D3DXVECTOR3 DX_c_Normal = { c_Normal.x, c_Normal.y, c_Normal.z };
//			DX_c_Normal *= -1;		// �x�N�g���𔽓]������
//
//			D3DXVECTOR3 HitObjPos = obj->GetPosition();
//			D3DXVECTOR3 HitObjSetpos = HitObjPos + (DX_c_Normal * 0.5);
//			obj->SetPosition(HitObjSetpos);
//		}
//
//
//		//t_Velocity.x = 0.0f;
//		//t_Velocity.y = 0.0f;
//		//t_Velocity.z = 0.0f;
//
//	}
//	else
//	{
//		// �Փ˂��Ă��Ȃ��Ȃ�
//		int aaasd = 4;
//	}
//}
