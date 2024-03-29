
#include "stdafx.h"


void ImageDisplayFull::Init()
{
	VERTEX_3D vertex[4];
	// 左上
	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 法線
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// 色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);						// テクスチャ座標(基本0.0f~1.0fだけど10.0fとかにすると10個表示される。ポリゴン数は同じ)

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);				// 右上
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);				// 左下
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);			// 右下
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	// 左奥、右奥、左手前、右手前、Zの字の形で

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	// テクスチャ読み込み
	// "asset/texture/SSE_Title.png"
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), 
		m_TextureName.c_str(),
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	// シェーダー関係、詳しくはGGで
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

}


void ImageDisplayFull::Uninit()
{
	// ダイレクトXはリリースが必要
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();



}


void ImageDisplayFull::Update()
{


}


void ImageDisplayFull::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// マトリクス設定
	// ダイレクトXでオブジェクトを回転したりするにはマトリクス設定が必要
	Renderer::SetWorldViewProjection2D();


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	//頂点バッファに入ってる頂点をどういう風につなげていくかの設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描写
	// 4ていうのは頂点数右のはスタート位置(ほぼほぼ0固定)
	Renderer::GetDeviceContext()->Draw(4, 0);

}