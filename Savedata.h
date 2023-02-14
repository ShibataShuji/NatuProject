#pragma once

#define _CRT_SECURE_NO_WARNINGS


#include "gameObject.h"

class Model;		// クラスの前方宣言は使うものがポインターしかない場合のみ可能
					// Model*でしか使わない場合OK！


class Savedata
{
private:
	//std::list<GameObject*> m_LoadedObjectList[3];	// STLのリスト構造

public:
	Savedata() {}

	virtual ~Savedata(){}

	//Model m_model;

	void Init();

	void SaveStage(std::string stagename);	// 配置したブロックのセーブ
	void SaveObject();	// LoadedObjectListにあるものをファイルに変換し保存する。名前で識別される

	void LoadStage(std::string stagename);	// 配置したブロックの読み込み。ステージを読み込むときに読み込む
	void LoadObject();	// ファイルにあるものをLoadedObjectListに読み込む。Initでやっている

	void ResetLoadedStageObjectList();
	void ResetLoadedStageObjectListIgnorePlayer();
	void AddObjectForLoadedStage();
	GameObject* AddObjectForLoadedObjectWithName(std::string objname);
	std::vector<std::string> GetStageNameListForFolder();

	// コリジョン関係
	void SaveCollisionResponse();	
	void LoadCollisionResponse();	// 下記のリストに読み込まれる
	std::vector<std::string>	m_Collision_ObjectTypeSaveList;		// 種類の名前リスト
	std::vector<int>			m_Collision_DefaultResponseList;	// デフォルト応答の設定
	void SetCollisionVisibility_LoadedObjectList(bool visibility);


	std::list<GameObject*> m_LoadedObjectList[3];				// 読み込んでいるオブジェクトのリスト

	std::list<StructPlacedObjectData> m_LoadedStageObjectList;	// 読み込んでいるステージに配置されているオブジェクトのリスト

	std::list<StructPlacedMeshFieldObjectData> m_LoadedStageMeshFieldObjectList;	// 読み込んでいるステージに配置されているメッシュフィールドオブジェクトのリスト
	
	std::list<Model*> m_LoadedModelList;	// 読み込んでいるモデルのリスト


	// 普通の変数だと変数しかもらえないけど、Tだと型を引き継げる
	//template <typename T>	//テンプレート関数
	//T* AddLoadedObject(int Layer)
	//{
	//	T* loadedObject = new T();
	//	loadedObject->Init();
	//	m_LoadedObject[Layer].push_back(loadedObject);

	//	return m_LoadedObject;		// 生成したインスタンスのポインタが入っている
	//}

	//GameObject* AddLoadedObject(int Layer)
	//{
	//	GameObject* loadedObject = new GameObject();
	//	loadedObject->Init();
	//	m_LoadedObjectList[Layer].push_back(loadedObject);

	//	return loadedObject;		// 生成したインスタンスのポインタが入っている
	//}

	void UnLoad_LoadedObjectList()
	{
		for (auto c : m_LoadedObjectList[1])
		{
			if (c != nullptr)
				delete c;
		}
		m_LoadedObjectList[1].clear();
	}

	template <typename T>	//テンプレート関数
	T* AddLoadedObject(int Layer)
	{
		T* loadedObject = new T();
		loadedObject->Init();
		m_LoadedObjectList[Layer].push_back(loadedObject);

		return loadedObject;		// 生成したインスタンスのポインタが入っている
	}

	// リストの末尾のオブジェクトを削除
	void DeleteLastLoadedObject()
	{
		m_LoadedObjectList[1].back()->Uninit();
		GameObject* temp = m_LoadedObjectList[1].back();
		m_LoadedObjectList[1].pop_back();		// 末尾を削除
		delete temp;
	}

