
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


// ふつうの関数の宣言
bool getFileNames(std::string folderPath, std::vector<std::string>& file_names);
bool isObjFile(const char* fileName);
bool isDatFile(const char* fileName);


void Savedata::Init()
{
	// コリジョンのオブジェクトタイプの情報をロード
	LoadCollisionResponse();

	// モデル.objの情報をロード
	LoadModel();

	// オブジェクトをロード
	LoadObject();
}




// 配置したブロックのセーブ
void Savedata::SaveStage(std::string stagename)
{
	// ステージ名をファイルパスにする
	std::string filepass = "Data/Stage/" + stagename + ".dat";
	std::string filepassB = "DataBackUp/Stage/" + stagename + ".dat";

	// 全てのゲームオブジェクトのリストをもらう
	Scene* scene = Manager::GetScene();
	std::vector<GameObject*> gameobjectlist = scene->GetAllGameObjects(1);
	int gameobjDoSaveNum = scene->GetGameObjectNumDoSave(gameobjectlist);		// セーブするオブジェクトの数を取得

	// 使用しているオブジェクト名、座標、回転、スケール
			// 座標、回転、スケール等の配置されているもののデータ
	std::ofstream Stagefile;
	std::ofstream StagefileB;
	Stagefile.open(filepass, std::ios::binary | std::ios::out | std::ios::trunc);
	StagefileB.open(filepassB, std::ios::binary | std::ios::out | std::ios::trunc);
	Stagefile.write((char*)&gameobjDoSaveNum, sizeof(gameobjDoSaveNum));		// 最初に保存するオブジェクトの数を書き込む
	StagefileB.write((char*)&gameobjDoSaveNum, sizeof(gameobjDoSaveNum));		// 最初に保存するオブジェクトの数を書き込む

	for (auto gameobject : gameobjectlist)
	{
		// それがセーブするものだったら m_DoSave = 1
		if (gameobject->GetDoSave())
		{
			StructPlacedObjectData PlacedObjectData = gameobject->GetStructPlacedObjectData();

			// 書き込む
			Stagefile.write((char*)&PlacedObjectData, sizeof(PlacedObjectData));
			StagefileB.write((char*)&PlacedObjectData, sizeof(PlacedObjectData));

			//StructPlacedObjectData* PlacedObjectData = &gameobject->GetStructPlacedObjectData();

			//// 書き込む
			//Stagefile.write((char*)PlacedObjectData, sizeof(&PlacedObjectData));
			//StagefileB.write((char*)PlacedObjectData, sizeof(&PlacedObjectData));

		}
	}

	// メッシュフィールドオブジェクトのセーブ
	{
		std::list<MeshFieldObject*> meshfieldobjectList = scene->GetMeshFieldObjectList();
		int MeshFieldObjectListSize = meshfieldobjectList.size();

		Stagefile.write((char*)&MeshFieldObjectListSize, sizeof(MeshFieldObjectListSize));
		StagefileB.write((char*)&MeshFieldObjectListSize, sizeof(MeshFieldObjectListSize));

		for (auto meshfieldobject : meshfieldobjectList)
		{
			StructPlacedMeshFieldObjectData SPMOD = meshfieldobject->GetStructPlacedMeshFieldObjectData();

			// 書き込む
			Stagefile.write((char*)&SPMOD, sizeof(SPMOD));
			StagefileB.write((char*)&SPMOD, sizeof(SPMOD));
		}
	}

	// 閉じる
	Stagefile.close();
	StagefileB.close();

	// セーブ終了
	int adsdsdaa = 4;
	_RPTN(_CRT_WARN, "Saved Stage %d\n", adsdsdaa);
}


