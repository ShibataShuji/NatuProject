
#include "stdafx.h"

//#include "ModelComponent.h"
//#include "renderer.h"
//#include "manager.h"
//#include "model.h"
//#include "Savedata.h"
//#include "gameObject.h"

 void ModelComponent::Init()
{
	m_CompName = "ModelComponent";
	m_CompId = ModelComponentId;

	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	//Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "DepthShadowMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "DepthShadowMappingPS.cso");

}

void ModelComponent::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	// �g�p����V�F�[�_��ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	CComponent::Uninit();
}

void ModelComponent::Update()
{

}

void ModelComponent::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// �}�g���N�X�ݒ�
	D3DXMATRIX world = GetWorldMatrixModelComponent();
	Renderer::SetWorldMatrix(&world);

	//// �����H�H
	//{
	//	// ���_�o�b�t�@�ݒ�
	//	UINT stride = sizeof(VERTEX_3D);
	//	UINT offset = 0;
	//	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		//// �}�e���A���ݒ�
		//MATERIAL material;
		//ZeroMemory(&material, sizeof(material));
		//material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//Renderer::SetMaterial(material);

	//	// �e�N�X�`���ݒ�
	//	//Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	//	// �e�N�X�`���ݒ�
	//	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	//}

	//�V���h�E�o�b�t�@�e�N�X�`�����P�ԂփZ�b�g
	ID3D11ShaderResourceView* shadowDepthTexture = Renderer::GetShadowDepthTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);

	// �ǉ��̐F�̐ݒ���V�F�[�_�[�ɑ���
	D3DXVECTOR4 sentColor = m_SynthesizeColor;
	if(!m_SynthesizeColorUse)
		sentColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	Renderer::SetSynthesizeColorToSharder(sentColor);


	Savedata* save = Manager::GetSavedata();
	Model* model = save->GetLoadedModelWithName(m_ModelName);

	model->Draw();

}

void ModelComponent::CopyThisComponent(CComponent* fromComponent)
{
	ModelComponent* from = dynamic_cast<ModelComponent*>(fromComponent);

	// m_ParentGameObject �|�C���^�[����AddComponent�̎��ɏo���Ă�̂ŃR�s�[���Ȃ�

	// �e�֐�����R�s�[�������
	m_CompId = from->GetComponentID();
	m_CompName = from->GetComponentName();

	// ���f���R���|�[�l���g�ŃR�s�[�������
	m_ModelName = from->GetModelName();
	m_ModelOffset = from->GetModelOffset();
	m_ModelRotation = from->GetModelRotation();
	m_ModelScale = from->GetModelScale();

	m_SynthesizeColor = from->m_SynthesizeColor;
	m_SynthesizeColorUse = from->m_SynthesizeColorUse;
}

void  ModelComponent::Save(std::ofstream* Objfile, std::ofstream* ObjfileB)
{
	// �����ł͂��̃R���|�[�l���g�̐ݒ����������
	StructModelComponentData smcd;
	smcd = RealtoSMCD();

	Objfile->write((char*)&smcd, sizeof(smcd));
	ObjfileB->write((char*)&smcd, sizeof(smcd));
}

void ModelComponent::Load(std::ifstream* Objfile)
{
	// �����ł͂��̃R���|�[�l���g�̐ݒ��ǂݍ���
	StructModelComponentData smcd;

	Objfile->read((char*)&smcd, sizeof(smcd));

	SMCDtoReal(smcd);		// �ǂݍ��񂾂��̂𔽉f������
}

StructModelComponentData ModelComponent::RealtoSMCD()
{
	StructModelComponentData re_smcd;
	StringToChar64(m_ModelName, re_smcd.sm_ModelName);
	re_smcd.sm_ModelOffset = m_ModelOffset;
	re_smcd.sm_ModelRotation = m_ModelRotation;
	re_smcd.sm_ModelScale = m_ModelScale;
	re_smcd.sm_SynthesizeColor = m_SynthesizeColor;
	re_smcd.sm_SynthesizeColorUse = m_SynthesizeColorUse;
	
	return re_smcd;
}

