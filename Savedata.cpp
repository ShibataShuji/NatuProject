

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Savedata.h"

#include "input.h"

#include "scene.h"
#include "manager.h"
#include "gameObject.h"

// 配置したブロックのセーブ
void Savedata::Save()
{
	// 全てのゲームオブジェクトのリストをもらう
	Scene* scene = Manager::GetScene();
	std::vector<GameObject*> gameobjectlist = scene->GetAllGameObjects(1);
	for (auto& gameobject : gameobjectlist)
	{
		// それがセーブするものだったら m_DoSave = 1
		if (gameobject->GetDoSave())
		{
			StructPlacedObjectData PlacedObjectData = gameobject->GetStructPlacedObjectData();
			// 使用しているオブジェクト名、座標、回転、スケール
			// 座標、回転、スケール等の配置されているもののデータ
			std::fstream Stagefile;
			Stagefile.open(".Data/Stage/TestStageA.dat", std::ios::binary | std::ios::out | std::ios::trunc);

			// 書き込む
			Stagefile.write((char*)&PlacedObjectData, sizeof(PlacedObjectData));

			// 閉じる　たくさんのおぶじぇくとがあるので本来はここではない
			Stagefile.close();



			// 使用しているオブジェクト名が、今回のセーブで初登場なら、セーブする

			// 配置されているブロックの中身のデータ。度のコンポーネントを持っているか、モデル名等
			// オブジェクト名、モデル名、コンポーネントリスト(コンポーネントの数とID)

			std::fstream Objectfile;
			Objectfile.open(".Data/Object/TestObjectA.dat", std::ios::binary | std::ios::out | std::ios::trunc);

			// 名前の書き込み
			Objectfile.write((char*)&PlacedObjectData.sm_name, sizeof(PlacedObjectData.sm_name));

			// モデルの名前の書き込み
			char ModelName[64];
			gameobject->GetModelName(ModelName);
			Objectfile.write((char*)&ModelName, sizeof(ModelName));	// sizeofじゃなくて64って指定したがいいかも？

			// 所持しているコンポーネントの数の書き込み
			int compnum;
			compnum = gameobject->GetComponentNum();
			Objectfile.write((char*)&compnum, sizeof(compnum));	// sizeofじゃなくて64って指定したがいいかも？

			// 所持しているコンポーネントの書き込み
			gameobject->SaveComponent(&Objectfile);

			// 閉じる
			Stagefile.close();



		}
	}
}


// 配置したブロックのロード
void Savedata::Load()
{
	//std::string filename = "./filename.dat";
	//// 配置したブロックのロード
	//if (Input::GetKeyTrigger(DIK_P))
	//{
	//	std::fstream file;
	//	file.open(filename, std::ios::binary | std::ios::in);

	//	// 読み込む
	//	file.read((char*)&n, sizeof(n));
	//	cout << "HP:" << n.hp << endl;
	//	cout << "Power:" << n.power << endl;
	//	cout << "Defense:" << n.defense << endl;
	//	cout << "Speed:" << n.speed << endl;

	//	// 閉じる
	//	file.close();
	//	}
}