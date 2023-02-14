
#include "stdafx.h"

//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include <filesystem>
//#include "Savedata.h"
//
//#include "input.h"
//
//#include "scene.h"
//#include "Collision.h"
//#include "manager.h"
//#include "LoadedObject.h"
//#include "OriginalBlock.h"


// �ӂ��̊֐��̐錾
bool getFileNames(std::string folderPath, std::vector<std::string>& file_names);
bool isObjFile(const char* fileName);
bool isDatFile(const char* fileName);


void Savedata::Init()
{
	// �R���W�����̃I�u�W�F�N�g�^�C�v�̏������[�h
	LoadCollisionResponse();

	// ���f��.obj�̏������[�h
	LoadModel();

	// �I�u�W�F�N�g�����[�h
	LoadObject();
}




// �z�u�����u���b�N�̃Z�[�u
void Savedata::SaveStage(std::string stagename)
{
	// �X�e�[�W�����t�@�C���p�X�ɂ���
	std::string filepass = "Data/Stage/" + stagename + ".dat";
	std::string filepassB = "DataBackUp/Stage/" + stagename + ".dat";

	// �S�ẴQ�[���I�u�W�F�N�g�̃��X�g�����炤
	Scene* scene = Manager::GetScene();
	std::vector<GameObject*> gameobjectlist = scene->GetAllGameObjects(1);
	int gameobjDoSaveNum = scene->GetGameObjectNumDoSave(gameobjectlist);		// �Z�[�u����I�u�W�F�N�g�̐����擾

	// �g�p���Ă���I�u�W�F�N�g���A���W�A��]�A�X�P�[��
			// ���W�A��]�A�X�P�[�����̔z�u����Ă�����̂̃f�[�^
	std::ofstream Stagefile;
	std::ofstream StagefileB;
	Stagefile.open(filepass, std::ios::binary | std::ios::out | std::ios::trunc);
	StagefileB.open(filepassB, std::ios::binary | std::ios::out | std::ios::trunc);
	Stagefile.write((char*)&gameobjDoSaveNum, sizeof(gameobjDoSaveNum));		// �ŏ��ɕۑ�����I�u�W�F�N�g�̐�����������
	StagefileB.write((char*)&gameobjDoSaveNum, sizeof(gameobjDoSaveNum));		// �ŏ��ɕۑ�����I�u�W�F�N�g�̐�����������

	for (auto gameobject : gameobjectlist)
	{
		// ���ꂪ�Z�[�u������̂������� m_DoSave = 1
		if (gameobject->GetDoSave())
		{
			StructPlacedObjectData PlacedObjectData = gameobject->GetStructPlacedObjectData();

			// ��������
			Stagefile.write((char*)&PlacedObjectData, sizeof(PlacedObjectData));
			StagefileB.write((char*)&PlacedObjectData, sizeof(PlacedObjectData));

			//StructPlacedObjectData* PlacedObjectData = &gameobject->GetStructPlacedObjectData();

			//// ��������
			//Stagefile.write((char*)PlacedObjectData, sizeof(&PlacedObjectData));
			//StagefileB.write((char*)PlacedObjectData, sizeof(&PlacedObjectData));

		}
	}

	// ���b�V���t�B�[���h�I�u�W�F�N�g�̃Z�[�u
	{
		std::list<MeshFieldObject*> meshfieldobjectList = scene->GetMeshFieldObjectList();
		int MeshFieldObjectListSize = meshfieldobjectList.size();

		Stagefile.write((char*)&MeshFieldObjectListSize, sizeof(MeshFieldObjectListSize));
		StagefileB.write((char*)&MeshFieldObjectListSize, sizeof(MeshFieldObjectListSize));

		for (auto meshfieldobject : meshfieldobjectList)
		{
			StructPlacedMeshFieldObjectData SPMOD = meshfieldobject->GetStructPlacedMeshFieldObjectData();

			// ��������
			Stagefile.write((char*)&SPMOD, sizeof(SPMOD));
			StagefileB.write((char*)&SPMOD, sizeof(SPMOD));
		}
	}

	// ����
	Stagefile.close();
	StagefileB.close();

	// �Z�[�u�I��
	int adsdsdaa = 4;
	_RPTN(_CRT_WARN, "Saved Stage %d\n", adsdsdaa);
}


