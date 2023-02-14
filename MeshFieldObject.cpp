#include "stdafx.h"
//#include "MeshFieldObject.h"


// array�^�͗v�f�����ȗ��ł��Ȃ��Bvector�^�͏ȗ��ł���
//std::array<std::array<float, FieldSize + 1>, FieldSize + 1> g_fieldHeight;


void MeshFieldObject::Init(Int2 areablock)
{
	m_AreaBlock = areablock;

	// ���b�V���t�B�[���h��1�}�X�̑傫���Ƃ��ɂ���č��W���Z�b�g����
	m_PosOffset.x = areablock.x * interval;
	m_PosOffset.z = (areablock.y + 1) * interval;

	// m_Position �ς����Ⴄ�ƂȂ��������B
	//m_Position = m_PosOffset;


	// ���_�o�b�t�@����
	{

		for (int x = 0; x <= FieldSize; x++)
		{
			for (int z = 0; z <= FieldSize; z++)
			{
				//m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3((x - m_PosOffset.x) * PanelSize, -m_PosOffset.y, (z - m_PosOffset.z) * -PanelSize);
				
				m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3(x * PanelSize, 0.0f, z * -PanelSize);
				m_Vertex[(x * (FieldSize + 1)) + z].Position += m_PosOffset;
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ���Ŗ@���x�N�g�������߂ăZ�b�g���Ă���
				m_Vertex[(x * (FieldSize + 1)) + z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[(x * (FieldSize + 1)) + z].TexCoord = D3DXVECTOR2(x * 0.05f, z * 0.05f);
				// 4�e�N�X�`���Z�b�g�ł��邯�Ǎŏ��͋���ۂɂ��Ă����B�S�������S��

				// �t���A�ς���
				m_Vertex[(x * (FieldSize + 1)) + z].FloorTexture0 = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
				m_Vertex[(x * (FieldSize + 1)) + z].FloorAlfa0 = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		// �@���x�N�g���Z�oNormal���v�Z�ŋ��߂�
		// x�����ׂ̗�2�_���Ȃ��x�N�g����
				// z�����ׂ̗�2�_���Ȃ��x�N�g����
				// �O�ς��Ă�����΁A����炵���x�N�g�����o�邩�ȁ`
		for (int x = 1; x <= (FieldSize - 1); x++)
		{
			for (int z = 1; z <= (FieldSize - 1); z++)
			{
				D3DXVECTOR3 vx, vz, vn;
				vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// �I�_����n�_�������Ƃ��̒����̕����x�N�g�������߂���
				vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 

				//D3DXVec3Cross(&vn, &vx, &vz);// �O�ρBvx�ɂ�vz�ɂ������ȃx�N�g�������܂�B�����鏇�ԂŌ������^�t�ɕς��B
				D3DXVec3Cross(&vn, &vz, &vx);// �O�ρBvx�ɂ�vz�ɂ������ȃx�N�g�������܂�B�����鏇�ԂŌ������^�t�ɕς��B
				D3DXVec3Normalize(&vn, &vn);// ���K��(����1�ɂ���)
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
			}
		}

		//���_�o�b�t�@����
		// ���_�f�[�^���p�ӂł����̂ŁA�_�C���N�g�G�b�N�X�p�̒��_�o�b�t�@������Ă����B
		//D3D11_BUFFER_DESC bd{};

		//ZeroMemory(&bd, sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(VERTEX_3D_FLOOR) * (FieldSize + 1) * (FieldSize + 1);
		//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//bd.CPUAccessFlags = 0;
		//
		// ���_�o�b�t�@����(���_������ŕς�����悤�ɂ��Ă���)
		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D_FLOOR) * (FieldSize + 1) * (FieldSize + 1);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//��
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//��

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	// �C���f�b�N�X�o�b�t�@����
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

			// �k�ޗp�̗]���ȃC���f�b�N�X�p
			if (x == (FieldSize - 1))
				break;

			m_Index[i] = (x + 1) * (FieldSize + 1) + FieldSize;
			i++;

			m_Index[i] = (x + 1) * (FieldSize + 1);
			i++;
		}

		// ���_�o�b�t�@�Ǝ��Ă邯�ǂ�����ƈႤ����C��t���Ă�
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
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;//��
		bd.CPUAccessFlags = 0;//��

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);

	}

	//�e�N�X�`���ǂݍ���
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

	// �m�[�}���}�b�v�̃e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Grass004_1K_NormalDX.jpg",	// park_dirt_nor_gl_1k.jpg , normalmap.png
		NULL,
		NULL,
		&m_TextureNormal,
		NULL);
	assert(m_TextureNormal);

	// �R�p�e�N�X�`���ǂݍ���,		// red.png , field001.jpg
	{
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/Rock020_2K_Color.jpg", NULL, NULL, &m_TextureOver, NULL);
		assert(m_TextureOver);
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/Rock020_2K_NormalDX.jpg", NULL, NULL, &m_TextureOverNormal, NULL);//Grass004_1K_NormalDX,Rock020_4K_NormalDX
		assert(m_TextureOverNormal);
	}

	// ���p�e�N�X�`���ǂݍ���
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


	// �V�F�[�_�[�̓ǂݍ���
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

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���b�N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&rot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);


	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D_FLOOR);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	// �m�[�}���}�b�v�p�e�N�X�`���ݒ�,0�Ԃ́��Ŏg���Ă��邩��2�Ԃɐݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &m_TextureNormal);
	Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, &m_TextureOver);
	Renderer::GetDeviceContext()->PSSetShaderResources(4, 1, &m_TextureOverNormal);
	// ���p�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(5, 1, &m_TextureFloor1);
	Renderer::GetDeviceContext()->PSSetShaderResources(6, 1, &m_TextureFloorNormal1);
	Renderer::GetDeviceContext()->PSSetShaderResources(7, 1, &m_TextureFloor2);
	Renderer::GetDeviceContext()->PSSetShaderResources(8, 1, &m_TextureFloorNormal2);
	// ���ŃZ�b�g���Ă��遫
	Renderer::GetDeviceContext()->PSSetShaderResources(9, 1, &m_TextureFloor3);
	Renderer::GetDeviceContext()->PSSetShaderResources(10, 1, &m_TextureFloorNormal3);
	Renderer::GetDeviceContext()->PSSetShaderResources(11, 1, &m_TextureFloor4);
	Renderer::GetDeviceContext()->PSSetShaderResources(12, 1, &m_TextureFloorNormal4);



	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��,�����ƈႤ��DrawIndexed�B�C���f�b�N�X���g���ĕ\��
	Renderer::GetDeviceContext()->DrawIndexed(((FieldSize + 2) * 2) * FieldSize - 2, 0, 0);
}

