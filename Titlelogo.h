#pragma once



class Titlelogo : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;				// 頂点バッファを保存する場所
	ID3D11ShaderResourceView* m_Texture = NULL;			// テクスチャを保存する場所

	ID3D11VertexShader* m_VertexShader;					// シェーダーのやつ
	ID3D11PixelShader* m_PixelShader;					// シェーダーのやつ
	ID3D11InputLayout* m_VertexLayout;					// シェーダーのやつ

public:
	// アクセス指定しをパブリックにしないと外部からアクセスできない
	void Init();
	void Uninit();
	void Update();
	void Draw();



};
