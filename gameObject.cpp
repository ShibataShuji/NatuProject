
#include "stdafx.h"



//#include "CComponent.h"
//#include "Collision.h"
//#include "Rigidbody.h"
//#include "gameObject.h"
//
//#include "ModelComponent.h"



void GameObject::Load()
{

}

void GameObject::Init()
{

}

void GameObject::Uninit()
{
	for (auto c : m_ComponentList)
		c->Uninit();

	for (auto c : m_Child)
		c->Uninit();

	//m_SimpleBoundingBox2D->Uninit();
	m_SimpleBoundingBox3D->Uninit();
}


void GameObject::Update()
{
	if (this == nullptr)
		return;

	// 座標の保存
	m_Old_Position = m_Position;
	m_Old_Rotation = m_Rotation;
	m_Old_Scale = m_Scale;
	m_Old_ScaleRate = m_ScaleRate;

	// 接地判定のTempのリセット。前フレームのが残らないようにここでリセットしておく。
	m_temp_OnTheGround = false;

	// バウンディングボックスの表示も毎フレームリセット
	m_SimpleBoundingBox3DUseDraw = false;


	// playerとか子クラスのUpdateの後に来てほしいので、playerとか子クラスのUpdateの最後に書く。
	/*for (auto c : m_ComponentList)
		c->Update();*/
		/*------->>>>>> ComponentUpdate()関数にまとめた*/


		// 子供の必要性がわからない；；
		/*for (auto c : m_Child)
			c->Update();*/
}


void GameObject::Draw()
{
	for (auto c : m_ComponentList)
		c->Draw();

	//if(m_SimpleBoundingBox2DUseDraw)
	//	m_SimpleBoundingBox2D->Draw();
	if (m_SimpleBoundingBox3DUseDraw)
		m_SimpleBoundingBox3D->Draw();
}


void GameObject::CopyGameObject(GameObject* from)
{
	m_Destroy = from->m_Destroy;


	m_Position = from->m_Position;
	m_Rotation = from->m_Rotation;
	m_Scale = from->m_Scale;
	m_InitScale = from->m_InitScale;
	m_ScaleRate = from->m_ScaleRate;

	m_Velocity = from->m_Velocity;

	m_TagNum = from->m_TagNum;

	m_name = from->m_name;
	m_modelname = from->m_modelname;

	m_DoSave = from->m_DoSave;

	m_OnTheGround = from->m_OnTheGround;
	m_temp_OnTheGround = from->m_temp_OnTheGround;


	// 計算用
	m_Old_Position = from->m_Old_Position;
	m_Old_Rotation = from->m_Old_Rotation;
	m_Old_Scale = from->m_Old_Scale;
	m_Old_ScaleRate = from->m_Old_ScaleRate;


	// SimpleBoundingBox3D系
	m_SimpleBoundingBox3DRadius = from->m_SimpleBoundingBox3DRadius;

	//// 追加する色
	//m_SynthesizeColor = from->m_SynthesizeColor;
	//m_SynthesizeColorUse = from->m_SynthesizeColorUse;


	// コンポーネント系はAddComponentするのでここではコピーしない
	//m_ComponentList = from->m_ComponentList;			// ポインターなので newで 作るx AddComponentでOK
	//m_ComponentNameList = from->m_ComponentNameList;	// この後AddObjするのでそこで勝手に追加される
	//m_Child = from->m_Child;							// ポインターなので newで 作るけど使ってないから放置


}


// コンポーネントのクラスを使うために、ComponentListManager.hをincludeするため
void GameObject::CopyComponentListToAdd(GameObject* from)
{
	for (auto fcomp : from->m_ComponentList)
	{
		if (typeid(*fcomp) == typeid(Collision))
		{
			auto collision = AddComponent<Collision>();		// コンポーネントを追加する	
			collision->CopyThisComponent(fcomp);			// コピーする
		}
		else if (typeid(*fcomp) == typeid(Rigidbody))
		{
			auto rigidbody = AddComponent<Rigidbody>();
			rigidbody->CopyThisComponent(fcomp);			// コピーする
		}
		else if (typeid(*fcomp) == typeid(ModelComponent))
		{
			auto modelcomponent = AddComponent<ModelComponent>();
			modelcomponent->CopyThisComponent(fcomp);			// コピーする
		}
	}
}



void GameObject::SaveComponent(std::ofstream* Objfile, std::ofstream* ObjfileB)
{
	// コンポーネントがあるだけ書き込む
	// ここではコンポーネントのIDだけを書き込む
	for (auto Comp : m_ComponentList)
	{
		int compid = Comp->GetComponentID();
		Objfile->write((char*)&compid, sizeof(compid));
		ObjfileB->write((char*)&compid, sizeof(compid));

		// 次にそのコンポーネントの設定を書き込む、これはコンポーネントによって内容も量も違う
		Comp->Save(Objfile, ObjfileB);

	}
}



void GameObject::DestroyComponent(int compCount)
{


	CComponent* temp = nullptr;

	int count = 0;
	for (auto Comp : m_ComponentList)
	{
		if (count == compCount)
		{
			temp = Comp;				// あとでdeleteするために保存しておく
			break;						// リストで消した後にdeleteする。がポインタが消えてしまうため。
		}
		count++;
	}


	auto it = m_ComponentList.begin();
	for (int i = 0; i < compCount; i++)
		it++;
	m_ComponentList.erase(it);

	temp->Uninit();
	return;
}


void GameObject::DestroyComponentList()
{

	std::list<CComponent*> templist;
	templist = m_ComponentList;

	m_ComponentList.clear();

	//for (auto tempc : templist)
	//{
	//	tempc->Uninit();
	//}

	// ↑上の範囲フォーループだと、deleteして次の範囲に行くとき、場所がずれちゃっておかしくなるのでダメ
	for (auto it = templist.begin(); it != templist.end(); ++it)
	{
		delete *it;
	}
}