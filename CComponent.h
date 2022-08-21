#pragma once

#include <d3dx9.h>
#include <iostream>
#include <fstream>
#include <string>

// デフォルトサイズ
static const float def_Size = 100.0f;

// 保存読み込み時に扱いやすいように数字で識別する
const static int NullId = 0;
const static int CollisionId = 1;
const static int RigidbodyId = 2;

class GameObject;

// CComponentのUpdateとかはgameObjectのUpdateとかで呼んでいる

class CComponent
{
protected:
	GameObject* m_ParentGameObject;		// 親になっているゲームオブジェクト
	std::string m_CompName = "null";	// コンポーネント名	
	int			m_CompId = 0;			// コンポーネントの識別ID

public:

	// コンストラクタ。既定の引き数なしコンストラクタは、絶対に親のオブジェクトを設定しないとダメにするために消している。
	CComponent() = delete;
	CComponent(GameObject* gameObject)
	{
		m_ParentGameObject = gameObject;
	}
	// デストラクタ
	virtual ~CComponent(){}

	virtual void Init() {}			// 仮想関数virtualをつけると同じCComponent.Init()を呼んでもoverrideした方の関数が呼ばれる
	virtual void Uninit() {}		// だからvirtualをつけて、overrideした関数の方で親の関数も呼ぶようにすれば親も自分も呼ばれてOK
	virtual void Update() {}
	virtual void Draw() {}

	virtual void Save(std::fstream* Objfile) {}

	std::string GetComponentName()
	{
		return m_CompName;
	}

	int GetComponentID()
	{
		return m_CompId;
	}

};

