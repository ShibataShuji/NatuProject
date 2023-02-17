
#include "stdafx.h"


// ファイルから読み込んだものをそのまま使うのではなくこれに保存して使う。



Model* LoadedObject::m_Model;

void LoadedObject::Init()
{


	m_TagNum = 0;
	m_name = "LoadedObject";
	m_modelname = "Torus";
	m_DoSave = 1;


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

}


void LoadedObject::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void LoadedObject::Update()
{

}


void LoadedObject::Draw()
{

}

void LoadedObject::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\Torus.obj");
}

void LoadedObject::Unload()
{
	m_Model->Unload();
	delete m_Model;
}


void LoadedObject::AddComponentWithID(std::ifstream* Objfile, int compID)
{
	if (compID == 1)
	{
		auto p_collision = AddComponent<Collision>();
		p_collision->Load(Objfile);		// 次にそのコンポーネントの設定をよみこむ

	}
	else if (compID == 2)
	{
		auto p_rigidbody = AddComponent<Rigidbody>();
	}
	else if (compID == 3)
	{
		auto p_model = AddComponent<ModelComponent>();
		p_model->Load(Objfile);
	}
}