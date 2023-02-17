#include "stdafx.h"


void Scene::SetCollisionVisibility_GameObjectList(bool visibility)
{
	m_Collision_Visility = visibility;

	//std::list<GameObject*> a = GetGameObjectList(1);

	for (auto gameobject : m_GameObjectList[1])
	{
		std::list<CComponent*> complist = gameobject->GetComponentList();
		for (auto comp : complist)
		{
			Collision* collision = dynamic_cast<Collision*>(comp);
			if (collision != nullptr)
			{
				collision->SetVisibility(visibility);
			}
		}
	}
}



void Scene::Init()
{
	//m_SecenChange = true;

	// プレイヤーとカメラのポインタの初期化
	m_Player = nullptr;
	m_Camera = nullptr;

	// 地面を-100.0fで初期化する。先にここが実行されてから、これを継承してるシーンのInitを呼んでるので、設定したければそっちでやる
	m_GroundMinHeight = -100.0f;
}

void Scene::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* object : m_GameObjectList[i])
		{
			assert(object);
			object->Uninit();
			delete object;
		}
		m_GameObjectList[i].clear();	// リストのクリア
		assert(m_GameObjectList[i].empty());
	}

	for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
	{
		meshfieldobject->Uninit();
		delete meshfieldobject;
	}
	m_MeshFieldObjectList.clear();
	assert(m_MeshFieldObjectList.empty());

	m_Player = nullptr;
	m_Camera = nullptr;
}

void Scene::Update()
{
	//// 配置したブロックのセーブ、書き込み
	//if (Input::GetKeyTrigger(DIK_P))
	//{
	//	m_Savedata->Save();
	//}
	//if (Input::GetKeyTrigger(DIK_O))
	//{
	//	m_Savedata->Load();
	//}
	//

	// シーンチェンジしてるよのリセット
	m_SecenChange = false;

	if (!m_GameObjectList->empty())
	{
		for (int i = 0; i < 3; i++)
		{
			if (i == 1)
			{
				// アップデートするオブジェクトだけアップデートする
				std::list<GameObject*> updateobjectlist = GetUpdateGameObjectList();

				for (auto object : updateobjectlist)
				{
					object->Update();
					if (m_SecenChange || m_GameObjectList->empty())
						return;
				}
			}
			else
			{
				for (GameObject* object : m_GameObjectList[i])	// 範囲forループ
				{
					object->Update();

					if (m_SecenChange || m_GameObjectList->empty())
						return;

				}
			}
		}
		for (int i = 0; i < 3; i++)
		{
			// Destroy関数で消す予約されていたら消す。すぐ消すんじゃなくてここで消すことで処理に問題が発生しない
			m_GameObjectList[i].remove_if([](GameObject* object) {return object->Destroy(); });		// ラムダ式
		}
	}

	if (!m_MeshFieldObjectList.empty())
	{
		for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
		{
			meshfieldobject->Update();
		}

		m_MeshFieldObjectList.remove_if([](MeshFieldObject* meshfieldobject) {return meshfieldobject->Destroy(); });		// ラムダ式
	}
}


void Scene::Draw()
{
	for (int i = 0; i < 3; i++)
	{
		if (i == 1)
		{
			// 表示するオブジェクトだけ描写する
			std::list<GameObject*> drawobjectlist = GetDrawGameObjectList();

			for (auto object : drawobjectlist)
			{
				object->Draw();
			}
		}
		else
		{
			for (GameObject* object : m_GameObjectList[i])
			{
				object->Draw();
			}
		}
	}

	// 描写するフィールドだけ描写する
	if (m_Player != nullptr)
	{
		//assert(m_Player);

		std::list<MeshFieldObject*> drawfieldlist = GetDrawMeshFieldObjectList();

		for (MeshFieldObject* meshfieldobject : drawfieldlist)
		{
			meshfieldobject->Draw();
		}
	}
	else if (m_Camera != nullptr)
	{
		std::list<MeshFieldObject*> drawfieldlist = GetDrawMeshFieldObjectList();

		for (MeshFieldObject* meshfieldobject : drawfieldlist)
		{
			meshfieldobject->Draw();
		}
	}
	else
	{
		for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
		{
			meshfieldobject->Draw();
		}
	}

}


