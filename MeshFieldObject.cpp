#include "stdafx.h"
//#include "MeshFieldObject.h"


// array型は要素数を省略できない。vector型は省略できる
//std::array<std::array<float, FieldSize + 1>, FieldSize + 1> g_fieldHeight;


void MeshFieldObject::Init(Int2 areablock)
{
	m_AreaBlock = areablock;

	// メッシュフィールドの1マスの大きさとかによって座標をセットする
	m_PosOffset.x = areablock.x * interval;
	m_PosOffset.z = (areablock.y + 1) * interval;

	// m_Position 変えちゃうとなぜかずれる。
	//m_Position = m_PosOffset;


	// 頂点バッファ生成
	{

		for (int x = 0; x <= FieldSize; x++)
		{
			for (int z = 0; z <= FieldSize; z++)
			{
				//m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3((x - m_PosOffset.x) * PanelSize, -m_PosOffset.y, (z - m_PosOffset.z) * -PanelSize);
				
				m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3(x * PanelSize, 0.0f, z * -PanelSize);
				m_Vertex[(x * (FieldSize + 1)) + z].Position += m_PosOffset;
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 下で法線ベクトルを求めてセットしている
				m_Vertex[(x * (FieldSize + 1)) + z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[(x * (FieldSize + 1)) + z].TexCoord = D3DXVECTOR2(x * 0.05f, z * 0.05f);
				// 4つテクスチャセットできるけど最初は空っぽにしておく。全部消しゴム

				// フロア変えた
				m_Vertex[(x * (FieldSize + 1)) + z].FloorTexture0 = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
				m_Vertex[(x * (FieldSize + 1)) + z].FloorAlfa0 = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
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
				vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// 終点から始点を引くとその直線の方向ベクトルを求められる
				vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 

				//D3DXVec3Cross(&vn, &vx, &vz);// 外積。vxにもvzにも垂直なベクトルが求まる。かける順番で向きが真逆に変わる。
				D3DXVec3Cross(&vn, &vz, &vx);// 外積。vxにもvzにも垂直なベクトルが求まる。かける順番で向きが真逆に変わる。
				D3DXVec3Normalize(&vn, &vn);// 正規化(長さ1にする)
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
			}
		}

		//頂点バッファ生成
		// 頂点データが用意できたので、ダイレクトエックス用の頂点バッファを作っていく。
		//D3D11_BUFFER_DESC bd{};

		//ZeroMemory(&bd, sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(VERTEX_3D_FLOOR) * (FieldSize + 1) * (FieldSize + 1);
		//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//bd.CPUAccessFlags = 0;
		//
		// 頂点バッファ生成(頂点情報を後で変えられるようにしてある)
		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D_FLOOR) * (FieldSize + 1) * (FieldSize + 1);
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
	//std::string sTexname = "asset/texture/" + m_TextureName;
	std::string sTexname = "asset/texture/Grass004_1K_Color.jpg";		// park_dirt_diff_1k , forrest_ground_01_diff_1k
	char cTexname[64];
	StringToChar64(sTexname, cTexname);
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		cTexname,
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	// ノーマルマップのテクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Grass004_1K_NormalDX.jpg",	// park_dirt_nor_gl_1k.jpg , normalmap.png
		NULL,
		NULL,
		&m_TextureNormal,
		NULL);
	assert(m_TextureNormal);

	// 崖用テクスチャ読み込み,		// red.png , field001.jpg
	{
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/Rock020_2K_Color.jpg", NULL, NULL, &m_TextureOver, NULL);
		assert(m_TextureOver);
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/Rock020_2K_NormalDX.jpg", NULL, NULL, &m_TextureOverNormal, NULL);//Grass004_1K_NormalDX,Rock020_4K_NormalDX
		assert(m_TextureOverNormal);
	}

	// 床用テクスチャ読み込み
	{
		// Ground001
		//D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/park_dirt_diff_1k.jpg", NULL, NULL, &m_TextureFloor[static_cast<int>(E_UsePaintBrush::Ground001) - 1], NULL);
		//assert(m_TextureFloor[static_cast<int>(E_UsePaintBrush::Ground001) - 1]);
		//D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/park_dirt_nor_dx_1k.jpg", NULL, NULL, &m_TextureFloorNormal[static_cast<int>(E_UsePaintBrush::Ground001) - 1], NULL);
		//assert(m_TextureFloorNormal[static_cast<int>(E_UsePaintBrush::Ground001) - 1]);
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/park_dirt_diff_1k.jpg", NULL, NULL, &m_TextureFloor1, NULL);
		assert(m_TextureFloor1);
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/park_dirt_nor_dx_1k.jpg", NULL, NULL, &m_TextureFloorNormal1, NULL);
		assert(m_TextureFloorNormal1);
	}
	{
		// Grass001
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/forrest_ground_01_diff_1k.jpg", NULL, NULL, &m_TextureFloor2, NULL);
		assert(m_TextureFloor2);
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/forrest_ground_01_nor_dx_1k.jpg", NULL, NULL, &m_TextureFloorNormal2, NULL);
		assert(m_TextureFloorNormal2);
	}
	{
		// Grass001
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/forrest_ground_01_diff_1k.jpg", NULL, NULL, &m_TextureFloor3, NULL);
		assert(m_TextureFloor3);
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/forrest_ground_01_nor_dx_1k.jpg", NULL, NULL, &m_TextureFloorNormal3, NULL);
		assert(m_TextureFloorNormal3);
	}
	{
		// Grass001
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/forrest_ground_01_diff_1k.jpg", NULL, NULL, &m_TextureFloor4, NULL);
		assert(m_TextureFloor4);
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/forrest_ground_01_nor_dx_1k.jpg", NULL, NULL, &m_TextureFloorNormal4, NULL);
		assert(m_TextureFloorNormal4);
	}


	// シェーダーの読み込み
	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "DepthShadowNormalVS.cso");
	//Renderer::CreatePixelShader(&m_PixelShader, "DepthShadowNormalPS.cso");


	Renderer::CreateVertexShader_Floor(&m_VertexShader, &m_VertexLayout, "DepthShadowNormalVS.cso");
	if (m_VertexShader == nullptr)
		int gtgtgt = 1;
	if (m_VertexLayout == nullptr)
		int gtgtgt = 1;
	Renderer::CreatePixelShader(&m_PixelShader, "DepthShadowNormalPS.cso");


	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	//if (m_VertexShader == nullptr)
	//	int gtgtgt = 1;
	//if (m_VertexLayout == nullptr)
	//	int gtgtgt = 1;
	//Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");


	OverWriteVertexBuffer();

}

