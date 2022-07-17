#pragma once

#include "CComponent.h"
//#include "gameObject.h"

class Rigidbody : public CComponent
{
private:
	float				m_gravity = 1.0f;		// �d��		���d��0.0f~1.0f�d��
	float				m_Mass = 1.0f;			// �d��
	float				m_Friction = 0.5f;		// ���C�A���ׂ�₷��	�~�܂�Ȃ�0.0f~1.0f�~�܂�
	
	// �v�Z�Ŏg���f�t�H���g�l
	float				m_gravity_def = 0.01f;

public:

	// �e�N���X�̃R���X�g���N�^������Ă����B
	using CComponent::CComponent;

	//Rigidbody(GameObject* gameObject) : CComponent(gameObject){}
	//virtual ~Rigidbody(){}
	

	void Init() override 
	{

	}

	void Uninit() override
	{

	}

	void Update() override
	{
		//// TempPosition���ŏI�I�ɕύX���邽�߂Ɍ��݂̂��̂��Q�b�g���Ă���
		//D3DXVECTOR3 t_Position = m_ParentGameObject->GetTempPosition();
		
		// �v�Z���₷���悤��Velocity���Q�b�g���ĉ��ŕۑ����Ă���
		D3DXVECTOR3 t_Position;
		D3DXVECTOR3 t_Velocity;


		
		// �d�͂̌v�Z	�܂�Mass���l������ĂȂ�
		t_Velocity = m_ParentGameObject->GetVelocity();		// �Q�b�g
		t_Velocity.y -= m_gravity_def * m_gravity;
		m_ParentGameObject->SetVelocity(t_Velocity);		// �Z�b�g
		//t_Velocity.y = m_ParentGameObject->GetVelocity().y - m_gravity_def * m_gravity;

		// ���C�Ȃǒ�R�̌v�Z
		// �܂�0.1�{�Ƃ��ł���Ă邯�ǁAFriction�Ƃ�Mass�Ƃ��g�����v�Z�ɂ������B
		t_Velocity = m_ParentGameObject->GetVelocity();		// �Q�b�g
		t_Velocity.x -= t_Velocity.x * 0.1;
		t_Velocity.y -= t_Velocity.y * 0.01;
		t_Velocity.z -= t_Velocity.z * 0.1;
		m_ParentGameObject->SetVelocity(t_Velocity);		// �Z�b�g
		//m_ParentGameObject->SetVelocity_x(t_Velocity.x - t_Velocity.x * 0.1);
		//m_ParentGameObject->SetVelocity_y(t_Velocity.y - t_Velocity.y * 0.01);
		//m_ParentGameObject->SetVelocity_z(t_Velocity.z - t_Velocity.z * 0.1);




		//// m_TempVelocity�̍X�V
		//m_ParentGameObject->SetTempVelocity(t_Velocity);
		
		// ���x�𑫂�
		t_Position = m_ParentGameObject->GetPosition();		// �Q�b�g
		t_Velocity = m_ParentGameObject->GetVelocity();		// �Q�b�g
		t_Position += t_Velocity;
		m_ParentGameObject->SetPosition(t_Position);		// �Z�b�g

		//// m_Position�̍X�V
		//m_ParentGameObject->SetTempPosition(t_Position);
	}

	void Draw() override
	{

	}

	float GetMass()
	{
		return m_Mass;
	}

	float GetFriction()
	{
		return m_Friction;
	}

};