void Savedata::SaveObject()
{
	// �I�u�W�F�N�g�̕ۑ��BLoaded����Ă���I�u�W�F�N�g��ۑ�����
	bool result;
	std::list<GameObject*>  LoadedObjectList = GetLoadedObjectList(&result);
	if (!result)
		return;
	int count = 0;
	for (auto object : LoadedObjectList)
	{
		char ObjectName[64];
		object->GetObjectNameChar64(ObjectName);
		std::string ObjectNameS;
		CharToString(ObjectName, &ObjectNameS);
		std::string filename = "Data/Object/" + ObjectNameS + ".dat";
		std::string filenameB = "DataBackUp/Object/" + ObjectNameS + ".dat";

		std::ofstream Objectfile;
		std::ofstream ObjectfileB;
		Objectfile.open(filename, std::ios::binary | std::ios::out | std::ios::trunc);
		ObjectfileB.open(filenameB, std::ios::binary | std::ios::out | std::ios::trunc);

		// ���O�̏�������
		Objectfile.write((char*)&ObjectName, sizeof(ObjectName));
		ObjectfileB.write((char*)&ObjectName, sizeof(ObjectName));

		// ���f���̖��O�̏�������
		char ModelName[64];
		object->GetModelName(ModelName);
		Objectfile.write((char*)&ModelName, sizeof(ModelName));	// sizeof����Ȃ���64���Ďw�肵�������������H�S��
		ObjectfileB.write((char*)&ModelName, sizeof(ModelName));	// sizeof����Ȃ���64���Ďw�肵�������������H�S��

		// ���f���̏��̏�������
		StructModelState SMS = object->GetStructModelState();
		Objectfile.write((char*)&SMS, sizeof(SMS));
		ObjectfileB.write((char*)&SMS, sizeof(SMS));

		// SimpleBoundingBox3D��Radius�̏�������
		{
			float BoundingBoxRadius = object->GetSimpleBoundingBox3DRadius();
			Objectfile.write((char*)&BoundingBoxRadius, sizeof(BoundingBoxRadius));
			ObjectfileB.write((char*)&BoundingBoxRadius, sizeof(BoundingBoxRadius));
		}

		//int tehetehe = 4;
		//// SimpleBoundingBox3D��Radius�̏�������
		//{
		//	D3DXVECTOR4 SynthesizeColor = object->GetSynthesizeColor();
		//	Objectfile.write((char*)&SynthesizeColor, sizeof(SynthesizeColor));
		//	ObjectfileB.write((char*)&SynthesizeColor, sizeof(SynthesizeColor));
		//}

		
		// �������Ă���R���|�[�l���g�̐��̏�������
		int compnum;
		compnum = object->GetComponentNum();
		Objectfile.write((char*)&compnum, sizeof(compnum));
		ObjectfileB.write((char*)&compnum, sizeof(compnum));


		// �������Ă���R���|�[�l���g�̏�������
		object->SaveComponent(&Objectfile, &ObjectfileB);


		// ����
		Objectfile.close();
		ObjectfileB.close();
		count++;
	}


	// �Z�[�u�I��
	int adsdsdaa = 4;
	//_RPTN(_CRT_WARN, "Saved Object %d\n", adsdsdaa);

}

// �����m_LoadedStageObjectList�ɓǂݍ��ނ����Ŕz�u�͂��Ă��Ȃ�
// ���̌��AddObjectForLoadedStage();���g���Ĕz�u�܂ł���
void Savedata::LoadStage(std::string stagename)
{
	// ���ݓǂݍ���ł���X�e�[�W�f�[�^��j������
	ResetLoadedStageObjectList();

	// �X�e�[�W�����t�@�C���p�X�ɂ���
	std::string filepass = "Data/Stage/" + stagename + ".dat";

	std::ifstream Stagefile;
	Stagefile.open(filepass, std::ios::binary | std::ios::in);	// Open

	// �X�e�[�W�ɔz�u����Ă���I�u�W�F�N�g�̕��ǂݍ���
	{
		int StageObjectNum = 0;
		Stagefile.read((char*)&StageObjectNum, sizeof(StageObjectNum));	// �ŏ��ɕۑ�����Ă���I�u�W�F�N�g�̐����擾�B

		// �ǂݍ��񂾐����ǂ�
		for (int i = 0; i < StageObjectNum; i++)
		{
			StructPlacedObjectData SPOD;
			Stagefile.read((char*)&SPOD, sizeof(SPOD));
			m_LoadedStageObjectList.push_back(SPOD);		// �ǂ�ŕۑ����Ă����B
		}
	}

	// ���b�V���t�B�[���h�̐���ǂݍ��݂��̕��ǂ�
	{
		int MeshFieldObjectNum = 0;
		Stagefile.read((char*)&MeshFieldObjectNum, sizeof(MeshFieldObjectNum));
		for (int i = 0; i < MeshFieldObjectNum; i++)
		{
			StructPlacedMeshFieldObjectData SPMOD;
			Stagefile.read((char*)&SPMOD, sizeof(SPMOD));
			m_LoadedStageMeshFieldObjectList.push_back(SPMOD);
		}
	}

	return;
}