void MeshFieldObject::OverWriteSPMOD(StructPlacedMeshFieldObjectData& data)
{
	m_AreaBlock = data.sm_AreaBlock;
	m_Position = data.sm_Position;
	m_PosOffset = data.sm_PosOffset;
	m_TextureName = data.sm_TexName;
	m_UseShadow = data.sm_UseShadow;
	memcpy(&m_Vertex, &data.sm_Vertex, sizeof(data.sm_Vertex));	// memcpy�ŃR�s�[����B

	// ���݂�m_Vertex�Ńo�[�e�b�N�X�o�b�t�@������������B
	OverWriteVertexBuffer();
}



// ���݂̃f�[�^���A�Z�[�u�p�\���̂ɕϊ����ĕԂ��B
StructPlacedMeshFieldObjectData MeshFieldObject::GetStructPlacedMeshFieldObjectData()
{

	//StructPlacedMeshFieldObjectData re;
	//memcpy(re.sm_TexName, m_TextureName.c_str(), strlen(m_TextureName.c_str()) + 1);	// string�^->char[64]�֕ϊ�
	//re.sm_Position = m_Position;
	//re.sm_AreaBlock = m_AreaBlock;
	//re.sm_PosOffset = m_PosOffset;
	//re.sm_UseShadow = m_UseShadow;
	//memcpy(&re.sm_Vertex, &m_Vertex, sizeof(m_Vertex));	// memcpy�ŃR�s�[����B

	StructPlacedMeshFieldObjectData* re = new StructPlacedMeshFieldObjectData;
	memcpy(re->sm_TexName, m_TextureName.c_str(), strlen(m_TextureName.c_str()) + 1);	// string�^->char[64]�֕ϊ�
	re->sm_Position = m_Position;
	re->sm_AreaBlock = m_AreaBlock;
	re->sm_PosOffset = m_PosOffset;
	re->sm_UseShadow = m_UseShadow;
	memcpy(re->sm_Vertex, &m_Vertex, sizeof(m_Vertex));	// memcpy�ŃR�s�[����B

	return *re;
}



void MeshFieldObject::OverWriteVertexBuffer()
{


	// ���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// �S�Ă�m_Vertex�Œl�����邾����OK
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	return;
}





