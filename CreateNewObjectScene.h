#pragma once

//#include "scene.h"

class Camera;
class OriginalBlock;


class CreateNewObjectScene : public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	OriginalBlock* m_object = nullptr;
};

