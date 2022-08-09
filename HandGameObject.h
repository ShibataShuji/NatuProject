#pragma once

#include "CComponent.h"
//#include "gameObject.h"
#include "Collision.h"

class GameObject;

// ブロックの配置をするときとかに、マインクラフトのように、今何を手に持っているかで配置するものをセットできる。
// また、配置する前の仮配置の時のモデル表示も行っている。
class HandGameObject : public CComponent
{
private:
	GameObject* m_pHandGameObject;

public:

	// 親クラスのコンストラクタをやってくれる。
	using CComponent::CComponent;


	void Init() override
	{

	}

	void Uninit() override
	{

	}

	void Update() override
	{

	}

	void Draw() override
	{

	}

	void SetHandGameObject(GameObject* HandGameObject)
	{
		m_pHandGameObject = HandGameObject; 

		std::list<CComponent*> ComponentList = m_pHandGameObject->GetComponentList();	// コンポーネントのリストを取得
		for (auto HandGameObject_Component : ComponentList)
		{
			// コンポーネントリストのそれがコリジョンクラスだったら
			if (typeid(*HandGameObject_Component) == typeid(Collision))
			{
				// CComponentクラスをダウンをキャストしてCollisionクラスにする
				Collision* collision = dynamic_cast <Collision*>(HandGameObject_Component);

				collision->SetMovable(false);
				collision->SetResponseType(RESPONSE_IGNORE);

			}
		}
	}

	GameObject* GetHandGameObject()
	{
		return m_pHandGameObject;
	}

};

