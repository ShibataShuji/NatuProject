#pragma once


struct VERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR2 TexCoord;
};

struct VERTEX_3D_FLOOR
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR2 TexCoord;
	D3DXVECTOR4	FloorTexture0;
	D3DXVECTOR4	FloorAlfa0;
};

struct VERTEX_3D_FLOOR2
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR2 TexCoord;
	D3DXVECTOR3	FloorTexture0;
	D3DXVECTOR3	FloorAlfa0;
};



struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;

	D3DXMATRIX ViewMatrix;			// ���C�g�J�����s��
	D3DXMATRIX ProjectionMatrix;	// ���C�g�v���W�F�N�V�����s��
};