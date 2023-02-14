// �萔�o�b�t�@�i�p��FConstant Buffer�A�ʏ́Fcbuffer�j�Ƃ́ACPU�œ����v���O�����̕ϐ��Q���A
// GPU�œ����v���O���}�u���V�F�[�_�[�ɒ萔�Q�i�o�b�t�@�j�Ƃ��ăh�X���Ƒ��荞�ދ@�\�������B
// 
// �萔�o�b�t�@(Constant Buffer)���g���Ƃ���cbuffer�L�[���[�h�g���A�\���̂̂悤�ɐ錾���܂��B 
// ���ƁA�A���I�[�_�[�h�A�N�Z�X�r���[(�ȉ��AUAV)�Ɠ����ŃX���b�g�ԍ����w��\�ł��B 
// �萔�o�b�t�@�̏ꍇ�́hb0�h�̂悤�ɐړ����b�����Ďw�肵�Ă��������B
// 
// �萔�o�b�t�@�Ƃ��Đ錾���ꂽ�ϐ��̓V�F�[�_���̂ǂ�����ł��g�����Ƃ��o���܂��B 
// ����΁Ac++�̃O���[�o���ϐ��̂悤�Ȃ��̂ł��B �O���[�o���ϐ��Ƃ̈Ⴂ�͑�����ł��Ȃ��Ƃ��������Ȃ̂ŁA
// const�錾���ꂽ�O���[�o���ϐ����Ƃ����F���ł����ł��傤�B


//���̃t�@�C���͑��̃V�F�[�_�[�t�@�C���փC���N���[�h����܂�
// �e��}�g���N�X���󂯎��ϐ���p��
// renderer.cpp��215�s�ڂ��炢�Ŏg�����������Ă���
cbuffer WorldBuffer : register(b0)//�萔�o�b�t�@ 0 ��
{
	matrix World;
}
cbuffer ViewBuffer : register(b1) //�萔�o�b�t�@ 1 ��
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2) //�萔�o�b�t�@ 2 ��
{
	matrix Projection;
}
cbuffer CameraBuffer : register(b5) //�o�b�t�@�̂T�ԂƂ���
{
	float4 CameraPosition; //�J�������W���󂯂Ƃ�ϐ�
}
cbuffer ParameterBuffer : register(b6)
{
	float4 Parameter;
}

// �F���������邽�߂̐F�̃o�b�t�@
cbuffer SynthesizeColorBuffer : register(b7)
{
	float4 SynthesizeColor;
}

//���_�V�F�[�_�[�֓��͂����f�[�^���\���̂̌`�ŕ\��
//����͒��_�o�b�t�@�̓��e���̂���
struct VS_IN
{
	float4 Position : POSITION0; //�Z�}���e�B�N�X�͒��_���C�A�E�g�ɑΉ�
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
};
//�s�N�Z���V�F�[�_�[�֓��͂����f�[�^���\���̂̌`�ŕ\��
struct PS_IN
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4 ShadowPosition : POSITION1; //���C�g�J�������猩���s�N�Z���̍��W
};

// Floor�p
struct VS_FLOOR_IN
{
	float4 Position : POSITION0; //�Z�}���e�B�N�X�͒��_���C�A�E�g�ɑΉ�
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;

	float4  FloorTexture0 : FLOORTEX;	// 1��̕`�ʂ�4�܂ł̃e�N�X�`�����u�����h�ł���悤�ɂ��Ă���
	float4  FloorAlfa0 : FLOORALFA;
};
struct PS_FLOOR_IN
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4 ShadowPosition : POSITION1; //���C�g�J�������猩���s�N�Z���̍��W

	float4  FloorTexture0 : FLOORTEX;
	float4  FloorAlfa0 : FLOORALFA;
};


struct VS_FLOOR2_IN
{
	float4 Position : POSITION0; //�Z�}���e�B�N�X�͒��_���C�A�E�g�ɑΉ�
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4  FloorTexture0 : FLOORTEX;	// 1��̕`�ʂ�4�܂ł̃e�N�X�`�����u�����h�ł���悤�ɂ��Ă���
	float4  FloorAlfa0 : FLOORALFA;
};
struct PS_FLOOR2_IN
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4 ShadowPosition : POSITION1; //���C�g�J�������猩���s�N�Z���̍��W
	float4  FloorTexture0 : FLOORTEX;
	float4  FloorAlfa0 : FLOORALFA;
};

//���C�g�I�u�W�F�N�g�\���̂ƃR���X�^���g�o�b�t�@
struct LIGHT
{
	bool  Enable;
	bool3 Dummy; //�z�u�A�h���X��4�̔{���ɂ���ƌ����������̂Œ����p
	float4 Direction; //����C����ł�������VisualStudio������Ă���Ă���B
	float4 Diffuse;
	float4 Ambient;

	matrix View; //���C�g�J�����̃r���[�s��
	matrix Projection; //���C�g�J�����̃v���W�F�N�V�����s��

};
cbuffer LightBuffer : register(b4)//�R���X�^���g�o�b�t�@�S�ԂƂ���
{
	LIGHT Light; //���C�g�\����
}




/////////////////////EOF//////////////////////
