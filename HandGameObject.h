#pragma once

#include "CComponent.h"
//#include "gameObject.h"
#include "Collision.h"

class GameObject;

// �u���b�N�̔z�u������Ƃ��Ƃ��ɁA�}�C���N���t�g�̂悤�ɁA��������Ɏ����Ă��邩�Ŕz�u������̂��Z�b�g�ł���B
// �܂��A�z�u����O�̉��z�u�̎��̃��f���\�����s���Ă���B
class HandGameObject : public CComponent
{
private:
	GameObject* m_pHandGameObject;

public:

	// �e�N���X�̃R���X�g���N�^������Ă����B
	using CComponent::CComponent;


	void Init() override
	{

	}

	void Uninit() override
	{

	}

	void Update() override
	{

	}

	void Draw() override
	{

	}

	void SetHandGameObject(GameObject* HandGameObject)
	{
		m_pHandGameObject = HandGameObject; 

		std::list<CComponent*> ComponentList = m_pHandGameObject->GetComponentList();	// �R���|�[�l���g�̃��X�g���擾
		for (auto HandGameObject_Component : ComponentList)
		{
			// �R���|�[�l���g���X�g�̂��ꂪ�R���W�����N���X��������
			if (typeid(*HandGameObject_Component) == typeid(Collision))
			{
				// CComponent�N���X���_�E�����L���X�g����Collision�N���X�ɂ���
				Collision* collision = dynamic_cast <Collision*>(HandGameObject_Component);

				collision->SetMovable(false);
				collision->SetResponseType(RESPONSE_IGNORE);

			}
		}
	}

	GameObject* GetHandGameObject()
	{
		return m_pHandGameObject;
	}

};

