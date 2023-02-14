
#include "stdafx.h"

//#include "main.h"
//#include "renderer.h"
//#include "input.h"
////#include "model.h"
//#include "camera.h"
////#include "player.h"
//#include "scene.h"
//#include "manager.h"
//#include "gameObject.h"
//#include "Bullet.h"
//#include "Sky.h"
//#include <d3dx9.h>
//#include <D3D11.h>
//#include <D3DX11.h>
//#include <cassert>
//#include "model.h"

class Gameobject;

void Sky::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\CollisionModel\\Sky01.obj");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(50.0f, 50.0f, 50.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

}


void Sky::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();



}


void Sky::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(0);

	D3DXVECTOR3 cameraPosition = camera->GetPosition();
	m_Position = cameraPosition;
	m_Rotation.y += 0.0003;
}


void Sky::Draw()
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