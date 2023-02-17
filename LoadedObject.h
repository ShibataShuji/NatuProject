#pragma once


class LoadedObject : public GameObject	// åpè≥
{
private:

	static class Model* m_Model;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static void Load();
	static void Unload();

	void AddComponentWithID(std::ifstream* Objfile, int compID);

};
