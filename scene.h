#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "gameObject.h"
#include "CComponent.h"

// Sceneクラスを継承してタイトル、ゲーム、リザルト、等作っていく。

class Scene
{
	// ↓protectedにしてるってことは継承して使うってこと～
protected:
	std::list<GameObject*> m_GameObject[3];	// STLのリスト構造


public:
	Scene(){}				// コンストラクタ
	virtual ~Scene(){}		// デストラクタ はばーちゃんるじゃないとダメ
							// これをやらないと継承先で呼ばれない；；

	virtual void Init() = 0;	// 純粋仮想関数

	// 普通の変数だと変数しかもらえないけど、Tだと型を引き継げる
	template <typename T>	//テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;		// 生成したインスタンスのポインタが入っている
	}

	//template <typename T>	//テンプレート関数
	//T* ChangeGameObject(int Layer, GameObject* gameobject)
	//{
	//	DeleteGameObject(gameobject);

	//	T* gameObject = new T();
	//	gameObject->Init();
	//	m_GameObject[Layer].push_back(gameObject);

	//	return gameObject;		// 生成したインスタンスのポインタが入っている
	//}


	// ひとつだけしかみつけれないやつ
	template <typename T>	//テンプレート関数
	T* GetGameObject(int Layer)
	{
		
			for (GameObject* object : m_GameObject[Layer])
			{
				// typeidを使うには #includee <typeinfo> が必要
				if (typeid(*object) == typeid(T))		// 型を調べる(RTTI動的型情報)
				{
					return (T*)object;
				}
			}
		
		return NULL;
	}


	// 複数見つけるやつ
	template <typename T>	//テンプレート関数
	std::vector<T*> GetGameObjects(int Layer)
	{
		std::vector<T*> objects; // STLの配列
		
			for (GameObject* object : m_GameObject[Layer])
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

	//	for (GameObject* Allobject : m_GameObject[Layer])
	//	{
	//		if (typeid(*Allobject) == typeid(T))
	//		{
	//			Allobjects.push_back((T*)object);
	//		}
	//	}

	//	return Allobjects;
	//}



	// 全て見つけるやつ
	// GameObjectのアドレスの場所を配列に格納していく。
	std::vector<GameObject*> GetAllGameObjects(int Layer)
	{
		std::vector<GameObject*> objects; // STLの配列

		for (GameObject* object : m_GameObject[Layer])
		{
			objects.push_back(object);
		}

		return objects;
	}



	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}
			m_GameObject[i].clear();	// リストのクリア
		}
	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])	// 範囲forループ
			{
				object->Update();
			}

			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });		// ラムダ式
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
	}
};

