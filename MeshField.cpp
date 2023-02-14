
#include "stdafx.h"

//#include <iostream>
//#include <string>
//#include <vector>
//#include <list>
//#include <array>
//#include <random>
//
//#include "gameObject.h"
//#include "main.h"
//#include "MeshField.h"
//#include "renderer.h"
//#include "primitive.h"
//#include <D3D11.h>
//#include <D3DX11.h>
//#include <cassert>


// array�^�͗v�f�����ȗ��ł��Ȃ��Bvector�^�͏ȗ��ł���
std::array<std::array<float, FieldSize + 1>, FieldSize + 1> g_fieldHeight;



float aaag_fieldHeight[FieldSize + 1][FieldSize + 1]
{
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 4.0f, 4.0f, 4.0f, 4.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, },
	//{10.0f, 1.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, },

	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
	{10.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, },
};


uint64_t get_rand_range(uint64_t min_val, uint64_t max_val)
{
	// ����������
	static std::mt19937_64 mt64(0);

	// [min_val, max_val] �̈�l���z���� (int) �̕��z������
	std::uniform_int_distribution<uint64_t> get_rand_uni_int(min_val, max_val);

	// �����𐶐�
	return get_rand_uni_int(mt64);

}

double get_rand_range_double(double min_val, double max_val)
{

	static std::mt19937_64 aaa(0);
	std::uniform_real_distribution<double> get_rand_uni_real(min_val, max_val);
	return get_rand_uni_real(aaa);

}


