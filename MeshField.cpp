
#include "stdafx.h"

//#include <iostream>
//#include <string>
//#include <vector>
//#include <list>
//#include <array>
//#include <random>
//
//#include "gameObject.h"
//#include "main.h"
//#include "MeshField.h"
//#include "renderer.h"
//#include "primitive.h"
//#include <D3D11.h>
//#include <D3DX11.h>
//#include <cassert>


// array型は要素数を省略できない。vector型は省略できる
std::array<std::array<float, FieldSize + 1>, FieldSize + 1> g_fieldHeight;



float aaag_fieldHeight[FieldSize + 1][FieldSize + 1]
{
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 4.0f, 4.0f, 4.0f, 4.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{10.0f, 1.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, },

	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
};


uint64_t get_rand_range(uint64_t min_val, uint64_t max_val)
{
	// 乱数生成器
	static std::mt19937_64 mt64(0);

	// [min_val, max_val] の一様分布整数 (int) の分布生成器
	std::uniform_int_distribution<uint64_t> get_rand_uni_int(min_val, max_val);

	// 乱数を生成
	return get_rand_uni_int(mt64);

}

double get_rand_range_double(double min_val, double max_val)
{

	static std::mt19937_64 aaa(0);
	std::uniform_real_distribution<double> get_rand_uni_real(min_val, max_val);
	return get_rand_uni_real(aaa);

}


