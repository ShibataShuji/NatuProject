#pragma once

#define OBJNUM (4)

#include "CComponent.h"
#include <string>
#include <list>

class GameObject
{
protected:	// �A�N�Z�X�w��q

	bool		m_Destroy = false;


	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_InitScale;
	D3DXVECTOR3 m_ScaleRate = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �ݒ肳�ꂽ�����l�ɑ΂��Ă̔{��

	D3DXVECTOR3	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̑��x�x�N�g��

	int			m_TagNum = 0;

	bool		m_OnTheGround = false;
	bool		m_temp_OnTheGround = false;


	// �v�Z�p
	D3DXVECTOR3 m_Old_Position;
	D3DXVECTOR3 m_Old_Rotation;
	D3DXVECTOR3 m_Old_Scale;
	D3DXVECTOR3 m_Old_ScaleRate;



	std::list<CComponent*> m_ComponentList;
	std::list<std::string> m_ComponentNameList;
	std::list<GameObject*> m_Child;

	/*GameObject*				m_Parent;*/

public:

	GameObject() {}

	virtual ~GameObject()
	{
		for (auto c : m_ComponentList)
			delete c;
		m_ComponentList.clear();

		//for (auto c : m_ComponentNameList)	// ����̓N���X����Ȃ��̂�delete����K�v�Ȃ�
		//	delete c;
		m_ComponentNameList.clear();

		for (auto c : m_Child)
			delete c;
		m_Child.clear();
	}

	int Gettagnum()
	{
		return m_TagNum;
	}

	virtual void Init()	// �������z�֐�
	{

	}


	virtual void Uninit()
	{
		for (auto c : m_ComponentList)
			c->Uninit();

		for (auto c : m_Child)
			c->Uninit();
	}

	// virtual(�������z�֐�)�Ȃ̂ŁA������p������player�N���X�Ȃǂ�Update�֐������Ƃ��������Ă΂��
	// ����͌Ă΂�Ȃ����Aplayer��update�̕��ł�����Ă�(�e�N���X��update)�ƌĂׂ�B
	// ����̎q�N���X�ł���player�Ƃ��̒��Ŗ����I��GameObject::Update���ĕ��ɌĂ΂Ȃ��Ƃ����Ȃ��B
	// Draw�Ƃ��Y�ꂪ���Ȃ̂Œ���
	virtual void Update()
	{
		// ���W�̕ۑ�
		m_Old_Position = m_Position;
		m_Old_Rotation = m_Rotation;
		m_Old_Scale = m_Scale;
		m_Old_ScaleRate = m_ScaleRate;

		// �ڒn�����Temp�̃��Z�b�g�B�O�t���[���̂��c��Ȃ��悤�ɂ����Ń��Z�b�g���Ă����B
		m_temp_OnTheGround = false;


		// player�Ƃ��q�N���X��Update�̌�ɗ��Ăق����̂ŁAplayer�Ƃ��q�N���X��Update�̍Ō�ɏ����B
		/*for (auto c : m_ComponentList)
			c->Update();*/
		/*------->>>>>> ComponentUpdate()�֐��ɂ܂Ƃ߂�*/


		// �q���̕K�v�����킩��Ȃ��G�G
		/*for (auto c : m_Child)
			c->Update();*/
	}


	virtual void Draw()
	{
		for (auto c : m_ComponentList)
			c->Draw();
	}


	// �R���|�[�l���g�̃A�b�v�f�[�g
	virtual void ComponentUpdate()
	{
		for (auto c : m_ComponentList)
			c->Update();
	}

	// Temp�̍X�V
	//virtual void TemporarySetUpdate()
	//{
	//	m_Position = m_temp_Position;
	//	m_Velocity = m_temp_Velocity;
	//}

	void SetPosition(D3DXVECTOR3 Position){	m_Position = Position;	}
	void SetPosition_x(float x){	m_Position.x = x;	}
	void SetPosition_y(float y){	m_Position.y = y;	}
	void SetPosition_z(float z){	m_Position.z = z;	}

	void SetRotation(D3DXVECTOR3 rotation){ m_Rotation = rotation;	}

	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXVECTOR3 GetScaleRate() { return m_ScaleRate; }

	D3DXVECTOR3 GetOldPosition() { return m_Old_Position; }
	D3DXVECTOR3 GetOldRotation() { return m_Old_Rotation; }
	D3DXVECTOR3 GetOldScale() { return m_Old_Scale; }
	D3DXVECTOR3 GetOldScaleRate() { return m_Old_ScaleRate; }
	//void SetOldPosition(D3DXVECTOR3 Position) { m_Old_Position = Position; }

	//D3DXVECTOR3 GetTempPosition() { return m_temp_Position; }
	//void SetTempPosition(D3DXVECTOR3 Position) { m_temp_Position = Position; }

	D3DXVECTOR3 GetVelocity() { return m_Velocity; }
	void SetVelocity(D3DXVECTOR3 Velocity) { m_Velocity = Velocity; }
	void SetVelocity_x(float x) { m_Velocity.x = x; }
	void SetVelocity_y(float y) { m_Velocity.y = y; }
	void SetVelocity_z(float z) { m_Velocity.z = z; }

	//D3DXVECTOR3 GetTempVelocity() { return m_temp_Velocity; }
	//void SetTempVelocity(D3DXVECTOR3 Velocity) { m_temp_Velocity = Velocity; }

	void SetDestroy() { m_Destroy = true; }

	// Init�ł͂�����g���ăX�P�[����ݒ肵�Ă�����
	void SetInitScale(D3DXVECTOR3 setscale) 
	{ 
		m_InitScale = setscale;
		m_Scale = setscale;
	}
	void SetScaleRate(D3DXVECTOR3 setscalerate) 
	{ 
		m_ScaleRate = setscalerate; 
		m_Scale.x = m_InitScale.x * m_ScaleRate.x;
		m_Scale.y = m_InitScale.y * m_ScaleRate.y;
		m_Scale.z = m_InitScale.z * m_ScaleRate.z;
	}

	// ���̃I�u�W�F�N�g��m_Scale,m_Rotation,m_Position���g���ă��[���h�}�g���N�X��0������
	D3DXMATRIX GetWorldMatrix()
	{
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans;
		return world;
	}


	D3DXVECTOR3 GetForward()	// �O�����x�N�g��
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;

	}

