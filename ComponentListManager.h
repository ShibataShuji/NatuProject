#pragma once

// 実装されているコンポーネント
//#include "Rigidbody.h"
//#include "Collision.h"


//
//class GameObject;
//
//const static int MaxComponentStrLen = 64;		// Nameの最大文字数
//
//
//// 保存読み込み時に扱いやすいように数字で識別する
//const static int NullId		 = 0;
//const static int CollisionId = 1;
//const static int RigidbodyId = 2;
//
//
////class ComponentListManager
////{
////private:
////	//static std::list<std::string> m_ComponentName;
////
////public:
////	/*ComponentListManager(){}
////	virtual~ComponentListManager() {}*/
////
////	//static void LoadList()
////	//{
////	//	// コンポーネント全ての名前をリストに並べていく
////	//	m_ComponentName.push_back("Rigidbody");
////	//	m_ComponentName.push_back("Collision");
////	// 
////	//}
////
////
////
////	static void CompareToComponentInit(std::fstream* p_file, GameObject* gameobject);
////	
////};
//
//void CompareToComponentInit(std::fstream* p_file, GameObject* gameobject);
//void ListToIntArray(GameObject* gameobject, int* listarray);