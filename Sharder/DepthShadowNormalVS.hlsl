
// シャドウマップ+法線マップ
// 現在メッシュフィールドオブジェクトだけで使っている
#include "common.hlsl"
//// フロア変えた
//void main(in VS_FLOOR_IN In, out PS_FLOOR_IN Out)
void main(in VS_FLOOR_IN In, out PS_FLOOR_IN Out)
{
	matrix wvp; //いつもの処理だが
	wvp = mul(World, View); //1パス目はライトカメラの行列
	wvp = mul(wvp, Projection); //2パス目は通常カメラの行列が設定される
	float4 worldNormal, normal; //法線のワールド変換
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	//光源処理
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);
	Out.Diffuse = light;
	Out.Diffuse.a = In.Diffuse.a;
	Out.Position = mul(In.Position, wvp); //頂点座標をワールド変換して出力
	Out.Normal = worldNormal; //法線を出力
	Out.TexCoord = In.TexCoord; //テクスチャ座標を出力
	matrix lightwvp;
	lightwvp = mul(World, Light.View);//ワールド行列＊ライトビュー行列
	lightwvp = mul(lightwvp, Light.Projection);//さらに＊ライトプロジェクション行列
	Out.ShadowPosition = mul(In.Position, lightwvp);//ライトカメラからみた頂点座標出力

	Out.FloorTexture0 = In.FloorTexture0;
	Out.FloorAlfa0 = In.FloorAlfa0;



	// 法線マップ
	{
		////頂点色を出力
		//Out.Diffuse *= In.Diffuse;
		////テクスチャ座標を出力

		//ワールド変換した頂点座標を出力
		Out.WorldPosition = mul(In.Position, World);
	}

}