// ���ݓǂݍ���ł���X�e�[�W�f�[�^��j������+�Q�[���I�u�W�F�N�g������
void Savedata::ResetLoadedStageObjectList()
{
	m_LoadedStageObjectList.clear();
	m_LoadedStageMeshFieldObjectList.clear();
	Manager::GetScene()->DestroyAllGameObjects();

}

// ���ݓǂݍ���ł���X�e�[�W�f�[�^��j������+�Q�[���I�u�W�F�N�g������
void Savedata::ResetLoadedStageObjectListIgnorePlayer()
{
	m_LoadedStageObjectList.clear();
	m_LoadedStageMeshFieldObjectList.clear();
	Manager::GetScene()->DestroyAllGameObjectsIgnorePlayer();
}

// �z�u�����u���b�N�̃��[�h
void Savedata::LoadObject()
{
	// Object�t�H���_�ɂ���t�@�C���������ׂĎ擾����
	std::string folderpass = "Data/Object";
	std::vector<std::string> Objectfilenamelist;
	if (!getFileNames(folderpass, Objectfilenamelist))
		exit(100);		// ���܂��ǂݍ��߂Ȃ������ꍇ�G���[

	for (auto &Rfilename : Objectfilenamelist)
	{
		// ������s����I�u�W�F�N�g�̖��O���쐬
		std::string filename = Rfilename;
		int gg = 3;


		// Loadedobject�Ƃ��Ēǉ�����
		auto addObject = AddLoadedObject<LoadedObject>(1);
		//addObject->AddComponent<Collision>();				 // �Ă��Ƃ悤
		//addObject->AddComponent<Collision>();
		//addObject->AddComponent<Collision>();

		std::ifstream Objectfile;
		Objectfile.open(filename, std::ios::binary | std::ios::in);	// Open
		char ObjectName[64]{};
		Objectfile.read((char*)&ObjectName, sizeof(ObjectName));	// ���O�̓ǂݍ���
		// ���̎��_�ł��܂��ǂݍ��߂ĂȂ�������I�u�W�F�N�g���Ȃ��������Ƃɂ��ēǂݍ��݂��I������
		if (!ObjectName)
		{
			DeleteLastLoadedObject();
			Objectfile.close();
			return;
		}
		addObject->SetObjectName(ObjectName);
		char ModelName[64]{};
		Objectfile.read((char*)&ModelName, sizeof(ModelName));	// ���f�����̓ǂݍ���
		addObject->SetModelName(ModelName);

		// ���f���̏��̓ǂݍ���
		StructModelState SMS;
		Objectfile.read((char*)&SMS, sizeof(SMS));
		addObject->SetStructModelState(SMS);

		// SimpleBoundingBox3D��Radius�̓ǂݍ���
		{
			float BoundingBoxRadius = 0.0f;
			Objectfile.read((char*)&BoundingBoxRadius, sizeof(BoundingBoxRadius));
			addObject->SetSimpleBoundingBox3DRadius(BoundingBoxRadius);
		}

		//int tehetehe = 4;
		//// �ǉ�����F�̓ǂݍ���
		//{
		//	
		//	D3DXVECTOR4 SynthesizeColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		//	Objectfile.read((char*)&SynthesizeColor, sizeof(SynthesizeColor));
		//	addObject->SetSynthesizeColor(SynthesizeColor);
		//}


		int compnum = 0;
		Objectfile.read((char*)&compnum, sizeof(compnum));	// �������Ă���R���|�[�l���g���̓ǂݍ���
		for (int i = 0; i < compnum; i++)
		{
			int compID = 0;
			Objectfile.read((char*)&compID, sizeof(compID));		// �������Ă���R���|�[�l���g��ID�ǂݍ���
			addObject->AddComponentWithID(&Objectfile, compID);		// ���̒��ł���ɃR���|�[�l���g�̐ݒ��ǂݍ���
		}

		// ���Ď��̃I�u�W�F�N�g��
		Objectfile.close();

	}
}

