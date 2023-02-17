#pragma once

#define OBJNUM (4)


#include "CComponent.h"
#include "SimpleBoundingBox3D.h"

// �Z�[�u�f�[�^�ɕۑ���������
// �z�u����Ă�����
struct StructPlacedObjectData		
{
	char		sm_name[64] = "null";
	D3DXVECTOR3 sm_Position = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 sm_Rotation = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 sm_Scale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 sm_ScaleRate = { 1.0f, 1.0f, 1.0f };

};

//// �I�u�W�F�N�g�̓����f�[�^�̏��
//struct StructObjectData
//{
//	char		sm_name[64] = "null";
//	char		sm_modelname[64] = "null";
//	int			sm_componentlist[32];			// ���� �������ł��ĂȂ��̂Ŏg���O�ɏ���������
//
//};

// ���f���̏��Z�[�u�ƃ��[�h�p�\����
struct StructModelState
{
	D3DXVECTOR3 sm_ModelOffset;
	D3DXVECTOR3 sm_ModelRotation;
	D3DXVECTOR3 sm_ModelScale;
};




class GameObject
{
protected:	// �A�N�Z�X�w��q

	bool		m_Destroy = false;

	bool		m_LoadSuccess = true;


	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_InitScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_ScaleRate = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �ݒ肳�ꂽ�����l�ɑ΂��Ă̔{��

	D3DXVECTOR3 m_ModelOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ModelRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ModelScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXVECTOR3	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̑��x�x�N�g��

	int			m_TagNum = 0;

	std::string		m_name = "null";
	std::string		m_modelname = "null";

	int			m_DoSave = 0;			// �ۑ�����Ƃ��ɃZ�[�u����邩�ǂ���

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


	// Collision�R���|�[�l���g��1�ł������Ă���Ƃ��Ɏg���悤�ϐ�
	std::list<GameObject*> m_Collision_HitObjectList;
	std::list<D3DXVECTOR3> m_Collision_HitPositionList;
	std::list<GameObject*> m_Collision_OverlapObjectList;
	std::list<D3DXVECTOR3> m_Collision_OverlapPositionList;


	// �V�F�[�_�[�֘A �̓��f���R���|�[�l���g�ŊǗ����Ă���B
	// �e���g�����ǂ����̓I�u�W�F�N�g�P�ʂł�肽���̂ł������ŊǗ�
	bool m_UseShadow = false;

	//// �ȈՕ\���p2D�o�E���f�B���O�{�b�N�X
	//SimpleBoundingBox2D*	m_SimpleBoundingBox2D = nullptr;
	//bool					m_SimpleBoundingBox2DUseDraw = false;		// �\�����邩���Ȃ���

		// �ȈՕ\���p3D�o�E���f�B���O�{�b�N�X
	SimpleBoundingBox3D*	m_SimpleBoundingBox3D = nullptr;
	float					m_SimpleBoundingBox3DRadius = 1.5f;			// ���ۂ̃I�u�W�F�N�g�̉�ʂł��̒l���������Ē�������
	bool					m_SimpleBoundingBox3DUseDraw = false;		// �\�����邩���Ȃ���

	//// �e�N�X�`���[�ɒǉ�����F�̐ݒ�
	//D3DXVECTOR4				m_SynthesizeColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	//bool					m_SynthesizeColorUse = false;


	/*GameObject*				m_Parent;*/

public:

	GameObject() 
	{
		//m_SimpleBoundingBox2D = new SimpleBoundingBox2D(this);
		//m_SimpleBoundingBox2D->Init();
		m_SimpleBoundingBox3D = new SimpleBoundingBox3D(this);
		m_SimpleBoundingBox3D->Init();
	}

	virtual ~GameObject()
	{
		for (auto c : m_ComponentList)
		{
			if(c != nullptr)
				delete c;
		}
		m_ComponentList.clear();

		m_ComponentNameList.clear();

		for (auto c : m_Child)
			delete c;
		m_Child.clear();

		//delete m_SimpleBoundingBox2D;
		delete m_SimpleBoundingBox3D;
	}

	int Gettagnum()
	{
		return m_TagNum;
	}

	void Load();

	virtual void Init();

	virtual void Uninit();

	// virtual(�������z�֐�)�Ȃ̂ŁA������p������player�N���X�Ȃǂ�Update�֐������Ƃ��������Ă΂��
	// ����͌Ă΂�Ȃ����Aplayer��update�̕��ł�����Ă�(�e�N���X��update)�ƌĂׂ�B
	// ����̎q�N���X�ł���player�Ƃ��̒��Ŗ����I��GameObject::Update���ĕ��ɌĂ΂Ȃ��Ƃ����Ȃ��B
	// Draw�Ƃ��Y�ꂪ���Ȃ̂Œ���
	virtual void Update();

	virtual void Draw();


	// �R���|�[�l���g�̃A�b�v�f�[�g
	virtual void ComponentUpdate()
	{
		//// �R���W�����n�̕ϐ��̃��Z�b�g
		ResetCollisionHitArray();

		for (int i = 0; i < UpdatePriority_Max; i++)
		{
			for (auto comp : m_ComponentList)
			{
				if(comp->GetComponentUpdatePriority() == i)
					comp->Update();
			}
		}
	}

	// Temp�̍X�V
	//virtual void TemporarySetUpdate()
	//{
	//	m_Position = m_temp_Position;
	//	m_Velocity = m_temp_Velocity;
	//}

	void ResetCollisionHitArray()
	{
		// �R���W�����n�̕ϐ��̃��Z�b�g
		m_Collision_HitObjectList.clear();
		m_Collision_HitPositionList.clear();
		m_Collision_OverlapObjectList.clear();
		m_Collision_OverlapPositionList.clear();
	}

	void AddPosition(D3DXVECTOR3 AddPosition) { m_Position += AddPosition; }

	void SetPosition(D3DXVECTOR3 Position){	m_Position = Position;	}
	void SetPosition_x(float x){	m_Position.x = x;	}
	void SetPosition_y(float y){	m_Position.y = y;	}
	void SetPosition_z(float z){	m_Position.z = z;	}

	void SetRotation(D3DXVECTOR3 rotation){ m_Rotation = rotation;	}

	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3* GetpPosition() { return &m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3* GetpRotation() { return &m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXVECTOR3* GetpScale() { return &m_Scale; }
	D3DXVECTOR3 GetScaleRate() { return m_ScaleRate; }
	D3DXVECTOR3* GetpScaleRate() { return &m_ScaleRate; }

	D3DXVECTOR3 GetOldPosition() { return m_Old_Position; }
	D3DXVECTOR3 GetOldRotation() { return m_Old_Rotation; }
	D3DXVECTOR3 GetOldScale() { return m_Old_Scale; }
	D3DXVECTOR3 GetOldScaleRate() { return m_Old_ScaleRate; }
	//void SetOldPosition(D3DXVECTOR3 Position) { m_Old_Position = Position; }

	//D3DXVECTOR3 GetTempPosition() { return m_temp_Position; }
	//void SetTempPosition(D3DXVECTOR3 Position) { m_temp_Position = Position; }

	void SetModelOffset(D3DXVECTOR3 offset) { m_ModelOffset = offset; }
	void SetModelRotation(D3DXVECTOR3 rotation) { m_ModelRotation = rotation; }
	void SetModelScale(D3DXVECTOR3 scale) { m_ModelScale = scale; }

	D3DXVECTOR3 GetModelOffset() { return m_ModelOffset; }
	D3DXVECTOR3 GetModelRotation() { return m_ModelRotation; }
	D3DXVECTOR3 GetModelScale() { return m_ModelScale; }


	D3DXVECTOR3 GetVelocity() { return m_Velocity; }
	void SetVelocity(D3DXVECTOR3 Velocity) { m_Velocity = Velocity; }
	void SetVelocity_x(float x) { m_Velocity.x = x; }
	void SetVelocity_y(float y) { m_Velocity.y = y; }
	void SetVelocity_z(float z) { m_Velocity.z = z; }

	const bool& GetUseShadow() { return m_UseShadow; }
	void SetUseShadow(const bool *shadow) { m_UseShadow = &shadow; }


	//D3DXVECTOR3 GetTempVelocity() { return m_temp_Velocity; }
	//void SetTempVelocity(D3DXVECTOR3 Velocity) { m_temp_Velocity = Velocity; }

	int GetDoSave()
	{
		return m_DoSave;
	}

	bool GetDestroyState()
	{
		if (this == nullptr)
			return true;
		return m_Destroy;
	}

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
		// ModelOffset�Ƃ�Model���Ȃ��Ƃ��̃o�[�W����
		//D3DXMATRIX world, scale, rot, trans;
		//D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		//D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
		//world = scale * rot * trans;
		//return world;

		// Model�n����
		D3DXVECTOR3 Scale;						// Scale �͊|���Z
		Scale.x = m_Scale.x * m_ModelScale.x;
		Scale.y = m_Scale.y * m_ModelScale.y;
		Scale.z = m_Scale.z * m_ModelScale.z;
		D3DXVECTOR3 Rotation;					// Rotation �͑����Z
		Rotation = m_Rotation + m_ModelRotation;
		D3DXVECTOR3 Position;					// Position �������Z
		Position = m_Position + m_ModelOffset;

		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
		D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
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



	StructPlacedObjectData GetStructPlacedObjectData()
	{
		
		StructPlacedObjectData re;
		std::memcpy(re.sm_name, m_name.c_str(), strlen(m_name.c_str()) + 1);	// string�^->char[64]�֕ϊ�
		re.sm_Position = m_Position;
		re.sm_Rotation = m_Rotation;
		re.sm_Scale = m_InitScale;
		re.sm_ScaleRate = m_ScaleRate;

		return re;
	}

	void GetObjectNameChar64(char* objectnamearray)
	{
		std::memcpy(objectnamearray, m_name.c_str(), strlen(m_name.c_str()) + 1);	// string�^->char[64]�֕ϊ�
	}
	std::string GetObjectNameString()
	{
		return m_name;
	}
	void SetObjectName(char* objectnamearray)
	{
		//std::string temp = std::string(objectnamearray);
		m_name = std::string(objectnamearray);						// char[64]->string�^�֕ϊ�
		//char* tempC[64];
		//strcpy(*tempC, objectnamearray);
		//std::string tempS(*tempC, sizeof(tempC));
		//std::string tempS(objectnamearray, 64);
		//m_name = tempS;
		//m_name(objectnamearray, sizeof(objectnamearray));
	}

	void GetModelName(char* modelnamearray)
	{
		std::memcpy(modelnamearray, m_modelname.c_str(), strlen(m_modelname.c_str()) + 1);	// string�^->char[64]�֕ϊ�
	}
	void SetModelName(char* modelnamearray)
	{
		//int a = sizeof(modelnamearray);	// ����̓A�h���X�Ȃ̂ŃT�C�Y�͐��4�ƂȂ��Ă��܂�
		std::string setname(modelnamearray, strlen((char*)modelnamearray));
		m_modelname = setname;
	}

	int GetComponentNum()
	{
		int count = 0;
		for (auto Comp : m_ComponentList)
		{
			count++;
		}
		return count;
	}

	void DestroyComponent(int compCount);

	void DestroyComponentList();

	// ����open���Ă���obj�t�@�C���̃A�h���X�����炤
	void SaveComponent(std::ofstream* Objfile, std::ofstream* ObjfileB);
	// �t�@�C�������̂�Savedata.cpp�̕��ł���Ă�

	void LoadComponent(std::ifstream* Objfile)
	{
		// �R���|�[�l���gID��ǂݎ���Ă����AddComponent����B���̌セ�̃R���|�[�l���g�̐ݒ��ǂݍ��ݒǉ�����B
		int compid = 0;
		Objfile->read((char*)&compid, sizeof(compid));	// �������Ă���R���|�[�l���g���̓ǂݍ���

		//// �����ł̓R���|�[�l���g��ID������ǂݍ���
		//for (auto Comp : m_ComponentList)
		//{
		//	int compid = Comp->GetComponentID();
		//	Objfile->write((char*)&compid, sizeof(compid));

		//	// ���ɂ��̃R���|�[�l���g�̐ݒ���������ށA����̓R���|�[�l���g�ɂ���ē��e���ʂ��Ⴄ
		//	Comp->Save(Objfile);

		//}
	}


	void CopyComponentListToAdd(GameObject* from);


	void CopyGameObject(GameObject* from);

	// �Z�[�u�ƃ��[�h�p
	StructModelState GetStructModelState()
	{
		StructModelState SMS;
		SMS.sm_ModelOffset = m_ModelOffset;
		SMS.sm_ModelRotation = m_ModelRotation;
		SMS.sm_ModelScale = m_ModelScale;

		return SMS;
	}
	// �Z�[�u�ƃ��[�h�p
	void SetStructModelState(StructModelState sms)
	{
		m_ModelOffset = sms.sm_ModelOffset;
		m_ModelRotation = sms.sm_ModelRotation;
		m_ModelScale = sms.sm_ModelScale;
	}

	// Collision�p
	void Collision_AddHitObject(GameObject* hitobject, D3DXVECTOR3 hitpos)
	{
		m_Collision_HitObjectList.push_back(hitobject);
		m_Collision_HitPositionList.push_back(hitpos);
	}
	void Collision_AddOverlapObject(GameObject* overlapobject, D3DXVECTOR3 overlappos)
	{
		m_Collision_OverlapObjectList.push_back(overlapobject);
		m_Collision_OverlapPositionList.push_back(overlappos);
	}


	//SimpleBoundingBox2D* GetSimpleBoundingBox2D() { return m_SimpleBoundingBox2D; }
	//void SetSimpleBoundingBox2DUseDraw(const bool& usedraw) { m_SimpleBoundingBox2DUseDraw = usedraw; }
	//bool GetSimpleBoundingBox2DUseDraw() { return m_SimpleBoundingBox2DUseDraw; }
	SimpleBoundingBox3D* GetSimpleBoundingBox3D() { return m_SimpleBoundingBox3D; }
	void SetSimpleBoundingBox3DUseDraw(const bool& usedraw) { m_SimpleBoundingBox3DUseDraw = usedraw; }
	bool GetSimpleBoundingBox3DUseDraw() { return m_SimpleBoundingBox3DUseDraw; }
	
	void SetSimpleBoundingBox3DRadius(const float& radius) { m_SimpleBoundingBox3DRadius = radius; }
	float GetSimpleBoundingBox3DRadius() { return m_SimpleBoundingBox3DRadius; }
	float* GetpSimpleBoundingBox3DRadius() { return &m_SimpleBoundingBox3DRadius; }

	bool GetLoadSuccess() { return m_LoadSuccess; }
	void SetLoadSuccess(const bool& success) { m_LoadSuccess = success; }

};

// private ���̃N���X���炵���A�N�Z�X�ł��Ȃ��B���̃N���X�̊֐��Ƃ��B
// prootected private + �p����̃N���X����̓A�N�Z�X�ł���B�Q�[���G���W�����Ƃ��ꂪ�������ȁ`
// public �A�N�Z�X�ł���