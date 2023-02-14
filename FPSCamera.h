#pragma once

static const float ForcusOffsetY = 1.0f;

struct FPSCamera
{

	double angleX = M_PI_2;	// xÇ™âEÇ…ê≥ÅAzÇ™ëOÇ…ê≥
	double angleYvalue = 0.0;
	float sensitivity = 1.0f;


	D3DXVECTOR3 GetFPSTarget(const D3DXVECTOR3& cameraposition);
	D3DXVECTOR3 GetTPSPosition(const D3DXVECTOR3& target, const float& radius);


	D3DXVECTOR3 GetDirection();


	void AddDirectionX(float magnification);

	void AddDirectionY(float magnification);

	void SetSensitivity(float value)
	{
		sensitivity = value;
	}

	D3DXVECTOR3 GetFocusPosition(const D3DXVECTOR3& eyePosition)
	{
		return (eyePosition + GetDirection());
	}

	D3DXVECTOR3 GetForward()
	{
		D3DXVECTOR3 dir = GetDirection();
		dir.y = 0.0;
		D3DXVec3Normalize(&dir, &dir);
		return dir;
	}

	D3DXVECTOR3 GetRight()
	{
		D3DXVECTOR3 forward = GetForward();
		D3DXVECTOR3 right = D3DXVECTOR3(forward.z, 0.0, -forward.x);
		return right;
	}

};



