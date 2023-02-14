
// シャドウマップ+法線マップ
// 現在メッシュフィールドオブジェクトだけで使っている
#include "common.hlsl"
Texture2D g_Texture : register(t0);			  // 通常テクスチャ
Texture2D g_TextureShadowDepth : register(t1);// シャドウマップ
Texture2D g_TextureNormal : register(t2);	  // 法線マップ用
Texture2D g_TextureOver : register(t3);			// 法線が一定以上の場合の崖用テクスチャ
Texture2D g_TextureOverNormal : register(t4);			// 法線が一定以上の場合の崖用テクスチャ
Texture2D g_TextureFloor1 : register(t5);	  // 床用テクスチャ
Texture2D g_TextureFloorNormal1 : register(t6);	  // 床用テクスチャ
Texture2D g_TextureFloor2 : register(t7);	  // 床用テクスチャ
Texture2D g_TextureFloorNormal2 : register(t8);	  // 床用テクスチャ
Texture2D g_TextureFloor3 : register(t9);	  // 床用テクスチャ
Texture2D g_TextureFloorNormal3 : register(t10);	  // 床用テクスチャ
Texture2D g_TextureFloor4 : register(t11);	  // 床用テクスチャ
Texture2D g_TextureFloorNormal4 : register(t12);	  // 床用テクスチャ
SamplerState g_SamplerState : register(s0);


void main(in PS_FLOOR_IN In, out float4 outDiffuse : SV_Target)
{
	// 通常テクスチャを受け取る
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);


	//In.Normal.xyz /= In.Normal.w;	// NormalはそのままでOK-1.0~1.0の値がxyzに入っている
