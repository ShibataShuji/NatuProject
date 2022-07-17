#pragma once

#include "CComponent.h"
//#include "gameObject.h"

class Rigidbody : public CComponent
{
private:
	float				m_gravity = 1.0f;		// 重力		無重力0.0f~1.0f重力
	float				m_Mass = 1.0f;			// 重さ
	float				m_Friction = 0.5f;		// 摩擦、すべりやすさ	止まらない0.0f~1.0f止まる
	
	// 計算で使うデフォルト値
	float				m_gravity_def = 0.01f;

public:

	// 親クラスのコンストラクタをやってくれる。
	using CComponent::CComponent;

	//Rigidbody(GameObject* gameObject) : CComponent(gameObject){}
	//virtual ~Rigidbody(){}
	

	void Init() override 
	{

	}

	void Uninit() override
	{

	}

	void Update() override
	{
		//// TempPositionを最終的に変更するために現在のものをゲットしておく
		//D3DXVECTOR3 t_Position = m_ParentGameObject->GetTempPosition();
		
		// 計算しやすいようにVelocityをゲットして仮で保存しておく
		D3DXVECTOR3 t_Position;
		D3DXVECTOR3 t_Velocity;


		
		// 重力の計算	まだMassが考慮されてない
		t_Velocity = m_ParentGameObject->GetVelocity();		// ゲット
		t_Velocity.y -= m_gravity_def * m_gravity;
		m_ParentGameObject->SetVelocity(t_Velocity);		// セット
		//t_Velocity.y = m_ParentGameObject->GetVelocity().y - m_gravity_def * m_gravity;

		// 摩擦など抵抗の計算
		// まだ0.1倍とかでやってるけど、FrictionとかMassとか使った計算にしたい。
		t_Velocity = m_ParentGameObject->GetVelocity();		// ゲット
		t_Velocity.x -= t_Velocity.x * 0.1;
		t_Velocity.y -= t_Velocity.y * 0.01;
		t_Velocity.z -= t_Velocity.z * 0.1;
		m_ParentGameObject->SetVelocity(t_Velocity);		// セット
		//m_ParentGameObject->SetVelocity_x(t_Velocity.x - t_Velocity.x * 0.1);
		//m_ParentGameObject->SetVelocity_y(t_Velocity.y - t_Velocity.y * 0.01);
		//m_ParentGameObject->SetVelocity_z(t_Velocity.z - t_Velocity.z * 0.1);




		//// m_TempVelocityの更新
		//m_ParentGameObject->SetTempVelocity(t_Velocity);
		
		// 速度を足す
		t_Position = m_ParentGameObject->GetPosition();		// ゲット
		t_Velocity = m_ParentGameObject->GetVelocity();		// ゲット
		t_Position += t_Velocity;
		m_ParentGameObject->SetPosition(t_Position);		// セット

		//// m_Positionの更新
		//m_ParentGameObject->SetTempPosition(t_Position);
	}

	void Draw() override
	{

	}

	float GetMass()
	{
		return m_Mass;
	}

	float GetFriction()
	{
		return m_Friction;
	}

};

