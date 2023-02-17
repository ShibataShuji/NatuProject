#pragma once

#define OBJNUM (4)


#include "CComponent.h"
#include "SimpleBoundingBox3D.h"

// セーブデータに保存されるもの
// 配置されている情報
struct StructPlacedObjectData		
{
	char		sm_name[64] = "null";
	D3DXVECTOR3 sm_Position = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 sm_Rotation = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 sm_Scale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 sm_ScaleRate = { 1.0f, 1.0f, 1.0f };

};

//// オブジェクトの内部データの情報
//struct StructObjectData
//{
//	char		sm_name[64] = "null";
//	char		sm_modelname[64] = "null";
//	int			sm_componentlist[32];			// 注意 初期化できてないので使う前に初期化する
//
//};

// モデルの情報セーブとロード用構造体
struct StructModelState
{
	D3DXVECTOR3 sm_ModelOffset;
	D3DXVECTOR3 sm_ModelRotation;
	D3DXVECTOR3 sm_ModelScale;
};




class GameObject
{
protected:	// アクセス指定子

	bool		m_Destroy = false;

	bool		m_LoadSuccess = true;


	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_InitScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_ScaleRate = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 設定された初期値に対しての倍率

	D3DXVECTOR3 m_ModelOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ModelRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ModelScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXVECTOR3	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 現在の速度ベクトル

	int			m_TagNum = 0;

	std::string		m_name = "null";
	std::string		m_modelname = "null";

	int			m_DoSave = 0;			// 保存するときにセーブされるかどうか

	bool		m_OnTheGround = false;
	bool		m_temp_OnTheGround = false;


	// 計算用
	D3DXVECTOR3 m_Old_Position;
	D3DXVECTOR3 m_Old_Rotation;
	D3DXVECTOR3 m_Old_Scale;
	D3DXVECTOR3 m_Old_ScaleRate;



	std::list<CComponent*> m_ComponentList;
	std::list<std::string> m_ComponentNameList;
	std::list<GameObject*> m_Child;


	// Collisionコンポーネントを1つでも持っているときに使うよう変数
	std::list<GameObject*> m_Collision_HitObjectList;
	std::list<D3DXVECTOR3> m_Collision_HitPositionList;
	std::list<GameObject*> m_Collision_OverlapObjectList;
	std::list<D3DXVECTOR3> m_Collision_OverlapPositionList;


	// シェーダー関連 はモデルコンポーネントで管理している。
	// 影を使うかどうかはオブジェクト単位でやりたいのでこっちで管理
	bool m_UseShadow = false;

	//// 簡易表示用2Dバウンディングボックス
	//SimpleBoundingBox2D*	m_SimpleBoundingBox2D = nullptr;
	//bool					m_SimpleBoundingBox2DUseDraw = false;		// 表示するかしないか

		// 簡易表示用3Dバウンディングボックス
	SimpleBoundingBox3D*	m_SimpleBoundingBox3D = nullptr;
	float					m_SimpleBoundingBox3DRadius = 1.5f;			// 実際のオブジェクトの画面でこの値をいじって調整する
	bool					m_SimpleBoundingBox3DUseDraw = false;		// 表示するかしないか

	//// テクスチャーに追加する色の設定
	//D3DXVECTOR4				m_SynthesizeColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	//bool					m_SynthesizeColorUse = false;


	/*GameObject*				m_Parent;*/

public:

	GameObject() 
	{
		//m_SimpleBoundingBox2D = new SimpleBoundingBox2D(this);
		//m_SimpleBoundingBox2D->Init();
		m_SimpleBoundingBox3D = new SimpleBoundingBox3D(this);
		m_SimpleBoundingBox3D->Init();
	}

	virtual ~GameObject()
	{
		for (auto c : m_ComponentList)
		{
			if(c != nullptr)
				delete c;
		}
		m_ComponentList.clear();

		m_ComponentNameList.clear();

		for (auto c : m_Child)
			delete c;
		m_Child.clear();

		//delete m_SimpleBoundingBox2D;
		delete m_SimpleBoundingBox3D;
	}

	int Gettagnum()
	{
		return m_TagNum;
	}

	void Load();

	virtual void Init();

	virtual void Uninit();

	// virtual(純粋仮想関数)なので、これを継承したplayerクラスなどでUpdate関数を作るとそっちが呼ばれて
	// これは呼ばれないが、playerのupdateの方でこれを呼ぶ(親クラスのupdate)と呼べる。
	// これの子クラスであるplayerとかの中で明示的にGameObject::Updateって風に呼ばないといけない。
	// Drawとか忘れがちなので注意
	virtual void Update();

