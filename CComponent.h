#pragma once


// デフォルトサイズ
static const float def_Size = 100.0f;

// 保存読み込み時に扱いやすいように数字で識別する
const static int NullId = 0;
const static int CollisionId = 1;
const static int RigidbodyId = 2;
const static int ModelComponentId = 3;

const static int UpdatePriority_Max = 3;	// コンポーネントのアップデートの優先度

class GameObject;

// CComponentのUpdateとかはgameObjectのUpdateとかで呼んでいる

class CComponent
{
protected:
	GameObject* m_ParentGameObject;		// 親になっているゲームオブジェクト
	std::string m_CompName = "null";	// コンポーネント名	
	int			m_CompId = 0;			// コンポーネントの識別ID

	int			m_UpdatePriority = 0;	// Updateの優先度。0から最初に実行されていき最も遅くて3(今のとこ)

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
	virtual void Uninit() { delete this; }		// だからvirtualをつけて、overrideした関数の方で親の関数も呼ぶようにすれば親も自分も呼ばれてOK
	virtual void Update() {}
	virtual void Draw() {}

	virtual void CopyThisComponent(CComponent* fromComponent) {}	// コピーコンストラクタのようなもの

	virtual void Save(std::ofstream* Objfile, std::ofstream* ObjfileB) {}			// オーバーライドする
	virtual void Load(std::ifstream* Objfile) {}			// オーバーライドする

	std::string GetComponentName()
	{
		return m_CompName;
	}

	int GetComponentID()
	{
		return m_CompId;
	}

	int GetComponentUpdatePriority()
	{
		return m_UpdatePriority;
	}

};

