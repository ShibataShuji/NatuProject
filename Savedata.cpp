

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Savedata.h"

#include "input.h"

#include "scene.h"
#include "manager.h"
#include "gameObject.h"

// �z�u�����u���b�N�̃Z�[�u
void Savedata::Save()
{
	// �S�ẴQ�[���I�u�W�F�N�g�̃��X�g�����炤
	Scene* scene = Manager::GetScene();
	std::vector<GameObject*> gameobjectlist = scene->GetAllGameObjects(1);
	for (auto& gameobject : gameobjectlist)
	{
		// ���ꂪ�Z�[�u������̂������� m_DoSave = 1
		if (gameobject->GetDoSave())
		{
			StructPlacedObjectData PlacedObjectData = gameobject->GetStructPlacedObjectData();
			// �g�p���Ă���I�u�W�F�N�g���A���W�A��]�A�X�P�[��
			// ���W�A��]�A�X�P�[�����̔z�u����Ă�����̂̃f�[�^
			std::fstream Stagefile;
			Stagefile.open(".Data/Stage/TestStageA.dat", std::ios::binary | std::ios::out | std::ios::trunc);

			// ��������
			Stagefile.write((char*)&PlacedObjectData, sizeof(PlacedObjectData));

			// ����@��������̂��Ԃ������Ƃ�����̂Ŗ{���͂����ł͂Ȃ�
			Stagefile.close();



			// �g�p���Ă���I�u�W�F�N�g�����A����̃Z�[�u�ŏ��o��Ȃ�A�Z�[�u����

			// �z�u����Ă���u���b�N�̒��g�̃f�[�^�B�x�̃R���|�[�l���g�������Ă��邩�A���f������
			// �I�u�W�F�N�g���A���f�����A�R���|�[�l���g���X�g(�R���|�[�l���g�̐���ID)

			std::fstream Objectfile;
			Objectfile.open(".Data/Object/TestObjectA.dat", std::ios::binary | std::ios::out | std::ios::trunc);

			// ���O�̏�������
			Objectfile.write((char*)&PlacedObjectData.sm_name, sizeof(PlacedObjectData.sm_name));

			// ���f���̖��O�̏�������
			char ModelName[64];
			gameobject->GetModelName(ModelName);
			Objectfile.write((char*)&ModelName, sizeof(ModelName));	// sizeof����Ȃ���64���Ďw�肵�������������H

			// �������Ă���R���|�[�l���g�̐��̏�������
			int compnum;
			compnum = gameobject->GetComponentNum();
			Objectfile.write((char*)&compnum, sizeof(compnum));	// sizeof����Ȃ���64���Ďw�肵�������������H

			// �������Ă���R���|�[�l���g�̏�������
			gameobject->SaveComponent(&Objectfile);

			// ����
			Stagefile.close();



		}
	}
}


// �z�u�����u���b�N�̃��[�h
void Savedata::Load()
{
	//std::string filename = "./filename.dat";
	//// �z�u�����u���b�N�̃��[�h
	//if (Input::GetKeyTrigger(DIK_P))
	//{
	//	std::fstream file;
	//	file.open(filename, std::ios::binary | std::ios::in);

	//	// �ǂݍ���
	//	file.read((char*)&n, sizeof(n));
	//	cout << "HP:" << n.hp << endl;
	//	cout << "Power:" << n.power << endl;
	//	cout << "Defense:" << n.defense << endl;
	//	cout << "Speed:" << n.speed << endl;

	//	// ����
	//	file.close();
	//	}
}