void MeshFieldObject::Uninit()
{

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_Texture->Release();
	m_TextureNormal->Release();
	m_TextureOver->Release();
	m_TextureOverNormal->Release();

	m_TextureFloor1->Release();
	m_TextureFloorNormal1->Release();
	m_TextureFloor2->Release();
	m_TextureFloorNormal2->Release();

	m_TextureFloor3->Release();
	m_TextureFloorNormal3->Release();
	m_TextureFloor4->Release();
	m_TextureFloorNormal4->Release();


	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void MeshFieldObject::Update()
{


}

void MeshFieldObject::Draw()
{
	if (this == nullptr)
		return;

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&rot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D_FLOOR);
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
	// ノーマルマップ用テクスチャ設定,0番は↑で使っているから2番に設定
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &m_TextureNormal);
	Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, &m_TextureOver);
	Renderer::GetDeviceContext()->PSSetShaderResources(4, 1, &m_TextureOverNormal);
	// 床用テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(5, 1, &m_TextureFloor1);
	Renderer::GetDeviceContext()->PSSetShaderResources(6, 1, &m_TextureFloorNormal1);
	Renderer::GetDeviceContext()->PSSetShaderResources(7, 1, &m_TextureFloor2);
	Renderer::GetDeviceContext()->PSSetShaderResources(8, 1, &m_TextureFloorNormal2);
	// 仮でセットしている↓
	Renderer::GetDeviceContext()->PSSetShaderResources(9, 1, &m_TextureFloor3);
	Renderer::GetDeviceContext()->PSSetShaderResources(10, 1, &m_TextureFloorNormal3);
	Renderer::GetDeviceContext()->PSSetShaderResources(11, 1, &m_TextureFloor4);
	Renderer::GetDeviceContext()->PSSetShaderResources(12, 1, &m_TextureFloorNormal4);



	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画,いつもと違うよDrawIndexed。インデックスを使って表示
	Renderer::GetDeviceContext()->DrawIndexed(((FieldSize + 2) * 2) * FieldSize - 2, 0, 0);
}

void MeshFieldObject::OverWriteSPMOD(StructPlacedMeshFieldObjectData& data)
{
	m_AreaBlock = data.sm_AreaBlock;
	m_Position = data.sm_Position;
	m_PosOffset = data.sm_PosOffset;
	m_TextureName = data.sm_TexName;
	m_UseShadow = data.sm_UseShadow;
	memcpy(&m_Vertex, &data.sm_Vertex, sizeof(data.sm_Vertex));	// memcpyでコピーする。

	// 現在のm_Vertexでバーテックスバッファを書き換える。
	OverWriteVertexBuffer();
}