// ���݃��[�h���Ă���X�e�[�W���̒ʂ�ɃI�u�W�F�N�g��z�u����(�ǉ�)
void Savedata::AddObjectForLoadedStage()
{
	Scene* scene = Manager::GetScene();

	// �Q�[���I�u�W�F�N�g�̔z�u
	for (auto &LoadObjSPOD : m_LoadedStageObjectList)
	{
		//LoadObjSPOD.sm_name;
		GameObject* loadedobj = GetLoadedObjectWithName(LoadObjSPOD.sm_name);	// ���O����ǂݍ��܂�Ă���I�u�W�F�N�g������
		OriginalBlock* addObj = scene->AddGameObject<OriginalBlock>(1);	// �Ƃ肠���������̃I�u�W�F�N�g��ǉ�
		addObj->CopyGameObject(loadedobj);	// m_Componentlist�ȊO���R�s�[(�|�C���^�[�Ȃ̂ŃR�s�[������_��)
		addObj->CopyComponentListToAdd(loadedobj);	// ���̒���AddComponent�œ������e�ɂ���B��List���X�V�����
		// ��ʂ�I�u�W�F�N�g�f�[�^���R�s�[������A���W�Ȃǂ����Ă���
		addObj->SetPosition(LoadObjSPOD.sm_Position);
		addObj->SetRotation(LoadObjSPOD.sm_Rotation);
		addObj->SetInitScale(LoadObjSPOD.sm_Scale);
		addObj->SetScaleRate(LoadObjSPOD.sm_ScaleRate);
	}


	// ���b�V���t�B�[���h�I�u�W�F�N�g�̔z�u
	for (auto& LoadObjSPMOD : m_LoadedStageMeshFieldObjectList)
	{
		MeshFieldObject* meshfieldobject = scene->AddMeshFieldObject(LoadObjSPMOD.sm_AreaBlock);
		//meshfieldobject->Init(LoadObjSPMOD.sm_AreaBlock); // ��Add��Init�s���Ă�
		meshfieldobject->OverWriteSPMOD(LoadObjSPMOD);
	}


	return;
}

// �w�肵�����O�̃I�u�W�F�N�g�����݂̃V�[���ɒǉ�����B
GameObject* Savedata::AddObjectForLoadedObjectWithName(std::string objname)
{
	Scene* scene = Manager::GetScene();
	char objnameC[64];
	StringToChar64(objname, objnameC);

	for (auto loadobj : m_LoadedObjectList[1])
	{
		char loadname[64];
		loadobj->GetObjectNameChar64(loadname);

		if (!strcmp(objnameC, loadname))
		{
			// �I�u�W�F�N�g��ǉ�
			OriginalBlock* addObj = scene->AddGameObject<OriginalBlock>(1);

			addObj->CopyGameObject(loadobj);
			addObj->CopyComponentListToAdd(loadobj);

			return addObj;
		}
	}
	return nullptr;
}


// �t�H���_�[����X�e�[�W�̖��O��ǂݍ��݃��X�g�̍쐬
std::vector<std::string> Savedata::GetStageNameListForFolder()
{
	std::vector<std::string> Stagenamelist;		// �X�e�[�W�������̃��X�g

	// Object�t�H���_�ɂ���t�@�C���������ׂĎ擾����
	std::string folderpass = "Data/Stage";
	std::vector<std::string> Stagefilenamelist;		// �X�e�[�W�̃t�@�C����Data/Stage/***.dat
	if (!getFileNames(folderpass, Stagefilenamelist))
		exit(100);		// ���܂��ǂݍ��߂Ȃ������ꍇ�G���[

	for (auto& Stagefilename : Stagefilenamelist)
	{

		// ���̎��_�ł� Data/Stage/aaa.dat
		std::string NamePlusDat = Stagefilename.substr(11);	// aaa.dat // substr(n)�ŁAn�Ԗڈȍ~�����o����aaa.dat
		std::string temp = NamePlusDat;					// NameToObj.erase������Ƃ��ꎩ�̂��ς���Ă��܂�����
		int length = NamePlusDat.length();
		length = length - 4;
		std::string Name = NamePlusDat.erase(length);		// aaa

		// ���O�������X�g�ɒǉ�����
		Stagenamelist.push_back(Name);
	}

	return Stagenamelist;
}




