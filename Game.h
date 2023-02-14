#pragma once


class Game : public Scene
{
private:
	bool m_OpenGameSettingGUI;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;


};

