
#include "stdafx.h"

void Brush::Init()
{
	for (int i = 0; i < PaintBrushSetListMax; i++)
	{
		for (int j = 0; j < PaintBrushSetMax; j++)
		{
			m_PaintBrushSetList[i][j] = -1;
		}
	}
	// ���X�g0�Ԃ�0�Ԃ�I�𒆂ɂ��Ă���
	m_PaintBrushNow = 0;
	m_PaintBrushNowList = 0;
	// �u���V�̓o�^�̓X�e�[�W�쐬�V�[���ł���Ă�

	m_MouseActiveState = E_MouseActiveState::NotActive;
	m_MouseTriggerState = E_MouseTriggerState::NotTrriger;

	m_Model = new Model();
	m_Model->Load("asset\\model\\CollisionModel\\sphere.obj");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, -3.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// Ray
	{
		Collision* p_Collision_Ray = AddComponent<Collision>("Ray");
		p_Collision_Ray->SetBeMoved(false);
		p_Collision_Ray->SetCollisionType(RAY_COLLISION);
		//p_Collision_Ray->LoadCollisionModel();

			// ���݂̃Z�[�u�f�[�^�ɂ���ݒ���擾
		Savedata* savedata = Manager::GetSavedata();
		std::vector<std::string> namelist;
		std::vector<int> responselist;
		savedata->Get_Collision_ObjectTypeSaveListData(&namelist, &responselist);
		int responselistsize = responselist.size();
		// �f�t�H���g�̂��̂��Z�b�g����
		p_Collision_Ray->SetResponseObjectList(responselist);
		for (int n = 0; n < responselistsize; n++)
		{
			p_Collision_Ray->SetResponseObject(n, RESPONSE_BLOCK);
		}
	}


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso"); 

	// �u���V�n�̕ϐ��̏�����
	m_BrushMode = E_CreateStageScene_Mode::Object;
	// �X�J���v�g�u���V
	m_UseSculptBrush = E_UseSculptBrush::Brush001;
	m_UseSettingOfSculptBrush = E_UseSettingOfSculptBrush::Setting001;
	// �y�C���g�u���V
	// ��ł���Ă�

}

void Brush::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Brush::Update()
{
	if (m_BrushMode == E_CreateStageScene_Mode::Object)
		return;

	// �N���b�N�Ƃ��̏�Ԃ��X�V
	BurushKyeUpdate();

	bool IsWindowHovered = false;
	IsWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	bool IsAnyItemActive = ImGui::IsAnyItemActive();

	bool GuiHovered = false;
	if (IsWindowHovered || IsAnyItemActive)
		GuiHovered = true;





	// �}�E�X�����[���h���W�ɕϊ�
	D3DXVECTOR3 MouseWorldOrigin;
	D3DXVECTOR3 MouseWorldDirection;
	POINT Sp = GetScreenMousePosition();
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(0);
	camera->ScreenToWorld(&MouseWorldOrigin, &MouseWorldDirection, Sp.x, Sp.y);


	{
		// �}�E�X�J�[�\���ƃ��b�V���t�B�[���h�I�u�W�F�N�g�Ƃ̏Փ˒n�_������Ă���B
		m_Position = CheckRayCollisionMeshFieldObject(30.0f, 30.0f, MouseWorldOrigin, MouseWorldDirection);
	}


	// �u���V�ɓn���l�̍X�V
	{
		// �X�J���v�g�u���V
		m_PassedSculptBrushState[static_cast<int>(m_UseSculptBrush)].Position = m_Position;
		m_PassedSculptBrushState[static_cast<int>(m_UseSculptBrush)].TriggerPosition = m_TriggerPosition;
		// �y�C���g�u���V
		m_PassedPaintBrushState[m_PaintBrushNowList][m_PaintBrushNow].Position = m_Position;
		m_PassedPaintBrushState[m_PaintBrushNowList][m_PaintBrushNow].TriggerPosition = m_TriggerPosition;

	}
	// ���b�V���t�B�[���h���u���V�ł�����I
	{
		if (!GuiHovered)
		{
			if (m_MouseActiveState == E_MouseActiveState::ActiveLeft)
			{
				std::list<MeshFieldObject*> fieldlist = scene->GetDrawMeshFieldObjectList();
				for (auto field : fieldlist)
				{
					// �X�J���v�g���[�h��������
					if (m_BrushMode == E_CreateStageScene_Mode::Sculpt)
						field->UseSculptBrushes(m_UseSculptBrush, &m_PassedSculptBrushState[static_cast<int>(m_UseSculptBrush)]);
					// �y�C���g���[�h��������
					else if (m_BrushMode == E_CreateStageScene_Mode::Paint)
						field->UsePaintBrush(m_PaintBrushNowList, m_PaintBrushNow, &m_PassedPaintBrushState[m_PaintBrushNowList][m_PaintBrushNow]);	// &m_PassedPaintBrushState[static_cast<int>(m_UsePaintBrush)]
					//field->UsePaintBrushes(m_UsePaintBrush, &m_PassedPaintBrushState[static_cast<int>(m_UsePaintBrush)]);
				}
			}
			// �E�N���b�N�͎��_�ړ��Ɣ�邽�ߕ���
			//if (m_MouseActiveState == E_MouseActiveState::ActiveRight)
			//{
			//	std::list<MeshFieldObject*> fieldlist = scene->GetDrawMeshFieldObjectList();
			//	for (auto field : fieldlist)
			//	{
			//		field->UseSculptBrush001(m_Position, 1.0f, -0.01f);
			//	}
			//}

		}

		// �}�E�X�𗣂����u�Ԃɖ@���̍X�V���s��
		if (m_MouseTriggerState == E_MouseTriggerState::TriggerOFFLeft || m_MouseTriggerState == E_MouseTriggerState::TriggerOFFRight)
		{
			std::list<MeshFieldObject*> fieldlist = scene->GetDrawMeshFieldObjectList();
			for (auto field : fieldlist)
			{
				field->UpdataMeshFieldObjectNormal();
			}
		}
	}
}