// �p�l���̃u���b�N�ԍ��Z�o�Btrue�ł��̃t�B�[���h�͈͓̔�
bool MeshFieldObject::GetPanelBlock(Int2* pOutInt2, const D3DXVECTOR3& Position)
{
	
	pOutInt2->x = (Position.x - m_PosOffset.x) / PanelSize;
	pOutInt2->y = (Position.z - m_PosOffset.z) / -PanelSize;

	// ���b�V���t�B�[���h���Ȃ������ꍇ-100.0f��Ԃ��B�������Ȃ��B
	if (pOutInt2->x < 0 || pOutInt2->y < 0 || pOutInt2->x >= FieldSize || pOutInt2->y >= FieldSize)
		return false;

	return true;
}


// �������̍��W��xz���g���āA�u���b�N���Z�o�����̃��b�V���ł̍��������߂�B
float MeshFieldObject::GetHeight(const D3DXVECTOR3& Position)
{
	Int2 panel;
	if (!GetPanelBlock(&panel, Position))
		return -100.0f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;


	// ���_�f�[�^��ǂ�

	pos0 = m_Vertex[(panel.x * (FieldSize + 1)) + panel.y].Position;
	pos1 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + panel.y].Position;
	pos2 = m_Vertex[(panel.x * (FieldSize + 1)) + (panel.y + 1)].Position;
	pos3 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + (panel.y + 1)].Position;
	// ���u���b�N�ԍ������܂����炻�̎����4���_�̍��W�����Ă���B


	D3DXVECTOR3 v12, v1p, c;
	v12 = pos2 - pos1;
	v1p = Position - pos1;

	// �O�p�`�|���S���̂ǂ����̎O�p�`�ɂ��邩�𔻒�
	// v12=�[��������[�����Bv1p=�[��������v���C���[
	// �O�ς͂����鏇�Ԃ��厖�ŁA���v��肾�ƕ\�����ɐ����ȃx�N�g�����o��A
	// �t���Ɨ������ɂȂ��Ă��܂��B�̂ŁA�O�p�`�|���S���̂ǂ��������ƕ\�����t���Ɨ��ނ��Ƀx�N�g�����o��
	D3DXVec3Cross(&c, &v12, &v1p);

	float py = 0.0;
	D3DXVECTOR3 n;
	// �O�ς̌��ʂ����Ȃ獶�̎O�p�`�|���S���̏�ɂ���B
	// �O�ς��g�����\�������3�����̓����蔻��ł悭�g���B
	if (c.y > 0.0f)
	{
		// ����|���S��
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);	// ���v�Z���t�ŗ��ނ��ɂȂ�������Ă�
	}
	else
	{
		// �E���|���S��
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	// �����擾
	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;


	return py;
}


// ���̃u���b�N�̃��b�V���̍����Ɩ@����Ԃ��B���������u���b�N�����b�V���͈̔͊O��������false��Ԃ��B
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


	// ���_�f�[�^��ǂ�

	pos0 = m_Vertex[(panel.x * (FieldSize + 1)) + panel.y].Position;
	pos1 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + panel.y].Position;
	pos2 = m_Vertex[(panel.x * (FieldSize + 1)) + (panel.y + 1)].Position;
	pos3 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + (panel.y + 1)].Position;
	// ���u���b�N�ԍ������܂����炻�̎����4���_�̍��W�����Ă���B


	D3DXVECTOR3 v12, v1p, c;
	v12 = pos2 - pos1;
	v1p = Position - pos1;

	// �O�p�`�|���S���̂ǂ����̎O�p�`�ɂ��邩�𔻒�
	// v12=�[��������[�����Bv1p=�[��������v���C���[
	// �O�ς͂����鏇�Ԃ��厖�ŁA���v��肾�ƕ\�����ɐ����ȃx�N�g�����o��A
	// �t���Ɨ������ɂȂ��Ă��܂��B�̂ŁA�O�p�`�|���S���̂ǂ��������ƕ\�����t���Ɨ��ނ��Ƀx�N�g�����o��
	D3DXVec3Cross(&c, &v12, &v1p);

	float py = 0.0;
	D3DXVECTOR3 normal;
	// �O�ς̌��ʂ����Ȃ獶�̎O�p�`�|���S���̏�ɂ���B
	// �O�ς��g�����\�������3�����̓����蔻��ł悭�g���B
	if (c.y > 0.0f)
	{
		// ����|���S��
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&normal, &v10, &v12);	// ���v�Z���t�ŗ��ނ��ɂȂ�������Ă�
	}
	else
	{
		// �E���|���S��
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&normal, &v12, &v13);
	}

	// �����擾
	py = -((Position.x - pos1.x) * normal.x + (Position.z - pos1.z) * normal.z) / normal.y + pos1.y;

	*pOut_height = py;
	*pOut_normal = normal;
	D3DXVec3Normalize(pOut_normal, pOut_normal);
	*pOut_normal *= -1;

	return true;
}