// 現在のデータを、セーブ用構造体に変換して返す。
StructPlacedMeshFieldObjectData MeshFieldObject::GetStructPlacedMeshFieldObjectData()
{

	//StructPlacedMeshFieldObjectData re;
	//memcpy(re.sm_TexName, m_TextureName.c_str(), strlen(m_TextureName.c_str()) + 1);	// string型->char[64]へ変換
	//re.sm_Position = m_Position;
	//re.sm_AreaBlock = m_AreaBlock;
	//re.sm_PosOffset = m_PosOffset;
	//re.sm_UseShadow = m_UseShadow;
	//memcpy(&re.sm_Vertex, &m_Vertex, sizeof(m_Vertex));	// memcpyでコピーする。

	StructPlacedMeshFieldObjectData* re = new StructPlacedMeshFieldObjectData;
	memcpy(re->sm_TexName, m_TextureName.c_str(), strlen(m_TextureName.c_str()) + 1);	// string型->char[64]へ変換
	re->sm_Position = m_Position;
	re->sm_AreaBlock = m_AreaBlock;
	re->sm_PosOffset = m_PosOffset;
	re->sm_UseShadow = m_UseShadow;
	memcpy(re->sm_Vertex, &m_Vertex, sizeof(m_Vertex));	// memcpyでコピーする。

	return *re;
}



void MeshFieldObject::OverWriteVertexBuffer()
{


	// 頂点データ書き換え。書き換えのための関数->Map関数を使う。
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// 全てをm_Vertexで値を入れるだけでOK
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	return;
}





// パネルのブロック番号算出。trueでそのフィールドの範囲内
bool MeshFieldObject::GetPanelBlock(Int2* pOutInt2, const D3DXVECTOR3& Position)
{
	
	pOutInt2->x = (Position.x - m_PosOffset.x) / PanelSize;
	pOutInt2->y = (Position.z - m_PosOffset.z) / -PanelSize;

	// メッシュフィールドがなかった場合-100.0fを返す。実質床なし。
	if (pOutInt2->x < 0 || pOutInt2->y < 0 || pOutInt2->x >= FieldSize || pOutInt2->y >= FieldSize)
		return false;

	return true;
}


// 引き数の座標のxzを使って、ブロックを算出しそのメッシュでの高さを求める。
float MeshFieldObject::GetHeight(const D3DXVECTOR3& Position)
{
	Int2 panel;
	if (!GetPanelBlock(&panel, Position))
		return -100.0f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;


	// 頂点データを読む

	pos0 = m_Vertex[(panel.x * (FieldSize + 1)) + panel.y].Position;
	pos1 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + panel.y].Position;
	pos2 = m_Vertex[(panel.x * (FieldSize + 1)) + (panel.y + 1)].Position;
	pos3 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + (panel.y + 1)].Position;
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


// そのブロックのメッシュの高さと法線を返す。そもそもブロックがメッシュの範囲外だったらfalseを返す。
bool MeshFieldObject::GetHeightandNormal(float* pOut_height, D3DXVECTOR3* pOut_normal, const D3DXVECTOR3& Position)
{
	Int2 panel;
	if (!GetPanelBlock(&panel, Position))
	{
		*pOut_height = -100.0f;
		*pOut_normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		return false;
	}

	D3DXVECTOR3 pos0, pos1, pos2, pos3;


	// 頂点データを読む

	pos0 = m_Vertex[(panel.x * (FieldSize + 1)) + panel.y].Position;
	pos1 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + panel.y].Position;
	pos2 = m_Vertex[(panel.x * (FieldSize + 1)) + (panel.y + 1)].Position;
	pos3 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + (panel.y + 1)].Position;
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
	D3DXVECTOR3 normal;
	// 外積の結果が正なら左の三角形ポリゴンの上にいる。
	// 外積を使った表裏判定は3次元の当たり判定でよく使う。
	if (c.y > 0.0f)
	{
		// 左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&normal, &v10, &v12);	// 今計算が逆で裏むきになっちゃってる
	}
	else
	{
		// 右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&normal, &v12, &v13);
	}

	// 高さ取得
	py = -((Position.x - pos1.x) * normal.x + (Position.z - pos1.z) * normal.z) / normal.y + pos1.y;

	*pOut_height = py;
	*pOut_normal = normal;
	D3DXVec3Normalize(pOut_normal, pOut_normal);
	*pOut_normal *= -1;

	return true;
}



