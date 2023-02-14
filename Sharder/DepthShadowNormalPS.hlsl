
// �V���h�E�}�b�v+�@���}�b�v
// ���݃��b�V���t�B�[���h�I�u�W�F�N�g�����Ŏg���Ă���
#include "common.hlsl"
Texture2D g_Texture : register(t0);			  // �ʏ�e�N�X�`��
Texture2D g_TextureShadowDepth : register(t1);// �V���h�E�}�b�v
Texture2D g_TextureNormal : register(t2);	  // �@���}�b�v�p
Texture2D g_TextureOver : register(t3);			// �@�������ȏ�̏ꍇ�̊R�p�e�N�X�`��
Texture2D g_TextureOverNormal : register(t4);			// �@�������ȏ�̏ꍇ�̊R�p�e�N�X�`��
Texture2D g_TextureFloor1 : register(t5);	  // ���p�e�N�X�`��
Texture2D g_TextureFloorNormal1 : register(t6);	  // ���p�e�N�X�`��
Texture2D g_TextureFloor2 : register(t7);	  // ���p�e�N�X�`��
Texture2D g_TextureFloorNormal2 : register(t8);	  // ���p�e�N�X�`��
Texture2D g_TextureFloor3 : register(t9);	  // ���p�e�N�X�`��
Texture2D g_TextureFloorNormal3 : register(t10);	  // ���p�e�N�X�`��
Texture2D g_TextureFloor4 : register(t11);	  // ���p�e�N�X�`��
Texture2D g_TextureFloorNormal4 : register(t12);	  // ���p�e�N�X�`��
SamplerState g_SamplerState : register(s0);


void main(in PS_FLOOR_IN In, out float4 outDiffuse : SV_Target)
{
	// �ʏ�e�N�X�`�����󂯎��
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);


	//In.Normal.xyz /= In.Normal.w;	// Normal�͂��̂܂܂�OK-1.0~1.0�̒l��xyz�ɓ����Ă���
