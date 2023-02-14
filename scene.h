#pragma once


#include "gameObject.h"
#include "ImageDisplayFull.h"
#include "CComponent.h"
#include "FPSCamera.h"
#include "camera.h"
#include "MeshFieldObject.h"

using namespace std;

// Sceneクラスを継承してタイトル、ゲーム、リザルト、等作っていく。

class Scene
{
private:
	// ↓protectedにしてるってことは継承して使うってこと～
	bool m_SecenChange = false;

protected:

	// よく使うものは、毎回forループで探すのも処理が増えるので、ここで取れるようにしておく。
	GameObject* m_Player = nullptr;
	Camera* m_Camera = nullptr;


	std::list<GameObject*> m_GameObjectList[3];			// STLのリスト構造
	std::list<MeshFieldObject*> m_MeshFieldObjectList;

	std::string m_SceneName;

	bool m_Collision_Visility = false;

	float m_GroundMinHeight = -100.0f;	// 地面の最低値をシーンごとに設定できる


public:
	Scene(){}				// コンストラクタ
	virtual ~Scene(){}		// デストラクタ はばーちゃんるじゃないとダメ
							// これをやらないと継承先で呼ばれない；；

	//virtual void Init() = 0;	// 純粋仮想関数
	virtual void Init();

	virtual void Uninit();

	virtual void Update();

	virtual void Draw();

	void ShadowDraw();

	void SetGroundMinHeight(const float& minheight) { m_GroundMinHeight = minheight; }
	float GetGroundMinHeight() { return m_GroundMinHeight; }


	// 普通の変数だと変数しかもらえないけど、Tだと型を引き継げる
	template <typename T>	//テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObjectList[Layer].push_back(gameObject);

