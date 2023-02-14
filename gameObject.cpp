
#include "stdafx.h"



//#include "CComponent.h"
//#include "Collision.h"
//#include "Rigidbody.h"
//#include "gameObject.h"
//
//#include "ModelComponent.h"



void GameObject::Load()
{

}

void GameObject::Init()
{

}

void GameObject::Uninit()
{
	for (auto c : m_ComponentList)
		c->Uninit();

	for (auto c : m_Child)
		c->Uninit();

	//m_SimpleBoundingBox2D->Uninit();
	m_SimpleBoundingBox3D->Uninit();
}


void GameObject::Update()
{
	if (this == nullptr)
		return;

	// ���W�̕ۑ�
	m_Old_Position = m_Position;
	m_Old_Rotation = m_Rotation;
	m_Old_Scale = m_Scale;
	m_Old_ScaleRate = m_ScaleRate;

	// �ڒn�����Temp�̃��Z�b�g�B�O�t���[���̂��c��Ȃ��悤�ɂ����Ń��Z�b�g���Ă����B
	m_temp_OnTheGround = false;

	// �o�E���f�B���O�{�b�N�X�̕\�������t���[�����Z�b�g
	m_SimpleBoundingBox3DUseDraw = false;


	// player�Ƃ��q�N���X��Update�̌�ɗ��Ăق����̂ŁAplayer�Ƃ��q�N���X��Update�̍Ō�ɏ����B
	/*for (auto c : m_ComponentList)
		c->Update();*/
		/*------->>>>>> ComponentUpdate()�֐��ɂ܂Ƃ߂�*/


		// �q���̕K�v�����킩��Ȃ��G�G
		/*for (auto c : m_Child)
			c->Update();*/
}


void GameObject::Draw()
{
	for (auto c : m_ComponentList)
		c->Draw();

	//if(m_SimpleBoundingBox2DUseDraw)
	//	m_SimpleBoundingBox2D->Draw();
	if (m_SimpleBoundingBox3DUseDraw)
		m_SimpleBoundingBox3D->Draw();
}


void GameObject::CopyGameObject(GameObject* from)
{
	m_Destroy = from->m_Destroy;


	m_Position = from->m_Position;
	m_Rotation = from->m_Rotation;
	m_Scale = from->m_Scale;
	m_InitScale = from->m_InitScale;
	m_ScaleRate = from->m_ScaleRate;

	m_Velocity = from->m_Velocity;

	m_TagNum = from->m_TagNum;

	m_name = from->m_name;
	m_modelname = from->m_modelname;

	m_DoSave = from->m_DoSave;

	m_OnTheGround = from->m_OnTheGround;
	m_temp_OnTheGround = from->m_temp_OnTheGround;


	// �v�Z�p
	m_Old_Position = from->m_Old_Position;
	m_Old_Rotation = from->m_Old_Rotation;
	m_Old_Scale = from->m_Old_Scale;
	m_Old_ScaleRate = from->m_Old_ScaleRate;


	// SimpleBoundingBox3D�n
	m_SimpleBoundingBox3DRadius = from->m_SimpleBoundingBox3DRadius;

	//// �ǉ�����F
	//m_SynthesizeColor = from->m_SynthesizeColor;
	//m_SynthesizeColorUse = from->m_SynthesizeColorUse;


	// �R���|�[�l���g�n��AddComponent����̂ł����ł̓R�s�[���Ȃ�
	//m_ComponentList = from->m_ComponentList;			// �|�C���^�[�Ȃ̂� new�� ���x AddComponent��OK
	//m_ComponentNameList = from->m_ComponentNameList;	// ���̌�AddObj����̂ł����ŏ���ɒǉ������
	//m_Child = from->m_Child;							// �|�C���^�[�Ȃ̂� new�� ��邯�ǎg���ĂȂ�������u


}


// �R���|�[�l���g�̃N���X���g�����߂ɁAComponentListManager.h��include���邽��
void GameObject::CopyComponentListToAdd(GameObject* from)
{
	for (auto fcomp : from->m_ComponentList)
	{
		if (typeid(*fcomp) == typeid(Collision))
		{
			auto collision = AddComponent<Collision>();		// �R���|�[�l���g��ǉ�����	
			collision->CopyThisComponent(fcomp);			// �R�s�[����
		}
		else if (typeid(*fcomp) == typeid(Rigidbody))
		{
			auto rigidbody = AddComponent<Rigidbody>();
			rigidbody->CopyThisComponent(fcomp);			// �R�s�[����
		}
		else if (typeid(*fcomp) == typeid(ModelComponent))
		{
			auto modelcomponent = AddComponent<ModelComponent>();
			modelcomponent->CopyThisComponent(fcomp);			// �R�s�[����
		}
	}
}



void GameObject::SaveComponent(std::ofstream* Objfile, std::ofstream* ObjfileB)
{
	// �R���|�[�l���g�����邾����������
	// �����ł̓R���|�[�l���g��ID��������������
	for (auto Comp : m_ComponentList)
	{
		int compid = Comp->GetComponentID();
		Objfile->write((char*)&compid, sizeof(compid));
		ObjfileB->write((char*)&compid, sizeof(compid));

		// ���ɂ��̃R���|�[�l���g�̐ݒ���������ށA����̓R���|�[�l���g�ɂ���ē��e���ʂ��Ⴄ
		Comp->Save(Objfile, ObjfileB);

	}
}



void GameObject::DestroyComponent(int compCount)
{


	CComponent* temp = nullptr;

	int count = 0;
	for (auto Comp : m_ComponentList)
	{
		if (count == compCount)
		{
			temp = Comp;				// ���Ƃ�delete���邽�߂ɕۑ����Ă���
			break;						// ���X�g�ŏ��������delete����B���|�C���^�������Ă��܂����߁B
		}
		count++;
	}


	auto it = m_ComponentList.begin();
	for (int i = 0; i < compCount; i++)
		it++;
	m_ComponentList.erase(it);

	temp->Uninit();
	return;
}


void GameObject::DestroyComponentList()
{

	std::list<CComponent*> templist;
	templist = m_ComponentList;

	m_ComponentList.clear();

	//for (auto tempc : templist)
	//{
	//	tempc->Uninit();
	//}

	// ����͈̔̓t�H�[���[�v���ƁAdelete���Ď��͈̔͂ɍs���Ƃ��A�ꏊ�����ꂿ����Ă��������Ȃ�̂Ń_��
	for (auto it = templist.begin(); it != templist.end(); ++it)
	{
		delete *it;
	}
}