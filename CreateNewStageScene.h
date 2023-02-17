#pragma once


class Camera;
class OriginalBlock;
class Brush;

enum class E_CreateStageScene_Mode
{
	Object,
	Sculpt,
	Paint
};


class CreateNewStageScene : public Scene
{
private:
	E_CreateStageScene_Mode m_Mode = E_CreateStageScene_Mode::Object;

	GameObject* m_PickupObject;
	Brush*		m_Brush;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
};

