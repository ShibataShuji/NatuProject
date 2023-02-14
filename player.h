#pragma once


static const int MaxBulletNum = 5;

class Player : public GameObject	// åpè≥
{
private:

	int m_BulletNum;

	Model* m_Model;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	class Audio* m_ShotSE;

	class Shadow* m_Shadow;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR3 GetPlayerPosition();

	void SetBulletNumMax();

};