void MeshField::Init()
{
	m_UseShadow = true;
	//// ���b�V���ŃX�e�[�W�̂ڂ��ڂ��̐ݒ�
	//for (int x = 0; x < FieldSize + 1; x++)
	//{
	//	for (int z = 0; z < FieldSize + 1; z++)
	//	{
	//		double aaa = get_rand_range_double(0.0, 5.0);
	//		g_fieldHeight[x][z] = (float)aaa;
	//	}
	//}

	// ���_�o�b�t�@����
	{

		for (int x = 0; x <= FieldSize; x++)
		{
			for (int z = 0; z <= FieldSize; z++)
			{
				float y = g_fieldHeight[x][z];

				m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3((x - m_PosOffset.x) * PanelSize, -m_PosOffset.y, (z - m_PosOffset.z) * -PanelSize);
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ���Ŗ@���x�N�g�������߂ăZ�b�g���Ă���
				m_Vertex[(x * (FieldSize + 1)) + z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[(x * (FieldSize + 1)) + z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
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
				//vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;	// �I�_����n�_�������Ƃ��̒����̕����x�N�g�������߂���
				//vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;	// 
				vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// �I�_����n�_�������Ƃ��̒����̕����x�N�g�������߂���
				vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 

				D3DXVec3Cross(&vn, &vz, &vx);// �O�ρBvx�ɂ�vz�ɂ������ȃx�N�g�������܂�B
				D3DXVec3Normalize(&vn, &vn);// ���K��(����1�ɂ���)
				//m_Vertex[x][z].Normal = vn;
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
			}
		}

		//���_�o�b�t�@����
		// ���_�f�[�^���p�ӂł����̂ŁA�_�C���N�g�G�b�N�X�p�̒��_�o�b�t�@������Ă����B
		//D3D11_BUFFER_DESC bd{};

		//ZeroMemory(&bd, sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(VERTEX_3D) * (FieldSize + 1) * (FieldSize + 1);
		//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//bd.CPUAccessFlags = 0;
		//
		// ���_�o�b�t�@����(���_������ŕς�����悤�ɂ��Ă���)
		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * (FieldSize + 1) * (FieldSize + 1);
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
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/field001.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	// �V�F�[�_�[�̓ǂݍ���
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "DepthShadowMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "DepthShadowMappingPS.cso");

	//// �����̃|�W�V��������
	//m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

}

void MeshField::Uninit() 
{

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void MeshField::Update() 
{


}

void MeshField::Draw() 
{

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���b�N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);


	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
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

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��,�����ƈႤ��DrawIndexed�B�C���f�b�N�X���g���ĕ\��
	Renderer::GetDeviceContext()->DrawIndexed(((FieldSize + 2) * 2) * FieldSize - 2, 0, 0);
}

// �������̍��W��xz���g���āA�u���b�N���Z�o�����̃��b�V���ł̍��������߂�B
float MeshField::GetHeight(D3DXVECTOR3 const &Position)
{
	int x, z;

	// �u���b�N�ԍ��Z�o�B�����͂��̃��b�V���t�B�[���h�̎��_�̈ʒu���I�t�Z�b�g�ǉ����Ȃ��Ƃ����Ȃ�
	x = Position.x / PanelSize + m_PosOffset.x;
	z = Position.z / -PanelSize + m_PosOffset.z;

	// ���b�V���t�B�[���h���Ȃ������ꍇ-100.0f��Ԃ��B�������Ȃ��B
	if (x < 0 || z < 0 || x >= FieldSize || z >= FieldSize)
		return -100.0f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;


	// ���_�f�[�^��ǂ�
	
	pos0 = m_Vertex[(x * (FieldSize + 1)) + z].Position;
	pos1 = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position;
	pos2 = m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;
	pos3 = m_Vertex[((x + 1) * (FieldSize + 1)) + (z + 1)].Position;
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


void MeshField::SetAreaBlock(Int2 areablock)
{
	m_AreaBlock = areablock;

	// ���b�V���t�B�[���h��1�}�X�̑傫���Ƃ��ɂ���č��W���Z�b�g����
	m_PosOffset.x = areablock.x * interval;
	m_PosOffset.z = areablock.y * interval;

	// ���_�o�b�t�@�Đ���
	{

		for (int x = 0; x <= FieldSize; x++)
		{
			for (int z = 0; z <= FieldSize; z++)
			{
				float y = g_fieldHeight[x][z];

				m_Vertex[(x * (FieldSize + 1)) + z].Position = D3DXVECTOR3((x - m_PosOffset.x) * PanelSize, -m_PosOffset.y, (z - m_PosOffset.z) * -PanelSize);
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ���Ŗ@���x�N�g�������߂ăZ�b�g���Ă���
				m_Vertex[(x * (FieldSize + 1)) + z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[(x * (FieldSize + 1)) + z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}


		for (int x = 1; x <= (FieldSize - 1); x++)
		{
			for (int z = 1; z <= (FieldSize - 1); z++)
			{
				D3DXVECTOR3 vx, vz, vn;
				//vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;	// �I�_����n�_�������Ƃ��̒����̕����x�N�g�������߂���
				//vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;	// 
				vx = m_Vertex[((x + 1) * (FieldSize + 1)) + z].Position - m_Vertex[((x - 1) * (FieldSize + 1)) + z].Position;	// �I�_����n�_�������Ƃ��̒����̕����x�N�g�������߂���
				vz = m_Vertex[(x * (FieldSize + 1)) + (z - 1)].Position - m_Vertex[(x * (FieldSize + 1)) + (z + 1)].Position;	// 

				D3DXVec3Cross(&vn, &vz, &vx);// �O�ρBvx�ɂ�vz�ɂ������ȃx�N�g�������܂�B
				D3DXVec3Normalize(&vn, &vn);// ���K��(����1�ɂ���)
				//m_Vertex[x][z].Normal = vn;
				m_Vertex[(x * (FieldSize + 1)) + z].Normal = vn;
			}
		}


		// ���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			// �S�Ă�m_Vertex�Œl�����邾����OK
			memcpy(vertex, m_Vertex, sizeof(m_Vertex));

			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
		}
	}
	return;
}



///////////////////////////// �u���V�Z�b�e�B���O////////////////////////////////////////


// ��ʓI�ȃu���V�B���̒��̒��_��y���W��ω�������
void MeshField::UseSculptBrush001(D3DXVECTOR3 const &Position, float const &radius, float const &value)
{
	// �܂�y�͍l������xz�Ńu���b�N�ŕ����čl����B���S�̃u���b�N���������߂�B

	int x, z;

	// �u���b�N�ԍ��Z�o�B�����͂��̃��b�V���t�B�[���h�̎��_�̈ʒu���I�t�Z�b�g�ǉ����Ȃ��Ƃ����Ȃ�
	x = Position.x / PanelSize + m_PosOffset.x;
	z = Position.z / -PanelSize + m_PosOffset.z;

	// �������b�V���t�B�[���h�̊O�������炱���ŏI��
	if (x < 0 || z < 0 || x >= FieldSize || z >= FieldSize)
		return;

	// �����܂łŒ��S�̃u���b�N�����܂��Ă���B

	// �l�p�ōl���āA���̂��Ƃɂ��̃u���b�N�̒��_�����͈͓̔������l����B
	int panelradius, minx, minz, maxx, maxz;
	panelradius = radius / PanelSize;		// �����ōl�������ɔ��a���ɉ����̃p�l�������邩
	minx = x - panelradius;
	minz = z - panelradius;
	maxx = x + panelradius;
	maxz = z + panelradius;
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
	for (int xx = minx; xx <= maxx; xx++ )
	{
		for (int zz = minz; zz <= maxz; zz++ )
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
		D3DXVECTOR3 distance = Position - m_Vertex[vertexNum].Position;
		float length = D3DXVec3LengthSq(&distance);
		if (length > pow((radius * 1.3f), 2))
			continue;

		NewverticesNum.push_back(vertexNum);
	}



	//���_�f�[�^���������B���������̂��߂̊֐�->Map�֐����g���B
	// Map�֐����g���ƁA���̃f�[�^�͑S��������̂ŁA�S�č�蒼���A�V���ɒl��ݒ肷��K�v������B
	// �S���������Ⴄ�̂ŁA���̂��߂�m_Vertex��p�ӂ��Ă���B
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// �܂��S�Ă�m_Vertex�Œl������B���̂��߂�m_Vertex�B
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{
		// ���_�̈ʒu��ύX�Bm_Vertex�̒l�������ɍX�V����
		m_Vertex[vertexNum].Position.y += value;
		vertex[vertexNum].Position.y += value;
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	return;
}


// ��ʓI�ȃu���V�B���̒��̒��_��y���W��ω�������
void MeshField::UseSculptBrush002(D3DXVECTOR3 const& TriggerPosition, D3DXVECTOR3 const& Position, float const& radius, float const& value)
{
	// �܂�y�͍l������xz�Ńu���b�N�ŕ����čl����B���S�̃u���b�N���������߂�B

	int x, z;

	// �u���b�N�ԍ��Z�o�B�����͂��̃��b�V���t�B�[���h�̎��_�̈ʒu���I�t�Z�b�g�ǉ����Ȃ��Ƃ����Ȃ�
	x = Position.x / PanelSize + m_PosOffset.x;
	z = Position.z / -PanelSize + m_PosOffset.z;

	// �������b�V���t�B�[���h�̊O�������炱���ŏI��
	if (x < 0 || z < 0 || x >= FieldSize || z >= FieldSize)
		return;

	// �����܂łŒ��S�̃u���b�N�����܂��Ă���B

	// �l�p�ōl���āA���̂��Ƃɂ��̃u���b�N�̒��_�����͈͓̔������l����B
	int panelradius, minx, minz, maxx, maxz;
	panelradius = radius / PanelSize;		// �����ōl�������ɔ��a���ɉ����̃p�l�������邩
	minx = x - panelradius;
	minz = z - panelradius;
	maxx = x + panelradius;
	maxz = z + panelradius;
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
		if (m_Vertex[vertexNum].Position.y > 0.1f)
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

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// �܂��S�Ă�m_Vertex�Œl������B���̂��߂�m_Vertex�B
	memcpy(vertex, m_Vertex, sizeof(m_Vertex));

	for (auto vertexNum : NewverticesNum)
	{
		if (m_Vertex[vertexNum].Position.y >= 0.2f)
			int gtg = 0;//�`�F�b�N�p


		// ���_�̈ʒu��ύX�Bm_Vertex�̒l�������ɍX�V����
		m_Vertex[vertexNum].Position.y += value;
		vertex[vertexNum].Position.y += value;
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