	virtual void Draw();


	// コンポーネントのアップデート
	virtual void ComponentUpdate()
	{
		//// コリジョン系の変数のリセット
		ResetCollisionHitArray();

		for (int i = 0; i < UpdatePriority_Max; i++)
		{
			for (auto comp : m_ComponentList)
			{
				if(comp->GetComponentUpdatePriority() == i)
					comp->Update();
			}
		}
	}

	// Tempの更新
	//virtual void TemporarySetUpdate()
	//{
	//	m_Position = m_temp_Position;
	//	m_Velocity = m_temp_Velocity;
	//}

	void ResetCollisionHitArray()
	{
		// コリジョン系の変数のリセット
		m_Collision_HitObjectList.clear();
		m_Collision_HitPositionList.clear();
		m_Collision_OverlapObjectList.clear();
		m_Collision_OverlapPositionList.clear();
	}

	void AddPosition(D3DXVECTOR3 AddPosition) { m_Position += AddPosition; }

	void SetPosition(D3DXVECTOR3 Position){	m_Position = Position;	}
	void SetPosition_x(float x){	m_Position.x = x;	}
	void SetPosition_y(float y){	m_Position.y = y;	}
	void SetPosition_z(float z){	m_Position.z = z;	}

	void SetRotation(D3DXVECTOR3 rotation){ m_Rotation = rotation;	}

	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3* GetpPosition() { return &m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3* GetpRotation() { return &m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXVECTOR3* GetpScale() { return &m_Scale; }
	D3DXVECTOR3 GetScaleRate() { return m_ScaleRate; }
	D3DXVECTOR3* GetpScaleRate() { return &m_ScaleRate; }

	D3DXVECTOR3 GetOldPosition() { return m_Old_Position; }
	D3DXVECTOR3 GetOldRotation() { return m_Old_Rotation; }
	D3DXVECTOR3 GetOldScale() { return m_Old_Scale; }
	D3DXVECTOR3 GetOldScaleRate() { return m_Old_ScaleRate; }
	//void SetOldPosition(D3DXVECTOR3 Position) { m_Old_Position = Position; }

	//D3DXVECTOR3 GetTempPosition() { return m_temp_Position; }
	//void SetTempPosition(D3DXVECTOR3 Position) { m_temp_Position = Position; }

	void SetModelOffset(D3DXVECTOR3 offset) { m_ModelOffset = offset; }
	void SetModelRotation(D3DXVECTOR3 rotation) { m_ModelRotation = rotation; }
	void SetModelScale(D3DXVECTOR3 scale) { m_ModelScale = scale; }

	D3DXVECTOR3 GetModelOffset() { return m_ModelOffset; }
	D3DXVECTOR3 GetModelRotation() { return m_ModelRotation; }
	D3DXVECTOR3 GetModelScale() { return m_ModelScale; }


	D3DXVECTOR3 GetVelocity() { return m_Velocity; }
	void SetVelocity(D3DXVECTOR3 Velocity) { m_Velocity = Velocity; }
	void SetVelocity_x(float x) { m_Velocity.x = x; }
	void SetVelocity_y(float y) { m_Velocity.y = y; }
	void SetVelocity_z(float z) { m_Velocity.z = z; }

	const bool& GetUseShadow() { return m_UseShadow; }
	void SetUseShadow(const bool *shadow) { m_UseShadow = &shadow; }


	//D3DXVECTOR3 GetTempVelocity() { return m_temp_Velocity; }
	//void SetTempVelocity(D3DXVECTOR3 Velocity) { m_temp_Velocity = Velocity; }

	int GetDoSave()
	{
		return m_DoSave;
	}

	bool GetDestroyState()
	{
		if (this == nullptr)
			return true;
		return m_Destroy;
	}

	void SetDestroy() { m_Destroy = true; }

	// Initではこれを使ってスケールを設定してあげる
	void SetInitScale(D3DXVECTOR3 setscale) 
	{ 
		m_InitScale = setscale;
		m_Scale = setscale;
	}


	void SetScaleRate(D3DXVECTOR3 setscalerate) 
	{ 
		m_ScaleRate = setscalerate; 
		m_Scale.x = m_InitScale.x * m_ScaleRate.x;
		m_Scale.y = m_InitScale.y * m_ScaleRate.y;
		m_Scale.z = m_InitScale.z * m_ScaleRate.z;
	}

	// このオブジェクトのm_Scale,m_Rotation,m_Positionを使ってワールドマトリクスを0から作る
	D3DXMATRIX GetWorldMatrix()
	{
		// ModelOffsetとかModelがないときのバージョン
		//D3DXMATRIX world, scale, rot, trans;
		//D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		//D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
		//world = scale * rot * trans;
		//return world;

		// Model系込み
		D3DXVECTOR3 Scale;						// Scale は掛け算
		Scale.x = m_Scale.x * m_ModelScale.x;
		Scale.y = m_Scale.y * m_ModelScale.y;
		Scale.z = m_Scale.z * m_ModelScale.z;
		D3DXVECTOR3 Rotation;					// Rotation は足し算
		Rotation = m_Rotation + m_ModelRotation;
		D3DXVECTOR3 Position;					// Position も足し算
		Position = m_Position + m_ModelOffset;

		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
		D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
		world = scale * rot * trans;
		return world;
	}


	D3DXVECTOR3 GetForward()	// 前方向ベクトル
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;

	}

	D3DXVECTOR3 GetRight()	// 右方向ベクトル
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}

