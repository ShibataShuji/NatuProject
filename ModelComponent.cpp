
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
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	// 使用するシェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	CComponent::Uninit();
}

void ModelComponent::Update()
{

}

void ModelComponent::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// マトリクス設定
	D3DXMATRIX world = GetWorldMatrixModelComponent();
	Renderer::SetWorldMatrix(&world);

	//// つかう？？
	//{
	//	// 頂点バッファ設定
	//	UINT stride = sizeof(VERTEX_3D);
	//	UINT offset = 0;
	//	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		//// マテリアル設定
		//MATERIAL material;
		//ZeroMemory(&material, sizeof(material));
		//material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//Renderer::SetMaterial(material);

	//	// テクスチャ設定
	//	//Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	//	// テクスチャ設定
	//	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	//}

	//シャドウバッファテクスチャを１番へセット
	ID3D11ShaderResourceView* shadowDepthTexture = Renderer::GetShadowDepthTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);

	// 追加の色の設定をシェーダーに送る
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

	// m_ParentGameObject ポインターだしAddComponentの時に出来てるのでコピーしない

	// 親関数からコピーするもの
	m_CompId = from->GetComponentID();
	m_CompName = from->GetComponentName();

	// モデルコンポーネントでコピーするもの
	m_ModelName = from->GetModelName();
	m_ModelOffset = from->GetModelOffset();
	m_ModelRotation = from->GetModelRotation();
	m_ModelScale = from->GetModelScale();

	m_SynthesizeColor = from->m_SynthesizeColor;
	m_SynthesizeColorUse = from->m_SynthesizeColorUse;
}

void  ModelComponent::Save(std::ofstream* Objfile, std::ofstream* ObjfileB)
{
	// ここではこのコンポーネントの設定を書き込む
	StructModelComponentData smcd;
	smcd = RealtoSMCD();

	Objfile->write((char*)&smcd, sizeof(smcd));
	ObjfileB->write((char*)&smcd, sizeof(smcd));
}

void ModelComponent::Load(std::ifstream* Objfile)
{
	// ここではこのコンポーネントの設定を読み込む
	StructModelComponentData smcd;

	Objfile->read((char*)&smcd, sizeof(smcd));

	SMCDtoReal(smcd);		// 読み込んだものを反映させる
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
	// 子(自分)の行列を作成
	D3DXMATRIX CworldM, CscaleM, CrotM, CtransM;
	D3DXMatrixScaling(&CscaleM, m_ModelScale.x, m_ModelScale.y, m_ModelScale.z);
	D3DXMatrixRotationYawPitchRoll(&CrotM, m_ModelRotation.y, m_ModelRotation.x, m_ModelRotation.z);
	D3DXMatrixTranslation(&CtransM, m_ModelOffset.x, m_ModelOffset.y, m_ModelOffset.z);
	CworldM = CscaleM * CrotM * CtransM;


	// 親の情報を取得
	D3DXVECTOR3 PScale = m_ParentGameObject->GetScaleRate();
	D3DXVECTOR3 PRotation = m_ParentGameObject->GetRotation();
	D3DXVECTOR3 PPosition = m_ParentGameObject->GetPosition();

	// 親の行列を作成
	D3DXMATRIX PworldM, PscaleM, ProtM, PtransM;
	D3DXMatrixScaling(&PscaleM, PScale.x, PScale.y, PScale.z);
	D3DXMatrixRotationYawPitchRoll(&ProtM, PRotation.y, PRotation.x, PRotation.z);
	D3DXMatrixTranslation(&PtransM, PPosition.x, PPosition.y, PPosition.z);
	PworldM = PscaleM * ProtM * PtransM;

	// 行列の掛け算。ワールド以外いらないが、値が欲しい場合必要。
	D3DXMATRIX RworldM, RscaleM, RrotM, RtransM;
	D3DXMatrixMultiply(&RworldM, &CworldM, &PworldM);	// ワールド

	D3DXMatrixMultiply(&RscaleM, &CscaleM, &PscaleM);	// スケール
	D3DXMatrixMultiply(&RrotM, &CrotM, &ProtM);			// 回転
	D3DXMatrixMultiply(&RtransM, &CtransM, &PtransM);	// 座標

	// 行列計算後の
	// スケールを取得。RscaleM
	D3DXVECTOR3 AfterScale = D3DXVECTOR3(RscaleM._11, RscaleM._22, RscaleM._33);

	// 回転を取得。RrotMから取得できるのは最終的な回転のやつなので、VecのRotにはできない。
	// ので普通に足し算する
	D3DXVECTOR3 AfterRotation = m_ModelRotation + PRotation;

	// 座標を取得。RworldM。worldからでもtransからでもどっちでもOK
	D3DXVECTOR3 AfterPosition = D3DXVECTOR3(RworldM._41, RworldM._42, RworldM._43);

	// Afterを使ったモデルの変数の更新等
	// モデルの変数で更新するものはない。

	return RworldM;

	// デバックで中身見やすくするよう
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