// ポジションのとこが衝突していたらtrueを返す。Positionは、足の位置でやる
int8_t MeshFieldObject::CheckCollisionMeshFieldObject(float* pOut_height, D3DXVECTOR3* pOut_normal, D3DXVECTOR3* pOut_beforpos, const D3DXVECTOR3& Position)
{
	Int2 panel;
	if (!GetPanelBlock(&panel, Position))
	{
		*pOut_height = -100.0f;
		*pOut_normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		return 0;
	}

	D3DXVECTOR3 pos0, pos1, pos2, pos3;


	// 頂点データを読む

	pos0 = m_Vertex[(panel.x * (FieldSize + 1)) + panel.y].Position;
	pos1 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + panel.y].Position;
	pos2 = m_Vertex[(panel.x * (FieldSize + 1)) + (panel.y + 1)].Position;
	pos3 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + (panel.y + 1)].Position;
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
	D3DXVECTOR3 normal;
	// 外積の結果が正なら左の三角形ポリゴンの上にいる。
	// 外積を使った表裏判定は3次元の当たり判定でよく使う。
	if (c.y > 0.0f)
	{
		// 左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&normal, &v10, &v12);	// 今計算が逆で裏むきになっちゃってる
	}
	else
	{
		// 右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&normal, &v12, &v13);
	}

	// 高さ取得
	py = -((Position.x - pos1.x) * normal.x + (Position.z - pos1.z) * normal.z) / normal.y + pos1.y;

	*pOut_height = py;
	*pOut_normal = normal;
	D3DXVec3Normalize(pOut_normal, pOut_normal);
	*pOut_normal *= -1;



	if (pOut_normal->y < 0.55 - 0.10) 	// 崖法線
	{
		// 斜めの場所だったら
		if (Position.y < py)
		{
			// 地面より下だったら
			// まず衝突直前の座標に戻す
			*pOut_beforpos = Position;

			D3DXVECTOR3 center;
			center = (pos0 + pos1 + pos2 + pos3) * 0.25f;


			return 2;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		// 普通に歩ける場所だったら
		if (Position.y < py)
		{
			// 地面より下だったら
			return 1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

// 現在使っていない。中身昔のままだから間違ってる多分
//void MeshFieldObject::SetAreaBlock(Int2 areablock)
//{
//	m_AreaBlock = areablock;
//
//	// メッシュフィールドの1マスの大きさとかによって座標をセットする
//	m_PosOffset.x = areablock.x * interval;
//	m_PosOffset.z = areablock.y * interval;
//
//	// 頂点バッファ再生成
//	{
//
//		for (int x = 0; x <= FieldSize; x++)
//		{
//			for (int z = 0; z <= FieldSize; z++)
//			{
//				m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3((x - m_PosOffset.x) * PanelSize, -m_PosOffset.y, (z - m_PosOffset.z) * -PanelSize);
//				m_Vertex[(x * (FieldSize + 1)) + z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 下で法線ベクトルを求めてセットしている
//				m_Vertex[(x * (FieldSize + 1)) + z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//				m_Vertex[(x * (FieldSize + 1)) + z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
//			}
//		}
//
//
//		for (int x = 1; x <= (FieldSize - 1); x++)
//		{
//			for (int z = 1; z <= (FieldSize - 1); z++)
//			{
//				D3DXVECTOR3 vx, vz, vn;
//				//vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;	// 終点から始点を引くとその直線の方向ベクトルを求められる
//				//vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;	// 
//				vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// 終点から始点を引くとその直線の方向ベクトルを求められる
//				vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 
//
//				D3DXVec3Cross(&vn, &vz, &vx);// 外積。vxにもvzにも垂直なベクトルが求まる。
//				D3DXVec3Normalize(&vn, &vn);// 正規化(長さ1にする)
//				//m_Vertex[x][z].Normal = vn;
//				m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
//			}
//		}
//
//
//		// 頂点データ書き換え。書き換えのための関数->Map関数を使う。
//		OverWriteVertexBuffer();
//	}
//	return;
//}



// 法線を現在の頂点座標を使って更新する
void MeshFieldObject::UpdataMeshFieldObjectNormal()
{

	//頂点データ書き換え。書き換えのための関数->Map関数を使う。
	// Map関数を使うと、元のデータは全部消えるので、全て作り直し、新たに値を設定する必要がある。
	// 全部消えちゃうので、そのためのm_Vertexを用意してある。
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// 法線ベクトル算出Normalを計算で求める
	// x方向の隣の2点をつなぐベクトルと
	// z方向の隣の2点をつなぐベクトルで
	// 外積してあげれば、それらしいベクトルが出るかな～
	for (int x = 1; x <= (FieldSize - 1); x++)
	{
		for (int z = 1; z <= (FieldSize - 1); z++)
		{
			D3DXVECTOR3 vx, vz, vn;
			vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// 終点から始点を引くとその直線の方向ベクトルを求められる
			vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 

			//D3DXVec3Cross(&vn, &vx, &vz);// 外積。vxにもvzにも垂直なベクトルが求まる。かける順番で向きが真逆に変わる。
			D3DXVec3Cross(&vn, &vz, &vx);// 外積。vxにもvzにも垂直なベクトルが求まる。かける順番で向きが真逆に変わる。
			D3DXVec3Normalize(&vn, &vn);// 正規化(長さ1にする)
			m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
		}
	}

	// 法線計算後に全てをm_Vertexで置き換える
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}








///////////////////////////// ブラシセッティング////////////////////////////////////////
///////////////////////////// スカルプトブラシ - ブラシセッティング//////////////////////////////////////////
void MeshFieldObject::UseSculptBrushes(const E_UseSculptBrush& usesculptbrush, const PassedSculptBrushState* ppassedsculptbrushstate)
{
	switch (static_cast<int>(usesculptbrush))
	{
	case static_cast<int>(E_UseSculptBrush::Brush001):
		UseSculptBrush001(ppassedsculptbrushstate);
		break;
	case static_cast<int>(E_UseSculptBrush::Brush002):
		UseSculptBrush002(ppassedsculptbrushstate);
		break;
	case static_cast<int>(E_UseSculptBrush::Brush003):
		UseSculptBrush003(ppassedsculptbrushstate);
		break;
	//case static_cast<int>(E_UseSculptBrush::Brush004):
	//	//UseSculptBrush004();
	//	break;

	default:
		break;
	}

	return;
}

void MeshFieldObject::UseSculptBrush001(const PassedSculptBrushState* ppassedsculptbrushstate)
{
	float StrengthMagnification = 0.05f;

	// まずyは考えずにxzでブロックで分けて考える。中心のブロックだけを求める。

	Int2 panel;
	if (!GetPanelBlock(&panel, ppassedsculptbrushstate->Position))
		return;


	// ここまでで中心のブロックが求まっている。

	// 四角で考えて、そのあとにそのブロックの頂点が球の範囲内かを考える。
	int panelradius, minx, minz, maxx, maxz;
	panelradius = ppassedsculptbrushstate->Radius / PanelSize;		// 直線で考えた時に半径内に何枚のパネルがあるか
	minx = panel.x - panelradius;
	minz = panel.y - panelradius;
	maxx = panel.x + panelradius;
	maxz = panel.y + panelradius;
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
		D3DXVECTOR3 distance = ppassedsculptbrushstate->Position - m_Vertex[vertexNum].Position;
		float length = D3DXVec3LengthSq(&distance);
		if (length > pow((ppassedsculptbrushstate->Radius * 1.3f), 2))
			continue;

		NewverticesNum.push_back(vertexNum);
	}



	//頂点データ書き換え。書き換えのための関数->Map関数を使う。
	// Map関数を使うと、元のデータは全部消えるので、全て作り直し、新たに値を設定する必要がある。
	// 全部消えちゃうので、そのためのm_Vertexを用意してある。
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// まず全てをm_Vertexで値を入れる。このためのm_Vertex。
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{
		// 頂点の位置を変更。m_Vertexの値も同時に更新する
		m_Vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * StrengthMagnification;
		vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * StrengthMagnification;
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}

void MeshFieldObject::UseSculptBrush002(const PassedSculptBrushState* ppassedsculptbrushstate)
{
	float StrengthMagnification = 0.05f;
	// まずyは考えずにxzでブロックで分けて考える。中心のブロックだけを求める。

	Int2 panel;
	if (!GetPanelBlock(&panel, ppassedsculptbrushstate->Position))
		return;


	// ここまでで中心のブロックが求まっている。

	// 四角で考えて、そのあとにそのブロックの頂点が球の範囲内かを考える。
	int panelradius, minx, minz, maxx, maxz;
	panelradius = ppassedsculptbrushstate->Radius / PanelSize;		// 直線で考えた時に半径内に何枚のパネルがあるか
	minx = panel.x - panelradius;
	minz = panel.y - panelradius;
	maxx = panel.x + panelradius;
	maxz = panel.y + panelradius;
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
		if (m_Vertex[vertexNum].Position.y > ppassedsculptbrushstate->Strength + ppassedsculptbrushstate->TriggerPosition.y)
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

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// まず全てをm_Vertexで値を入れる。このためのm_Vertex。
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{
		if (m_Vertex[vertexNum].Position.y >= 0.2f)
			int gtg = 0;//チェック用


		// 頂点の位置を変更。m_Vertexの値も同時に更新する
		m_Vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * StrengthMagnification;
		vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * StrengthMagnification;
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

// トリガーポイントに寄せるブラシ
void MeshFieldObject::UseSculptBrush003(const PassedSculptBrushState* ppassedsculptbrushstate)
{
	// まずyは考えずにxzでブロックで分けて考える。中心のブロックだけを求める。

	Int2 panel;
	if (!GetPanelBlock(&panel, ppassedsculptbrushstate->Position))
		return;


	// ここまでで中心のブロックが求まっている。

	// 四角で考えて、そのあとにそのブロックの頂点が球の範囲内かを考える。
	int panelradius, minx, minz, maxx, maxz;
	panelradius = ppassedsculptbrushstate->Radius / PanelSize;		// 直線で考えた時に半径内に何枚のパネルがあるか
	minx = panel.x - panelradius;
	minz = panel.y - panelradius;
	maxx = panel.x + panelradius;
	maxz = panel.y + panelradius;
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
		// yを無視する
		D3DXVECTOR3 PP = D3DXVECTOR3(ppassedsculptbrushstate->Position.x, 0.0f, ppassedsculptbrushstate->Position.z);
		D3DXVECTOR3 VP = D3DXVECTOR3(m_Vertex[vertexNum].Position.x, 0.0f, m_Vertex[vertexNum].Position.z);
		// 半径よりデカかったら除外。
		D3DXVECTOR3 distance = PP - VP;
		float lengthsq = D3DXVec3LengthSq(&distance);
		if (lengthsq > pow((ppassedsculptbrushstate->Radius * 1.0f), 2))
			continue;

		NewverticesNum.push_back(vertexNum);
	}



	//頂点データ書き換え。書き換えのための関数->Map関数を使う。
	// Map関数を使うと、元のデータは全部消えるので、全て作り直し、新たに値を設定する必要がある。
	// 全部消えちゃうので、そのためのm_Vertexを用意してある。
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// まず全てをm_Vertexで値を入れる。このためのm_Vertex。
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{

		// 頂点の位置を変更。m_Vertexの値も同時に更新する
		m_Vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * 0.1f;
		if (m_Vertex[vertexNum].Position.y > ppassedsculptbrushstate->TriggerPosition.y)
		{
			m_Vertex[vertexNum].Position.y -= ppassedsculptbrushstate->Strength * 0.1f;
			if (m_Vertex[vertexNum].Position.y > ppassedsculptbrushstate->TriggerPosition.y)
				m_Vertex[vertexNum].Position.y = ppassedsculptbrushstate->TriggerPosition.y;
		}
		else if (m_Vertex[vertexNum].Position.y < ppassedsculptbrushstate->TriggerPosition.y)
		{
			m_Vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * 0.1f;
			if (m_Vertex[vertexNum].Position.y < ppassedsculptbrushstate->TriggerPosition.y)
				m_Vertex[vertexNum].Position.y = ppassedsculptbrushstate->TriggerPosition.y;
		}

		vertex[vertexNum].Position.y = m_Vertex[vertexNum].Position.y;
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}

void MeshFieldObject::UseSculptBrush004(const PassedSculptBrushState* ppassedsculptbrushstate)
{
	// 未使用
	return;
}

///////////////////////////// ペイントブラシ - ブラシセッティング//////////////////////////////////////////
//void MeshFieldObject::UsePaintBrushes(const E_UsePaintBrush& usepaintbrush, const PassedPaintBrushState* ppassedpaintbrushstate)
//{
//	//switch (static_cast<int>(usepaintbrush))
//	//{
//	//case static_cast<int>(E_UsePaintBrush::Brush001):
//	//	UsePaintBrush001(ppassedpaintbrushstate);
//	//	break;
//	//case static_cast<int>(E_UsePaintBrush::Brush002):
//	//	UsePaintBrush002(ppassedpaintbrushstate);
//	//	break;
//	//case static_cast<int>(E_UsePaintBrush::Brush003):
//	//	//UsePaintBrush003();
//	//	break;
//
//	//default:
//	//	break;
//	//}
//
//	//return;
//}

//void MeshFieldObject::UsePaintBrush001(const PassedPaintBrushState* ppassedpaintbrushstate)
//{
//	// まずyは考えずにxzでブロックで分けて考える。中心のブロックだけを求める。
//
//	Int2 panel;
//	if (!GetPanelBlock(&panel, ppassedpaintbrushstate->Position))
//		return;
//
//
//	// ここまでで中心のブロックが求まっている。
//
//	// 四角で考えて、そのあとにそのブロックの頂点が球の範囲内かを考える。
//	int panelradius, minx, minz, maxx, maxz;
//	panelradius = ppassedpaintbrushstate->Radius / PanelSize;		// 直線で考えた時に半径内に何枚のパネルがあるか
//	minx = panel.x - panelradius;
//	minz = panel.y - panelradius;
//	maxx = panel.x + panelradius;
//	maxz = panel.y + panelradius;
//	{
//		if (minx < 0)
//			minx = 0;
//		if (minz < 0)
//			minz = 0;
//		if (maxx > FieldSize)
//			maxx = FieldSize;
//		if (maxz > FieldSize)
//			maxz = FieldSize;
//	}
//
//	// 変更する頂点たちを配列で作っちゃう
//	std::list<int> verticesNum;
//
//	// 四角の範囲内のブロックの周りの4頂点を追加していく
//	for (int xx = minx; xx <= maxx; xx++)
//	{
//		for (int zz = minz; zz <= maxz; zz++)
//		{
//			// 周りの４頂点を追加
//			verticesNum.push_back((xx * (FieldSize + 1)) + zz);
//			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + zz);
//			verticesNum.push_back((xx * (FieldSize + 1)) + (zz + 1));
//			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + (zz + 1));
//		}
//	}
//
//	// 頂点の重複を排除。unique関数はソートされていないと使えない。
//	verticesNum.sort();
//	verticesNum.unique();
//
//	// 頂点と中心の距離で球の中かを計算する
//	std::list<int> NewverticesNum;
//	for (auto vertexNum : verticesNum)
//	{
//		if (m_Vertex[vertexNum].Position.y > 0.1f)
//		{
//			continue;
//		}
//
//		//// まずyが離れすぎていたら除外。高さの0.2倍以上だったらダメ。value * 0.2f
//		//if (fabsf(TriggerPosition.y - m_Vertex[vertexNum].Position.y) > value * 0.2f)
//		//{
//		//	//if (TriggerPosition.y - m_Vertex[vertexNum].Position.y < -value * 0.1f)
//		//	//{
//		//	//	continue;
//		//	//}
//		//	continue;
//		//}
//
//		NewverticesNum.push_back(vertexNum);
//	}
//
//
//
//	//頂点データ書き換え。書き換えのための関数->Map関数を使う。
//	// Map関数を使うと、元のデータは全部消えるので、全て作り直し、新たに値を設定する必要がある。
//	// 全部消えちゃうので、そのためのm_Vertexを用意してある。
//	D3D11_MAPPED_SUBRESOURCE msr;
//	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
//
//	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;
//
//	// まず全てをm_Vertexで値を入れる。このためのm_Vertex。
//	memcpy(vertex, m_Vertex, sizeof(m_Vertex));
//
//	for (auto vertexNum : NewverticesNum)
//	{
//		// 頂点の位置を変更。m_Vertexの値も同時に更新する
//		m_Vertex[vertexNum].Position.y += ppassedpaintbrushstate->Strength;
//		vertex[vertexNum].Position.y += ppassedpaintbrushstate->Strength;
//	}
//
//	int ff = 0;
//	for (auto ind : m_Index)
//	{
//		if (ind == 1)
//			int rhgrg = 3;
//		ff++;
//	}
//
//	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
//
//
//	return;
//}
//
//void MeshFieldObject::UsePaintBrush002(const PassedPaintBrushState* ppassedpaintbrushstate)
//{
//
//	return;
//}


// brushnumにPaintBrushSetMax-1を渡すことで消しゴム扱いになる
// 現在ブラシリスト0の場合のみしか考えられていない
void MeshFieldObject::UsePaintBrush(const int listnum, const int brushnum, PassedPaintBrushState* ppassedpaintbrushstate)
{
	// まずyは考えずにxzでブロックで分けて考える。中心のブロックだけを求める。

	Int2 panel;
	if (!GetPanelBlock(&panel, ppassedpaintbrushstate->Position))
		return;


	// ここまでで中心のブロックが求まっている。

	// 四角で考えて、そのあとにそのブロックの頂点が球の範囲内かを考える。
	int panelradius, minx, minz, maxx, maxz;
	panelradius = ppassedpaintbrushstate->Radius / PanelSize;		// 直線で考えた時に半径内に何枚のパネルがあるか
	minx = panel.x - panelradius;
	minz = panel.y - panelradius;
	maxx = panel.x + panelradius;
	maxz = panel.y + panelradius;
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
	std::list<float> NewverticesNumDistanceWariai;	// 距離の割合用。近いほど0.0~遠いほど~1.0
	for (auto vertexNum : verticesNum)
	{
		// 半径よりデカかったら除外。
		D3DXVECTOR3 distance = ppassedpaintbrushstate->Position - m_Vertex[vertexNum].Position;
		float lengthsq = D3DXVec3LengthSq(&distance);
		float radiussq = pow((ppassedpaintbrushstate->Radius * 1.0f), 2);
		if (lengthsq > radiussq)
			continue;

		float wariai = 1.0f - (lengthsq / radiussq);

		NewverticesNum.push_back(vertexNum);
		NewverticesNumDistanceWariai.push_back(wariai);
	}



	//頂点データ書き換え。書き換えのための関数->Map関数を使う。
	// Map関数を使うと、元のデータは全部消えるので、全て作り直し、新たに値を設定する必要がある。
	// 全部消えちゃうので、そのためのm_Vertexを用意してある。
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// まず全てをm_Vertexで値を入れる。このためのm_Vertex。
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));


	auto wariaiItr = NewverticesNumDistanceWariai.begin();
	for (auto vertexNum : NewverticesNum)
	{
		// 消しゴムだったら現在のブラシセットの全ての割合を下げる
		if (brushnum == PaintBrushSetMax - 1)
		{
			// フロア変えた
			m_Vertex[vertexNum].FloorAlfa0.x -= *wariaiItr;
			if (m_Vertex[vertexNum].FloorAlfa0.x < 0.0f)
				m_Vertex[vertexNum].FloorAlfa0.x = 0.0f;
			m_Vertex[vertexNum].FloorAlfa0.y -= *wariaiItr;
			if (m_Vertex[vertexNum].FloorAlfa0.y < 0.0f)
				m_Vertex[vertexNum].FloorAlfa0.y = 0.0f;
			m_Vertex[vertexNum].FloorAlfa0.z -= *wariaiItr;
			if (m_Vertex[vertexNum].FloorAlfa0.z < 0.0f)
				m_Vertex[vertexNum].FloorAlfa0.z = 0.0f;
			m_Vertex[vertexNum].FloorAlfa0.w -= *wariaiItr;
			if (m_Vertex[vertexNum].FloorAlfa0.w < 0.0f)
				m_Vertex[vertexNum].FloorAlfa0.w = 0.0f;
			vertex[vertexNum].FloorAlfa0.x = m_Vertex[vertexNum].FloorAlfa0.x;
			vertex[vertexNum].FloorAlfa0.y = m_Vertex[vertexNum].FloorAlfa0.y;
			vertex[vertexNum].FloorAlfa0.z = m_Vertex[vertexNum].FloorAlfa0.z;
			vertex[vertexNum].FloorAlfa0.w = m_Vertex[vertexNum].FloorAlfa0.w;

			++wariaiItr;
			continue;
		}

		// アルファは強さと距離に応じて変える。更新するのよりアルファ値が小さかったら更新する
		switch (brushnum)
		{
		case 0:
			if (m_Vertex[vertexNum].FloorAlfa0.x < *wariaiItr)
			{
				m_Vertex[vertexNum].FloorAlfa0.x = *wariaiItr;
			}
			break;
		case 1:
			if (m_Vertex[vertexNum].FloorAlfa0.y < *wariaiItr)
			{
				m_Vertex[vertexNum].FloorAlfa0.y = *wariaiItr;
			}
			break;
		case 2:
			if (m_Vertex[vertexNum].FloorAlfa0.z < *wariaiItr)
			{
				m_Vertex[vertexNum].FloorAlfa0.z = *wariaiItr;
			}
			break;
		case 3:
			// フロア変えた
			if (m_Vertex[vertexNum].FloorAlfa0.w < *wariaiItr)
			{
				m_Vertex[vertexNum].FloorAlfa0.w = *wariaiItr;
			}
			break;
		default:
			break;
		}

		// そして自分以外を下げる
		float Down = *wariaiItr * 0.1f;
		DownOtherVertexAlfa(brushnum, Down, vertexNum);

		vertex[vertexNum].FloorAlfa0.x = m_Vertex[vertexNum].FloorAlfa0.x;
		vertex[vertexNum].FloorAlfa0.y = m_Vertex[vertexNum].FloorAlfa0.y;
		vertex[vertexNum].FloorAlfa0.z = m_Vertex[vertexNum].FloorAlfa0.z;
		// フロア変えた
		vertex[vertexNum].FloorAlfa0.w = m_Vertex[vertexNum].FloorAlfa0.w;

		// FloorTexture0.xに現在のブラシをセットする。これで最後に使ったブラシで上書きになる。
		m_Vertex[vertexNum].FloorTexture0.x = brushnum;
		vertex[vertexNum].FloorTexture0.x = brushnum;


		++wariaiItr;
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}

// 自分以外の３つの頂点のアルファを下げる。セットリストは0にしか対応してない
void MeshFieldObject::DownOtherVertexAlfa(const int selfnum, const float downvalue, const int vertexnum)
{
	// フロア変えた
	switch (selfnum)
	{
	case 0:
		m_Vertex[vertexnum].FloorAlfa0.y -= downvalue;
		m_Vertex[vertexnum].FloorAlfa0.z -= downvalue;
		m_Vertex[vertexnum].FloorAlfa0.w -= downvalue;
		break;
	case 1:
		m_Vertex[vertexnum].FloorAlfa0.x -= downvalue;
		m_Vertex[vertexnum].FloorAlfa0.z -= downvalue;
		m_Vertex[vertexnum].FloorAlfa0.w -= downvalue;
		break;
	case 2:
		m_Vertex[vertexnum].FloorAlfa0.x -= downvalue;
		m_Vertex[vertexnum].FloorAlfa0.y -= downvalue;
		m_Vertex[vertexnum].FloorAlfa0.w -= downvalue;
		break;
	case 3:
		m_Vertex[vertexnum].FloorAlfa0.x -= downvalue;
		m_Vertex[vertexnum].FloorAlfa0.y -= downvalue;
		m_Vertex[vertexnum].FloorAlfa0.z -= downvalue;
		break;
	default:
		break;
	}

	if (m_Vertex[vertexnum].FloorAlfa0.x < 0.0f)
		m_Vertex[vertexnum].FloorAlfa0.x = 0.0f;
	if (m_Vertex[vertexnum].FloorAlfa0.y < 0.0f)
		m_Vertex[vertexnum].FloorAlfa0.y = 0.0f;
	if (m_Vertex[vertexnum].FloorAlfa0.z < 0.0f)
		m_Vertex[vertexnum].FloorAlfa0.z = 0.0f;
	if (m_Vertex[vertexnum].FloorAlfa0.w < 0.0f)
		m_Vertex[vertexnum].FloorAlfa0.w = 0.0f;

	return;
}