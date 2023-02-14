#pragma once


class OriginalBlock : public GameObject	// åpè≥
{
private:


public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool PlayerObjectUpdate();

};