void Scene::ShadowDraw()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* object : m_GameObjectList[i])
		{
			if (object->GetUseShadow())
				object->Draw();
		}
	}

	for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
	{
		if (meshfieldobject->GetUseShadow())
			meshfieldobject->Draw();
	}
}


//	引き数のポジションから、描写するメッシュフィールドを抜粋したリストを取得
std::list<MeshFieldObject*> Scene::GetDrawMeshFieldObjectList()
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	if (m_Camera != nullptr)
	{
		float drawCenterLength = 20.0f;
		D3DXVECTOR3 cameraDirection = m_Camera->GetCameraForwardIgnoreY();
		D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
		pos = cameraPos + cameraDirection * drawCenterLength;
	}


	std::list<MeshFieldObject*> re;

	// 現在いるエリアブロックを求める
	Int2   areablockI = GetAreaBlockInt2(pos);
	Float2 areablockF = GetAreaBlockFloat2(pos);

	// 現在のエリアブロックを追加する
	MeshFieldObject* additem = GetMeshFieldObject(areablockI);
	if(additem != nullptr)
		re.push_back(additem);

	{
		Int2 viewareablock = areablockI;
		// xが
		{
			if (FindoutBeforethehalf(areablockF.x, 1.0f))
			{
				// 前半だったら隣のエリアブロックを追加する
				viewareablock.x -= 1;
			}
			else
			{
				// 後半だったら隣のエリアブロックを追加する
				viewareablock.x += 1;
			}
		}
		// yが
		{
			if (FindoutBeforethehalf(areablockF.y, 1.0f))
			{
				// 前半だったら隣のエリアブロックを追加する
				viewareablock.y -= 1;
			}
			else
			{
				// 後半だったら隣のエリアブロックを追加する
				viewareablock.y += 1;

			}
		}
		// 描写するエリアブロックを追加する
		MeshFieldObject* additem;
		// x
		additem = GetMeshFieldObject(Int2(viewareablock.x, areablockI.y));
		if (additem != nullptr)
			re.push_back(additem);
		// z
		additem = GetMeshFieldObject(Int2(areablockI.x, viewareablock.y));
		if (additem != nullptr)
			re.push_back(additem);
		// xz
		additem = GetMeshFieldObject(Int2(viewareablock.x, viewareablock.y));
		if (additem != nullptr)
			re.push_back(additem);



	}
	return re;
}


// 引き数の座標でのメッシュフィールドオブジェクトのそのポリゴンでの高さを求める。そこにフィールドがない場合-100を返す。
float Scene::GetMeshFieldObjectHeight(Int2* pOut_block, const D3DXVECTOR3& pos)
{
	// 現在いるエリアブロックを求める
	Int2   areablockI = GetAreaBlockInt2(pos);
	*pOut_block = areablockI;

	// 現在のエリアブロックのメッシュフィールドオブジェクトを求める
	MeshFieldObject* field = GetMeshFieldObject(areablockI);
	if (field == nullptr)
		return -100.0f;

	return field->GetHeight(pos);
}

// そのブロックのメッシュの高さと法線を返す。そもそもブロックがメッシュの範囲外だったらfalseを返す。
bool Scene::GetMeshFieldObjectHeightandNormal(Int2* pOut_block, float* pOut_height, D3DXVECTOR3* pOut_normal, const D3DXVECTOR3& pos)
{
	// 現在いるエリアブロックを求める
	Int2   areablockI = GetAreaBlockInt2(pos);
	*pOut_block = areablockI;

	// 現在のエリアブロックのメッシュフィールドオブジェクトを求める
	MeshFieldObject* field = GetMeshFieldObject(areablockI);
	if (field == nullptr)
		return false;

	return field->GetHeightandNormal(pOut_height, pOut_normal, pos);
}

