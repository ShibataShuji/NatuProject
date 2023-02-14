
#include "stdafx.h"

// ��l�̎��_�Ńv���C����Ƃ��̃^�[�Q�b�g�ƂȂ�|�W�V�������Q�b�g����
D3DXVECTOR3 FPSCamera::GetFPSTarget(const D3DXVECTOR3& position)
{
	return position + GetDirection();
}

// �O�l�̎��_�Ńv���C����Ƃ��̃|�W�V�������Q�b�g����
D3DXVECTOR3 FPSCamera::GetTPSPosition(const D3DXVECTOR3& target, const float& radius)
{
	// �~�����
	const D3DXVECTOR3 dirX = D3DXVECTOR3(sin(angleX), 0.0f, -cos(angleX));

	float wariai;
	if (abs(angleYvalue) <= 0.0001)
		wariai = 1.0f;
	else
		wariai =  1.0f - abs(angleYvalue) * 2.0f;
		//wariai = abs(angleYvalue) / 0.49;
	float xzlength = radius * wariai;

	D3DXVECTOR3 re = target;
	re += dirX * xzlength;

	re.y += radius * angleYvalue * 2.0f;

	return re;
}

D3DXVECTOR3 FPSCamera::GetDirection()
{
	D3DXVECTOR3 re;

	// ���a�P�ŁAangleX�x�~���ɉ����Đi�񂾓_�̈ʒu�����܂�
	// ray�̃X�^�[�g�n�_�ɂȂ�B1.0�ɂ��Ă���̂́A|�Z <-���̐}�̂悤�ɔ��烌�C���X�^�[�g�����邽�߂ɁB
	const D3DXVECTOR2 dirX = D3DXVECTOR2(sin(angleX), -cos(angleX));

	// ���ƃ��C���g����FPS���_�ł̃J�����𐧌䂷�� 
	D3DXVECTOR3 rayS(dirX.x, angleYvalue, dirX.y);			
	D3DXVECTOR3 rayDirection(-dirX.x, 0.0, -dirX.y);		// �X�^�[�g�n�_����~�̒��S������B
	D3DXVec3Normalize(&rayDirection, &rayDirection);
	D3DXVECTOR3 q1, q2;

	if (calcRaySphere(rayS, rayDirection, D3DXVECTOR3(0.0, 0.0, 0.0), 0.5, &q1, &q2))
	{
		return q1;
	}
	else
	{
		return D3DXVECTOR3(0.0, 1.0, 0.0);
	}


	return D3DXVECTOR3(0.0, 1.0, 0.0);
}



void FPSCamera::AddDirectionX(float magnification)
{
	double def_variation_x = M_PI_180 * 0.2f;
	angleX += def_variation_x * magnification * sensitivity;

	if (M_PI_x2 < angleX)
	{
		angleX -= M_PI_x2;
	}
	if (angleX < 0.0)
	{
		angleX += M_PI_x2;
	}
}


void FPSCamera::AddDirectionY(float magnification)
{
	double def_variation_y = 0.001;
	angleYvalue += def_variation_y * magnification * sensitivity;
	if (angleYvalue > 0.49)
		angleYvalue = 0.49;
	if (angleYvalue < -0.49)
		angleYvalue = -0.49;
}