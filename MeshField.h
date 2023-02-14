#pragma once


//#include "GameObject.h"

//Int2 GetAreaBlock(const D3DXVECTOR3& pos);
//Int2 GetAreaBlock(const D3DXVECTOR3& pos)
//{
//	Int2 re;
//	// �u���b�N�ԍ��Z�o�B�����͂��̃��b�V���t�B�[���h�̎��_�̈ʒu���I�t�Z�b�g�ǉ����Ȃ��Ƃ����Ȃ�
//	re.x = pos.x / interval;
//	re.y = pos.z / interval;
//
//	return re;
//}


class MeshField : public GameObject
{
private:

	Int2						m_AreaBlock;	// ���̃��b�V���t�B�[���h���ǂ̒n��敪�Ȃ̂�
	D3DXVECTOR3					m_PosOffset;


	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11Buffer*				m_IndexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	VERTEX_3D					m_Vertex[(FieldSize + 1) * (FieldSize + 1)];
	DWORD						m_Index[((FieldSize + 2) * 2) * FieldSize - 2];	// �C���f�b�N�X�f�[�^(�����̐����������Ă���)



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

	// Add�����シ���ɂ��Bint�^�Ńu���b�N�ԍ�����邾���Ń|�W�V�����Ƃ��I�t�Z�b�g��ݒ�ł���
	void SetAreaBlock(Int2 areablock);
};