	D3DXVECTOR3 GetUp()	// 上方向ベクトル
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	// コンポーネントを追加する。引き数で名前を付けることができる。空白の場合勝手に命名される
	template <typename T>
	T* AddComponent()
	{
		// このオブジェクトが追加するコンポーネントの親になるので、コンストラクタでthisを設定する
		T* ccomponent  = new T(this);
		ccomponent->Init();
		m_ComponentList.push_back(ccomponent);
		m_ComponentNameList.push_back("noname");

		return ccomponent;
	}
	template <typename T>
	T* AddComponent(std::string name)
	{
		T* ccomponent = new T(this);
		ccomponent->Init();
		m_ComponentList.push_back(ccomponent);
		m_ComponentNameList.push_back(name);

		return ccomponent;
	}

	template <typename T>
	T* GetComponent()
	{
		for (auto c : m_ComponentList)
		{
			if (typeid(*c) == typeid(T))
			{
				return (T*)c;
			}
		}
		return nullptr;
	}

	// このゲームオブジェクトの中に引き数の名前をつけているコンポーネントがあればそのコンポーネントを返す
	// あってはならないが複数あったらリストの最初のものが返る
	template <typename T>
	T* GetComponentWithName(std::string name)
	{
		int itrCount = 0;
		for (auto cName : m_ComponentNameList)
		{
			// 文字列が同じものがあったら
			if (name == cName)
			{
				auto itr = m_ComponentList.begin();		// 本当は std::list<int>::iterator itr だけどautoで省略
				for (int i = 0; i < itrCount; i++)
				{
					++itr;
				}
				return (T*)*itr;						// itrはポインタなので*itrで中身を返してあげる
			}

			itrCount++;
		}
		return nullptr;
	}
	
	std::list<CComponent*> GetComponentList()
	{
		return m_ComponentList;
	}

	template <typename T>
	bool HasComponent()
	{
		for (auto c : m_ComponentList)
		{
			if (typeid(*c) == typeid(T))
			{
				return true;
			}
		}
		return false;
	}



	bool GetOnTheGround()
	{
		return m_OnTheGround;
	}
	// tempも同時にセットされる
	void SetOnTheGround(bool setbool)
	{
		m_temp_OnTheGround = setbool;
		m_OnTheGround = setbool;
	}
	bool GetTempOnTheGround()
	{
		return m_temp_OnTheGround;
	}
	void SetTempOnTheGround(bool settempbool)
	{
		m_temp_OnTheGround = settempbool;
	}



	StructPlacedObjectData GetStructPlacedObjectData()
	{
		
		StructPlacedObjectData re;
		std::memcpy(re.sm_name, m_name.c_str(), strlen(m_name.c_str()) + 1);	// string型->char[64]へ変換
		re.sm_Position = m_Position;
		re.sm_Rotation = m_Rotation;
		re.sm_Scale = m_InitScale;
		re.sm_ScaleRate = m_ScaleRate;

		return re;
	}

	void GetObjectNameChar64(char* objectnamearray)
	{
		std::memcpy(objectnamearray, m_name.c_str(), strlen(m_name.c_str()) + 1);	// string型->char[64]へ変換
	}
	std::string GetObjectNameString()
	{
		return m_name;
	}
	void SetObjectName(char* objectnamearray)
	{
		//std::string temp = std::string(objectnamearray);
		m_name = std::string(objectnamearray);						// char[64]->string型へ変換
		//char* tempC[64];
		//strcpy(*tempC, objectnamearray);
		//std::string tempS(*tempC, sizeof(tempC));
		//std::string tempS(objectnamearray, 64);
		//m_name = tempS;
		//m_name(objectnamearray, sizeof(objectnamearray));
	}

