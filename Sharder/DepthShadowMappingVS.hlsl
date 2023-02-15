// �V���h�E�}�b�v
// ���݃��b�V���t�B�[���h�I�u�W�F�N�g�ȊO�̃I�u�W�F�N�g�A���f���̕`�ʂŎg���Ă���

#include "common.hlsl"
void main(in VS_IN In, out PS_IN Out)
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
}