// Normal.yが一定以上だとテクスチャを崖用に変える
	if (In.Normal.y < 0.55)	// 崖法線
	{
		outDiffuse = g_TextureOver.Sample(g_SamplerState, In.TexCoord);	// In.TexCoord
		normalMap = g_TextureOverNormal.Sample(g_SamplerState, In.TexCoord);
	}
	else if ((int)In.FloorTexture0.x == 0)
	{
		// まず使うテクスチャで色を作る
		float4 texcol = g_TextureFloor1.Sample(g_SamplerState, In.TexCoord);
		// まず差を出す
		float4 DistT = texcol - outDiffuse;
		// そのあとに足してあげる
		outDiffuse += DistT * In.FloorAlfa0.x;

		// 法線マップも同じ
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


	// normalMapのスケーリング
	normalMap = normalMap * 2 - 1.0;
	//元に戻したテクスチャRGBAデータを並び替えて法線用変数へ
	float4 normal;
	normal.x = -normalMap.r;	// x=r, y=b, z=g なので注意
	normal.y = normalMap.b;
	normal.z = normalMap.g;
	normal.w = 0.0;

	////ピクセルの法線を正規化
	normal = normalize(normal);



	// wで割ってあげると、wが1の世界となる。1の世界は我々(投影面)の見ている世界。
	In.ShadowPosition.xyz /= In.ShadowPosition.w; //投影面でのピクセルの座標つくる
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //テクスチャ座標に変換
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //

	// 法線マップ
	{
		//光源計算をする
		float light = -dot(normal.xyz, Light.Direction.xyz);

		//スペキュラの計算
		//カメラからピクセルへ向かうベクトル
		float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
		eyev = normalize(eyev); //正規化する
		//光の反射ベクトルを計算
		float3 refv = reflect(Light.Direction.xyz, normal.xyz);
		refv = normalize(refv); //正規化する

		float specular = -dot(eyev, refv); //鏡面反射の計算
		specular = saturate(specular); //値をサチュレート
		specular = pow(specular, 100); //ここでは３０乗してみる。低いほどてかてかになる

		outDiffuse.rgb *= light;
		outDiffuse.a *= In.Diffuse.a; //α別計算

		//スペキュラ値をデフューズとして足しこむ
		if (In.Normal.y >= 0.55)	// 崖法線
			outDiffuse.rgb += specular * 0.1;	// 明るすぎるので下げてる
	}

	// 最後に影分かけてあげる
	//シャドウマップテクスチャより、ライトカメラからピクセルまでの距離（深度値）を取得
	float depth = g_TextureShadowDepth.Sample(g_SamplerState,
		In.ShadowPosition.xy);
	//取得値が通常カメラからピクセルへの距離より小さい
	if (depth < In.ShadowPosition.z - 0.01) //0.01はZファイティング補正値（後述）
	{
		outDiffuse.rgb *= 0.5; //色を暗くする
	}

	//{
	//	// カメラからピクセルへまでの距離に応じて白くする。
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


// 複数のテクスチャブレンドしたかったけど重かった；；↓
//void main(in PS_FLOOR_IN In, out float4 outDiffuse : SV_Target)
//{
//	//outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
//	outDiffuse.x = 0.0f;
//	outDiffuse.y = 0.0f;
//	outDiffuse.z = 0.0f;
//	outDiffuse.w = 0.0f;
//
//	// 全てのアルファの合計
//	float AllAlfa = In.FloorAlfa0.x + In.FloorAlfa0.y + In.FloorAlfa0.z + In.FloorAlfa0.w;
//	// 通常テクスチャのアルファ値。AlfaGeneral
//	float AlfaG = 1.0f - AllAlfa;
//	if (AlfaG < 0.0f)
//		AlfaG = 0.0f;
//
//	float4 Alfa;	// 4つのアルファの計算しなおしたものを格納する
//	Alfa.x = 0.0f;
//	Alfa.y = 0.0f;
//	Alfa.z = 0.0f;
//	Alfa.w = 0.0f;
//	float RemainingAlfa = 1.0f; // 残りの全体の割合
//
//	// 1番アルファ値が高いものに合わせる
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
//		// xが1番大きい	xはそのまま使う。
//		Alfa.x = In.FloorAlfa0.x;
//		// 残りの全体の割合を出す
//		RemainingAlfa = 1.0f - Alfa.x;
//		// 残りの割合をみんなで分け合う。
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
//		// 表示するテクスチャを作成
//		float4 tg = g_Texture.Sample(g_SamplerState, In.TexCoord);
//		float4 tx = g_TextureFloor1.Sample(g_SamplerState, In.TexCoord);
//		float4 ty = g_TextureFloor2.Sample(g_SamplerState, In.TexCoord);
//		float4 tz = g_TextureFloor3.Sample(g_SamplerState, In.TexCoord);
//		float4 tw = g_TextureFloor4.Sample(g_SamplerState, In.TexCoord);
//
//		// 最初に出したアルファ値をかけて色を足していく
//		outDiffuse += (tg * AlfaG) + (tx * Alfa.x) + (ty * Alfa.y) + (tz * Alfa.z) + (tw * Alfa.w);
//	}
//
//	float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
//	//normalMap.x = 0.0f;
//	//normalMap.y = 0.0f;
//	//normalMap.z = 0.0f;
//	//normalMap.w = 0.0f;
//	{
//		// 法線マップを作成
//		float4 ng = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
//		float4 nx = g_TextureFloorNormal1.Sample(g_SamplerState, In.TexCoord);
//		float4 ny = g_TextureFloorNormal2.Sample(g_SamplerState, In.TexCoord);
//		float4 nz = g_TextureFloorNormal3.Sample(g_SamplerState, In.TexCoord);
//		float4 nw = g_TextureFloorNormal4.Sample(g_SamplerState, In.TexCoord);
//
//		// 最初に出したアルファ値をかけて色を足していく
//		normalMap += (ng * AlfaG) + (nx * Alfa.x) + (ny * Alfa.y) + (nz * Alfa.z) + (nw * Alfa.w);
//	}
//
//	//In.Normal.xyz /= In.Normal.w;	// NormalはそのままでOK-1.0~1.0の値がxyzに入っている
//	// Normal.yが一定以上だとテクスチャを崖用に変える
//	if (In.Normal.y < 0.7)
//	{
//		outDiffuse = g_TextureOver.Sample(g_SamplerState, In.TexCoord);	// In.TexCoord
//		normalMap = g_TextureOverNormal.Sample(g_SamplerState, In.TexCoord);
//	}
//
//
//	// normalMapのスケーリング
//	normalMap = normalMap * 2 - 1.0;
//	//元に戻したテクスチャRGBAデータを並び替えて法線用変数へ
//	float4 normal;
//	normal.x = -normalMap.r;			// x=r, y=b, z=g なので注意
//	normal.y = normalMap.b;
//	normal.z = normalMap.g;
//	normal.w = 0.0;
//
//	////ピクセルの法線を正規化
//	normal = normalize(normal);
//
//
//
//	// wで割ってあげると、wが1の世界となる。1の世界は我々(投影面)の見ている世界。
//	In.ShadowPosition.xyz /= In.ShadowPosition.w; //投影面でのピクセルの座標つくる
//	In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //テクスチャ座標に変換
//	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //
//
//	// 法線マップ
//	{
//		//光源計算をする
//		float light = -dot(normal.xyz, Light.Direction.xyz);
//
//		//スペキュラの計算
//		//カメラからピクセルへ向かうベクトル
//		float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
//		eyev = normalize(eyev); //正規化する
//		//光の反射ベクトルを計算
//		float3 refv = reflect(Light.Direction.xyz, normal.xyz);
//		refv = normalize(refv); //正規化する
//
//		float specular = -dot(eyev, refv); //鏡面反射の計算
//		specular = saturate(specular); //値をサチュレート
//		specular = pow(specular, 100); //ここでは３０乗してみる。低いほどてかてかになる
//
//		outDiffuse.rgb *= light;
//		outDiffuse.a *= In.Diffuse.a; //α別計算
//
//		//スペキュラ値をデフューズとして足しこむ
//		if (In.Normal.y >= 0.7)
//		outDiffuse.rgb += specular * 0.1;	// 明るすぎるので下げてる
//	}
//
//	// 最後に影分かけてあげる
//	//シャドウマップテクスチャより、ライトカメラからピクセルまでの距離（深度値）を取得
//	float depth = g_TextureShadowDepth.Sample(g_SamplerState,
//		In.ShadowPosition.xy);
//	//取得値が通常カメラからピクセルへの距離より小さい
//	if (depth < In.ShadowPosition.z - 0.01) //0.01はZファイティング補正値（後述）
//	{
//		outDiffuse.rgb *= 0.5; //色を暗くする
//	}
//
//}