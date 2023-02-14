#pragma once

class ImageDisplayFull : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;				// ���_�o�b�t�@��ۑ�����ꏊ
	ID3D11ShaderResourceView* m_Texture = NULL;			// �e�N�X�`����ۑ�����ꏊ

	ID3D11VertexShader* m_VertexShader;					// �V�F�[�_�[�̂��
	ID3D11PixelShader* m_PixelShader;					// �V�F�[�_�[�̂��
	ID3D11InputLayout* m_VertexLayout;					// �V�F�[�_�[�̂��

	// "asset/texture/SSE_Title.png"�̂悤�ɑS�ē����B
	std::string		m_TextureName;	


public:

	ImageDisplayFull() = delete;
	ImageDisplayFull(std::string texname)
	{
		m_TextureName = texname;
	}

	// �A�N�Z�X�w�肵���p�u���b�N�ɂ��Ȃ��ƊO������A�N�Z�X�ł��Ȃ�
	void Init();
	void Uninit();
	void Update();
	void Draw();



};