	D3DXVECTOR3 GetRight()	// �E�����x�N�g��
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}

	D3DXVECTOR3 GetUp()	// ������x�N�g��
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	// �R���|�[�l���g��ǉ�����B�������Ŗ��O��t���邱�Ƃ��ł���B�󔒂̏ꍇ����ɖ��������
	template <typename T>
	T* AddComponent()
	{
		// ���̃I�u�W�F�N�g���ǉ�����R���|�[�l���g�̐e�ɂȂ�̂ŁA�R���X�g���N�^��this��ݒ肷��
		T* ccomponent  = new T(this);
		ccomponent->Init();
		m_ComponentList.push_back(ccomponent);
		m_ComponentNameList.push_back("noname");

		return ccomponent;
	}
	template <typename T>
	T* AddComponent(std::string name)
	{
		T* ccomponent = new T(this);
		ccomponent->Init();
		m_ComponentList.push_back(ccomponent);
		m_ComponentNameList.push_back(name);

		return ccomponent;
	}

	template <typename T>
	T* GetComponent()
	{
		for (auto c : m_ComponentList)
		{
			if (typeid(*c) == typeid(T))
			{
				return (T*)c;
			}
		}
		return nullptr;
	}

	// ���̃Q�[���I�u�W�F�N�g�̒��Ɉ������̖��O�����Ă���R���|�[�l���g������΂��̃R���|�[�l���g��Ԃ�
	// �����Ă͂Ȃ�Ȃ��������������烊�X�g�̍ŏ��̂��̂��Ԃ�
	template <typename T>
	T* GetComponentWithName(std::string name)
	{
		int itrCount = 0;
		for (auto cName : m_ComponentNameList)
		{
			// �����񂪓������̂���������
			if (name == cName)
			{
				auto itr = m_ComponentList.begin();		// �{���� std::list<int>::iterator itr ������auto�ŏȗ�
				for (int i = 0; i < itrCount; i++)
				{
					++itr;
				}
				return (T*)*itr;						// itr�̓|�C���^�Ȃ̂�*itr�Œ��g��Ԃ��Ă�����
			}

			itrCount++;
		}
		return nullptr;
	}
	
	std::list<CComponent*> GetComponentList()
	{
		return m_ComponentList;
	}

	template <typename T>
	bool HasComponent()
	{
		for (auto c : m_ComponentList)
		{
			if (typeid(*c) == typeid(T))
			{
				return true;
			}
		}
		return false;
	}



	bool GetOnTheGround()
	{
		return m_OnTheGround;
	}
	// temp�������ɃZ�b�g�����
	void SetOnTheGround(bool setbool)
	{
		m_temp_OnTheGround = setbool;
		m_OnTheGround = setbool;
	}
	bool GetTempOnTheGround()
	{
		return m_temp_OnTheGround;
	}
	void SetTempOnTheGround(bool settempbool)
	{
		m_temp_OnTheGround = settempbool;
	}


};

// private ���̃N���X���炵���A�N�Z�X�ł��Ȃ��B���̃N���X�̊֐��Ƃ��B
// prootected private + �p����̃N���X����̓A�N�Z�X�ł���B�Q�[���G���W�����Ƃ��ꂪ�������ȁ`
// public �A�N�Z�X�ł���