// �|�W�V�����̂Ƃ����Փ˂��Ă�����true��Ԃ��BPosition�́A���̈ʒu�ł��
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


	// ���_�f�[�^��ǂ�

	pos0 = m_Vertex[(panel.x * (FieldSize + 1)) + panel.y].Position;
	pos1 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + panel.y].Position;
	pos2 = m_Vertex[(panel.x * (FieldSize + 1)) + (panel.y + 1)].Position;
	pos3 = m_Vertex[((panel.x + 1) * (FieldSize + 1)) + (panel.y + 1)].Position;
	// ���u���b�N�ԍ������܂����炻�̎����4���_�̍��W�����Ă���B


	D3DXVECTOR3 v12, v1p, c;
	v12 = pos2 - pos1;
	v1p = Position - pos1;

	// �O�p�`�|���S���̂ǂ����̎O�p�`�ɂ��邩�𔻒�
	// v12=�[��������[�����Bv1p=�[��������v���C���[
	// �O�ς͂����鏇�Ԃ��厖�ŁA���v��肾�ƕ\�����ɐ����ȃx�N�g�����o��A
	// �t���Ɨ������ɂȂ��Ă��܂��B�̂ŁA�O�p�`�|���S���̂ǂ��������ƕ\�����t���Ɨ��ނ��Ƀx�N�g�����o��
	D3DXVec3Cross(&c, &v12, &v1p);

	float py = 0.0;
	D3DXVECTOR3 normal;
	// �O�ς̌��ʂ����Ȃ獶�̎O�p�`�|���S���̏�ɂ���B
	// �O�ς��g�����\�������3�����̓����蔻��ł悭�g���B
	if (c.y > 0.0f)
	{
		// ����|���S��
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&normal, &v10, &v12);	// ���v�Z���t�ŗ��ނ��ɂȂ�������Ă�
	}
	else
	{
		// �E���|���S��
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&normal, &v12, &v13);
	}

	// �����擾
	py = -((Position.x - pos1.x) * normal.x + (Position.z - pos1.z) * normal.z) / normal.y + pos1.y;

	*pOut_height = py;
	*pOut_normal = normal;
	D3DXVec3Normalize(pOut_normal, pOut_normal);
	*pOut_normal *= -1;



	if (pOut_normal->y < 0.55 - 0.10) 	// �R�@��
	{
		// �΂߂̏ꏊ��������
		if (Position.y < py)
		{
			// �n�ʂ�艺��������
			// �܂��Փ˒��O�̍��W�ɖ߂�
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
		// ���ʂɕ�����ꏊ��������
		if (Position.y < py)
		{
			// �n�ʂ�艺��������
			return 1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

// ���ݎg���Ă��Ȃ��B���g�̂̂܂܂�����Ԉ���Ă鑽��
//void MeshFieldObject::SetAreaBlock(Int2 areablock)
//{
//	m_AreaBlock = areablock;
//
//	// ���b�V���t�B�[���h��1�}�X�̑傫���Ƃ��ɂ���č��W���Z�b�g����
//	m_PosOffset.x = areablock.x * interval;
//	m_PosOffset.z = areablock.y * interval;
//
//	// ���_�o�b�t�@�Đ���
//	{
//
//		for (int x = 0; x <= FieldSize; x++)
//		{
//			for (int z = 0; z <= FieldSize; z++)
//			{
//				m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3((x - m_PosOffset.x) * PanelSize, -m_PosOffset.y, (z - m_PosOffset.z) * -PanelSize);
//				m_Vertex[(x * (FieldSize + 1)) + z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ���Ŗ@���x�N�g�������߂ăZ�b�g���Ă���
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
//				//vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;	// �I�_����n�_�������Ƃ��̒����̕����x�N�g�������߂���
//				//vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;	// 
//				vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// �I�_����n�_�������Ƃ��̒����̕����x�N�g�������߂���
//				vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 
//
//				D3DXVec3Cross(&vn, &vz, &vx);// �O�ρBvx�ɂ�vz�ɂ������ȃx�N�g�������܂�B
//				D3DXVec3Normalize(&vn, &vn);// ���K��(����1�ɂ���)
//				//m_Vertex[x][z].Normal = vn;
//				m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
//			}
//		}
//
//
//		// ���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
//		OverWriteVertexBuffer();
//	}
//	return;
//}



// �@�������݂̒��_���W���g���čX�V����
void MeshFieldObject::UpdataMeshFieldObjectNormal()
{

	//���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
	// Map�֐����g���ƁA���̃f�[�^�͑S��������̂ŁA�S�č�蒼���A�V���ɒl��ݒ肷��K�v������B
	// �S���������Ⴄ�̂ŁA���̂��߂�m_Vertex��p�ӂ��Ă���B
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// �@���x�N�g���Z�oNormal���v�Z�ŋ��߂�
	// x�����ׂ̗�2�_���Ȃ��x�N�g����
	// z�����ׂ̗�2�_���Ȃ��x�N�g����
	// �O�ς��Ă�����΁A����炵���x�N�g�����o�邩�ȁ`
	for (int x = 1; x <= (FieldSize - 1); x++)
	{
		for (int z = 1; z <= (FieldSize - 1); z++)
		{
			D3DXVECTOR3 vx, vz, vn;
			vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// �I�_����n�_�������Ƃ��̒����̕����x�N�g�������߂���
			vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 

			//D3DXVec3Cross(&vn, &vx, &vz);// �O�ρBvx�ɂ�vz�ɂ������ȃx�N�g�������܂�B�����鏇�ԂŌ������^�t�ɕς��B
			D3DXVec3Cross(&vn, &vz, &vx);// �O�ρBvx�ɂ�vz�ɂ������ȃx�N�g�������܂�B�����鏇�ԂŌ������^�t�ɕς��B
			D3DXVec3Normalize(&vn, &vn);// ���K��(����1�ɂ���)
			m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
		}
	}

	// �@���v�Z��ɑS�Ă�m_Vertex�Œu��������
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}








///////////////////////////// �u���V�Z�b�e�B���O////////////////////////////////////////
///////////////////////////// �X�J���v�g�u���V - �u���V�Z�b�e�B���O//////////////////////////////////////////
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

	// �܂�y�͍l������xz�Ńu���b�N�ŕ����čl����B���S�̃u���b�N���������߂�B

	Int2 panel;
	if (!GetPanelBlock(&panel, ppassedsculptbrushstate->Position))
		return;


	// �����܂łŒ��S�̃u���b�N�����܂��Ă���B

	// �l�p�ōl���āA���̂��Ƃɂ��̃u���b�N�̒��_�����͈͓̔������l����B
	int panelradius, minx, minz, maxx, maxz;
	panelradius = ppassedsculptbrushstate->Radius / PanelSize;		// �����ōl�������ɔ��a���ɉ����̃p�l�������邩
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

	// �ύX���钸�_������z��ō�����Ⴄ
	std::list<int> verticesNum;

	// �l�p�͈͓̔��̃u���b�N�̎����4���_��ǉ����Ă���
	for (int xx = minx; xx <= maxx; xx++)
	{
		for (int zz = minz; zz <= maxz; zz++)
		{
			// ����̂S���_��ǉ�
			verticesNum.push_back((xx * (FieldSize + 1)) + zz);
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + zz);
			verticesNum.push_back((xx * (FieldSize + 1)) + (zz + 1));
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + (zz + 1));
		}
	}

	//// ����̂S���_��ǉ�
	//verticesNum.push_back((x * (FieldSize + 1)) + z);
	//verticesNum.push_back(((x + 1) * (FieldSize + 1)) + z);
	//verticesNum.push_back((x * (FieldSize + 1)) + (z + 1));
	//verticesNum.push_back(((x + 1) * (FieldSize + 1)) + (z + 1));

	// ���_�̏d����r��
	verticesNum.unique();

	// ���_�ƒ��S�̋����ŋ��̒������v�Z����
	std::list<int> NewverticesNum;
	for (auto vertexNum : verticesNum)
	{
		// ���a���f�J�������珜�O�B�Ȃ񂾂��ǂ��̂܂ܔ��a�g���Ƃ��т��тɂȂ邩��኱�����邭����
		D3DXVECTOR3 distance = ppassedsculptbrushstate->Position - m_Vertex[vertexNum].Position;
		float length = D3DXVec3LengthSq(&distance);
		if (length > pow((ppassedsculptbrushstate->Radius * 1.3f), 2))
			continue;

		NewverticesNum.push_back(vertexNum);
	}



	//���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
	// Map�֐����g���ƁA���̃f�[�^�͑S��������̂ŁA�S�č�蒼���A�V���ɒl��ݒ肷��K�v������B
	// �S���������Ⴄ�̂ŁA���̂��߂�m_Vertex��p�ӂ��Ă���B
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// �܂��S�Ă�m_Vertex�Œl������B���̂��߂�m_Vertex�B
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{
		// ���_�̈ʒu��ύX�Bm_Vertex�̒l�������ɍX�V����
		m_Vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * StrengthMagnification;
		vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * StrengthMagnification;
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}

void MeshFieldObject::UseSculptBrush002(const PassedSculptBrushState* ppassedsculptbrushstate)
{
	float StrengthMagnification = 0.05f;
	// �܂�y�͍l������xz�Ńu���b�N�ŕ����čl����B���S�̃u���b�N���������߂�B

	Int2 panel;
	if (!GetPanelBlock(&panel, ppassedsculptbrushstate->Position))
		return;


	// �����܂łŒ��S�̃u���b�N�����܂��Ă���B

	// �l�p�ōl���āA���̂��Ƃɂ��̃u���b�N�̒��_�����͈͓̔������l����B
	int panelradius, minx, minz, maxx, maxz;
	panelradius = ppassedsculptbrushstate->Radius / PanelSize;		// �����ōl�������ɔ��a���ɉ����̃p�l�������邩
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

	// �ύX���钸�_������z��ō�����Ⴄ
	std::list<int> verticesNum;

	// �l�p�͈͓̔��̃u���b�N�̎����4���_��ǉ����Ă���
	for (int xx = minx; xx <= maxx; xx++)
	{
		for (int zz = minz; zz <= maxz; zz++)
		{
			// ����̂S���_��ǉ�
			verticesNum.push_back((xx * (FieldSize + 1)) + zz);
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + zz);
			verticesNum.push_back((xx * (FieldSize + 1)) + (zz + 1));
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + (zz + 1));
		}
	}

	// ���_�̏d����r���Bunique�֐��̓\�[�g����Ă��Ȃ��Ǝg���Ȃ��B
	verticesNum.sort();
	verticesNum.unique();

	// ���_�ƒ��S�̋����ŋ��̒������v�Z����
	std::list<int> NewverticesNum;
	for (auto vertexNum : verticesNum)
	{
		if (m_Vertex[vertexNum].Position.y > ppassedsculptbrushstate->Strength + ppassedsculptbrushstate->TriggerPosition.y)
		{
			//if (m_Vertex[vertexNum].Position.y < 0.8f)
			//	int gg = 0;
			continue;
		}

		//// �܂�y�����ꂷ���Ă����珜�O�B������0.2�{�ȏゾ������_���Bvalue * 0.2f
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



	//���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
	// Map�֐����g���ƁA���̃f�[�^�͑S��������̂ŁA�S�č�蒼���A�V���ɒl��ݒ肷��K�v������B
	// �S���������Ⴄ�̂ŁA���̂��߂�m_Vertex��p�ӂ��Ă���B
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// �܂��S�Ă�m_Vertex�Œl������B���̂��߂�m_Vertex�B
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{
		if (m_Vertex[vertexNum].Position.y >= 0.2f)
			int gtg = 0;//�`�F�b�N�p


		// ���_�̈ʒu��ύX�Bm_Vertex�̒l�������ɍX�V����
		m_Vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * StrengthMagnification;
		vertex[vertexNum].Position.y += ppassedsculptbrushstate->Strength * StrengthMagnification;
	}

	// ���_�̈ʒu��ύX�Bm_Vertex�̒l�������ɍX�V����
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

// �g���K�[�|�C���g�Ɋ񂹂�u���V
void MeshFieldObject::UseSculptBrush003(const PassedSculptBrushState* ppassedsculptbrushstate)
{
	// �܂�y�͍l������xz�Ńu���b�N�ŕ����čl����B���S�̃u���b�N���������߂�B

	Int2 panel;
	if (!GetPanelBlock(&panel, ppassedsculptbrushstate->Position))
		return;


	// �����܂łŒ��S�̃u���b�N�����܂��Ă���B

	// �l�p�ōl���āA���̂��Ƃɂ��̃u���b�N�̒��_�����͈͓̔������l����B
	int panelradius, minx, minz, maxx, maxz;
	panelradius = ppassedsculptbrushstate->Radius / PanelSize;		// �����ōl�������ɔ��a���ɉ����̃p�l�������邩
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

	// �ύX���钸�_������z��ō�����Ⴄ
	std::list<int> verticesNum;

	// �l�p�͈͓̔��̃u���b�N�̎����4���_��ǉ����Ă���
	for (int xx = minx; xx <= maxx; xx++)
	{
		for (int zz = minz; zz <= maxz; zz++)
		{
			// ����̂S���_��ǉ�
			verticesNum.push_back((xx * (FieldSize + 1)) + zz);
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + zz);
			verticesNum.push_back((xx * (FieldSize + 1)) + (zz + 1));
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + (zz + 1));
		}
	}

	// ���_�̏d����r���Bunique�֐��̓\�[�g����Ă��Ȃ��Ǝg���Ȃ��B
	verticesNum.sort();
	verticesNum.unique();

	// ���_�ƒ��S�̋����ŋ��̒������v�Z����
	std::list<int> NewverticesNum;
	for (auto vertexNum : verticesNum)
	{
		// y�𖳎�����
		D3DXVECTOR3 PP = D3DXVECTOR3(ppassedsculptbrushstate->Position.x, 0.0f, ppassedsculptbrushstate->Position.z);
		D3DXVECTOR3 VP = D3DXVECTOR3(m_Vertex[vertexNum].Position.x, 0.0f, m_Vertex[vertexNum].Position.z);
		// ���a���f�J�������珜�O�B
		D3DXVECTOR3 distance = PP - VP;
		float lengthsq = D3DXVec3LengthSq(&distance);
		if (lengthsq > pow((ppassedsculptbrushstate->Radius * 1.0f), 2))
			continue;

		NewverticesNum.push_back(vertexNum);
	}



	//���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
	// Map�֐����g���ƁA���̃f�[�^�͑S��������̂ŁA�S�č�蒼���A�V���ɒl��ݒ肷��K�v������B
	// �S���������Ⴄ�̂ŁA���̂��߂�m_Vertex��p�ӂ��Ă���B
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// �܂��S�Ă�m_Vertex�Œl������B���̂��߂�m_Vertex�B
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{

		// ���_�̈ʒu��ύX�Bm_Vertex�̒l�������ɍX�V����
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
	// ���g�p
	return;
}

