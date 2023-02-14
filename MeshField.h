#pragma once


//#include "GameObject.h"

//Int2 GetAreaBlock(const D3DXVECTOR3& pos);
//Int2 GetAreaBlock(const D3DXVECTOR3& pos)
//{
//	Int2 re;
//	// ブロック番号算出。ここはこのメッシュフィールドの視点の位置分オフセット追加しないといけない
//	re.x = pos.x / interval;
//	re.y = pos.z / interval;
//
//	return re;
//}


class MeshField : public GameObject
{
private:

	Int2						m_AreaBlock;	// このメッシュフィールドがどの地域区分なのか
	D3DXVECTOR3					m_PosOffset;


	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11Buffer*				m_IndexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	VERTEX_3D					m_Vertex[(FieldSize + 1) * (FieldSize + 1)];
	DWORD						m_Index[((FieldSize + 2) * 2) * FieldSize - 2];	// インデックスデータ(索引の整数が入っている)



	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	float GetHeight(D3DXVECTOR3 const &Position);
	void UseSculptBrush001(D3DXVECTOR3 const &Position, float const &radius, float const &value);
	void UseSculptBrush002(D3DXVECTOR3 const& TriggerPosition, D3DXVECTOR3 const& Position, float const& radius, float const& value);

	// Addした後すぐにやる。int型でブロック番号入れるだけでポジションとかオフセットを設定できる
	void SetAreaBlock(Int2 areablock);
};

