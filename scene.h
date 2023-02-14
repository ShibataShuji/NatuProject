#pragma once


#include "gameObject.h"
#include "ImageDisplayFull.h"
#include "CComponent.h"
#include "FPSCamera.h"
#include "camera.h"
#include "MeshFieldObject.h"

using namespace std;

// Scene�N���X���p�����ă^�C�g���A�Q�[���A���U���g�A������Ă����B

class Scene
{
private:
	// ��protected�ɂ��Ă���Ă��Ƃ͌p�����Ďg�����Ă��Ɓ`
	bool m_SecenChange = false;

protected:

	// �悭�g�����̂́A����for���[�v�ŒT���̂�������������̂ŁA�����Ŏ���悤�ɂ��Ă����B
	GameObject* m_Player = nullptr;
	Camera* m_Camera = nullptr;


	std::list<GameObject*> m_GameObjectList[3];			// STL�̃��X�g�\��
	std::list<MeshFieldObject*> m_MeshFieldObjectList;

	std::string m_SceneName;

	bool m_Collision_Visility = false;

	float m_GroundMinHeight = -100.0f;	// �n�ʂ̍Œ�l���V�[�����Ƃɐݒ�ł���


public:
	Scene(){}				// �R���X�g���N�^
	virtual ~Scene(){}		// �f�X�g���N�^ �͂΁[�����邶��Ȃ��ƃ_��
							// ��������Ȃ��ƌp����ŌĂ΂�Ȃ��G�G

	//virtual void Init() = 0;	// �������z�֐�
	virtual void Init();

	virtual void Uninit();

	virtual void Update();

	virtual void Draw();

	void ShadowDraw();

	void SetGroundMinHeight(const float& minheight) { m_GroundMinHeight = minheight; }
	float GetGroundMinHeight() { return m_GroundMinHeight; }


	// ���ʂ̕ϐ����ƕϐ��������炦�Ȃ����ǁAT���ƌ^�������p����
	template <typename T>	//�e���v���[�g�֐�
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObjectList[Layer].push_back(gameObject);