void MeshField::Init()
{
	m_UseShadow = true;
	//// メッシュでステージのぼこぼこの設定
	//for (int x = 0; x < FieldSize + 1; x++)
	//{
	//	for (int z = 0; z < FieldSize + 1; z++)
	//	{
	//		double aaa = get_rand_range_double(0.0, 5.0);
	//		g_fieldHeight[x][z] = (float)aaa;
	//	}
	//}

	// 頂点バッファ生成
	{

		for (int x = 0; x <= FieldSize; x++)
		{
			for (int z = 0; z <= FieldSize; z++)
			{
				float y = g_fieldHeight[x][z];

				m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3((x - m_PosOffset.x) * PanelSize, -m_PosOffset.y, (z - m_PosOffset.z) * -PanelSize);
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 下で法線ベクトルを求めてセットしている
				m_Vertex[(x * (FieldSize + 1)) + z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[(x * (FieldSize + 1)) + z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}

		// 法線ベクトル算出Normalを計算で求める
		// x方向の隣の2点をつなぐベクトルと
				// z方向の隣の2点をつなぐベクトルで
				// 外積してあげれば、それらしいベクトルが出るかな～
		for (int x = 1; x <= (FieldSize - 1); x++)
		{
			for (int z = 1; z <= (FieldSize - 1); z++)
			{
				D3DXVECTOR3 vx, vz, vn;
				//vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;	// 終点から始点を引くとその直線の方向ベクトルを求められる
				//vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;	// 
				vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// 終点から始点を引くとその直線の方向ベクトルを求められる
				vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 

				D3DXVec3Cross(&vn, &vz, &vx);// 外積。vxにもvzにも垂直なベクトルが求まる。
				D3DXVec3Normalize(&vn, &vn);// 正規化(長さ1にする)
				//m_Vertex[x][z].Normal = vn;
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
			}
		}

		//頂点バッファ生成
		// 頂点データが用意できたので、ダイレクトエックス用の頂点バッファを作っていく。
		//D3D11_BUFFER_DESC bd{};

		//ZeroMemory(&bd, sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(VERTEX_3D) * (FieldSize + 1) * (FieldSize + 1);
		//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//bd.CPUAccessFlags = 0;
		//
		// 頂点バッファ生成(頂点情報を後で変えられるようにしてある)
		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * (FieldSize + 1) * (FieldSize + 1);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//←
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//←

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	// インデックスバッファ生成
	{
		//unsigned int index[((FieldSize + 2) * 2) * FieldSize - 2]{};

		int i = 0;
		for (int x = 0; x < FieldSize; x++)
		{
			for (int z = 0; z < (FieldSize + 1); z++)
			{
				m_Index[i] = x * (FieldSize + 1) + z;
				i++;

				m_Index[i] = (x + 1) * (FieldSize + 1) + z;
				i++;
			}

			// 縮退用の余分なインデックス用
			if (x == (FieldSize - 1))
				break;

			m_Index[i] = (x + 1) * (FieldSize + 1) + FieldSize;
			i++;

			m_Index[i] = (x + 1) * (FieldSize + 1);
			i++;
		}

		// 頂点バッファと似てるけどちょっと違うから気を付けてね
		//D3D11_BUFFER_DESC bd;
		//ZeroMemory(&bd, sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(unsigned int) * (((FieldSize + 2) * 2) * FieldSize - 2);
		//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//bd.CPUAccessFlags = 0;

		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (((FieldSize + 2) * 2) * FieldSize - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;//←
		bd.CPUAccessFlags = 0;//←

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);

	}

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/field001.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	// シェーダーの読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "DepthShadowMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "DepthShadowMappingPS.cso");

	//// いつものポジションたち
	//m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

}

void MeshField::Uninit() 
{

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void MeshField::Update() 
{


}

void MeshField::Draw() 
{

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画,いつもと違うよDrawIndexed。インデックスを使って表示
	Renderer::GetDeviceContext()->DrawIndexed(((FieldSize + 2) * 2) * FieldSize - 2, 0, 0);
}

// 引き数の座標のxzを使って、ブロックを算出しそのメッシュでの高さを求める。
float MeshField::GetHeight(D3DXVECTOR3 const &Position)
{
	int x, z;

	// ブロック番号算出。ここはこのメッシュフィールドの視点の位置分オフセット追加しないといけない
	x = Position.x / PanelSize + m_PosOffset.x;
	z = Position.z / -PanelSize + m_PosOffset.z;

	// メッシュフィールドがなかった場合-100.0fを返す。実質床なし。
	if (x < 0 || z < 0 || x >= FieldSize || z >= FieldSize)
		return -100.0f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;


	// 頂点データを読む
	
	pos0 = m_Vertex[(x * (FieldSize + 1)) + z].Position;
	pos1 = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position;
	pos2 = m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;
	pos3 = m_Vertex[((x + 1) * (FieldSize + 1)) + (z + 1)].Position;
	// ↑ブロック番号が求まったらその周りの4頂点の座標を入れている。


	D3DXVECTOR3 v12, v1p, c;
	v12 = pos2 - pos1;
	v1p = Position - pos1;

	// 三角形ポリゴンのどっちの三角形にいるかを判定
	// v12=端っこから端っこ。v1p=端っこからプレイヤー
	// 外積はかける順番が大事で、時計回りだと表方向に垂直なベクトルが出る、
	// 逆だと裏方向になってしまう。ので、三角形ポリゴンのどっちかだと表向き逆だと裏むきにベクトルが出る
	D3DXVec3Cross(&c, &v12, &v1p);

	float py = 0.0;
	D3DXVECTOR3 n;
	// 外積の結果が正なら左の三角形ポリゴンの上にいる。
	// 外積を使った表裏判定は3次元の当たり判定でよく使う。
	if (c.y > 0.0f)
	{
		// 左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);	// 今計算が逆で裏むきになっちゃってる
	}
	else
	{
		// 右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	// 高さ取得
	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;


	return py;
}


void MeshField::SetAreaBlock(Int2 areablock)
{
	m_AreaBlock = areablock;

	// メッシュフィールドの1マスの大きさとかによって座標をセットする
	m_PosOffset.x = areablock.x * interval;
	m_PosOffset.z = areablock.y * interval;

	// 頂点バッファ再生成
	{

		for (int x = 0; x <= FieldSize; x++)
		{
			for (int z = 0; z <= FieldSize; z++)
			{
				float y = g_fieldHeight[x][z];

				m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3((x - m_PosOffset.x) * PanelSize, -m_PosOffset.y, (z - m_PosOffset.z) * -PanelSize);
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 下で法線ベクトルを求めてセットしている
				m_Vertex[(x * (FieldSize + 1)) + z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[(x * (FieldSize + 1)) + z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}


		for (int x = 1; x <= (FieldSize - 1); x++)
		{
			for (int z = 1; z <= (FieldSize - 1); z++)
			{
				D3DXVECTOR3 vx, vz, vn;
				//vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;	// 終点から始点を引くとその直線の方向ベクトルを求められる
				//vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;	// 
				vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// 終点から始点を引くとその直線の方向ベクトルを求められる
				vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 

				D3DXVec3Cross(&vn, &vz, &vx);// 外積。vxにもvzにも垂直なベクトルが求まる。
				D3DXVec3Normalize(&vn, &vn);// 正規化(長さ1にする)
				//m_Vertex[x][z].Normal = vn;
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
			}
		}


		// 頂点データ書き換え。書き換えのための関数->Map関数を使う。
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			// 全てをm_Vertexで値を入れるだけでOK
			memcpy(vertex, m_Vertex, sizeof(m_Vertex));

			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
		}
	}
	return;
}



///////////////////////////// ブラシセッティング////////////////////////////////////////


// 一般的なブラシ。球の中の頂点のy座標を変化させる
void MeshField::UseSculptBrush001(D3DXVECTOR3 const &Position, float const &radius, float const &value)
{
	// まずyは考えずにxzでブロックで分けて考える。中心のブロックだけを求める。

	int x, z;

	// ブロック番号算出。ここはこのメッシュフィールドの視点の位置分オフセット追加しないといけない
	x = Position.x / PanelSize + m_PosOffset.x;
	z = Position.z / -PanelSize + m_PosOffset.z;

	// もしメッシュフィールドの外だったらここで終了
	if (x < 0 || z < 0 || x >= FieldSize || z >= FieldSize)
		return;

	// ここまでで中心のブロックが求まっている。

	// 四角で考えて、そのあとにそのブロックの頂点が球の範囲内かを考える。
	int panelradius, minx, minz, maxx, maxz;
	panelradius = radius / PanelSize;		// 直線で考えた時に半径内に何枚のパネルがあるか
	minx = x - panelradius;
	minz = z - panelradius;
	maxx = x + panelradius;
	maxz = z + panelradius;
	{
		if (minx < 0)
			minx = 0;
		if (minz < 0)
			minz = 0;
		if (maxx > FieldSize)
			maxx = FieldSize;
		if (maxz > FieldSize)
			maxz = FieldSize;
	}

	// 変更する頂点たちを配列で作っちゃう
	std::list<int> verticesNum;

	// 四角の範囲内のブロックの周りの4頂点を追加していく
	for (int xx = minx; xx <= maxx; xx++ )
	{
		for (int zz = minz; zz <= maxz; zz++ )
		{
			// 周りの４頂点を追加
			verticesNum.push_back((xx * (FieldSize + 1)) + zz);
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + zz);
			verticesNum.push_back((xx * (FieldSize + 1)) + (zz + 1));
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + (zz + 1));
		}
	}

	//// 周りの４頂点を追加
	//verticesNum.push_back((x * (FieldSize + 1)) + z);
	//verticesNum.push_back(((x + 1) * (FieldSize + 1)) + z);
	//verticesNum.push_back((x * (FieldSize + 1)) + (z + 1));
	//verticesNum.push_back(((x + 1) * (FieldSize + 1)) + (z + 1));

	// 頂点の重複を排除
	verticesNum.unique();

	// 頂点と中心の距離で球の中かを計算する
	std::list<int> NewverticesNum;
	for (auto vertexNum : verticesNum)
	{
		// 半径よりデカかったら除外。なんだけどそのまま半径使うとがびがびになるから若干判定ゆるくする
		D3DXVECTOR3 distance = Position - m_Vertex[vertexNum].Position;
		float length = D3DXVec3LengthSq(&distance);
		if (length > pow((radius * 1.3f), 2))
			continue;

		NewverticesNum.push_back(vertexNum);
	}



	//頂点データ書き換え。書き換えのための関数->Map関数を使う。
	// Map関数を使うと、元のデータは全部消えるので、全て作り直し、新たに値を設定する必要がある。
	// 全部消えちゃうので、そのためのm_Vertexを用意してある。
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// まず全てをm_Vertexで値を入れる。このためのm_Vertex。
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{
		// 頂点の位置を変更。m_Vertexの値も同時に更新する
		m_Vertex[vertexNum].Position.y += value;
		vertex[vertexNum].Position.y += value;
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}


// 一般的なブラシ。球の中の頂点のy座標を変化させる
void MeshField::UseSculptBrush002(D3DXVECTOR3 const& TriggerPosition, D3DXVECTOR3 const& Position, float const& radius, float const& value)
{
	// まずyは考えずにxzでブロックで分けて考える。中心のブロックだけを求める。

	int x, z;

	// ブロック番号算出。ここはこのメッシュフィールドの視点の位置分オフセット追加しないといけない
	x = Position.x / PanelSize + m_PosOffset.x;
	z = Position.z / -PanelSize + m_PosOffset.z;

	// もしメッシュフィールドの外だったらここで終了
	if (x < 0 || z < 0 || x >= FieldSize || z >= FieldSize)
		return;

	// ここまでで中心のブロックが求まっている。

	// 四角で考えて、そのあとにそのブロックの頂点が球の範囲内かを考える。
	int panelradius, minx, minz, maxx, maxz;
	panelradius = radius / PanelSize;		// 直線で考えた時に半径内に何枚のパネルがあるか
	minx = x - panelradius;
	minz = z - panelradius;
	maxx = x + panelradius;
	maxz = z + panelradius;
	{
		if (minx < 0)
			minx = 0;
		if (minz < 0)
			minz = 0;
		if (maxx > FieldSize)
			maxx = FieldSize;
		if (maxz > FieldSize)
			maxz = FieldSize;
	}

	// 変更する頂点たちを配列で作っちゃう
	std::list<int> verticesNum;

	// 四角の範囲内のブロックの周りの4頂点を追加していく
	for (int xx = minx; xx <= maxx; xx++)
	{
		for (int zz = minz; zz <= maxz; zz++)
		{
			// 周りの４頂点を追加
			verticesNum.push_back((xx * (FieldSize + 1)) + zz);
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + zz);
			verticesNum.push_back((xx * (FieldSize + 1)) + (zz + 1));
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + (zz + 1));
		}
	}

	// 頂点の重複を排除。unique関数はソートされていないと使えない。
	verticesNum.sort();
	verticesNum.unique();

	// 頂点と中心の距離で球の中かを計算する
	std::list<int> NewverticesNum;
	for (auto vertexNum : verticesNum)
	{
		if (m_Vertex[vertexNum].Position.y > 0.1f)
		{
			//if (m_Vertex[vertexNum].Position.y < 0.8f)
			//	int gg = 0;
			continue;
		}

		//// まずyが離れすぎていたら除外。高さの0.2倍以上だったらダメ。value * 0.2f
		//if (fabsf(TriggerPosition.y - m_Vertex[vertexNum].Position.y) > value * 0.2f)
		//{
		//	//if (TriggerPosition.y - m_Vertex[vertexNum].Position.y < -value * 0.1f)
		//	//{
		//	//	continue;
		//	//}
		//	continue;
		//}

		NewverticesNum.push_back(vertexNum);
	}



	//頂点データ書き換え。書き換えのための関数->Map関数を使う。
	// Map関数を使うと、元のデータは全部消えるので、全て作り直し、新たに値を設定する必要がある。
	// 全部消えちゃうので、そのためのm_Vertexを用意してある。
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// まず全てをm_Vertexで値を入れる。このためのm_Vertex。
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{
		if (m_Vertex[vertexNum].Position.y >= 0.2f)
			int gtg = 0;//チェック用


		// 頂点の位置を変更。m_Vertexの値も同時に更新する
		m_Vertex[vertexNum].Position.y += value;
		vertex[vertexNum].Position.y += value;
	}

	// 頂点の位置を変更。m_Vertexの値も同時に更新する
	//m_Vertex[m_Index[0]].Position.y = value;
	//vertex[m_Index[0]].Position.y = value;
	//m_Vertex[m_Index[2052]].Position.y = value;
	//vertex[m_Index[2052]].Position.y = value;
	//m_Vertex[1025].Position.y = value;
	//vertex[1025].Position.y = value;
	//for (int num = 0; num < 4; num++)
	//{
	//	m_Vertex[num].Position.y = value;
	//	vertex[num].Position.y = value;
	//}

	int ff = 0;
	for (auto ind : m_Index)
	{
		if (ind == 1)
			int rhgrg = 3;
		ff++;
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}