void Savedata::SaveObject()
{
	// オブジェクトの保存。Loadedされているオブジェクトを保存する
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

		// 名前の書き込み
		Objectfile.write((char*)&ObjectName, sizeof(ObjectName));
		ObjectfileB.write((char*)&ObjectName, sizeof(ObjectName));

		// モデルの名前の書き込み
		char ModelName[64];
		object->GetModelName(ModelName);
		Objectfile.write((char*)&ModelName, sizeof(ModelName));	// sizeofじゃなくて64って指定したがいいかも？全部
		ObjectfileB.write((char*)&ModelName, sizeof(ModelName));	// sizeofじゃなくて64って指定したがいいかも？全部

		// モデルの情報の書き込み
		StructModelState SMS = object->GetStructModelState();
		Objectfile.write((char*)&SMS, sizeof(SMS));
		ObjectfileB.write((char*)&SMS, sizeof(SMS));

		// SimpleBoundingBox3DのRadiusの書き込み
		{
			float BoundingBoxRadius = object->GetSimpleBoundingBox3DRadius();
			Objectfile.write((char*)&BoundingBoxRadius, sizeof(BoundingBoxRadius));
			ObjectfileB.write((char*)&BoundingBoxRadius, sizeof(BoundingBoxRadius));
		}

		//int tehetehe = 4;
		//// SimpleBoundingBox3DのRadiusの書き込み
		//{
		//	D3DXVECTOR4 SynthesizeColor = object->GetSynthesizeColor();
		//	Objectfile.write((char*)&SynthesizeColor, sizeof(SynthesizeColor));
		//	ObjectfileB.write((char*)&SynthesizeColor, sizeof(SynthesizeColor));
		//}

		
		// 所持しているコンポーネントの数の書き込み
		int compnum;
		compnum = object->GetComponentNum();
		Objectfile.write((char*)&compnum, sizeof(compnum));
		ObjectfileB.write((char*)&compnum, sizeof(compnum));


		// 所持しているコンポーネントの書き込み
		object->SaveComponent(&Objectfile, &ObjectfileB);


		// 閉じる
		Objectfile.close();
		ObjectfileB.close();
		count++;
	}


	// セーブ終了
	int adsdsdaa = 4;
	//_RPTN(_CRT_WARN, "Saved Object %d\n", adsdsdaa);

}