///////////////////////////// �y�C���g�u���V - �u���V�Z�b�e�B���O//////////////////////////////////////////
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
//	// �܂�y�͍l������xz�Ńu���b�N�ŕ����čl����B���S�̃u���b�N���������߂�B
//
//	Int2 panel;
//	if (!GetPanelBlock(&panel, ppassedpaintbrushstate->Position))
//		return;
//
//
//	// �����܂łŒ��S�̃u���b�N�����܂��Ă���B
//
//	// �l�p�ōl���āA���̂��Ƃɂ��̃u���b�N�̒��_�����͈͓̔������l����B
//	int panelradius, minx, minz, maxx, maxz;
//	panelradius = ppassedpaintbrushstate->Radius / PanelSize;		// �����ōl�������ɔ��a���ɉ����̃p�l�������邩
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
//	// �ύX���钸�_������z��ō�����Ⴄ
//	std::list<int> verticesNum;
//
//	// �l�p�͈͓̔��̃u���b�N�̎����4���_��ǉ����Ă���
//	for (int xx = minx; xx <= maxx; xx++)
//	{
//		for (int zz = minz; zz <= maxz; zz++)
//		{
//			// ����̂S���_��ǉ�
//			verticesNum.push_back((xx * (FieldSize + 1)) + zz);
//			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + zz);
//			verticesNum.push_back((xx * (FieldSize + 1)) + (zz + 1));
//			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + (zz + 1));
//		}
//	}
//
//	// ���_�̏d����r���Bunique�֐��̓\�[�g����Ă��Ȃ��Ǝg���Ȃ��B
//	verticesNum.sort();
//	verticesNum.unique();
//
//	// ���_�ƒ��S�̋����ŋ��̒������v�Z����
//	std::list<int> NewverticesNum;
//	for (auto vertexNum : verticesNum)
//	{
//		if (m_Vertex[vertexNum].Position.y > 0.1f)
//		{
//			continue;
//		}
//
//		//// �܂�y�����ꂷ���Ă����珜�O�B������0.2�{�ȏゾ������_���Bvalue * 0.2f
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
//	//���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
//	// Map�֐����g���ƁA���̃f�[�^�͑S��������̂ŁA�S�č�蒼���A�V���ɒl��ݒ肷��K�v������B
//	// �S���������Ⴄ�̂ŁA���̂��߂�m_Vertex��p�ӂ��Ă���B
//	D3D11_MAPPED_SUBRESOURCE msr;
//	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
//
//	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;
//
//	// �܂��S�Ă�m_Vertex�Œl������B���̂��߂�m_Vertex�B
//	memcpy(vertex, m_Vertex, sizeof(m_Vertex));
//
//	for (auto vertexNum : NewverticesNum)
//	{
//		// ���_�̈ʒu��ύX�Bm_Vertex�̒l�������ɍX�V����
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