	void GetModelName(char* modelnamearray)
	{
		std::memcpy(modelnamearray, m_modelname.c_str(), strlen(m_modelname.c_str()) + 1);	// string型->char[64]へ変換
	}
	void SetModelName(char* modelnamearray)
	{
		//int a = sizeof(modelnamearray);	// これはアドレスなのでサイズは絶対4となってしまう
		std::string setname(modelnamearray, strlen((char*)modelnamearray));
		m_modelname = setname;
	}

	int GetComponentNum()
	{
		int count = 0;
		for (auto Comp : m_ComponentList)
		{
			count++;
		}
		return count;
	}

	void DestroyComponent(int compCount);

	void DestroyComponentList();

	// 既にopenしてあるobjファイルのアドレスをもらう
	void SaveComponent(std::ofstream* Objfile, std::ofstream* ObjfileB);
	// ファイルを閉じるのはSavedata.cppの方でやってる

	void LoadComponent(std::ifstream* Objfile)
	{
		// コンポーネントIDを読み取ってそれをAddComponentする。その後そのコンポーネントの設定を読み込み追加する。
		int compid = 0;
		Objfile->read((char*)&compid, sizeof(compid));	// 所持しているコンポーネント数の読み込み

		//// ここではコンポーネントのIDだけを読み込む
		//for (auto Comp : m_ComponentList)
		//{
		//	int compid = Comp->GetComponentID();
		//	Objfile->write((char*)&compid, sizeof(compid));

		//	// 次にそのコンポーネントの設定を書き込む、これはコンポーネントによって内容も量も違う
		//	Comp->Save(Objfile);

		//}
	}


	void CopyComponentListToAdd(GameObject* from);


	void CopyGameObject(GameObject* from);

	// セーブとロード用
	StructModelState GetStructModelState()
	{
		StructModelState SMS;
		SMS.sm_ModelOffset = m_ModelOffset;
		SMS.sm_ModelRotation = m_ModelRotation;
		SMS.sm_ModelScale = m_ModelScale;

		return SMS;
	}
	// セーブとロード用
	void SetStructModelState(StructModelState sms)
	{
		m_ModelOffset = sms.sm_ModelOffset;
		m_ModelRotation = sms.sm_ModelRotation;
		m_ModelScale = sms.sm_ModelScale;
	}

	// Collision用
	void Collision_AddHitObject(GameObject* hitobject, D3DXVECTOR3 hitpos)
	{
		m_Collision_HitObjectList.push_back(hitobject);
		m_Collision_HitPositionList.push_back(hitpos);
	}
	void Collision_AddOverlapObject(GameObject* overlapobject, D3DXVECTOR3 overlappos)
	{
		m_Collision_OverlapObjectList.push_back(overlapobject);
		m_Collision_OverlapPositionList.push_back(overlappos);
	}


	//SimpleBoundingBox2D* GetSimpleBoundingBox2D() { return m_SimpleBoundingBox2D; }
	//void SetSimpleBoundingBox2DUseDraw(const bool& usedraw) { m_SimpleBoundingBox2DUseDraw = usedraw; }
	//bool GetSimpleBoundingBox2DUseDraw() { return m_SimpleBoundingBox2DUseDraw; }
	SimpleBoundingBox3D* GetSimpleBoundingBox3D() { return m_SimpleBoundingBox3D; }
	void SetSimpleBoundingBox3DUseDraw(const bool& usedraw) { m_SimpleBoundingBox3DUseDraw = usedraw; }
	bool GetSimpleBoundingBox3DUseDraw() { return m_SimpleBoundingBox3DUseDraw; }
	
	void SetSimpleBoundingBox3DRadius(const float& radius) { m_SimpleBoundingBox3DRadius = radius; }
	float GetSimpleBoundingBox3DRadius() { return m_SimpleBoundingBox3DRadius; }
	float* GetpSimpleBoundingBox3DRadius() { return &m_SimpleBoundingBox3DRadius; }

	bool GetLoadSuccess() { return m_LoadSuccess; }
	void SetLoadSuccess(const bool& success) { m_LoadSuccess = success; }

};

// private そのクラスからしかアクセスできない。そのクラスの関数とか。
// prootected private + 継承先のクラスからはアクセスできる。ゲームエンジンだとこれが多いかな～
// public アクセスできる