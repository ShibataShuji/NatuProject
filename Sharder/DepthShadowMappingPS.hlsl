// �V���h�E�}�b�v
// ���݃��b�V���t�B�[���h�I�u�W�F�N�g�ȊO�̃I�u�W�F�N�g�A���f���̕`�ʂŎg���Ă���

#include "common.hlsl"
Texture2D g_Texture : register(t0);//�ʏ�e�N�X�`��
Texture2D g_TextureShadowDepth : register(t1);//�V���h�E�}�b�v
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//�����ł̐F�����ʂ̃e�N�X�`�������_�F���쐬���Ă���
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse *= In.Diffuse;

	// �ǉ��̐F�̌v�Z
// �܂������o��
	float DistR = SynthesizeColor.x - outDiffuse.r;
	float DistG = SynthesizeColor.y - outDiffuse.g;
	float DistB = SynthesizeColor.z - outDiffuse.b;
	// ���̂��Ƃɑ����Ă�����
	outDiffuse.r += DistR * SynthesizeColor.w;
	outDiffuse.g += DistG * SynthesizeColor.w;
	outDiffuse.b += DistB * SynthesizeColor.w;

	// w�Ŋ����Ă�����ƁAw��1�̐��E�ƂȂ�B1�̐��E�͉�X(���e��)�̌��Ă��鐢�E�B
	In.ShadowPosition.xyz /= In.ShadowPosition.w; //���e�ʂł̃s�N�Z���̍��W����
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //�e�N�X�`�����W�ɕϊ�
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //

	//�V���h�E�}�b�v�e�N�X�`�����A���C�g�J��������s�N�Z���܂ł̋����i�[�x�l�j���擾
	float depth = g_TextureShadowDepth.Sample(g_SamplerState,
		In.ShadowPosition.xy);
	//�擾�l���ʏ�J��������s�N�Z���ւ̋�����菬����
	if (depth < In.ShadowPosition.z - 0.01) //0.01��Z�t�@�C�e�B���O�␳�l�i��q�j
	{
		outDiffuse.rgb *= 0.5; //�F���Â�����
	}



	////�֊s��������
	////�s�N�Z���̖@���𐳋K��
	//float4 normal = normalize(In.Normal);
	////�J��������s�N�Z���֌������x�N�g��
	//float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	//eyev = normalize(eyev); //���K������

	////�����x�N�g���Ɩ@���Ƃœ��ς��v�Z����
	//float d = -dot(eyev, normal.xyz);

	//if (d < 0.1)
	//{
	//	outDiffuse.r = 0.0;
	//	outDiffuse.g = 0.0;
	//	outDiffuse.b = 0.0;
	//}


}