void Brush::Draw()
{
	if (m_BrushMode == E_CreateStageScene_Mode::Object)
		return;

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���b�N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;


	Renderer::SetWorldMatrix(&world);


	m_Model->Draw();
}


void Brush::BurushKyeUpdate()
{
	//// �A�N�e�B�u����Ȃ��Ƃ��ɁA�N���b�N�����u�ԂɃg���K�[�|�W�V�������X�V����B
	//if (Input::IsMouseLeftTriggered() || Input::IsMouseRightTriggered())
	//{
	//	if (m_MouseActiveState == E_MouseActiveState::NotActive)
	//	{
	//		SetTriggerPosition(m_Position);
	//	}
	//}

	// State�̏�����
	//m_MouseActiveState = E_MouseActiveState::NotActive;

	// �g���K�[�͖���ŏ��Ƀ��Z�b�g
	m_MouseTriggerState = E_MouseTriggerState::NotTrriger;

	// ���N���b�N�𒷉������Ă�����B
	if (Input::IsMouseLeftPressed())
	{
		if (m_MouseActiveState != E_MouseActiveState::ActiveLeft)
		{
			m_MouseTriggerState = E_MouseTriggerState::TriggerONLeft;
			SetTriggerPosition(m_Position);
		}
		m_MouseActiveState = E_MouseActiveState::ActiveLeft;
	}
	// �E�N���b�N�𒷉������Ă�����B
	else if (Input::IsMouseRightPressed())
	{
		if (m_MouseActiveState != E_MouseActiveState::ActiveRight)
		{
			m_MouseTriggerState = E_MouseTriggerState::TriggerONRight;
			SetTriggerPosition(m_Position);
		}
		m_MouseActiveState = E_MouseActiveState::ActiveRight;
	}
	else
	{
		// �ǂ����������Ă��Ȃ��ꍇ

		// �܂��g���K�[�����
		if (m_MouseActiveState == E_MouseActiveState::ActiveLeft)
			m_MouseTriggerState = E_MouseTriggerState::TriggerOFFLeft;
		if (m_MouseActiveState == E_MouseActiveState::ActiveRight)
			m_MouseTriggerState = E_MouseTriggerState::TriggerOFFRight;

		// �Ō�ɃA�N�e�B�u�����Z�b�g
		m_MouseActiveState = E_MouseActiveState::NotActive;

	}





	enum class E_MouseTriggerState
	{
		NotTrriger,
		TriggerONLeft,
		TriggerONRight,
		TriggerOFFLeft,
		TriggerOFFRight
	};

}