// ���f���̃��[�h�t�@�C���ɂ���obj��m_LoadedModelList�Ƀ��[�h���ē������
void Savedata::LoadModel()
{
	// ���f���t�H���_�ɂ���t�@�C���������ׂĎ擾����Bobj�t�@�C���ȊO���擾���Ă��܂�
	std::string folderpass = "asset/model";
	std::vector<std::string> Objectfilenamelist;
	if (!getFileNames(folderpass, Objectfilenamelist))
		exit(100);		// ���܂��ǂݍ��߂Ȃ������ꍇ�G���[

	for (auto& Rfilename : Objectfilenamelist)
	{
		// ���ꂪobj�t�@�C������Ȃ�������R���e�j���[
		if (!isObjFile(Rfilename.c_str()))
			continue;

		std::string NameToObj = Rfilename.substr(12);	// aaa.obj
		std::string temp = NameToObj;					// NameToObj.erase������Ƃ��ꎩ�̂��ς���Ă��܂�����
		int length = NameToObj.length();
		length = length - 4;
		std::string Name = NameToObj.erase(length);		// aaa
		NameToObj = temp;

		

		// ������s����I�u�W�F�N�g�̖��O���쐬
		std::string filename = "asset\\model\\" + NameToObj;	//12�����ڈȍ~->asset/model/�ƂȂ��Ă���̂ł�����r��

		//std::list<Model*> m_LoadedModelList;	// �ǂݍ���ł��郂�f���̃��X�g

		Model* loadedModel = new Model();
		loadedModel->SetModelName(Name);
		loadedModel->Load(filename.c_str());
		m_LoadedModelList.push_back(loadedModel);

	}
}

Model* Savedata::GetLoadedModelWithName(std::string modelname)
{
	for (auto model : m_LoadedModelList)
	{
		//int a = model->GetModelName().length();
		//int b = modelname.length();				// string�̃T�C�Y����r�Ɋ֌W���邩�璍��
		if (model->GetModelName() == modelname)
		{
			return model;
		}
	}

	
	exit(2);
	return nullptr;
}

std::list<Model*> Savedata::GetLoadedModelList()
{
	return m_LoadedModelList;
}

int Savedata::GetLoadedModelNum()
{
	int re = 0;
	for (auto model : m_LoadedModelList)
	{
		re++;
	}


	return re;
}



std::list<std::string> Savedata::GetLoadedObjectNameList()
{
	std::list<std::string> re_LoadedObjectNameList;
	for (auto& object : m_LoadedObjectList[1])
	{
		char ObjectNameC[64];
		std::string ObjectNameS;
		object->GetObjectNameChar64(ObjectNameC);
		CharToString(ObjectNameC, &ObjectNameS);
		re_LoadedObjectNameList.push_back(ObjectNameS);
	}
	return re_LoadedObjectNameList;
}


// �R���W�����̃��X�|���X�̃Z�[�u
void Savedata::SaveCollisionResponse()
{
	std::string filepass = "Data/Collision/Response.dat";
	std::string filepassB = "DataBackUp/Collision/Response.dat";

	std::ofstream Responsefile;
	std::ofstream ResponsefileB;
	Responsefile.open(filepass, std::ios::binary | std::ios::out | std::ios::trunc);
	ResponsefileB.open(filepassB, std::ios::binary | std::ios::out | std::ios::trunc);


	// �ŏ��ɃI�u�W�F�N�g�^�C�v�̎�ސ�����������
	int objectnum = m_Collision_DefaultResponseList.size();
	Responsefile.write((char*)&objectnum, sizeof(objectnum));
	ResponsefileB.write((char*)&objectnum, sizeof(objectnum));

	for (int n = 0; n < objectnum; n++)
	{
		// �I�u�W�F�N�g�^�C�v�̖��O�̏�������
		std::string ObjectType = m_Collision_ObjectTypeSaveList[n];
		char ObjectTypeC[64];
		StringToChar64(ObjectType, ObjectTypeC);
		Responsefile.write((char*)&ObjectTypeC, sizeof(ObjectTypeC));
		ResponsefileB.write((char*)&ObjectTypeC, sizeof(ObjectTypeC));

		// �f�t�H���g�̃��X�|���X�̏�������
		int defResponse = m_Collision_DefaultResponseList[n];
		Responsefile.write((char*)&defResponse, sizeof(defResponse));
		ResponsefileB.write((char*)&defResponse, sizeof(defResponse));

	}

	// ����
	Responsefile.close();
	ResponsefileB.close();

	// �Z�[�u�I��
	int adsdsdaa = 4;
	_RPTN(_CRT_WARN, "Saved Response %d\n", adsdsdaa);
}


