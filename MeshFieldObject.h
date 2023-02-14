#pragma once

// ゲームオブジェクトのMeshFieldObject専用バージョン
// ゲームオブジェクトとは別なので、別でシーンでリストを作って呼ばないといけない。


#define DEFAULT_MeshFieldTexName "Gravel033_4K_Color.jpg"


// セーブデータに保存されるもの
struct StructPlacedMeshFieldObjectData
{
	char		sm_TexName[64] = DEFAULT_MeshFieldTexName;
	Int2		sm_AreaBlock = Int2(0, 0);
	D3DXVECTOR3 sm_Position = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 sm_PosOffset = { 0.0f, 0.0f, 0.0f };
	bool		sm_UseShadow = true;

	VERTEX_3D_FLOOR	sm_Vertex[(FieldSize + 1) * (FieldSize + 1)];
	//VERTEX_3D_FLOOR2	sm_Vertex[(FieldSize + 1) * (FieldSize + 1)];
};



class MeshFieldObject
{
private:
	Int2						m_AreaBlock = Int2(0, 0);	// このメッシュフィールドがどの地域区分なのか
	D3DXVECTOR3					m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3					m_PosOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	std::string					m_TextureName = DEFAULT_MeshFieldTexName;

	bool						m_UseShadow = true;

	bool						m_Destroy = false;

	// ID3D11ShaderResourceView*　を配列にして使っちゃダメ。使えない。
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11ShaderResourceView* m_TextureNormal = NULL;		// 法線マップテクスチャ
	ID3D11ShaderResourceView* m_TextureOver = NULL;		// 崖テクスチャ
	ID3D11ShaderResourceView* m_TextureOverNormal = NULL;		// 崖テクスチャ
	// 床テクスチャ -2は、消しゴムとEND分。シェーダーの方でg_TextureFloorを設定してあげる
	ID3D11ShaderResourceView* m_TextureFloor1 = NULL;
	ID3D11ShaderResourceView* m_TextureFloorNormal1 = NULL;
	ID3D11ShaderResourceView* m_TextureFloor2 = NULL;
	ID3D11ShaderResourceView* m_TextureFloorNormal2 = NULL;
	// 3,4は未使用だけど仮でセットする
	ID3D11ShaderResourceView* m_TextureFloor3 = NULL;
	ID3D11ShaderResourceView* m_TextureFloorNormal3 = NULL;
	ID3D11ShaderResourceView* m_TextureFloor4 = NULL;
	ID3D11ShaderResourceView* m_TextureFloorNormal4 = NULL;

	VERTEX_3D_FLOOR				m_Vertex[(FieldSize + 1) * (FieldSize + 1)];
	//VERTEX_3D_FLOOR2				m_Vertex[(FieldSize + 1) * (FieldSize + 1)];
	DWORD						m_Index[((FieldSize + 2) * 2) * FieldSize - 2];	// インデックスデータ(索引の整数が入っている)



	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

public:
	void Init(Int2 areablock);
	void Uninit();
	void Update();
	void Draw();

	bool GetDestroyState()
	{
		if (this == nullptr)
			return true;
		return m_Destroy;
	}
	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}


	// 現在のデータを、セーブ用構造体に変換して返す。
	StructPlacedMeshFieldObjectData GetStructPlacedMeshFieldObjectData();

	void OverWriteSPMOD(StructPlacedMeshFieldObjectData& data);


	// 現在のm_Vertexでバーテックスバッファを書き換える。法線の更新もここで行わせたい。
	void OverWriteVertexBuffer();

	// Initでやるように変えた。<-Addした後すぐにやる。int型でブロック番号入れるだけでポジションとかオフセットを設定できる
	//void SetAreaBlock(Int2 areablock);// 現在使っていない。中身昔のままだから間違ってる多分
	Int2 GetAreaBlock() { return m_AreaBlock; }

	bool GetUseShadow() { return m_UseShadow; }
	void SetUseShadow(bool shadow) { m_UseShadow = shadow; }

	// パネルのブロック番号算出。
	bool GetPanelBlock(Int2* pOutInt2, const D3DXVECTOR3& Position);

	float GetHeight(const D3DXVECTOR3& Position);
	bool GetHeightandNormal(float* pOut_height, D3DXVECTOR3* pOut_normal, const D3DXVECTOR3& Position);
	int8_t CheckCollisionMeshFieldObject(float* pOut_height, D3DXVECTOR3* pOut_normal, D3DXVECTOR3* pOut_beforpos, const D3DXVECTOR3& Position);

	void UpdataMeshFieldObjectNormal();


	// ブラシ系
	// スカルプトブラシ
	void UseSculptBrushes(const E_UseSculptBrush& usesculptbrush, const PassedSculptBrushState* ppassedsculptbrushstate);	// この関数の中でUseSculptBrush001()達を使う。
	void UseSculptBrush001(const PassedSculptBrushState* ppassedsculptbrushstate);
	void UseSculptBrush002(const PassedSculptBrushState* ppassedsculptbrushstate);
	void UseSculptBrush003(const PassedSculptBrushState* ppassedsculptbrushstate);
	void UseSculptBrush004(const PassedSculptBrushState* ppassedsculptbrushstate);
	// ペイントブラシ　はひとつでOK
	//void UsePaintBrushes(const E_UsePaintBrush& usepaintbrush, const PassedPaintBrushState* ppassedpaintbrushstate);	// この関数の中でUseSculptBrush001()達を使う。
	//void UsePaintBrush001(const PassedPaintBrushState* ppassedpaintbrushstate);
	//void UsePaintBrush002(const PassedPaintBrushState* ppassedpaintbrushstate);

	void UsePaintBrush(const int listnum, const int brushnum, PassedPaintBrushState* ppassedpaintbrushstate);

	void DownOtherVertexAlfa(const int selfnum, const float downvalue, const int vertexnum);

};

