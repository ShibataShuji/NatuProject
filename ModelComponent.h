#pragma once


////#include "gameObject.h"


// objファイルのモデルを1つもつコンポーネント

struct StructModelComponentData
{
	char		sm_ModelName[64] = "Torus";

	D3DXVECTOR3 sm_ModelOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 sm_ModelRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 sm_ModelScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// 追加する色
	D3DXVECTOR4				sm_SynthesizeColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	bool					sm_SynthesizeColorUse = false;

};

class ModelComponent : public CComponent
{
private:

	std::string		m_ModelName = "Torus";

	D3DXVECTOR3 m_ModelOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ModelRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ModelScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// 上記つと親を考慮した最終的な表示場所
	D3DXVECTOR3 m_DisplayPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャーに追加する色の設定
	D3DXVECTOR4				m_SynthesizeColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	bool					m_SynthesizeColorUse = false;


	//static class Model*			m_Model;


	// シェーダー関連
	ID3D11VertexShader* m_VertexShader;		//頂点シェーダーオブジェクト
	ID3D11PixelShader* m_PixelShader;		//ピクセルシェーダーオブジェクト
	ID3D11InputLayout* m_VertexLayout;		//頂点レイアウトオブジェクト

	// bool m_UseShadow = false; これはゲームオブジェクトの方で管理してる

public:

	// 親クラスのコンストラクタをやってくれる。
	using CComponent::CComponent;



	void Init() override;

	void Uninit() override;

	void Update() override;

	void Draw() override;

	void CopyThisComponent(CComponent* fromComponent)	override;

	void Save(std::ofstream* Objfile, std::ofstream* ObjfileB) override;

	void Load(std::ifstream* Objfile) override;


	D3DXMATRIX GetWorldMatrixModelComponent();

	std::string GetModelName() { return m_ModelName; }
	void SetModelName(std::string modelname) { m_ModelName = modelname; }

	D3DXVECTOR3 GetModelOffset() { return m_ModelOffset; }
	void SetModelOffset(D3DXVECTOR3 modeloffset) { m_ModelOffset = modeloffset; }
	D3DXVECTOR3 GetModelRotation() { return m_ModelRotation; }
	void SetModelRotation(D3DXVECTOR3 modelrotation) { m_ModelRotation = modelrotation; }
	D3DXVECTOR3 GetModelScale() { return m_ModelScale; }
	void SetModelScale(D3DXVECTOR3 modelscale) { m_ModelScale = modelscale; }

	StructModelComponentData RealtoSMCD();
	void SMCDtoReal(StructModelComponentData smcd);

	// 追加する色
	void SetSynthesizeColorUse(const bool& use) { m_SynthesizeColorUse = use; }
	bool GetSynthesizeColorUse() { return m_SynthesizeColorUse; }

	void SetSynthesizeColor(const D3DXVECTOR4& color) { m_SynthesizeColor = color; }
	D3DXVECTOR4 GetSynthesizeColor() { return m_SynthesizeColor; }
	D3DXVECTOR4* GetpSynthesizeColor() { return &m_SynthesizeColor; }

};


