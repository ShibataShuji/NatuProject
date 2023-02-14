
#include "stdafx.h"

//#include "main.h"
//#include "renderer.h"
//#include "Bullet.h"
//#include "scene.h"
//#include "manager.h"
//#include "gameObject.h"
//#include "Explosion.h"
//#include "Enemy.h"
//#include <d3dx9.h>
//#include <D3D11.h>

Model* Bullet::m_Model;	// スタティックメンバ変数
						// バレットクラスでstatic class で作ってるので。
						// staticの宣言はcpp側でもう一回宣言しないといけない
						// これ忘れるとリンカエラー出ちゃう
						// static系でエラー出てたらだいたいこれ

void Bullet::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}


void Bullet::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void Bullet::Update()
{
	// モデルを移動したりするときはここに書いたりする
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	if (m_Position.z > 100.0f)
	{
		SetDestroy();
		return;
	}


	Scene * scene = Manager::GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(1);

	for (Enemy* enemy : enemyList)
	{
		D3DXVECTOR3 enemyPosition = enemy->GetPosition();

		D3DXVECTOR3 direction = m_Position - enemyPosition;
		float length = D3DXVec3Length(&direction);

		if (length < 2.0f)
		{
			scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);

			enemy->SetDestroy();
			SetDestroy();
			return;
		}
	}
}


void Bullet::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ワールドマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}


void Bullet::Load()
{
	// Init でモデルのロードをすると、バレットが作られるたびにモデルの読み込みが起きてしまう
	// のでべつにわけてつくる

	// staticメソッド(staticのメンバ関数)の中では
	// そこのcppとかの中で書いているものしか使えない。
	// staticにすると普通のグローバル変数と、普通の関数って感じの扱いと同じだから。

	m_Model = new Model();
	m_Model->Load("asset\\model\\Torus.obj");
}

void Bullet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}