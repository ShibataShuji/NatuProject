#pragma once

class ImageDisplayFull : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;				// 頂点バッファを保存する場所
	ID3D11ShaderResourceView* m_Texture = NULL;			// テクスチャを保存する場所

	ID3D11VertexShader* m_VertexShader;					// シェーダーのやつ
	ID3D11PixelShader* m_PixelShader;					// シェーダーのやつ
	ID3D11InputLayout* m_VertexLayout;					// シェーダーのやつ

	// "asset/texture/SSE_Title.png"のように全て入れる。
	std::string		m_TextureName;	


public:

	ImageDisplayFull() = delete;
	ImageDisplayFull(std::string texname)
	{
		m_TextureName = texname;
	}

	// アクセス指定しをパブリックにしないと外部からアクセスできない
	void Init();
	void Uninit();
	void Update();
	void Draw();



};