		return gameObject;		// 生成したインスタンスのポインタが入っている
	}

	ImageDisplayFull* AddImageDisplayFull(const std::string& texname)
	{
		ImageDisplayFull* gameObject = new ImageDisplayFull(texname);
		gameObject->Init();
		m_GameObjectList[2].push_back(gameObject);

		return gameObject;		// 生成したインスタンスのポインタが入っている
	}

	//template <typename T>	//テンプレート関数
	//T* ChangeGameObject(int Layer, GameObject* gameobject)
	//{
	//	DeleteGameObject(gameobject);

	//	T* gameObject = new T();
	//	gameObject->Init();
	//	m_GameObjectList[Layer].push_back(gameObject);

	//	return gameObject;		// 生成したインスタンスのポインタが入っている
	//}


		// ゲームオブジェクトのプレイヤーを探す。いなかったりした場合はnullptrを返す。
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
	// ゲームオブジェクトのカメラを探す。いなかったりした場合はnullptrを返す。
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
	// プレイヤーとカメラのポインタのセット
	void SetPlayerObject(GameObject* player)
	{
		m_Player = player;
	}
	void SetCameraObject(Camera* camera)
	{
		m_Camera = camera;
	}


	// ひとつだけしかみつけれないやつ
	template <typename T>	//テンプレート関数
	T* GetGameObject(int Layer)
	{
		
			for (GameObject* object : m_GameObjectList[Layer])
			{
				// typeidを使うには #includee <typeinfo> が必要
				if (typeid(*object) == typeid(T))		// 型を調べる(RTTI動的型情報)
				{
					return (T*)object;
				}
			}
		
		return nullptr;
	}

	// 名前で見つける。始めに見つけた1つのみ。
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



	// 複数見つけるやつ
	template <typename T>	//テンプレート関数
	std::vector<T*> GetGameObjects(int Layer)
	{
		std::vector<T*> objects; // STLの配列
		
			for (GameObject* object : m_GameObjectList[Layer])
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		
		return objects;
	}

	//// 全て見つけるやつ
	//template <typename T>	//テンプレート関数
	//std::vector<T*> GetAllGameObjects(int Layer)
	//{
	//	std::vector<T*> Allobjects; // STLの配列

	//	for (GameObject* Allobject : m_GameObjectList[Layer])
	//	{
	//		if (typeid(*Allobject) == typeid(T))
	//		{
	//			Allobjects.push_back((T*)object);
	//		}
	//	}

	//	return Allobjects;
	//}



	// 全てのゲームオブジェクトのリストをゲットする(レイヤーは別)
	std::vector<GameObject*> GetAllGameObjects(int Layer)
	{
		std::vector<GameObject*> objects; // STLの配列

		for (GameObject* object : m_GameObjectList[Layer])
		{
			objects.push_back(object);
		}

		return objects;
	}

	// 全てのセーブされるオブジェクトをSetDestroyする
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

	// 全てのセーブされるゲームオブジェクトをSetDestroyする。プレイヤー以外
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

	// 引き数のリストの中からm_DoSaveのものの数を返す
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

	// 全てのゲームオブジェクトのm_DoSaveのリストをゲットする(レイヤーは別)
	std::vector<GameObject*> GetAllGameObjectsDoSave(int Layer)
	{
		std::vector<GameObject*> list; // STLの配列

		for (GameObject* object : m_GameObjectList[Layer])
		{
			if (object->GetDoSave())
				list.push_back(object);
		}

		return list;
	}

	// 全てのゲームオブジェクトの指定したコンポーネント持っているオブジェクトのリストを返す
	template <typename T>	//テンプレート関数
	std::vector<GameObject*> GetGameObjectListHasComponent(int Layer)
	{
		std::vector<GameObject*> list; // STLの配列

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

	// 現在配置してあるゲームオブジェクト全てのコリジョンをみえるようにする
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


	// MeshFieldObject系----------------------------------------------------------------------------------------------
	// メッシュフィールドを追加する
	MeshFieldObject* AddMeshFieldObject(Int2 areablock)
	{
		MeshFieldObject* meshfieldobject = new MeshFieldObject();
		meshfieldobject->Init(areablock);
		m_MeshFieldObjectList.push_back(meshfieldobject);

		return meshfieldobject;		// 生成したインスタンスのポインタが入っている
	}

	// メッシュフィールドのリストを取得
	std::list<MeshFieldObject*> GetMeshFieldObjectList()
	{
		return m_MeshFieldObjectList;
	}

	//	引き数のポジションの中心から、描写するメッシュフィールドを抜粋したリストを取得
	std::list<MeshFieldObject*> GetDrawMeshFieldObjectList();

	// true:配置済み。そのエリアブロックに既にメッシュフィールドが配置済みかどうか
	bool CheckMeshFieldObjectUsed(Int2 areablock)
	{
		for (auto meshfieldobject : m_MeshFieldObjectList)
		{
			if (meshfieldobject->GetAreaBlock() == areablock)
				return true;
		}

		return false;
	}

	// 指定したエリアブロックのメッシュフィールドオブジェクトを返す。なければnullptrを返す。
	MeshFieldObject* GetMeshFieldObject(Int2 areablock)
	{
		for (auto meshfieldobject : m_MeshFieldObjectList)
		{
			// 同じ位置のものがあったら
			if (meshfieldobject->GetAreaBlock() == areablock)
				return meshfieldobject;
		}
		return nullptr;
	}

	// 引き数の座標でのメッシュフィールドオブジェクトのそのポリゴンでの高さを求める。そこにフィールドがない場合-100を返す。
	float GetMeshFieldObjectHeight(Int2* pOut_block, const D3DXVECTOR3& pos);
	bool GetMeshFieldObjectHeightandNormal(Int2* pOut_block, float* pOut_height, D3DXVECTOR3* pOut_normal, const D3DXVECTOR3& Position);
	int8_t CheckCollisionMeshFieldObjects(Int2* pOut_block, float* pOut_height, D3DXVECTOR3* pOut_normal, D3DXVECTOR3* pOut_beforpos, const D3DXVECTOR3& Position);
	//void SetALLSimpleBoundingBox2DUseDraw(const bool& usedraw);
	void SetALLSimpleBoundingBox3DUseDraw(const bool& usedraw);
	GameObject* GetCollisionRaySimpleBoundingBox3D(D3DXVECTOR3* q1, D3DXVECTOR3* q2);

	std::list<GameObject*> GetDrawGameObjectList();
	std::list<GameObject*> GetUpdateGameObjectList();

};