// �R���W�����̃��X�|���X�̃��[�h
void Savedata::LoadCollisionResponse()
{
	std::string filepass = "Data/Collision/Response.dat";

	std::ifstream Responsefile;
	Responsefile.open(filepass, std::ios::binary | std::ios::in);


	// �ŏ��ɃI�u�W�F�N�g�^�C�v�̎�ސ���ǂݍ��ށB
	int objectnum = 0;
	Responsefile.read((char*)&objectnum, sizeof(objectnum));

	for (int n = 0; n < objectnum; n++)
	{
		// �I�u�W�F�N�g�^�C�v�̖��O�̓ǂݍ���
		char ObjectTypeC[64]{};
		Responsefile.read((char*)&ObjectTypeC, sizeof(ObjectTypeC));
		// �v�b�V���o�b�N�Œǉ�����(���ꂾ�Ǝ����I�ɔz�񂪊g�������)
		std::string ObjectType;
		CharToString(ObjectTypeC, &ObjectType);
		m_Collision_ObjectTypeSaveList.push_back(ObjectType);

		// �f�t�H���g�̃��X�|���X�̓ǂݍ���
		int defResponse = 0;
		Responsefile.read((char*)&defResponse, sizeof(defResponse));
		m_Collision_DefaultResponseList.push_back(defResponse);
	}

	// ����
	Responsefile.close();

	// ���[�h�I��
	int adsdsdaa = 4;
	_RPTN(_CRT_WARN, "LoadCollisionResponse %d\n", adsdsdaa);
}


// �I�u�W�F�N�g�^�C�v�̎�ނ�ǉ�����
void Savedata::Add_Collision_ObjectTypeSaveList(std::string ObjectTypeName, int ResponseNum)
{
	m_Collision_ObjectTypeSaveList.push_back(ObjectTypeName);
	m_Collision_DefaultResponseList.push_back(ResponseNum);

	// Add����ꍇ�͌��݃��[�h���S�ẴI�u�W�F�N�g�ɒǉ�����(����z�u���悤�Ƃ�����̂͂����OK)
	bool result = false;
	std::list<GameObject*> LoadedObjectList;
	LoadedObjectList = GetLoadedObjectList(&result);
	if (!result)
		return;
	for (auto obj : LoadedObjectList)
	{
		std::list<CComponent*>	complist = obj->GetComponentList();
		for (CComponent* comp : complist)
		{
			Collision* collision = dynamic_cast<Collision*>(comp);
			if (collision != nullptr)
			{
				collision->AddResponseObjectList(ResponseNum);
			}
		}
	}
	// ���ɔz�u����Ă�����̂ɂ��ǉ����Ă�����
	std::vector<GameObject*> PlacedObjectList;
	PlacedObjectList = Manager::GetScene()->GetAllGameObjectsDoSave(1);
	for (auto obj : PlacedObjectList)
	{
		std::list<CComponent*>	complist = obj->GetComponentList();
		for (CComponent* comp : complist)
		{
			Collision* collision = dynamic_cast<Collision*>(comp);
			if (collision != nullptr)
			{
				collision->AddResponseObjectList(ResponseNum);
			}
		}
	}

	// �R���W�����̏����t�@�C���ɕۑ�����
	SaveCollisionResponse();
}

