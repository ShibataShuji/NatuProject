//thsozai ==>前回までの状態　ZIP　　パスワード　nabe


//BlinnPhongLightingPS.hlsl
//BlinnPhongLightingVS.hlsl


#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//ピクセルの法線を正規化
	float4 normal = normalize(In.Normal);
	//光源計算をする
	float light = -dot(normal.xyz, Light.Direction.xyz);

	//明るさの補正を行う
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

	//テクスチャのピクセル色を取得
	outDiffuse = 
		g_Texture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse.rgb *=
		In.Diffuse.rgb * light; //明るさと色を乗算
	outDiffuse.a *= 
		In.Diffuse.a; //α別計算


	//輪郭線をつくる

	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	//視線ベクトルと法線とで内積を計算する
	float d = -dot(eyev, normal.xyz);

	if (d < 0.2)
	{
		outDiffuse.r = 1.0;
		outDiffuse.g = 0.0;
		outDiffuse.b = 0.0;
	}

}


