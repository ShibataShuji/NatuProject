#pragma once

#define PaintBrushSetListMax (10)
#define PaintBrushSetMax (5)	// 5コメは特殊ブラシ用消しゴムとか


enum class E_MouseActiveState
{
	NotActive,
	ActiveLeft,
	ActiveRight
};

enum class E_MouseTriggerState
{
	NotTrriger,
	TriggerONLeft,
	TriggerONRight,
	TriggerOFFLeft,
	TriggerOFFRight
};

// スカルプトブラシ
enum class E_UseSculptBrush
{
	Brush001,
	Brush002,
	Brush003,
	ENDNUM		// enum class の列挙の最後に１つ付け足しておくことで、これがこの列挙型の要素数となる。
};
struct PassedSculptBrushState
{
	D3DXVECTOR3 Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 TriggerPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Radius = 1.0f;
	float Strength = 1.0f;
};
// 使っているスカルプトブラシの中のセッティング。プラスかマイナスかみないな。もこもこか減らすかみたいな。
enum class E_UseSettingOfSculptBrush
{
	Setting001,
	Setting002,
	Setting003
};

// ペイントブラシ一覧
enum class E_PaintBrushType
{
	Ground001,
	Grass001,
	Eraser,
	ENDNUM
};
struct PassedPaintBrushState
{
	D3DXVECTOR3 Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 TriggerPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Radius = 1.0f;
	float Strength = 1.0f;
};