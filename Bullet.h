#pragma once

#include "gameObject.h"

class Bullet : public GameObject	// 継承
{
private:

	D3DXVECTOR3 m_Velocity;

	static class Model* m_Model;				// ここもstatic

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:

	// staticにするとインスタンスを作らなくても読んだり使ったりすることができる。
	// これによって初期化の時とか、インスタンスが存在しないときに
	// 呼ぶことができるのでstaticは必須

	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetVelocity(D3DXVECTOR3 vel) { m_Velocity = vel; }




};
