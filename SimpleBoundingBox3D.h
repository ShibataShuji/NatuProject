#pragma once

class GameObject;
class Model;

class SimpleBoundingBox3D
{
private:
	GameObject* m_ParentGameObject;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	Model* m_Model;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:
	SimpleBoundingBox3D() = delete;
	SimpleBoundingBox3D(GameObject* gameObject)
	{
		m_ParentGameObject = gameObject;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

};