	bool CheckSaveObjectName(char* checkname)
	{
		for (auto loaded : m_LoadedObjectList[1])
		{
			char loadname[64];
			loaded->GetObjectNameChar64(loadname);

			// 同じものがあった場合true
			if (!strcmp(checkname, loadname))
			{
				return true;
			}
		}
		return false;
	}

	void CheckSaveObjectList(GameObject* addobject)
	{
		for (auto loadobj : m_LoadedObjectList[1])
		{
			char loadname[64];
			loadobj->GetObjectNameChar64(loadname);
			char addname[64];
			addobject->GetObjectNameChar64(addname);

			if (!strcmp(loadname, addname))
			{
				loadobj->CopyGameObject(addobject);
				loadobj->CopyComponentListToAdd(addobject);

				GameObject* temp = m_LoadedObjectList[1].back();
				m_LoadedObjectList[1].pop_back();
				delete temp;
				return;
			}
		}
	}

	int GetLoadedObjectNum()
	{
		int num = 0;
		for (GameObject* object : m_LoadedObjectList[1])
		{
			num++;
		}
		return num;
	}

	std::list<GameObject*> GetLoadedObjectList(bool* result);

	template <typename T>
	std::list<GameObject*> GetHasComponentLoadedObjectList()
	{
		std::list<GameObject*> reList;
		for (auto obj : m_LoadedObjectList[1])
		{
			if (obj->HasComponent<T>())
			{
				reList.push_back(obj);
			}
		}
		return reList;
	}

	GameObject* GetLoadedObject()
	{
		for (GameObject* object : m_LoadedObjectList[1])
		{
			return object;
		}
	}

	GameObject* GetLoadedObjectWithName(char* NameArray)
	{
		for (GameObject* object : m_LoadedObjectList[1])
		{
			char ObjectName[64]{};
			object->GetObjectNameChar64(ObjectName);
			if (!strcmp(NameArray, ObjectName))	// 同じ名前を発見したら
			{
				return object;
			}
		}
		return nullptr;
	}

	std::list<StructPlacedObjectData> GetLoadedStageObjectList()
	{
		return m_LoadedStageObjectList;
	}

	std::list<StructPlacedMeshFieldObjectData> GetLoadedStageMeshFieldObjectListList()
	{
		return m_LoadedStageMeshFieldObjectList;
	}

	std::list<std::string> GetLoadedObjectNameList();

	void LoadModel();		// フォルダにあるobjファイルを全て読み込みリストに入れる
	Model* GetLoadedModelWithName(std::string modelname);
	std::list<Model*> GetLoadedModelList();
	int GetLoadedModelNum();



	bool CheckSaveStageName(char* checkname)
	{
		for (auto loaded : m_LoadedObjectList[1])
		{
			char loadname[64];
			loaded->GetObjectNameChar64(loadname);

			// 同じものがあった場合true
			if (!strcmp(checkname, loadname))
			{
				return true;
			}
		}
		return false;
	}


	// オブジェクトタイプの種類を追加する
	void Add_Collision_ObjectTypeSaveList(std::string ObjectTypeName, int ResponseNum);

	// オブジェクトタイプの種類を追加する
	void Delete_Collision_ObjectTypeSaveList(int DeleteNum);

	// オブジェクトタイプのリストをもらう
	void Get_Collision_ObjectTypeSaveListData(std::vector<std::string> *reObjectTypeNameList, std::vector<int> *reResponseNum)
	{
		*reObjectTypeNameList = m_Collision_ObjectTypeSaveList;
		*reResponseNum = m_Collision_DefaultResponseList;
	}

	// 指数のオブジェクトタイプのリストをセットする
	void Set_Collision_ObjectTypeSaveNum(int objNum, std::string ObjectTypeName, int ResponseNum)
	{
		m_Collision_ObjectTypeSaveList[objNum] = ObjectTypeName;
		m_Collision_DefaultResponseList[objNum] = ResponseNum;
	}


	void OutputErrorMessages(const std::string& message);


};

                                                                                                                                                                                                             