// brushnum��PaintBrushSetMax-1��n�����Ƃŏ����S�������ɂȂ�
// ���݃u���V���X�g0�̏ꍇ�݂̂����l�����Ă��Ȃ�
void MeshFieldObject::UsePaintBrush(const int listnum, const int brushnum, PassedPaintBrushState* ppassedpaintbrushstate)
{
	// �܂�y�͍l������xz�Ńu���b�N�ŕ����čl����B���S�̃u���b�N���������߂�B

	Int2 panel;
	if (!GetPanelBlock(&panel, ppassedpaintbrushstate->Position))
		return;


	// �����܂łŒ��S�̃u���b�N�����܂��Ă���B

	// �l�p�ōl���āA���̂��Ƃɂ��̃u���b�N�̒��_�����͈͓̔������l����B
	int panelradius, minx, minz, maxx, maxz;
	panelradius = ppassedpaintbrushstate->Radius / PanelSize;		// �����ōl�������ɔ��a���ɉ����̃p�l�������邩
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

	// �ύX���钸�_������z��ō�����Ⴄ
	std::list<int> verticesNum;

	// �l�p�͈͓̔��̃u���b�N�̎����4���_��ǉ����Ă���
	for (int xx = minx; xx <= maxx; xx++)
	{
		for (int zz = minz; zz <= maxz; zz++)
		{
			// ����̂S���_��ǉ�
			verticesNum.push_back((xx * (FieldSize + 1)) + zz);
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + zz);
			verticesNum.push_back((xx * (FieldSize + 1)) + (zz + 1));
			verticesNum.push_back(((xx + 1) * (FieldSize + 1)) + (zz + 1));
		}
	}

	// ���_�̏d����r���Bunique�֐��̓\�[�g����Ă��Ȃ��Ǝg���Ȃ��B
	verticesNum.sort();
	verticesNum.unique();

	// ���_�ƒ��S�̋����ŋ��̒������v�Z����
	std::list<int> NewverticesNum;
	std::list<float> NewverticesNumDistanceWariai;	// �����̊����p�B�߂��ق�0.0~�����ق�~1.0
	for (auto vertexNum : verticesNum)
	{
		// ���a���f�J�������珜�O�B
		D3DXVECTOR3 distance = ppassedpaintbrushstate->Position - m_Vertex[vertexNum].Position;
		float lengthsq = D3DXVec3LengthSq(&distance);
		float radiussq = pow((ppassedpaintbrushstate->Radius * 1.0f), 2);
		if (lengthsq > radiussq)
			continue;

		float wariai = 1.0f - (lengthsq / radiussq);

		NewverticesNum.push_back(vertexNum);
		NewverticesNumDistanceWariai.push_back(wariai);
	}



	//���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
	// Map�֐����g���ƁA���̃f�[�^�͑S��������̂ŁA�S�č�蒼���A�V���ɒl��ݒ肷��K�v������B
	// �S���������Ⴄ�̂ŁA���̂��߂�m_Vertex��p�ӂ��Ă���B
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D_FLOOR* vertex = (VERTEX_3D_FLOOR*)msr.pData;

	// �܂��S�Ă�m_Vertex�Œl������B���̂��߂�m_Vertex�B
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));


	auto wariaiItr = NewverticesNumDistanceWariai.begin();
	for (auto vertexNum : NewverticesNum)
	{
		// �����S���������猻�݂̃u���V�Z�b�g�̑S�Ă̊�����������
		if (brushnum == PaintBrushSetMax - 1)
		{
			// �t���A�ς���
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

		// �A���t�@�͋����Ƌ����ɉ����ĕς���B�X�V����̂��A���t�@�l��������������X�V����
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
			// �t���A�ς���
			if (m_Vertex[vertexNum].FloorAlfa0.w < *wariaiItr)
			{
				m_Vertex[vertexNum].FloorAlfa0.w = *wariaiItr;
			}
			break;
		default:
			break;
		}

		// �����Ď����ȊO��������
		float Down = *wariaiItr * 0.1f;
		DownOtherVertexAlfa(brushnum, Down, vertexNum);

		vertex[vertexNum].FloorAlfa0.x = m_Vertex[vertexNum].FloorAlfa0.x;
		vertex[vertexNum].FloorAlfa0.y = m_Vertex[vertexNum].FloorAlfa0.y;
		vertex[vertexNum].FloorAlfa0.z = m_Vertex[vertexNum].FloorAlfa0.z;
		// �t���A�ς���
		vertex[vertexNum].FloorAlfa0.w = m_Vertex[vertexNum].FloorAlfa0.w;

		// FloorTexture0.x�Ɍ��݂̃u���V���Z�b�g����B����ōŌ�Ɏg�����u���V�ŏ㏑���ɂȂ�B
		m_Vertex[vertexNum].FloorTexture0.x = brushnum;
		vertex[vertexNum].FloorTexture0.x = brushnum;


		++wariaiItr;
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}

// �����ȊO�̂R�̒��_�̃A���t�@��������B�Z�b�g���X�g��0�ɂ����Ή����ĂȂ�
void MeshFieldObject::DownOtherVertexAlfa(const int selfnum, const float downvalue, const int vertexnum)
{
	// �t���A�ς���
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