// Normal.y�����ȏゾ�ƃe�N�X�`�����R�p�ɕς���
	if (In.Normal.y < 0.55)	// �R�@��
	{
		outDiffuse = g_TextureOver.Sample(g_SamplerState, In.TexCoord);	// In.TexCoord
		normalMap = g_TextureOverNormal.Sample(g_SamplerState, In.TexCoord);
	}
	else if ((int)In.FloorTexture0.x == 0)
	{
		// �܂��g���e�N�X�`���ŐF�����
		float4 texcol = g_TextureFloor1.Sample(g_SamplerState, In.TexCoord);
		// �܂������o��
		float4 DistT = texcol - outDiffuse;
		// ���̂��Ƃɑ����Ă�����
		outDiffuse += DistT * In.FloorAlfa0.x;

		// �@���}�b�v������
		float4 TnormalMap = g_TextureFloorNormal1.Sample(g_SamplerState, In.TexCoord);
		float4 DistN = TnormalMap - normalMap;
		normalMap += DistN * In.FloorAlfa0.x;
	}
	else if ((int)In.FloorTexture0.x == 1)
	{
		float4 texcol = g_TextureFloor2.Sample(g_SamplerState, In.TexCoord);
		float4 DistT = texcol - outDiffuse;
		outDiffuse += DistT * In.FloorAlfa0.y;
		float4 TnormalMap = g_TextureFloorNormal1.Sample(g_SamplerState, In.TexCoord);
		float4 DistN = TnormalMap - normalMap;
		normalMap += DistN * In.FloorAlfa0.y;
	}
	else if ((int)In.FloorTexture0.x == 2)
	{
		float4 texcol = g_TextureFloor3.Sample(g_SamplerState, In.TexCoord);
		float4 DistT = texcol - outDiffuse;
		outDiffuse += DistT * In.FloorAlfa0.z;
		float4 TnormalMap = g_TextureFloorNormal1.Sample(g_SamplerState, In.TexCoord);
		float4 DistN = TnormalMap - normalMap;
		normalMap += DistN * In.FloorAlfa0.z;
	}
	else if ((int)In.FloorTexture0.x == 3)
	{
		float4 texcol = g_TextureFloor4.Sample(g_SamplerState, In.TexCoord);
		float4 DistT = texcol - outDiffuse;
		outDiffuse += DistT * In.FloorAlfa0.w;
		float4 TnormalMap = g_TextureFloorNormal1.Sample(g_SamplerState, In.TexCoord);
		float4 DistN = TnormalMap - normalMap;
		normalMap += DistN * In.FloorAlfa0.w;
	}


	// normalMap�̃X�P�[�����O
	normalMap = normalMap * 2 - 1.0;
	//���ɖ߂����e�N�X�`��RGBA�f�[�^����ёւ��Ė@���p�ϐ���
	float4 normal;
	normal.x = -normalMap.r;	// x=r, y=b, z=g �Ȃ̂Œ���
	normal.y = normalMap.b;
	normal.z = normalMap.g;
	normal.w = 0.0;

	////�s�N�Z���̖@���𐳋K��
	normal = normalize(normal);



	// w�Ŋ����Ă�����ƁAw��1�̐��E�ƂȂ�B1�̐��E�͉�X(���e��)�̌��Ă��鐢�E�B
	In.ShadowPosition.xyz /= In.ShadowPosition.w; //���e�ʂł̃s�N�Z���̍��W����
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //�e�N�X�`�����W�ɕϊ�
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //

	// �@���}�b�v
	{
		//�����v�Z������
		float light = -dot(normal.xyz, Light.Direction.xyz);

		//�X�y�L�����̌v�Z
		//�J��������s�N�Z���֌������x�N�g��
		float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
		eyev = normalize(eyev); //���K������
		//���̔��˃x�N�g�����v�Z
		float3 refv = reflect(Light.Direction.xyz, normal.xyz);
		refv = normalize(refv); //���K������

		float specular = -dot(eyev, refv); //���ʔ��˂̌v�Z
		specular = saturate(specular); //�l���T�`�����[�g
		specular = pow(specular, 100); //�����ł͂R�O�悵�Ă݂�B�Ⴂ�قǂĂ��Ă��ɂȂ�

		outDiffuse.rgb *= light;
		outDiffuse.a *= In.Diffuse.a; //���ʌv�Z

		//�X�y�L�����l���f�t���[�Y�Ƃ��đ�������
		if (In.Normal.y >= 0.55)	// �R�@��
			outDiffuse.rgb += specular * 0.1;	// ���邷����̂ŉ����Ă�
	}

	// �Ō�ɉe�������Ă�����
	//�V���h�E�}�b�v�e�N�X�`�����A���C�g�J��������s�N�Z���܂ł̋����i�[�x�l�j���擾
	float depth = g_TextureShadowDepth.Sample(g_SamplerState,
		In.ShadowPosition.xy);
	//�擾�l���ʏ�J��������s�N�Z���ւ̋�����菬����
	if (depth < In.ShadowPosition.z - 0.01) //0.01��Z�t�@�C�e�B���O�␳�l�i��q�j
	{
		outDiffuse.rgb *= 0.5; //�F���Â�����
	}

	//{
	//	// �J��������s�N�Z���ւ܂ł̋����ɉ����Ĕ�������B
	//	//float3 distance = In.WorldPosition.xyz - CameraPosition.xyz;
	//	float dist = distance(In.WorldPosition.xz, CameraPosition.xz);
	//	if (dist > 30.0)
	//	{
	//		//outDiffuse.rgb *= 1.8;
	//		outDiffuse.a *= 0.6;
	//		if (dist > 40.0)
	//		{
	//			//outDiffuse.rgb *= 1.8;
	//			outDiffuse.a *= 0.6;
	//			if (dist > 50.0)
	//			{
	//				//outDiffuse.rgb *= 1.8;
	//				outDiffuse.a *= 0.6;
	//			}
	//		}
	//	}
	//}
}


