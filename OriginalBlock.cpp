

#include "main.h"
#include "renderer.h"
#include "model.h"
#include "gameObject.h"
#include "OriginalBlock.h"
#include "ComponentListManager.h"
#include "input.h"

Model* OriginalBlock::m_Model;

void OriginalBlock::Init()
{
	//AddComponent<Rigidbody>(); ��AddGameObject���Ă�����

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	SetInitScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));


	m_TagNum = 0;
	m_name = "OriginalBlock";
	m_modelname = "torus";
	m_DoSave = 1;

	// Component�̐ݒ�
	// <Collision>
	Collision* p_Collision = GetComponent<Collision>();
	//�@�J�v�Z���T�C�Y�̐ݒ�
	//GetComponent<Collision>()->SetCapsule(Point(m_Position.x, m_Position.y, m_Position.z), 100.0f, 100.0f);

	p_Collision->SetMovable(false);
	p_Collision->SetCollisionType(BOX_COLLISION);
	p_Collision->LoadCollisionModel();
	p_Collision->SetBoxScale(D3DXVECTOR3(2.0f, 0.5f, 2.0f));

	//p_Collision->SetMovable(false);
	//p_Collision->SetCollisionType(CAPSULE_COLLISION);
	//p_Collision->LoadCollisionModel();
	//p_Collision->SetCapsuleScale(2.0f, 4.5f);

	//p_Collision->SetMovable(false);
	//p_Collision->SetCollisionType(SPHERE_COLLISION);
	//p_Collision->LoadCollisionModel();
	//p_Collision->SetSphereScale(2.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

}


void OriginalBlock::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void OriginalBlock::Update()
{
	GameObject::Update();




	// �W�����v
	if (Input::GetKeyTrigger(DIK_K))
	{
		m_Velocity.y = 0.25f;
	}

	// �f�o�b�O�p��]
	if (Input::GetKeyTrigger(DIK_O))
	{
		m_Rotation.x += 0.1f;
	}



	GameObject::ComponentUpdate();
	//GameObject::TemporarySetUpdate();
}


void OriginalBlock::Draw()
{
	GameObject::Draw();


	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ���[���h�}�g���N�X�ݒ�
	//D3DXMATRIX world, scale, rot, trans;
	//D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	//D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	//world = scale * rot * trans;
	D3DXMATRIX world = GetWorldMatrix();	// �֐��ɂ܂Ƃ߂�
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}

void OriginalBlock::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");
}

void OriginalBlock::Unload()
{
	m_Model->Unload();
	delete m_Model;
}