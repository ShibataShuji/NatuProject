// シャドウマップ
// 現在メッシュフィールドオブジェクト以外のオブジェクト、モデルの描写で使っている

#include "common.hlsl"
Texture2D g_Texture : register(t0);//通常テクスチャ
Texture2D g_TextureShadowDepth : register(t1);//シャドウマップ
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//日向での色＝普通のテクスチャ＊頂点色を作成しておく
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse *= In.Diffuse;

	// 追加の色の計算
// まず差を出す
	float DistR = SynthesizeColor.x - outDiffuse.r;
	float DistG = SynthesizeColor.y - outDiffuse.g;
	float DistB = SynthesizeColor.z - outDiffuse.b;
	// そのあとに足してあげる
	outDiffuse.r += DistR * SynthesizeColor.w;
	outDiffuse.g += DistG * SynthesizeColor.w;
	outDiffuse.b += DistB * SynthesizeColor.w;

	// wで割ってあげると、wが1の世界となる。1の世界は我々(投影面)の見ている世界。
	In.ShadowPosition.xyz /= In.ShadowPosition.w; //投影面でのピクセルの座標つくる
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //テクスチャ座標に変換
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //

	//シャドウマップテクスチャより、ライトカメラからピクセルまでの距離（深度値）を取得
	float depth = g_TextureShadowDepth.Sample(g_SamplerState,
		In.ShadowPosition.xy);
	//取得値が通常カメラからピクセルへの距離より小さい
	if (depth < In.ShadowPosition.z - 0.01) //0.01はZファイティング補正値（後述）
	{
		outDiffuse.rgb *= 0.5; //色を暗くする
	}



	////輪郭線をつくる
	////ピクセルの法線を正規化
	//float4 normal = normalize(In.Normal);
	////カメラからピクセルへ向かうベクトル
	//float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	//eyev = normalize(eyev); //正規化する

	////視線ベクトルと法線とで内積を計算する
	//float d = -dot(eyev, normal.xyz);

	//if (d < 0.1)
	//{
	//	outDiffuse.r = 0.0;
	//	outDiffuse.g = 0.0;
	//	outDiffuse.b = 0.0;
	//}


}