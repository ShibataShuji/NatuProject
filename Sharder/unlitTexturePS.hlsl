//ピクセルシェーダー
#include "common.hlsl" //必ずインクルード
//
//in は入力されてくるデーター
//out は出力するデータ
//

//DirectXのテクスチャの設定を受け継ぐ
Texture2D g_Texture : register(t0);//テクスチャー０番
//DirectXのサンプラーステート設定を受け継ぐ
SamplerState g_SamplerState : register(s0);//テクスチャサンプ

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//このピクセルに使われるテクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	//入力されたピクセルの色をそのまま出力
	outDiffuse *= In.Diffuse;

	////グレースケールにしてみる
	//outDiffuse.rgb = 
	//	  outDiffuse.r * 0.3
	//	+ outDiffuse.g * 0.6
	//	+ outDiffuse.b * 0.1;

	//////セピア調変換してみる
	//float4 sepia = float4(0.960, 0.784, 0.580, 1);
	//outDiffuse *= sepia;

	////outDiffuse = pow(outDiffuse,6);


	outDiffuse.a = 1.0f;
}