// これはm_LoadedStageObjectListに読み込むだけで配置はしていない
// この後にAddObjectForLoadedStage();を使って配置までする
void Savedata::LoadStage(std::string stagename)
{
	// 現在読み込んでいるステージデータを破棄する
	ResetLoadedStageObjectList();

	// ステージ名をファイルパスにする
	std::string filepass = "Data/Stage/" + stagename + ".dat";

	std::ifstream Stagefile;
	Stagefile.open(filepass, std::ios::binary | std::ios::in);	// Open

	// ステージに配置されているオブジェクトの分読み込む
	{
		int StageObjectNum = 0;
		Stagefile.read((char*)&StageObjectNum, sizeof(StageObjectNum));	// 最初に保存されているオブジェクトの数を取得。

		// 読み込んだ数文読む
		for (int i = 0; i < StageObjectNum; i++)
		{
			StructPlacedObjectData SPOD;
			Stagefile.read((char*)&SPOD, sizeof(SPOD));
			m_LoadedStageObjectList.push_back(SPOD);		// 読んで保存していく。
		}
	}

	// メッシュフィールドの数を読み込みその分読む
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

// 現在読み込んでいるステージデータを破棄する+ゲームオブジェクトも消す
void Savedata::ResetLoadedStageObjectList()
{
	m_LoadedStageObjectList.clear();
	m_LoadedStageMeshFieldObjectList.clear();
	Manager::GetScene()->DestroyAllGameObjects();

}

// 現在読み込んでいるステージデータを破棄する+ゲームオブジェクトも消す
void Savedata::ResetLoadedStageObjectListIgnorePlayer()
{
	m_LoadedStageObjectList.clear();
	m_LoadedStageMeshFieldObjectList.clear();
	Manager::GetScene()->DestroyAllGameObjectsIgnorePlayer();
}

// 配置したブロックのロード
void Savedata::LoadObject()
{
	// Objectフォルダにあるファイル名をすべて取得する
	std::string folderpass = "Data/Object";
	std::vector<std::string> Objectfilenamelist;
	if (!getFileNames(folderpass, Objectfilenamelist))
		exit(100);		// うまく読み込めなかった場合エラー

	for (auto &Rfilename : Objectfilenamelist)
	{
		// 今回実行するオブジェクトの名前を作成
		std::string filename = Rfilename;
		int gg = 3;


		// Loadedobjectとして追加する
		auto addObject = AddLoadedObject<LoadedObject>(1);
		//addObject->AddComponent<Collision>();				 // てすとよう
		//addObject->AddComponent<Collision>();
		//addObject->AddComponent<Collision>();

		std::ifstream Objectfile;
		Objectfile.open(filename, std::ios::binary | std::ios::in);	// Open
		char ObjectName[64]{};
		Objectfile.read((char*)&ObjectName, sizeof(ObjectName));	// 名前の読み込み
		// この時点でうまく読み込めてなかったらオブジェクトをなかったことにして読み込みを終了する
		if (!ObjectName)
		{
			DeleteLastLoadedObject();
			Objectfile.close();
			return;
		}
		addObject->SetObjectName(ObjectName);
		char ModelName[64]{};
		Objectfile.read((char*)&ModelName, sizeof(ModelName));	// モデル名の読み込み
		addObject->SetModelName(ModelName);

		// モデルの情報の読み込み
		StructModelState SMS;
		Objectfile.read((char*)&SMS, sizeof(SMS));
		addObject->SetStructModelState(SMS);

		// SimpleBoundingBox3DのRadiusの読み込み
		{
			float BoundingBoxRadius = 0.0f;
			Objectfile.read((char*)&BoundingBoxRadius, sizeof(BoundingBoxRadius));
			addObject->SetSimpleBoundingBox3DRadius(BoundingBoxRadius);
		}

		//int tehetehe = 4;
		//// 追加する色の読み込み
		//{
		//	
		//	D3DXVECTOR4 SynthesizeColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		//	Objectfile.read((char*)&SynthesizeColor, sizeof(SynthesizeColor));
		//	addObject->SetSynthesizeColor(SynthesizeColor);
		//}


		int compnum = 0;
		Objectfile.read((char*)&compnum, sizeof(compnum));	// 所持しているコンポーネント数の読み込み
		for (int i = 0; i < compnum; i++)
		{
			int compID = 0;
			Objectfile.read((char*)&compID, sizeof(compID));		// 所持しているコンポーネントのID読み込み
			addObject->AddComponentWithID(&Objectfile, compID);		// この中でさらにコンポーネントの設定を読み込む
		}

		// 閉じて次のオブジェクトへ
		Objectfile.close();

	}
}

// 現在ロードしているステージ情報の通りにオブジェクトを配置する(追加)
void Savedata::AddObjectForLoadedStage()
{
	Scene* scene = Manager::GetScene();

	// ゲームオブジェクトの配置
	for (auto &LoadObjSPOD : m_LoadedStageObjectList)
	{
		//LoadObjSPOD.sm_name;
		GameObject* loadedobj = GetLoadedObjectWithName(LoadObjSPOD.sm_name);	// 名前から読み込まれているオブジェクトを検索
		OriginalBlock* addObj = scene->AddGameObject<OriginalBlock>(1);	// とりあえずただのオブジェクトを追加
		addObj->CopyGameObject(loadedobj);	// m_Componentlist以外をコピー(ポインターなのでコピーしたらダメ)
		addObj->CopyComponentListToAdd(loadedobj);	// この中でAddComponentで同じ内容にする。とListも更新される
		// 一通りオブジェクトデータをコピーしたら、座標などを入れていく
		addObj->SetPosition(LoadObjSPOD.sm_Position);
		addObj->SetRotation(LoadObjSPOD.sm_Rotation);
		addObj->SetInitScale(LoadObjSPOD.sm_Scale);
		addObj->SetScaleRate(LoadObjSPOD.sm_ScaleRate);
	}


	// メッシュフィールドオブジェクトの配置
	for (auto& LoadObjSPMOD : m_LoadedStageMeshFieldObjectList)
	{
		MeshFieldObject* meshfieldobject = scene->AddMeshFieldObject(LoadObjSPMOD.sm_AreaBlock);
		//meshfieldobject->Init(LoadObjSPMOD.sm_AreaBlock); // ↑AddでInit行ってる
		meshfieldobject->OverWriteSPMOD(LoadObjSPMOD);
	}


	return;
}

// 指定した名前のオブジェクトを現在のシーンに追加する。
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
			// オブジェクトを追加
			OriginalBlock* addObj = scene->AddGameObject<OriginalBlock>(1);

			addObj->CopyGameObject(loadobj);
			addObj->CopyComponentListToAdd(loadobj);

			return addObj;
		}
	}
	return nullptr;
}


