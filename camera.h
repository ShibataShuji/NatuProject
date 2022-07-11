#pragma once

#include "gameObject.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#define	D3DX_PI_HARF	(1.570796327f)
#define	D3DX_PI_QUARTER	(0.785398164f)

class Camera : public GameObject
{
private:

	//D3DXVECTOR3		m_Position;
	D3DXVECTOR3			m_Target;
	//D3DXVECTOR3			m_Rotation;			// カメラの回転

	D3DXMATRIX m_ViewMatrix;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix;}

};

