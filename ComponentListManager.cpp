//
//#include <list>
//#include <string>
//
//#include "ComponentListManager.h"
//// 実装されているコンポーネント
//#include "Rigidbody.h"
//#include "Collision.h"
//
//
////static void ComponentListManager::CompareToComponentInit(std::fstream* p_file, GameObject* gameobject)
//void CompareToComponentInit(std::fstream* p_file, GameObject* gameobject)
//{
//	// ファイルのReadの途中。コンポーネント名の読みからここにくる。
//	// コンポーネント名->コンポーネントの設定。をあるだけ繰り返す。
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
//	//file->open(filename, std::ios::binary | std::ios::in);  既にopenはしてあるからしなくていい。
//
//	// まずはコンポーネント名を読み込む
//	char readstr[MaxComponentStrLen];		// 64文字分のサイズ
//	p_file->read((char*)&readstr, sizeof(readstr));
//	std::string str = readstr;		// string型にする
//
//	//// コンポーネント全ての名前をリストに並べていく
//	//m_ComponentName.push_back("Rigidbody");
//	//m_ComponentName.push_back("Collision");
//
//	// 実装されているコンポーネントごとに書いていく
//	if (str == "Rigidbody")
//	{
//		Rigidbody* p_Rigidbody = gameobject->AddComponent<Rigidbody>();
//		// Init処理？
//	}
//	else if (str == "Collision")
//	{
//		Collision* p_Collision = gameobject->AddComponent<Collision>();	// 今処理中の gameobject にコンポーネントを追加する
//		p_Collision->InitforSaveData(p_file);		// そのコンポーネントの変数とかをデフォ値から変更する
//	}
//
//
//}
//
//
//void ListToIntArray(GameObject* gameobject, int* listarray)
//{
//
//	// リストの初期化
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