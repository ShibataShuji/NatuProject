//thsozai ==>�O��܂ł̏�ԁ@ZIP�@�@�p�X���[�h�@nabe


//BlinnPhongLightingPS.hlsl
//BlinnPhongLightingVS.hlsl


#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//�s�N�Z���̖@���𐳋K��
	float4 normal = normalize(In.Normal);
	//�����v�Z������
	float light = -dot(normal.xyz, Light.Direction.xyz);

	//���邳�̕␳���s��
	if (light > 0.7) 
	{
		light = 1.0;
	}
	else if (light > 0.5)
	{
		light = 0.8;
	}
	else
	{
		light = 0.4;
	}

	//�e�N�X�`���̃s�N�Z���F���擾
	outDiffuse = 
		g_Texture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse.rgb *=
		In.Diffuse.rgb * light; //���邳�ƐF����Z
	outDiffuse.a *= 
		In.Diffuse.a; //���ʌv�Z


	//�֊s��������

	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	//�����x�N�g���Ɩ@���Ƃœ��ς��v�Z����
	float d = -dot(eyev, normal.xyz);

	if (d < 0.2)
	{
		outDiffuse.r = 1.0;
		outDiffuse.g = 0.0;
		outDiffuse.b = 0.0;
	}

}