		return gameObject;		// ���������C���X�^���X�̃|�C���^�������Ă���
	}

	ImageDisplayFull* AddImageDisplayFull(const std::string& texname)
	{
		ImageDisplayFull* gameObject = new ImageDisplayFull(texname);
		gameObject->Init();
		m_GameObjectList[2].push_back(gameObject);

		return gameObject;		// ���������C���X�^���X�̃|�C���^�������Ă���
	}

	//template <typename T>	//�e���v���[�g�֐�
	//T* ChangeGameObject(int Layer, GameObject* gameobject)
	//{
	//	DeleteGameObject(gameobject);

	//	T* gameObject = new T();
	//	gameObject->Init();
	//	m_GameObjectList[Layer].push_back(gameObject);

	//	return gameObject;		// ���������C���X�^���X�̃|�C���^�������Ă���
	//}


		// �Q�[���I�u�W�F�N�g�̃v���C���[��T���B���Ȃ������肵���ꍇ��nullptr��Ԃ��B
	GameObject* GetPlayerObject()
	{
		if (m_Player != nullptr)
		{
			if (!m_Player->GetDestroyState())
			{
				return m_Player;
			}
		}
		return nullptr;
	}
	// �Q�[���I�u�W�F�N�g�̃J������T���B���Ȃ������肵���ꍇ��nullptr��Ԃ��B
	Camera* GetCameraObject()
	{
		if (m_Camera != nullptr)
		{
			if (!m_Camera->GetDestroyState())
			{
				return m_Camera;
			}
		}
		return nullptr;
	}
	// �v���C���[�ƃJ�����̃|�C���^�̃Z�b�g
	void SetPlayerObject(GameObject* player)
	{
		m_Player = player;
	}
	void SetCameraObject(Camera* camera)
	{
		m_Camera = camera;
	}


	// �ЂƂ��������݂���Ȃ����
	template <typename T>	//�e���v���[�g�֐�
	T* GetGameObject(int Layer)
	{
		
			for (GameObject* object : m_GameObjectList[Layer])
			{
				// typeid���g���ɂ� #includee <typeinfo> ���K�v
				if (typeid(*object) == typeid(T))		// �^�𒲂ׂ�(RTTI���I�^���)
				{
					return (T*)object;
				}
			}
		
		return nullptr;
	}

	// ���O�Ō�����B�n�߂Ɍ�����1�̂݁B
	GameObject* GetGameObjectWithName(int Layer, std::string name)
	{

		for (GameObject* object : m_GameObjectList[Layer])
		{
			char objname[64];
			object->GetObjectNameChar64(objname);
			if (strcmp(objname, name.c_str()) == 0)
				return object;
		}
		return nullptr;
	}



	// ������������
	template <typename T>	//�e���v���[�g�֐�
	std::vector<T*> GetGameObjects(int Layer)
	{
		std::vector<T*> objects; // STL�̔z��
		
			for (GameObject* object : m_GameObjectList[Layer])
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		
		return objects;
	}

	//// �S�Č�������
	//template <typename T>	//�e���v���[�g�֐�
	//std::vector<T*> GetAllGameObjects(int Layer)
	//{
	//	std::vector<T*> Allobjects; // STL�̔z��

	//	for (GameObject* Allobject : m_GameObjectList[Layer])
	//	{
	//		if (typeid(*Allobject) == typeid(T))
	//		{
	//			Allobjects.push_back((T*)object);
	//		}
	//	}

	//	return Allobjects;
	//}



	// �S�ẴQ�[���I�u�W�F�N�g�̃��X�g���Q�b�g����(���C���[�͕�)
	std::vector<GameObject*> GetAllGameObjects(int Layer)
	{
		std::vector<GameObject*> objects; // STL�̔z��

		for (GameObject* object : m_GameObjectList[Layer])
		{
			objects.push_back(object);
		}

		return objects;
	}

	// �S�ẴZ�[�u�����I�u�W�F�N�g��SetDestroy����
	void DestroyAllGameObjects()
	{
		for (GameObject* object : m_GameObjectList[1])
		{
			if (object->GetDoSave())
			{
				object->SetDestroy();
			}
		}

		for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
		{
			meshfieldobject->SetDestroy();
		}
	}

	// �S�ẴZ�[�u�����Q�[���I�u�W�F�N�g��SetDestroy����B�v���C���[�ȊO
	void DestroyAllGameObjectsIgnorePlayer()
	{
		for (GameObject* object : m_GameObjectList[1])
		{
			if (object->GetDoSave())
			{
				char objname[64];
				object->GetObjectNameChar64(objname);
				char playername[64] = "player";
				if (strcmp(objname, playername) == 0)
					continue;

				object->SetDestroy();
			}
		}

		for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
		{
			meshfieldobject->SetDestroy();
		}
	}

	// �������̃��X�g�̒�����m_DoSave�̂��̂̐���Ԃ�
	int GetGameObjectNumDoSave(std::vector<GameObject*> gameobjectlist)
	{
		int count = 0;
		for (GameObject* object : gameobjectlist)
		{
			if (object->GetDoSave())
				count++;
		}
		return count;
	}

	// �S�ẴQ�[���I�u�W�F�N�g��m_DoSave�̃��X�g���Q�b�g����(���C���[�͕�)
	std::vector<GameObject*> GetAllGameObjectsDoSave(int Layer)
	{
		std::vector<GameObject*> list; // STL�̔z��

		for (GameObject* object : m_GameObjectList[Layer])
		{
			if (object->GetDoSave())
				list.push_back(object);
		}

		return list;
	}

	// �S�ẴQ�[���I�u�W�F�N�g�̎w�肵���R���|�[�l���g�����Ă���I�u�W�F�N�g�̃��X�g��Ԃ�
	template <typename T>	//�e���v���[�g�֐�
	std::vector<GameObject*> GetGameObjectListHasComponent(int Layer)
	{
		std::vector<GameObject*> list; // STL�̔z��

		for (GameObject* object : m_GameObjectList[Layer])
		{
			CComponent* comp = object->GetComponentList();
			T* temp = dynamic_cast<T*>(comp);
			if (temp != nullptr)
			{
				list.push_back(object);
			}
		}
		return list;
	}

	// ���ݔz�u���Ă���Q�[���I�u�W�F�N�g�S�ẴR���W�������݂���悤�ɂ���
	void SetCollisionVisibility_GameObjectList(bool visibility);

	std::list<GameObject*> GetGameObjectList(int Layer)
	{
		return m_GameObjectList[Layer];
	}

	std::string GetSceneName()
	{
		return m_SceneName;
	}
	void SetSceneName(std::string scenename)
	{
		m_SceneName = scenename;
	}

	void SetSecenChange(bool secenchange)
	{
		m_SecenChange = secenchange;
	}

	bool GetCollision_Visility()
	{
		return m_Collision_Visility;
	}
	/*void SetCollision_Visility(bool visility)
	{
		m_Collision_Visility = visility;
	}*/


	// MeshFieldObject�n----------------------------------------------------------------------------------------------
	// ���b�V���t�B�[���h��ǉ�����
	MeshFieldObject* AddMeshFieldObject(Int2 areablock)
	{
		MeshFieldObject* meshfieldobject = new MeshFieldObject();
		meshfieldobject->Init(areablock);
		m_MeshFieldObjectList.push_back(meshfieldobject);

		return meshfieldobject;		// ���������C���X�^���X�̃|�C���^�������Ă���
	}

	// ���b�V���t�B�[���h�̃��X�g���擾
	std::list<MeshFieldObject*> GetMeshFieldObjectList()
	{
		return m_MeshFieldObjectList;
	}

	//	�������̃|�W�V�����̒��S����A�`�ʂ��郁�b�V���t�B�[���h�𔲐��������X�g���擾
	std::list<MeshFieldObject*> GetDrawMeshFieldObjectList();

	// true:�z�u�ς݁B���̃G���A�u���b�N�Ɋ��Ƀ��b�V���t�B�[���h���z�u�ς݂��ǂ���
	bool CheckMeshFieldObjectUsed(Int2 areablock)
	{
		for (auto meshfieldobject : m_MeshFieldObjectList)
		{
			if (meshfieldobject->GetAreaBlock() == areablock)
				return true;
		}

		return false;
	}

	// �w�肵���G���A�u���b�N�̃��b�V���t�B�[���h�I�u�W�F�N�g��Ԃ��B�Ȃ����nullptr��Ԃ��B
	MeshFieldObject* GetMeshFieldObject(Int2 areablock)
	{
		for (auto meshfieldobject : m_MeshFieldObjectList)
		{
			// �����ʒu�̂��̂���������
			if (meshfieldobject->GetAreaBlock() == areablock)
				return meshfieldobject;
		}
		return nullptr;
	}

	// �������̍��W�ł̃��b�V���t�B�[���h�I�u�W�F�N�g�̂��̃|���S���ł̍��������߂�B�����Ƀt�B�[���h���Ȃ��ꍇ-100��Ԃ��B
	float GetMeshFieldObjectHeight(Int2* pOut_block, const D3DXVECTOR3& pos);
	bool GetMeshFieldObjectHeightandNormal(Int2* pOut_block, float* pOut_height, D3DXVECTOR3* pOut_normal, const D3DXVECTOR3& Position);
	int8_t CheckCollisionMeshFieldObjects(Int2* pOut_block, float* pOut_height, D3DXVECTOR3* pOut_normal, D3DXVECTOR3* pOut_beforpos, const D3DXVECTOR3& Position);
	//void SetALLSimpleBoundingBox2DUseDraw(const bool& usedraw);
	void SetALLSimpleBoundingBox3DUseDraw(const bool& usedraw);
	GameObject* GetCollisionRaySimpleBoundingBox3D(D3DXVECTOR3* q1, D3DXVECTOR3* q2);

	std::list<GameObject*> GetDrawGameObjectList();
	std::list<GameObject*> GetUpdateGameObjectList();

};