/*
static BOOL IsMouseLeftPressed(void);      // ���N���b�N�������
static BOOL IsMouseLeftTriggered(void);    // ���N���b�N�����u��
static BOOL IsMouseRightPressed(void);     // �E�N���b�N�������
static BOOL IsMouseRightTriggered(void);   // �E�N���b�N�����u��
static BOOL IsMouseCenterPressed(void);    // ���N���b�N�������
static BOOL IsMouseCenterTriggered(void);  // ���N���b�N�����u��
*/

// ���C������ă��b�V���t�B�[���h�I�u�W�F�N�g�Ƃ̏Փ˓_��m_Position�ɃZ�b�g����B�Փ˂��Ă��Ȃ�������w�肵������̍��W���Ԃ�
D3DXVECTOR3 Brush::CheckRayCollisionMeshFieldObject(const float& maxlength, const float& nohitlength, const D3DXVECTOR3& MouseWorldOrigin, const D3DXVECTOR3& MouseWorldDirection)
{
	Scene* scene = Manager::GetScene();

	//float MaxLength = 30.0f;	// Ray�̍ő咷��
	int	  MaxDiv = 100;			// ������		(MaxLength / MaxDiv)��PanelSize�������Ȃ���Α��v
	D3DXVECTOR3 DivValue = (maxlength / MaxDiv) * MouseWorldDirection;	// 1�����̑傫���B����𑫂��Ă����B

	//m_Position = MouseWorldOrigin + MaxLength * MouseWorldDirection;

	// ��������������������ɐi��ł��邩�����߂Ă����B
	bool DirectUp = false; {}
	if (MouseWorldDirection.y > 0)
		DirectUp = true;

	// ���b�V���t�B�[���h�Ƃ̓����蔻�����O����Ƃ��Ă����BRay�B
	// ���̎���xz���g���āA���̃u���b�N�̍��������߂āAy���r����Ray�̕����Ⴂ(����)�ʒu�ɂ�����ՓˁB
	D3DXVECTOR3 nowPos = MouseWorldOrigin;
	bool col = false;
	for (int div = 0; div <= MaxDiv; div++)
	{
		// ���b�V���t�B�[���h�����擾
		Int2 block;
		float meshHeight = scene->GetMeshFieldObjectHeight(&block, nowPos);

		if (DirectUp)
		{
			if (nowPos.y > meshHeight)
			{
				col = true;
				break;
			}
		}
		else
		{
			if (nowPos.y < meshHeight)
			{
				col = true;
				break;
			}
		}

		// �Ō�Ɏ��Ɍ����č��W�����Z����
		nowPos += DivValue;
	}

	// �Փ˂��Ă����炻���̍��W���B�Ђ��Ƃ��Ă��Ȃ�������ݒ肵�������̍��W��Ԃ�
	if (col)
		return nowPos;
	else
		return MouseWorldOrigin + (nohitlength * MouseWorldDirection);

	return nowPos;
}

void Brush::SetPaintBrushSet(const E_PaintBrushType& paintbrushtype, const int listnum, const int num)
{
	m_PaintBrushSetList[listnum][num] = static_cast<int>(paintbrushtype);
	return;
}


int Brush::GetPaintBrushType(const int listnum, const int num)
{
	return m_PaintBrushSetList[listnum][num];
}

E_PaintBrushType Brush::GetPaintBrushType_E(const int listnum, const int num)
{
	return static_cast<E_PaintBrushType>(m_PaintBrushSetList[listnum][num]);
}


void Brush::SetToEraserPaintBrushNow()
{
	if (m_PaintBrushNow != 4)
	{
		m_TempPaintBrushNow = m_PaintBrushNow;		// �Y�t�ɕۑ����ď����S���ɕς���
		m_PaintBrushNow = 4;
	}
	return;
}

// 
void Brush::SetToTempPaintBrushNow()
{
	if (m_PaintBrushNow == 4)
	{
		m_PaintBrushNow = m_TempPaintBrushNow;
	}
	return;
}