// �����̃e�N�X�`���u�����h�������������Ǐd�������G�G��
//void main(in PS_FLOOR_IN In, out float4 outDiffuse : SV_Target)
//{
//	//outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
//	outDiffuse.x = 0.0f;
//	outDiffuse.y = 0.0f;
//	outDiffuse.z = 0.0f;
//	outDiffuse.w = 0.0f;
//
//	// �S�ẴA���t�@�̍��v
//	float AllAlfa = In.FloorAlfa0.x + In.FloorAlfa0.y + In.FloorAlfa0.z + In.FloorAlfa0.w;
//	// �ʏ�e�N�X�`���̃A���t�@�l�BAlfaGeneral
//	float AlfaG = 1.0f - AllAlfa;
//	if (AlfaG < 0.0f)
//		AlfaG = 0.0f;
//
//	float4 Alfa;	// 4�̃A���t�@�̌v�Z���Ȃ��������̂��i�[����
//	Alfa.x = 0.0f;
//	Alfa.y = 0.0f;
//	Alfa.z = 0.0f;
//	Alfa.w = 0.0f;
//	float RemainingAlfa = 1.0f; // �c��̑S�̂̊���
//
//	// 1�ԃA���t�@�l���������̂ɍ��킹��
//	if (AlfaG >= In.FloorAlfa0.x && AlfaG >= In.FloorAlfa0.y && AlfaG >= In.FloorAlfa0.z && AlfaG >= In.FloorAlfa0.w)
//	{
//		RemainingAlfa = 1.0f - AlfaG;
//		float SumOther = In.FloorAlfa0.x + In.FloorAlfa0.y + In.FloorAlfa0.z + In.FloorAlfa0.w;
//		if (SumOther > 0.01f)
//		{
//			Alfa.x = In.FloorAlfa0.x * RemainingAlfa / SumOther;
//			Alfa.y = In.FloorAlfa0.y * RemainingAlfa / SumOther;
//			Alfa.z = In.FloorAlfa0.z * RemainingAlfa / SumOther;
//			Alfa.w = In.FloorAlfa0.w * RemainingAlfa / SumOther;
//		}
//	}
//	else if (In.FloorAlfa0.x >= AlfaG && In.FloorAlfa0.x >= In.FloorAlfa0.y && In.FloorAlfa0.x >= In.FloorAlfa0.z && In.FloorAlfa0.x >= In.FloorAlfa0.w)
//	{
//		// x��1�ԑ傫��	x�͂��̂܂܎g���B
//		Alfa.x = In.FloorAlfa0.x;
//		// �c��̑S�̂̊������o��
//		RemainingAlfa = 1.0f - Alfa.x;
//		// �c��̊������݂�Ȃŕ��������B
//		float SumOther = AlfaG + In.FloorAlfa0.y + In.FloorAlfa0.z + In.FloorAlfa0.w;
//		if (SumOther > 0.01f)
//		{
//			AlfaG = AlfaG * RemainingAlfa / SumOther;
//			Alfa.y = In.FloorAlfa0.y * RemainingAlfa / SumOther;
//			Alfa.z = In.FloorAlfa0.z * RemainingAlfa / SumOther;
//			Alfa.w = In.FloorAlfa0.w * RemainingAlfa / SumOther;
//		}
//	}
//	else if (In.FloorAlfa0.y >= AlfaG && In.FloorAlfa0.y >= In.FloorAlfa0.x && In.FloorAlfa0.y >= In.FloorAlfa0.z && In.FloorAlfa0.y >= In.FloorAlfa0.w)
//	{
//		Alfa.y = In.FloorAlfa0.y;
//		RemainingAlfa = 1.0f - Alfa.y;
//		float SumOther = AlfaG + In.FloorAlfa0.x + In.FloorAlfa0.z + In.FloorAlfa0.w;
//		if (SumOther > 0.01f)
//		{
//			AlfaG = AlfaG * RemainingAlfa / SumOther;
//			Alfa.x = In.FloorAlfa0.x * RemainingAlfa / SumOther;
//			Alfa.z = In.FloorAlfa0.z * RemainingAlfa / SumOther;
//			Alfa.w = In.FloorAlfa0.w * RemainingAlfa / SumOther;
//		}
//	}
//	else if (In.FloorAlfa0.z >= AlfaG && In.FloorAlfa0.z >= In.FloorAlfa0.x && In.FloorAlfa0.z >= In.FloorAlfa0.y && In.FloorAlfa0.z >= In.FloorAlfa0.w)
//	{
//		Alfa.z = In.FloorAlfa0.z;
//		RemainingAlfa = 1.0f - Alfa.z;
//		float SumOther = AlfaG + In.FloorAlfa0.x + In.FloorAlfa0.y + In.FloorAlfa0.w;
//		if (SumOther > 0.01f)
//		{
//			AlfaG = AlfaG * RemainingAlfa / SumOther;
//			Alfa.x = In.FloorAlfa0.x * RemainingAlfa / SumOther;
//			Alfa.y = In.FloorAlfa0.y * RemainingAlfa / SumOther;
//			Alfa.w = In.FloorAlfa0.w * RemainingAlfa / SumOther;
//		}
//	}
//	else if (In.FloorAlfa0.w >= AlfaG && In.FloorAlfa0.w >= In.FloorAlfa0.x && In.FloorAlfa0.w >= In.FloorAlfa0.y && In.FloorAlfa0.w >= In.FloorAlfa0.z)
//	{
//		Alfa.w = In.FloorAlfa0.w;
//		RemainingAlfa = 1.0f - Alfa.w;
//		float SumOther = AlfaG + In.FloorAlfa0.x + In.FloorAlfa0.y + In.FloorAlfa0.z;
//		if (SumOther > 0.01f)
//		{
//			AlfaG = AlfaG * RemainingAlfa / SumOther;
//			Alfa.x = In.FloorAlfa0.x * RemainingAlfa / SumOther;
//			Alfa.y = In.FloorAlfa0.y * RemainingAlfa / SumOther;
//			Alfa.z = In.FloorAlfa0.z * RemainingAlfa / SumOther;
//		}
//	}
//
//
//	{
//		// �\������e�N�X�`�����쐬
//		float4 tg = g_Texture.Sample(g_SamplerState, In.TexCoord);
//		float4 tx = g_TextureFloor1.Sample(g_SamplerState, In.TexCoord);
//		float4 ty = g_TextureFloor2.Sample(g_SamplerState, In.TexCoord);
//		float4 tz = g_TextureFloor3.Sample(g_SamplerState, In.TexCoord);
//		float4 tw = g_TextureFloor4.Sample(g_SamplerState, In.TexCoord);
//
//		// �ŏ��ɏo�����A���t�@�l�������ĐF�𑫂��Ă���
//		outDiffuse += (tg * AlfaG) + (tx * Alfa.x) + (ty * Alfa.y) + (tz * Alfa.z) + (tw * Alfa.w);
//	}
//
//	float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
//	//normalMap.x = 0.0f;
//	//normalMap.y = 0.0f;
//	//normalMap.z = 0.0f;
//	//normalMap.w = 0.0f;
//	{
//		// �@���}�b�v���쐬
//		float4 ng = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
//		float4 nx = g_TextureFloorNormal1.Sample(g_SamplerState, In.TexCoord);
//		float4 ny = g_TextureFloorNormal2.Sample(g_SamplerState, In.TexCoord);
//		float4 nz = g_TextureFloorNormal3.Sample(g_SamplerState, In.TexCoord);
//		float4 nw = g_TextureFloorNormal4.Sample(g_SamplerState, In.TexCoord);
//
//		// �ŏ��ɏo�����A���t�@�l�������ĐF�𑫂��Ă���
//		normalMap += (ng * AlfaG) + (nx * Alfa.x) + (ny * Alfa.y) + (nz * Alfa.z) + (nw * Alfa.w);
//	}
//
//	//In.Normal.xyz /= In.Normal.w;	// Normal�͂��̂܂܂�OK-1.0~1.0�̒l��xyz�ɓ����Ă���
//	// Normal.y�����ȏゾ�ƃe�N�X�`�����R�p�ɕς���
//	if (In.Normal.y < 0.7)
//	{
//		outDiffuse = g_TextureOver.Sample(g_SamplerState, In.TexCoord);	// In.TexCoord
//		normalMap = g_TextureOverNormal.Sample(g_SamplerState, In.TexCoord);
//	}
//
//
//	// normalMap�̃X�P�[�����O
//	normalMap = normalMap * 2 - 1.0;
//	//���ɖ߂����e�N�X�`��RGBA�f�[�^����ёւ��Ė@���p�ϐ���
//	float4 normal;
//	normal.x = -normalMap.r;			// x=r, y=b, z=g �Ȃ̂Œ���
//	normal.y = normalMap.b;
//	normal.z = normalMap.g;
//	normal.w = 0.0;
//
//	////�s�N�Z���̖@���𐳋K��
//	normal = normalize(normal);
//
//
//
//	// w�Ŋ����Ă�����ƁAw��1�̐��E�ƂȂ�B1�̐��E�͉�X(���e��)�̌��Ă��鐢�E�B
//	In.ShadowPosition.xyz /= In.ShadowPosition.w; //���e�ʂł̃s�N�Z���̍��W����
//	In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //�e�N�X�`�����W�ɕϊ�
//	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //
//
//	// �@���}�b�v
//	{
//		//�����v�Z������
//		float light = -dot(normal.xyz, Light.Direction.xyz);
//
//		//�X�y�L�����̌v�Z
//		//�J��������s�N�Z���֌������x�N�g��
//		float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
//		eyev = normalize(eyev); //���K������
//		//���̔��˃x�N�g�����v�Z
//		float3 refv = reflect(Light.Direction.xyz, normal.xyz);
//		refv = normalize(refv); //���K������
//
//		float specular = -dot(eyev, refv); //���ʔ��˂̌v�Z
//		specular = saturate(specular); //�l���T�`�����[�g
//		specular = pow(specular, 100); //�����ł͂R�O�悵�Ă݂�B�Ⴂ�قǂĂ��Ă��ɂȂ�
//
//		outDiffuse.rgb *= light;
//		outDiffuse.a *= In.Diffuse.a; //���ʌv�Z
//
//		//�X�y�L�����l���f�t���[�Y�Ƃ��đ�������
//		if (In.Normal.y >= 0.7)
//		outDiffuse.rgb += specular * 0.1;	// ���邷����̂ŉ����Ă�
//	}
//
//	// �Ō�ɉe�������Ă�����
//	//�V���h�E�}�b�v�e�N�X�`�����A���C�g�J��������s�N�Z���܂ł̋����i�[�x�l�j���擾
//	float depth = g_TextureShadowDepth.Sample(g_SamplerState,
//		In.ShadowPosition.xy);
//	//�擾�l���ʏ�J��������s�N�Z���ւ̋�����菬����
//	if (depth < In.ShadowPosition.z - 0.01) //0.01��Z�t�@�C�e�B���O�␳�l�i��q�j
//	{
//		outDiffuse.rgb *= 0.5; //�F���Â�����
//	}
//
//}