void ModelComponent::SMCDtoReal(StructModelComponentData smcd)
{
	m_ModelName = smcd.sm_ModelName;
	m_ModelOffset = smcd.sm_ModelOffset;
	m_ModelRotation = smcd.sm_ModelRotation;
	m_ModelScale = smcd.sm_ModelScale;
	m_SynthesizeColor = smcd.sm_SynthesizeColor;
	m_SynthesizeColorUse = smcd.sm_SynthesizeColorUse;
}

D3DXMATRIX ModelComponent::GetWorldMatrixModelComponent()
{
	// �q(����)�̍s����쐬
	D3DXMATRIX CworldM, CscaleM, CrotM, CtransM;
	D3DXMatrixScaling(&CscaleM, m_ModelScale.x, m_ModelScale.y, m_ModelScale.z);
	D3DXMatrixRotationYawPitchRoll(&CrotM, m_ModelRotation.y, m_ModelRotation.x, m_ModelRotation.z);
	D3DXMatrixTranslation(&CtransM, m_ModelOffset.x, m_ModelOffset.y, m_ModelOffset.z);
	CworldM = CscaleM * CrotM * CtransM;


	// �e�̏����擾
	D3DXVECTOR3 PScale = m_ParentGameObject->GetScaleRate();
	D3DXVECTOR3 PRotation = m_ParentGameObject->GetRotation();
	D3DXVECTOR3 PPosition = m_ParentGameObject->GetPosition();

	// �e�̍s����쐬
	D3DXMATRIX PworldM, PscaleM, ProtM, PtransM;
	D3DXMatrixScaling(&PscaleM, PScale.x, PScale.y, PScale.z);
	D3DXMatrixRotationYawPitchRoll(&ProtM, PRotation.y, PRotation.x, PRotation.z);
	D3DXMatrixTranslation(&PtransM, PPosition.x, PPosition.y, PPosition.z);
	PworldM = PscaleM * ProtM * PtransM;

	// �s��̊|���Z�B���[���h�ȊO����Ȃ����A�l���~�����ꍇ�K�v�B
	D3DXMATRIX RworldM, RscaleM, RrotM, RtransM;
	D3DXMatrixMultiply(&RworldM, &CworldM, &PworldM);	// ���[���h

	D3DXMatrixMultiply(&RscaleM, &CscaleM, &PscaleM);	// �X�P�[��
	D3DXMatrixMultiply(&RrotM, &CrotM, &ProtM);			// ��]
	D3DXMatrixMultiply(&RtransM, &CtransM, &PtransM);	// ���W

	// �s��v�Z���
	// �X�P�[�����擾�BRscaleM
	D3DXVECTOR3 AfterScale = D3DXVECTOR3(RscaleM._11, RscaleM._22, RscaleM._33);

	// ��]���擾�BRrotM����擾�ł���͍̂ŏI�I�ȉ�]�̂�Ȃ̂ŁAVec��Rot�ɂ͂ł��Ȃ��B
	// �̂ŕ��ʂɑ����Z����
	D3DXVECTOR3 AfterRotation = m_ModelRotation + PRotation;

	// ���W���擾�BRworldM�Bworld����ł�trans����ł��ǂ����ł�OK
	D3DXVECTOR3 AfterPosition = D3DXVECTOR3(RworldM._41, RworldM._42, RworldM._43);

	// After���g�������f���̕ϐ��̍X�V��
	// ���f���̕ϐ��ōX�V������̂͂Ȃ��B

	return RworldM;

	// �f�o�b�N�Œ��g���₷������悤
	//D3DXMATRIX RworldM, RscaleM, RrotM, RtransM;
	//D3DXMATRIX RotMat = RrotM;

	//RotMat._14;
	//RotMat._24;
	//RotMat._34;

	//RotMat._11;
	//RotMat._12;
	//RotMat._13;
	//RotMat._14;

	//RotMat._21;
	//RotMat._22;
	//RotMat._23;
	//RotMat._24;

	//RotMat._31;
	//RotMat._32;
	//RotMat._33;
	//RotMat._34;

	//RotMat._41;
	//RotMat._42;
	//RotMat._43;
	//RotMat._44;
}