// そのブロックのメッシュの高さと法線を返す。衝突していたら1~3を返す
int8_t Scene::CheckCollisionMeshFieldObjects(Int2* pOut_block, float* pOut_height, D3DXVECTOR3* pOut_normal, D3DXVECTOR3* pOut_beforpos, const D3DXVECTOR3& pos)
{
	// 現在いるエリアブロックを求める
	Int2   areablockI = GetAreaBlockInt2(pos);
	*pOut_block = areablockI;

	// 現在のエリアブロックのメッシュフィールドオブジェクトを求める
	MeshFieldObject* field = GetMeshFieldObject(areablockI);
	if (field == nullptr)
		return 0;

	return field->CheckCollisionMeshFieldObject(pOut_height, pOut_normal, pOut_beforpos, pos);
}


//// ゲームオブジェクト全ての簡易バウンディングボックス2Dを表示オフにする
//void Scene::SetALLSimpleBoundingBox2DUseDraw(const bool& usedraw)
//{
//	for (GameObject* object : m_GameObjectList[1])
//	{
//		object->SetSimpleBoundingBox2DUseDraw(false);
//	}
//	return;
//}

// ゲームオブジェクト全ての簡易バウンディングボックス3Dを表示オフにする
void Scene::SetALLSimpleBoundingBox3DUseDraw(const bool& usedraw)
{
	for (GameObject* object : m_GameObjectList[1])
	{
		object->SetSimpleBoundingBox3DUseDraw(false);
	}
	return;
}

// ゲームオブジェクト全ての簡易バウンディングボックス3Dと、マウスカーソルで衝突判定をとる。1番近かったGameObjectを返す
GameObject* Scene::GetCollisionRaySimpleBoundingBox3D(D3DXVECTOR3* q1, D3DXVECTOR3* q2)
{
	// マウスをワールド座標に変換
	D3DXVECTOR3 MouseWorldOrigin;
	D3DXVECTOR3 MouseWorldDirection;
	POINT Sp = GetScreenMousePosition();
	m_Camera->ScreenToWorld(&MouseWorldOrigin, &MouseWorldDirection, Sp.x, Sp.y);

	float lengthminsq = 999.0f;	// 最短長さsq
	GameObject* minobject = nullptr;

	for (GameObject* object : m_GameObjectList[1])
	{
		// OriginalBlockじゃなかったら次へ。例えば空とかだったらそれはレイに含めたくないので
		OriginalBlock* OBobject = dynamic_cast<OriginalBlock*>(object);
		if (OBobject == nullptr)
			continue;

		float radius = object->GetSimpleBoundingBox3DRadius() * 1.20f;	// 球を少し大きめにとる
		D3DXVECTOR3 pos = object->GetPosition();

		if (calcRaySphere(MouseWorldOrigin, MouseWorldDirection, pos, radius, q1, q2))
		{
			D3DXVECTOR3 distance = *q1 - MouseWorldOrigin;
			float lengthsq = D3DXVec3LengthSq(&distance);
			// 最短の更新
			if (lengthsq < pow(lengthminsq, 2))
			{
				lengthminsq = lengthsq;
				minobject = object;
			}
		}
	}

	// 衝突がなかった場合nullptrを返す
	return minobject;
}


