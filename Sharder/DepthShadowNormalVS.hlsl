
// �V���h�E�}�b�v+�@���}�b�v
// ���݃��b�V���t�B�[���h�I�u�W�F�N�g�����Ŏg���Ă���
#include "common.hlsl"
//// �t���A�ς���
//void main(in VS_FLOOR_IN In, out PS_FLOOR_IN Out)
void main(in VS_FLOOR_IN In, out PS_FLOOR_IN Out)
{
	matrix wvp; //�����̏�������
	wvp = mul(World, View); //1�p�X�ڂ̓��C�g�J�����̍s��
	wvp = mul(wvp, Projection); //2�p�X�ڂ͒ʏ�J�����̍s�񂪐ݒ肳���
	float4 worldNormal, normal; //�@���̃��[���h�ϊ�
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	//��������
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);
	Out.Diffuse = light;
	Out.Diffuse.a = In.Diffuse.a;
	Out.Position = mul(In.Position, wvp); //���_���W�����[���h�ϊ����ďo��
	Out.Normal = worldNormal; //�@�����o��
	Out.TexCoord = In.TexCoord; //�e�N�X�`�����W���o��
	matrix lightwvp;
	lightwvp = mul(World, Light.View);//���[���h�s�񁖃��C�g�r���[�s��
	lightwvp = mul(lightwvp, Light.Projection);//����Ɂ����C�g�v���W�F�N�V�����s��
	Out.ShadowPosition = mul(In.Position, lightwvp);//���C�g�J��������݂����_���W�o��

	Out.FloorTexture0 = In.FloorTexture0;
	Out.FloorAlfa0 = In.FloorAlfa0;



	// �@���}�b�v
	{
		////���_�F���o��
		//Out.Diffuse *= In.Diffuse;
		////�e�N�X�`�����W���o��

		//���[���h�ϊ��������_���W���o��
		Out.WorldPosition = mul(In.Position, World);
	}

}