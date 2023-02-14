// 定数バッファ（英語：Constant Buffer、通称：cbuffer）とは、CPUで動くプログラムの変数群を、
// GPUで動くプログラマブルシェーダーに定数群（バッファ）としてドスンと送り込む機構をいう。
// 
// 定数バッファ(Constant Buffer)を使うときはcbufferキーワード使い、構造体のように宣言します。 
// あと、アンオーダードアクセスビュー(以下、UAV)と同じでスロット番号も指定可能です。 
// 定数バッファの場合は”b0”のように接頭語にbをつけて指定してください。
// 
// 定数バッファとして宣言された変数はシェーダ内のどこからでも使うことが出来ます。 
// いわば、c++のグローバル変数のようなものです。 グローバル変数との違いは代入ができないというだけなので、
// const宣言されたグローバル変数だという認識でいいでしょう。


//このファイルは他のシェーダーファイルへインクルードされます
// 各種マトリクスを受け取る変数を用意
// renderer.cppの215行目くらいで使う準備をしている
cbuffer WorldBuffer : register(b0)//定数バッファ 0 番
{
	matrix World;
}
cbuffer ViewBuffer : register(b1) //定数バッファ 1 番
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2) //定数バッファ 2 番
{
	matrix Projection;
}
cbuffer CameraBuffer : register(b5) //バッファの５番とする
{
	float4 CameraPosition; //カメラ座標を受けとる変数
}
cbuffer ParameterBuffer : register(b6)
{
	float4 Parameter;
}

// 色を合成するための色のバッファ
cbuffer SynthesizeColorBuffer : register(b7)
{
	float4 SynthesizeColor;
}

//頂点シェーダーへ入力されるデータを構造体の形で表現
//これは頂点バッファの内容そのもの
struct VS_IN
{
	float4 Position : POSITION0; //セマンティクスは頂点レイアウトに対応
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
};
//ピクセルシェーダーへ入力されるデータを構造体の形で表現
struct PS_IN
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4 ShadowPosition : POSITION1; //ライトカメラから見たピクセルの座標
};

// Floor用
struct VS_FLOOR_IN
{
	float4 Position : POSITION0; //セマンティクスは頂点レイアウトに対応
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;

	float4  FloorTexture0 : FLOORTEX;	// 1回の描写で4つまでのテクスチャをブレンドできるようにしている
	float4  FloorAlfa0 : FLOORALFA;
};
struct PS_FLOOR_IN
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4 ShadowPosition : POSITION1; //ライトカメラから見たピクセルの座標

	float4  FloorTexture0 : FLOORTEX;
	float4  FloorAlfa0 : FLOORALFA;
};


struct VS_FLOOR2_IN
{
	float4 Position : POSITION0; //セマンティクスは頂点レイアウトに対応
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4  FloorTexture0 : FLOORTEX;	// 1回の描写で4つまでのテクスチャをブレンドできるようにしている
	float4  FloorAlfa0 : FLOORALFA;
};
struct PS_FLOOR2_IN
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
	float4 ShadowPosition : POSITION1; //ライトカメラから見たピクセルの座標
	float4  FloorTexture0 : FLOORTEX;
	float4  FloorAlfa0 : FLOORALFA;
};

//ライトオブジェクト構造体とコンスタントバッファ
struct LIGHT
{
	bool  Enable;
	bool3 Dummy; //配置アドレスを4の倍数にすると効率がいいので調整用
	float4 Direction; //実はC言語でも同じでVisualStudioがやってくれている。
	float4 Diffuse;
	float4 Ambient;

	matrix View; //ライトカメラのビュー行列
	matrix Projection; //ライトカメラのプロジェクション行列

};
cbuffer LightBuffer : register(b4)//コンスタントバッファ４番とする
{
	LIGHT Light; //ライト構造体
}




/////////////////////EOF//////////////////////