// フォルダーからステージの名前を読み込みリストの作成
std::vector<std::string> Savedata::GetStageNameListForFolder()
{
	std::vector<std::string> Stagenamelist;		// ステージ名だけのリスト

	// Objectフォルダにあるファイル名をすべて取得する
	std::string folderpass = "Data/Stage";
	std::vector<std::string> Stagefilenamelist;		// ステージのファイル名Data/Stage/***.dat
	if (!getFileNames(folderpass, Stagefilenamelist))
		exit(100);		// うまく読み込めなかった場合エラー

	for (auto& Stagefilename : Stagefilenamelist)
	{

		// この時点では Data/Stage/aaa.dat
		std::string NamePlusDat = Stagefilename.substr(11);	// aaa.dat // substr(n)で、n番目以降を取り出せるaaa.dat
		std::string temp = NamePlusDat;					// NameToObj.eraseをするとこれ自体も変わってしまうため
		int length = NamePlusDat.length();
		length = length - 4;
		std::string Name = NamePlusDat.erase(length);		// aaa

		// 名前だけリストに追加する
		Stagenamelist.push_back(Name);
	}

	return Stagenamelist;
}




// モデルのロードファイルにあるobjをm_LoadedModelListにロードして入れられる
void Savedata::LoadModel()
{
	// モデルフォルダにあるファイル名をすべて取得する。objファイル以外も取得してしまう
	std::string folderpass = "asset/model";
	std::vector<std::string> Objectfilenamelist;
	if (!getFileNames(folderpass, Objectfilenamelist))
		exit(100);		// うまく読み込めなかった場合エラー

	for (auto& Rfilename : Objectfilenamelist)
	{
		// それがobjファイルじゃなかったらコンテニュー
		if (!isObjFile(Rfilename.c_str()))
			continue;

		std::string NameToObj = Rfilename.substr(12);	// aaa.obj
		std::string temp = NameToObj;					// NameToObj.eraseをするとこれ自体も変わってしまうため
		int length = NameToObj.length();
		length = length - 4;
		std::string Name = NameToObj.erase(length);		// aaa
		NameToObj = temp;

		

		// 今回実行するオブジェクトの名前を作成
		std::string filename = "asset\\model\\" + NameToObj;	//12文字目以降->asset/model/となっているのでそこを排除

		//std::list<Model*> m_LoadedModelList;	// 読み込んでいるモデルのリスト

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
		//int b = modelname.length();				// stringのサイズも比較に関係あるから注意
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


// コリジョンのレスポンスのセーブ
void Savedata::SaveCollisionResponse()
{
	std::string filepass = "Data/Collision/Response.dat";
	std::string filepassB = "DataBackUp/Collision/Response.dat";

	std::ofstream Responsefile;
	std::ofstream ResponsefileB;
	Responsefile.open(filepass, std::ios::binary | std::ios::out | std::ios::trunc);
	ResponsefileB.open(filepassB, std::ios::binary | std::ios::out | std::ios::trunc);


	// 最初にオブジェクトタイプの種類数を書き込む
	int objectnum = m_Collision_DefaultResponseList.size();
	Responsefile.write((char*)&objectnum, sizeof(objectnum));
	ResponsefileB.write((char*)&objectnum, sizeof(objectnum));

	for (int n = 0; n < objectnum; n++)
	{
		// オブジェクトタイプの名前の書き込み
		std::string ObjectType = m_Collision_ObjectTypeSaveList[n];
		char ObjectTypeC[64];
		StringToChar64(ObjectType, ObjectTypeC);
		Responsefile.write((char*)&ObjectTypeC, sizeof(ObjectTypeC));
		ResponsefileB.write((char*)&ObjectTypeC, sizeof(ObjectTypeC));

		// デフォルトのレスポンスの書き込み
		int defResponse = m_Collision_DefaultResponseList[n];
		Responsefile.write((char*)&defResponse, sizeof(defResponse));
		ResponsefileB.write((char*)&defResponse, sizeof(defResponse));

	}

	// 閉じる
	Responsefile.close();
	ResponsefileB.close();

	// セーブ終了
	int adsdsdaa = 4;
	_RPTN(_CRT_WARN, "Saved Response %d\n", adsdsdaa);
}


// コリジョンのレスポンスのロード
void Savedata::LoadCollisionResponse()
{
	std::string filepass = "Data/Collision/Response.dat";

	std::ifstream Responsefile;
	Responsefile.open(filepass, std::ios::binary | std::ios::in);


	// 最初にオブジェクトタイプの種類数を読み込む。
	int objectnum = 0;
	Responsefile.read((char*)&objectnum, sizeof(objectnum));

	for (int n = 0; n < objectnum; n++)
	{
		// オブジェクトタイプの名前の読み込み
		char ObjectTypeC[64]{};
		Responsefile.read((char*)&ObjectTypeC, sizeof(ObjectTypeC));
		// プッシュバックで追加する(これだと自動的に配列が拡張される)
		std::string ObjectType;
		CharToString(ObjectTypeC, &ObjectType);
		m_Collision_ObjectTypeSaveList.push_back(ObjectType);

		// デフォルトのレスポンスの読み込み
		int defResponse = 0;
		Responsefile.read((char*)&defResponse, sizeof(defResponse));
		m_Collision_DefaultResponseList.push_back(defResponse);
	}

	// 閉じる
	Responsefile.close();

	// ロード終了
	int adsdsdaa = 4;
	_RPTN(_CRT_WARN, "LoadCollisionResponse %d\n", adsdsdaa);
}


// オブジェクトタイプの種類を追加する
void Savedata::Add_Collision_ObjectTypeSaveList(std::string ObjectTypeName, int ResponseNum)
{
	m_Collision_ObjectTypeSaveList.push_back(ObjectTypeName);
	m_Collision_DefaultResponseList.push_back(ResponseNum);

	// Addする場合は現在ロード中全てのオブジェクトに追加する(今後配置しようとするものはこれでOK)
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
	// 既に配置されているものにも追加してあげる
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

	// コリジョンの情報をファイルに保存する
	SaveCollisionResponse();
}

// オブジェクトタイプの種類を削除して詰める
void Savedata::Delete_Collision_ObjectTypeSaveList(int DeleteNum)
{
	m_Collision_ObjectTypeSaveList.erase(std::cbegin(m_Collision_ObjectTypeSaveList) + DeleteNum);
	m_Collision_DefaultResponseList.erase(std::cbegin(m_Collision_DefaultResponseList) + DeleteNum);

	// Deleteする場合は現在ロード中全てのオブジェクトでDeleteする
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
	// 既に配置されているものも削除する
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

	// コリジョンの情報をファイルに保存する
	SaveCollisionResponse();
}



// 読み込んでるリストのコリジョン全ての見えるかどうかを設定する
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
* @brief フォルダ以下のファイル一覧を取得する関数
* @param[in]    folderPath  フォルダパス
* @param[out]   file_names  ファイル名一覧
* return        true:成功, false:失敗
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

	/* エラー処理 */
	if (err) {
		std::cout << err.value() << std::endl;
		std::cout << err.message() << std::endl;
		return false;
	}
	return true;
}

// それの拡張子が.objであるかどうかを判別
bool isObjFile(const char* fileName) 
{
	std::string fname = fileName;
	int found = fname.find(".");
	if (found == std::string::npos)
		return false;

	const char* ext = strrchr(fileName, '.');
	return strcmp(".obj", ext) == 0;
}

// それの拡張子が.datであるかどうかを判別
bool isDatFile(const char* fileName)
{
	const char* ext = strrchr(fileName, '.');
	return strcmp(".dat", ext) == 0;
}


// Errorフォルダのエラーファイルにメッセージが追加出力される
void Savedata::OutputErrorMessages(const std::string& message)
{
	std::string filename = "Error/ErrorMessages.txt";
	std::ofstream EMfile;

	EMfile.open(filename, std::ios::app);

	// 名前の書き込み
	EMfile.write((char*)&message, sizeof(message));

	// 閉じる
	EMfile.close();

	return;
}


// リストが空だったりして取得できたらresultでtrueを返す
std::list<GameObject*> Savedata::GetLoadedObjectList(bool* result)
{

	// 空っぽだったら失敗
	if (m_LoadedObjectList[1].empty())
	{
		*result = false;
		return m_LoadedObjectList[1];
	}

	// 成功したらtrueで返す
	*result = true;
	return m_LoadedObjectList[1];
}