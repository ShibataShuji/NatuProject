#pragma once

#include <d3dx9.h>

// デフォルトサイズ
static const float def_Size = 100.0f;

class GameObject;

// CComponentのUpdateとかはgameObjectのUpdateとかで呼んでいる

class CComponent
{
protected:
	GameObject* m_ParentGameObject;		// 親になっているゲームオブジェクト

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

};

