#pragma once

#define PaintBrushSetListMax (10)
#define PaintBrushSetMax (5)	// 5�R���͓���u���V�p�����S���Ƃ�


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

// �X�J���v�g�u���V
enum class E_UseSculptBrush
{
	Brush001,
	Brush002,
	Brush003,
	ENDNUM		// enum class �̗񋓂̍Ō�ɂP�t�������Ă������ƂŁA���ꂪ���̗񋓌^�̗v�f���ƂȂ�B
};
struct PassedSculptBrushState
{
	D3DXVECTOR3 Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 TriggerPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Radius = 1.0f;
	float Strength = 1.0f;
};
// �g���Ă���X�J���v�g�u���V�̒��̃Z�b�e�B���O�B�v���X���}�C�i�X���݂Ȃ��ȁB�������������炷���݂����ȁB
enum class E_UseSettingOfSculptBrush
{
	Setting001,
	Setting002,
	Setting003
};

// �y�C���g�u���V�ꗗ
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