#pragma once


class Brush : public GameObject
{
private:

	class Model* m_Model;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	E_MouseActiveState  m_MouseActiveState;
	E_MouseTriggerState m_MouseTriggerState;
	D3DXVECTOR3			m_TriggerPosition;


	// ブラシのモード
	E_CreateStageScene_Mode	   m_BrushMode = E_CreateStageScene_Mode::Object;

	// スカルプトブラシ
	E_UseSculptBrush		   m_UseSculptBrush = E_UseSculptBrush::Brush001;					// 使っているブラシ
	E_UseSettingOfSculptBrush  m_UseSettingOfSculptBrush = E_UseSettingOfSculptBrush::Setting001;	// + や - といった使っているブラシの中でもどの設定を使っているか。
	PassedSculptBrushState     m_PassedSculptBrushState[static_cast<int>(E_UseSculptBrush::ENDNUM)];	// 関数に渡すブラシの設定をブラシごとに持つ。
	// ペイントブラシ
	// ペイントブラシのセットリスト。1つのリストに4つまでブラシをセットできる。それを最大１０箱用意。
	int			  m_PaintBrushSetList[PaintBrushSetListMax][PaintBrushSetMax];
	int			  m_PaintBrushNow;			// 現在使っているブラシのリスト内での番号
	int			  m_TempPaintBrushNow = 0;
	int			  m_PaintBrushNowList;		// 現在使っているブラシリストの番号
	PassedPaintBrushState      m_PassedPaintBrushState[PaintBrushSetListMax][PaintBrushSetMax];		// リストの数*リストにブラシが4つ入ってるので


public:

	//override スペルミスとか、ミス防止のためにつける
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	// ブラシモード
	void SetBrushMode(const E_CreateStageScene_Mode& mode) { m_BrushMode = mode; }
	E_CreateStageScene_Mode GetBrushMode() { return m_BrushMode; }
	E_CreateStageScene_Mode* GetpBrushMode() { return &m_BrushMode; }

	// スカルプトブラシ
	// m_UseSculptBrush
	void SetUseSculptBrush(const E_UseSculptBrush& usesculptbrush) { m_UseSculptBrush = usesculptbrush; }
	E_UseSculptBrush GetUseSculptBrush() { return m_UseSculptBrush; }
	E_UseSculptBrush* GetpUseSculptBrush() { return &m_UseSculptBrush; }
	// m_PassedSculptBrushState
	void SetPassedSculptBrushState(const PassedSculptBrushState& passedsculptbrushstate, const E_UseSculptBrush& usesculptbrush) { m_PassedSculptBrushState[static_cast<int>(usesculptbrush)] = passedsculptbrushstate; }
	PassedSculptBrushState GetPassedSculptBrushState(const E_UseSculptBrush& usesculptbrush) { return m_PassedSculptBrushState[static_cast<int>(usesculptbrush)]; }
	PassedSculptBrushState* GetpPassedSculptBrushState(const E_UseSculptBrush& usesculptbrush) { return &m_PassedSculptBrushState[static_cast<int>(usesculptbrush)]; }
	PassedSculptBrushState* GetpPassedSculptBrushStateList() { return m_PassedSculptBrushState; }
	void AddStrengthSculptBrushState(const float& addStr) { m_PassedSculptBrushState[static_cast<int>(m_UseSculptBrush)].Strength += addStr; }
	// m_UseSettingOfSculptBrush
	void SetUseSettingOfSculptBrush(const E_UseSettingOfSculptBrush& usesettingofsculptbrush) { m_UseSettingOfSculptBrush = usesettingofsculptbrush; }
	E_UseSettingOfSculptBrush GetUseSettingOfSculptBrush() { return m_UseSettingOfSculptBrush; }
	E_UseSettingOfSculptBrush* GetpUseSettingOfSculptBrush() { return &m_UseSettingOfSculptBrush; }


	// ペイントブラシ
	//// m_UsePaintBrush
	//void SetUsePaintBrush(const E_UsePaintBrush& usepaintbrush) { m_UsePaintBrush = usepaintbrush; }
	//E_UsePaintBrush GetUsePaintBrush() { return m_UsePaintBrush; }
	//E_UsePaintBrush* GetpUsePaintBrush() { return &m_UsePaintBrush; }
	// m_PassedPaintBrushState
	void SetPassedPaintBrushState(const PassedPaintBrushState& passedpaintbrushstate, const int listnum, const int num) { m_PassedPaintBrushState[listnum][num] = passedpaintbrushstate; }
	PassedPaintBrushState GetPassedPaintBrushState(const int listnum, const int num) { return m_PassedPaintBrushState [listnum][num]; }
	PassedPaintBrushState* GetpPassedPaintBrushState(const int listnum, const int num) { return &m_PassedPaintBrushState[listnum][num]; }
	PassedPaintBrushState* GetpPassedPaintBrushStateList() { return *m_PassedPaintBrushState; }

	void BurushKyeUpdate();

	void SetTriggerPosition(D3DXVECTOR3 const& pos) { m_TriggerPosition = pos; }
	D3DXVECTOR3 GetTriggerPosition() { return m_TriggerPosition; }

	D3DXVECTOR3 CheckRayCollisionMeshFieldObject(const float& maxlength, const float& nohitlength, const D3DXVECTOR3& MouseWorldOrigin, const D3DXVECTOR3& MouseWorldDirection);






	// 何のブラシを何番目のリストの何個目にセットするか
	void SetPaintBrushSet(const E_PaintBrushType& paintbrushtype, const int listnum, const int num);

	// どのリストかを指定してそのりすとをもらう
	int* GetPaintBrushSetList() { return &m_PaintBrushNowList; }


	// 今どのリストの何番のブラシを使っているかをゲットする
	int GetPaintBrushNowList() { return m_PaintBrushNowList; }
	int GetPaintBrushNow() { return m_PaintBrushNow; }

	void SetPaintBrushNowList(const int listnum) { m_PaintBrushNowList = listnum; }
	void SetPaintBrushNow(const int brushnum) { m_PaintBrushNow = brushnum; }

	int GetPaintBrushType(const int listnum, const int num);
	E_PaintBrushType GetPaintBrushType_E(const int listnum, const int num);

	// 消しゴムにセットする
	void SetToEraserPaintBrushNow();
	void SetToTempPaintBrushNow();

};

