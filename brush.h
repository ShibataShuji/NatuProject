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


	// �u���V�̃��[�h
	E_CreateStageScene_Mode	   m_BrushMode = E_CreateStageScene_Mode::Object;

	// �X�J���v�g�u���V
	E_UseSculptBrush		   m_UseSculptBrush = E_UseSculptBrush::Brush001;					// �g���Ă���u���V
	E_UseSettingOfSculptBrush  m_UseSettingOfSculptBrush = E_UseSettingOfSculptBrush::Setting001;	// + �� - �Ƃ������g���Ă���u���V�̒��ł��ǂ̐ݒ���g���Ă��邩�B
	PassedSculptBrushState     m_PassedSculptBrushState[static_cast<int>(E_UseSculptBrush::ENDNUM)];	// �֐��ɓn���u���V�̐ݒ���u���V���ƂɎ��B
	// �y�C���g�u���V
	// �y�C���g�u���V�̃Z�b�g���X�g�B1�̃��X�g��4�܂Ńu���V���Z�b�g�ł���B������ő�P�O���p�ӁB
	int			  m_PaintBrushSetList[PaintBrushSetListMax][PaintBrushSetMax];
	int			  m_PaintBrushNow;			// ���ݎg���Ă���u���V�̃��X�g���ł̔ԍ�
	int			  m_TempPaintBrushNow = 0;
	int			  m_PaintBrushNowList;		// ���ݎg���Ă���u���V���X�g�̔ԍ�
	PassedPaintBrushState      m_PassedPaintBrushState[PaintBrushSetListMax][PaintBrushSetMax];		// ���X�g�̐�*���X�g�Ƀu���V��4�����Ă�̂�


public:

	//override �X�y���~�X�Ƃ��A�~�X�h�~�̂��߂ɂ���
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	// �u���V���[�h
	void SetBrushMode(const E_CreateStageScene_Mode& mode) { m_BrushMode = mode; }
	E_CreateStageScene_Mode GetBrushMode() { return m_BrushMode; }
	E_CreateStageScene_Mode* GetpBrushMode() { return &m_BrushMode; }

	// �X�J���v�g�u���V
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


	// �y�C���g�u���V
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






	// ���̃u���V�����Ԗڂ̃��X�g�̉��ڂɃZ�b�g���邩
	void SetPaintBrushSet(const E_PaintBrushType& paintbrushtype, const int listnum, const int num);

	// �ǂ̃��X�g�����w�肵�Ă��̂肷�Ƃ����炤
	int* GetPaintBrushSetList() { return &m_PaintBrushNowList; }


	// ���ǂ̃��X�g�̉��Ԃ̃u���V���g���Ă��邩���Q�b�g����
	int GetPaintBrushNowList() { return m_PaintBrushNowList; }
	int GetPaintBrushNow() { return m_PaintBrushNow; }

	void SetPaintBrushNowList(const int listnum) { m_PaintBrushNowList = listnum; }
	void SetPaintBrushNow(const int brushnum) { m_PaintBrushNow = brushnum; }

	int GetPaintBrushType(const int listnum, const int num);
	E_PaintBrushType GetPaintBrushType_E(const int listnum, const int num);

	// �����S���ɃZ�b�g����
	void SetToEraserPaintBrushNow();
	void SetToTempPaintBrushNow();

};

