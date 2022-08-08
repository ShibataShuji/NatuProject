#pragma once

#define OBJNUM (4)

#include "CComponent.h"
#include <string>
#include <list>

class GameObject
{
protected:	// アクセス指定子

	bool		m_Destroy = false;


	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_InitScale;
	D3DXVECTOR3 m_ScaleRate = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 設定された初期値に対しての倍率

	D3DXVECTOR3	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 現在の速度ベクトル

	int			m_TagNum = 0;

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

	/*GameObject*				m_Parent;*/

public:

	GameObject() {}

	virtual ~GameObject()
	{
		for (auto c : m_ComponentList)
			delete c;
		m_ComponentList.clear();

		//for (auto c : m_ComponentNameList)	// これはクラスじゃないのでdeleteする必要ない
		//	delete c;
		m_ComponentNameList.clear();

		for (auto c : m_Child)
			delete c;
		m_Child.clear();
	}

	int Gettagnum()
	{
		return m_TagNum;
	}

	virtual void Init()	// 純粋仮想関数
	{

	}


	virtual void Uninit()
	{
		for (auto c : m_ComponentList)
			c->Uninit();

		for (auto c : m_Child)
			c->Uninit();
	}

	// virtual(純粋仮想関数)なので、これを継承したplayerクラスなどでUpdate関数を作るとそっちが呼ばれて
	// これは呼ばれないが、playerのupdateの方でこれを呼ぶ(親クラスのupdate)と呼べる。
	// これの子クラスであるplayerとかの中で明示的にGameObject::Updateって風に呼ばないといけない。
	// Drawとか忘れがちなので注意
	virtual void Update()
	{
		// 座標の保存
		m_Old_Position = m_Position;
		m_Old_Rotation = m_Rotation;
		m_Old_Scale = m_Scale;
		m_Old_ScaleRate = m_ScaleRate;

		// 接地判定のTempのリセット。前フレームのが残らないようにここでリセットしておく。
		m_temp_OnTheGround = false;


		// playerとか子クラスのUpdateの後に来てほしいので、playerとか子クラスのUpdateの最後に書く。
		/*for (auto c : m_ComponentList)
			c->Update();*/
		/*------->>>>>> ComponentUpdate()関数にまとめた*/


		// 子供の必要性がわからない；；
		/*for (auto c : m_Child)
			c->Update();*/
	}


	virtual void Draw()
	{
		for (auto c : m_ComponentList)
			c->Draw();
	}


	// コンポーネントのアップデート
	virtual void ComponentUpdate()
	{
		for (auto c : m_ComponentList)
			c->Update();
	}

	// Tempの更新
	//virtual void TemporarySetUpdate()
	//{
	//	m_Position = m_temp_Position;
	//	m_Velocity = m_temp_Velocity;
	//}

	void SetPosition(D3DXVECTOR3 Position){	m_Position = Position;	}
	void SetPosition_x(float x){	m_Position.x = x;	}
	void SetPosition_y(float y){	m_Position.y = y;	}
	void SetPosition_z(float z){	m_Position.z = z;	}

	void SetRotation(D3DXVECTOR3 rotation){ m_Rotation = rotation;	}

	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXVECTOR3 GetScaleRate() { return m_ScaleRate; }

	D3DXVECTOR3 GetOldPosition() { return m_Old_Position; }
	D3DXVECTOR3 GetOldRotation() { return m_Old_Rotation; }
	D3DXVECTOR3 GetOldScale() { return m_Old_Scale; }
	D3DXVECTOR3 GetOldScaleRate() { return m_Old_ScaleRate; }
	//void SetOldPosition(D3DXVECTOR3 Position) { m_Old_Position = Position; }

	//D3DXVECTOR3 GetTempPosition() { return m_temp_Position; }
	//void SetTempPosition(D3DXVECTOR3 Position) { m_temp_Position = Position; }

	D3DXVECTOR3 GetVelocity() { return m_Velocity; }
	void SetVelocity(D3DXVECTOR3 Velocity) { m_Velocity = Velocity; }
	void SetVelocity_x(float x) { m_Velocity.x = x; }
	void SetVelocity_y(float y) { m_Velocity.y = y; }
	void SetVelocity_z(float z) { m_Velocity.z = z; }

	//D3DXVECTOR3 GetTempVelocity() { return m_temp_Velocity; }
	//void SetTempVelocity(D3DXVECTOR3 Velocity) { m_temp_Velocity = Velocity; }

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
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
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


};

// private そのクラスからしかアクセスできない。そのクラスの関数とか。
// prootected private + 継承先のクラスからはアクセスできる。ゲームエンジンだとこれが多いかな～
// public アクセスできる