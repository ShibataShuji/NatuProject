//
//#include <list>
//#include <string>
//
//#include "ComponentListManager.h"
//// ��������Ă���R���|�[�l���g
//#include "Rigidbody.h"
//#include "Collision.h"
//
//
////static void ComponentListManager::CompareToComponentInit(std::fstream* p_file, GameObject* gameobject)
//void CompareToComponentInit(std::fstream* p_file, GameObject* gameobject)
//{
//	// �t�@�C����Read�̓r���B�R���|�[�l���g���̓ǂ݂��炱���ɂ���B
//	// �R���|�[�l���g��->�R���|�[�l���g�̐ݒ�B�����邾���J��Ԃ��B
//
//	std::list<CComponent*> complist = gameobject->GetComponentList();
//	for (auto comp : complist)
//	{
//		if (typeid(*comp) == typeid(Rigidbody))
//		{
//
//		}
//	}
//
//	//file->open(filename, std::ios::binary | std::ios::in);  ����open�͂��Ă��邩�炵�Ȃ��Ă����B
//
//	// �܂��̓R���|�[�l���g����ǂݍ���
//	char readstr[MaxComponentStrLen];		// 64�������̃T�C�Y
//	p_file->read((char*)&readstr, sizeof(readstr));
//	std::string str = readstr;		// string�^�ɂ���
//
//	//// �R���|�[�l���g�S�Ă̖��O�����X�g�ɕ��ׂĂ���
//	//m_ComponentName.push_back("Rigidbody");
//	//m_ComponentName.push_back("Collision");
//
//	// ��������Ă���R���|�[�l���g���Ƃɏ����Ă���
//	if (str == "Rigidbody")
//	{
//		Rigidbody* p_Rigidbody = gameobject->AddComponent<Rigidbody>();
//		// Init�����H
//	}
//	else if (str == "Collision")
//	{
//		Collision* p_Collision = gameobject->AddComponent<Collision>();	// ���������� gameobject �ɃR���|�[�l���g��ǉ�����
//		p_Collision->InitforSaveData(p_file);		// ���̃R���|�[�l���g�̕ϐ��Ƃ����f�t�H�l����ύX����
//	}
//
//
//}
//
//
//void ListToIntArray(GameObject* gameobject, int* listarray)
//{
//
//	// ���X�g�̏�����
//	for (int i = 0; i < 32; i++)
//	{
//		listarray[i] = 0;
//	}
//
//	std::list<CComponent*> complist = gameobject->GetComponentList();
//	int count = 0;
//	for (auto comp : complist)
//	{
//		if (typeid(*comp) == typeid(Collision))
//		{
//			listarray[count] = CollisionId;
//		}
//		else if (typeid(*comp) == typeid(Rigidbody))
//		{
//			listarray[count] = RigidbodyId;
//		}
//
//
//		count++;
//	}
//}