//	描写するゲームオブジェクトを抜粋したリストを取得
std::list<GameObject*> Scene::GetDrawGameObjectList()
{
	if (m_Camera == nullptr)
		return m_GameObjectList[1];
	// カメラの方向に球を数個配置するようにして、その球の内側にいるならば表示という風にする

	std::list<GameObject*> re;

	//std::array<std::array<float, FieldSize + 1>, FieldSize + 1> g_fieldHeight;

	const int snum = 3;	//spherenum
	SPHERE sphere[snum];
	D3DXVECTOR3 cameraDirection = m_Camera->GetCameraForward();
	D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
	sphere[0].m_Center = cameraPos;
	sphere[0].m_Radius = 4.0f;
	sphere[1].m_Center = cameraPos + cameraDirection * 13.0f;
	sphere[1].m_Radius = 16.0f;
	sphere[2].m_Center = cameraPos + cameraDirection * 37.0f;
	sphere[2].m_Radius = 35.0f;
	//sphere[3].m_Center = cameraPos + cameraDirection * 65.0f;
	//sphere[3].m_Radius = 40.0f;


	for (auto obj : GetGameObjectList(1))
	{
		D3DXVECTOR3 objPos = obj->GetPosition();

		D3DXVECTOR3 dist = cameraPos - objPos;
		float CameraLenthsq = D3DXVec3LengthSq(&dist);
		float Lenthsq;

		dist = sphere[0].m_Center - objPos;
		Lenthsq = D3DXVec3LengthSq(&dist);
		if (Lenthsq < pow(sphere[0].m_Radius, 2))
			re.push_back(obj);
		else
		{
			dist = sphere[1].m_Center - objPos;
			Lenthsq = D3DXVec3LengthSq(&dist);
			if (Lenthsq < pow(sphere[1].m_Radius, 2))
				re.push_back(obj);
			else
			{
				dist = sphere[2].m_Center - objPos;
				Lenthsq = D3DXVec3LengthSq(&dist);
				if (Lenthsq < pow(sphere[2].m_Radius, 2))
					re.push_back(obj);
				//else
				//{
				//	dist = sphere[3].m_Center - objPos;
				//	Lenthsq = D3DXVec3LengthSq(&dist);
				//	if (Lenthsq < pow(sphere[3].m_Radius, 2))
				//		re.push_back(obj);
				//}
			}
		}
	}
	
	return re;
}


//	アップデートするゲームオブジェクトを抜粋したリストを取得。Drawからsphere[0]のとこ変えただけ
std::list<GameObject*> Scene::GetUpdateGameObjectList()
{
	if (m_Camera == nullptr || m_Player == nullptr)
		return m_GameObjectList[1];

	// カメラの方向に球を数個配置するようにして、その球の内側にいるならば表示という風にする

	std::list<GameObject*> re;

	//std::array<std::array<float, FieldSize + 1>, FieldSize + 1> g_fieldHeight;

	const int snum = 3;	//spherenum
	SPHERE sphere[snum];
	D3DXVECTOR3 cameraDirection = m_Camera->GetCameraForward();
	D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
	sphere[0].m_Center = m_Player->GetPosition();
	sphere[0].m_Radius = 25.0f;
	sphere[1].m_Center = cameraPos + cameraDirection * 13.0f;
	sphere[1].m_Radius = 16.0f;
	sphere[2].m_Center = cameraPos + cameraDirection * 37.0f;
	sphere[2].m_Radius = 35.0f;
	//sphere[3].m_Center = cameraPos + cameraDirection * 65.0f;
	//sphere[3].m_Radius = 40.0f;


	for (auto obj : GetGameObjectList(1))
	{
		{
			D3DXVECTOR3 objPos = obj->GetPosition();

			D3DXVECTOR3 dist = cameraPos - objPos;
			float CameraLenthsq = D3DXVec3LengthSq(&dist);
			float Lenthsq;

			dist = sphere[0].m_Center - objPos;
			Lenthsq = D3DXVec3LengthSq(&dist);
			if (Lenthsq < pow(sphere[0].m_Radius, 2))
				re.push_back(obj);
			else
			{
				dist = sphere[1].m_Center - objPos;
				Lenthsq = D3DXVec3LengthSq(&dist);
				if (Lenthsq < pow(sphere[1].m_Radius, 2))
					re.push_back(obj);
				else
				{
					dist = sphere[2].m_Center - objPos;
					Lenthsq = D3DXVec3LengthSq(&dist);
					if (Lenthsq < pow(sphere[2].m_Radius, 2))
						re.push_back(obj);
					//else
					//{
					//	dist = sphere[3].m_Center - objPos;
					//	Lenthsq = D3DXVec3LengthSq(&dist);
					//	if (Lenthsq < pow(sphere[3].m_Radius, 2))
					//		re.push_back(obj);
					//}
				}
			}
		}
	}

	return re;
}