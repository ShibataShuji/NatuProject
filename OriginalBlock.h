#pragma once


class OriginalBlock : public GameObject	// �p��
{
private:


public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool PlayerObjectUpdate();

};