// �I�u�W�F�N�g�^�C�v�̎�ނ��폜���ċl�߂�
void Savedata::Delete_Collision_ObjectTypeSaveList(int DeleteNum)
{
	m_Collision_ObjectTypeSaveList.erase(std::cbegin(m_Collision_ObjectTypeSaveList) + DeleteNum);
	m_Collision_DefaultResponseList.erase(std::cbegin(m_Collision_DefaultResponseList) + DeleteNum);

	// Delete����ꍇ�͌��݃��[�h���S�ẴI�u�W�F�N�g��Delete����
	bool result = false;
	std::list<GameObject*> LoadedObjectList;
	LoadedObjectList = GetLoadedObjectList(&result);
	if (!result)
		return;
	for (auto obj : LoadedObjectList)
	{
		std::list<CComponent*>	complist = obj->GetComponentList();
		for (CComponent* comp : complist)
		{
			Collision* collision = dynamic_cast<Collision*>(comp);
			if (collision != nullptr)
			{
				collision->DeleteResponseObjectList(DeleteNum);
			}
		}
	}
	// ���ɔz�u����Ă�����̂��폜����
	std::vector<GameObject*> PlacedObjectList;
	PlacedObjectList = Manager::GetScene()->GetAllGameObjectsDoSave(1);
	for (auto obj : PlacedObjectList)
	{
		std::list<CComponent*>	complist = obj->GetComponentList();
		for (CComponent* comp : complist)
		{
			Collision* collision = dynamic_cast<Collision*>(comp);
			if (collision != nullptr)
			{
				collision->DeleteResponseObjectList(DeleteNum);
			}
		}
	}

	// �R���W�����̏����t�@�C���ɕۑ�����
	SaveCollisionResponse();
}



// �ǂݍ���ł郊�X�g�̃R���W�����S�Ă̌����邩�ǂ�����ݒ肷��
void Savedata::SetCollisionVisibility_LoadedObjectList(bool visibility)
{
	bool result = false;
	std::list<GameObject*> LoadedObjectList;
	LoadedObjectList = GetLoadedObjectList(&result);
	if (!result)
		return;
	for (auto obj : LoadedObjectList)
	{
		std::list<CComponent*>	complist = obj->GetComponentList();
		for (CComponent* comp : complist)
		{
			Collision* collision = dynamic_cast<Collision*>(comp);
			if (collision != nullptr)
			{
				collision->SetVisibility(visibility);
			}
		}
	}
}















/**
* @brief �t�H���_�ȉ��̃t�@�C���ꗗ���擾����֐�
* @param[in]    folderPath  �t�H���_�p�X
* @param[out]   file_names  �t�@�C�����ꗗ
* return        true:����, false:���s
*/
bool getFileNames(std::string folderPath, std::vector<std::string>& file_names)
{
	using namespace std::filesystem;
	directory_iterator iter(folderPath), end;
	std::error_code err;

	for (; iter != end && !err; iter.increment(err)) {
		const directory_entry entry = *iter;

		file_names.push_back(entry.path().string());
		printf("%s\n", file_names.back().c_str());
	}

	/* �G���[���� */
	if (err) {
		std::cout << err.value() << std::endl;
		std::cout << err.message() << std::endl;
		return false;
	}
	return true;
}

// ����̊g���q��.obj�ł��邩�ǂ����𔻕�
bool isObjFile(const char* fileName) 
{
	std::string fname = fileName;
	int found = fname.find(".");
	if (found == std::string::npos)
		return false;

	const char* ext = strrchr(fileName, '.');
	return strcmp(".obj", ext) == 0;
}

// ����̊g���q��.dat�ł��邩�ǂ����𔻕�
bool isDatFile(const char* fileName)
{
	const char* ext = strrchr(fileName, '.');
	return strcmp(".dat", ext) == 0;
}


// Error�t�H���_�̃G���[�t�@�C���Ƀ��b�Z�[�W���ǉ��o�͂����
void Savedata::OutputErrorMessages(const std::string& message)
{
	std::string filename = "Error/ErrorMessages.txt";
	std::ofstream EMfile;

	EMfile.open(filename, std::ios::app);

	// ���O�̏�������
	EMfile.write((char*)&message, sizeof(message));

	// ����
	EMfile.close();

	return;
}


// ���X�g���󂾂����肵�Ď擾�ł�����result��true��Ԃ�
std::list<GameObject*> Savedata::GetLoadedObjectList(bool* result)
{

	// ����ۂ������玸�s
	if (m_LoadedObjectList[1].empty())
	{
		*result = false;
		return m_LoadedObjectList[1];
	}

	// ����������true�ŕԂ�
	*result = true;
	return m_LoadedObjectList[1];
}