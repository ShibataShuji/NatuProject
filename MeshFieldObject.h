#pragma once

// �Q�[���I�u�W�F�N�g��MeshFieldObject��p�o�[�W����
// �Q�[���I�u�W�F�N�g�Ƃ͕ʂȂ̂ŁA�ʂŃV�[���Ń��X�g������ČĂ΂Ȃ��Ƃ����Ȃ��B


#define DEFAULT_MeshFieldTexName "Gravel033_4K_Color.jpg"


// �Z�[�u�f�[�^�ɕۑ���������
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
	Int2						m_AreaBlock = Int2(0, 0);	// ���̃��b�V���t�B�[���h���ǂ̒n��敪�Ȃ̂�
	D3DXVECTOR3					m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3					m_PosOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	std::string					m_TextureName = DEFAULT_MeshFieldTexName;

	bool						m_UseShadow = true;

	bool						m_Destroy = false;

	// ID3D11ShaderResourceView*�@��z��ɂ��Ďg������_���B�g���Ȃ��B
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11ShaderResourceView* m_TextureNormal = NULL;		// �@���}�b�v�e�N�X�`��
	ID3D11ShaderResourceView* m_TextureOver = NULL;		// �R�e�N�X�`��
	ID3D11ShaderResourceView* m_TextureOverNormal = NULL;		// �R�e�N�X�`��
	// ���e�N�X�`�� -2�́A�����S����END���B�V�F�[�_�[�̕���g_TextureFloor��ݒ肵�Ă�����
	ID3D11ShaderResourceView* m_TextureFloor1 = NULL;
	ID3D11ShaderResourceView* m_TextureFloorNormal1 = NULL;
	ID3D11ShaderResourceView* m_TextureFloor2 = NULL;
	ID3D11ShaderResourceView* m_TextureFloorNormal2 = NULL;
	// 3,4�͖��g�p�����ǉ��ŃZ�b�g����
	ID3D11ShaderResourceView* m_TextureFloor3 = NULL;
	ID3D11ShaderResourceView* m_TextureFloorNormal3 = NULL;
	ID3D11ShaderResourceView* m_TextureFloor4 = NULL;
	ID3D11ShaderResourceView* m_TextureFloorNormal4 = NULL;

	VERTEX_3D_FLOOR				m_Vertex[(FieldSize + 1) * (FieldSize + 1)];
	//VERTEX_3D_FLOOR2				m_Vertex[(FieldSize + 1) * (FieldSize + 1)];
	DWORD						m_Index[((FieldSize + 2) * 2) * FieldSize - 2];	// �C���f�b�N�X�f�[�^(�����̐����������Ă���)



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


	// ���݂̃f�[�^���A�Z�[�u�p�\���̂ɕϊ����ĕԂ��B
	StructPlacedMeshFieldObjectData GetStructPlacedMeshFieldObjectData();

	void OverWriteSPMOD(StructPlacedMeshFieldObjectData& data);


	// ���݂�m_Vertex�Ńo�[�e�b�N�X�o�b�t�@������������B�@���̍X�V�������ōs�킹�����B
	void OverWriteVertexBuffer();

	// Init�ł��悤�ɕς����B<-Add�����シ���ɂ��Bint�^�Ńu���b�N�ԍ�����邾���Ń|�W�V�����Ƃ��I�t�Z�b�g��ݒ�ł���
	//void SetAreaBlock(Int2 areablock);// ���ݎg���Ă��Ȃ��B���g�̂̂܂܂�����Ԉ���Ă鑽��
	Int2 GetAreaBlock() { return m_AreaBlock; }

	bool GetUseShadow() { return m_UseShadow; }
	void SetUseShadow(bool shadow) { m_UseShadow = shadow; }

	// �p�l���̃u���b�N�ԍ��Z�o�B
	bool GetPanelBlock(Int2* pOutInt2, const D3DXVECTOR3& Position);

	float GetHeight(const D3DXVECTOR3& Position);
	bool GetHeightandNormal(float* pOut_height, D3DXVECTOR3* pOut_normal, const D3DXVECTOR3& Position);
	int8_t CheckCollisionMeshFieldObject(float* pOut_height, D3DXVECTOR3* pOut_normal, D3DXVECTOR3* pOut_beforpos, const D3DXVECTOR3& Position);

	void UpdataMeshFieldObjectNormal();


	// �u���V�n
	// �X�J���v�g�u���V
	void UseSculptBrushes(const E_UseSculptBrush& usesculptbrush, const PassedSculptBrushState* ppassedsculptbrushstate);	// ���̊֐��̒���UseSculptBrush001()�B���g���B
	void UseSculptBrush001(const PassedSculptBrushState* ppassedsculptbrushstate);
	void UseSculptBrush002(const PassedSculptBrushState* ppassedsculptbrushstate);
	void UseSculptBrush003(const PassedSculptBrushState* ppassedsculptbrushstate);
	void UseSculptBrush004(const PassedSculptBrushState* ppassedsculptbrushstate);
	// �y�C���g�u���V�@�͂ЂƂ�OK
	//void UsePaintBrushes(const E_UsePaintBrush& usepaintbrush, const PassedPaintBrushState* ppassedpaintbrushstate);	// ���̊֐��̒���UseSculptBrush001()�B���g���B
	//void UsePaintBrush001(const PassedPaintBrushState* ppassedpaintbrushstate);
	//void UsePaintBrush002(const PassedPaintBrushState* ppassedpaintbrushstate);

	void UsePaintBrush(const int listnum, const int brushnum, PassedPaintBrushState* ppassedpaintbrushstate);

	void DownOtherVertexAlfa(const int selfnum, const float downvalue, const int vertexnum);

};

