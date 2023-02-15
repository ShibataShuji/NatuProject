
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
	// リスト0番の0番を選択中にしておく
	m_PaintBrushNow = 0;
	m_PaintBrushNowList = 0;
	// ブラシの登録はステージ作成シーンでやってる

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

			// 現在のセーブデータにある設定を取得
		Savedata* savedata = Manager::GetSavedata();
		std::vector<std::string> namelist;
		std::vector<int> responselist;
		savedata->Get_Collision_ObjectTypeSaveListData(&namelist, &responselist);
		int responselistsize = responselist.size();
		// デフォルトのものをセットする
		p_Collision_Ray->SetResponseObjectList(responselist);
		for (int n = 0; n < responselistsize; n++)
		{
			p_Collision_Ray->SetResponseObject(n, RESPONSE_BLOCK);
		}
	}


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso"); 

	// ブラシ系の変数の初期化
	m_BrushMode = E_CreateStageScene_Mode::Object;
	// スカルプトブラシ
	m_UseSculptBrush = E_UseSculptBrush::Brush001;
	m_UseSettingOfSculptBrush = E_UseSettingOfSculptBrush::Setting001;
	// ペイントブラシ
	// 上でやってる

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

	// クリックとかの状態を更新
	BurushKyeUpdate();

	bool IsWindowHovered = false;
	IsWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	bool IsAnyItemActive = ImGui::IsAnyItemActive();

	bool GuiHovered = false;
	if (IsWindowHovered || IsAnyItemActive)
		GuiHovered = true;





	// マウスをワールド座標に変換
	D3DXVECTOR3 MouseWorldOrigin;
	D3DXVECTOR3 MouseWorldDirection;
	POINT Sp = GetScreenMousePosition();
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(0);
	camera->ScreenToWorld(&MouseWorldOrigin, &MouseWorldDirection, Sp.x, Sp.y);


	{
		// マウスカーソルとメッシュフィールドオブジェクトとの衝突地点を取っている。
		m_Position = CheckRayCollisionMeshFieldObject(30.0f, 30.0f, MouseWorldOrigin, MouseWorldDirection);
	}


	// ブラシに渡す値の更新
	{
		// スカルプトブラシ
		m_PassedSculptBrushState[static_cast<int>(m_UseSculptBrush)].Position = m_Position;
		m_PassedSculptBrushState[static_cast<int>(m_UseSculptBrush)].TriggerPosition = m_TriggerPosition;
		// ペイントブラシ
		m_PassedPaintBrushState[m_PaintBrushNowList][m_PaintBrushNow].Position = m_Position;
		m_PassedPaintBrushState[m_PaintBrushNowList][m_PaintBrushNow].TriggerPosition = m_TriggerPosition;

	}
	// メッシュフィールドをブラシでいじる！
	{
		if (!GuiHovered)
		{
			if (m_MouseActiveState == E_MouseActiveState::ActiveLeft)
			{
				std::list<MeshFieldObject*> fieldlist = scene->GetDrawMeshFieldObjectList();
				for (auto field : fieldlist)
				{
					// スカルプトモードだったら
					if (m_BrushMode == E_CreateStageScene_Mode::Sculpt)
						field->UseSculptBrushes(m_UseSculptBrush, &m_PassedSculptBrushState[static_cast<int>(m_UseSculptBrush)]);
					// ペイントモードだったら
					else if (m_BrushMode == E_CreateStageScene_Mode::Paint)
						field->UsePaintBrush(m_PaintBrushNowList, m_PaintBrushNow, &m_PassedPaintBrushState[m_PaintBrushNowList][m_PaintBrushNow]);	// &m_PassedPaintBrushState[static_cast<int>(m_UsePaintBrush)]
					//field->UsePaintBrushes(m_UsePaintBrush, &m_PassedPaintBrushState[static_cast<int>(m_UsePaintBrush)]);
				}
			}
			// 右クリックは視点移動と被るため封印
			//if (m_MouseActiveState == E_MouseActiveState::ActiveRight)
			//{
			//	std::list<MeshFieldObject*> fieldlist = scene->GetDrawMeshFieldObjectList();
			//	for (auto field : fieldlist)
			//	{
			//		field->UseSculptBrush001(m_Position, 1.0f, -0.01f);
			//	}
			//}

		}

		// マウスを離した瞬間に法線の更新を行う
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

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリックス設定
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
	//// アクティブじゃないときに、クリックした瞬間にトリガーポジションを更新する。
	//if (Input::IsMouseLeftTriggered() || Input::IsMouseRightTriggered())
	//{
	//	if (m_MouseActiveState == E_MouseActiveState::NotActive)
	//	{
	//		SetTriggerPosition(m_Position);
	//	}
	//}

	// Stateの初期化
	//m_MouseActiveState = E_MouseActiveState::NotActive;

	// トリガーは毎回最初にリセット
	m_MouseTriggerState = E_MouseTriggerState::NotTrriger;

	// 左クリックを長押ししていたら。
	if (Input::IsMouseLeftPressed())
	{
		if (m_MouseActiveState != E_MouseActiveState::ActiveLeft)
		{
			m_MouseTriggerState = E_MouseTriggerState::TriggerONLeft;
			SetTriggerPosition(m_Position);
		}
		m_MouseActiveState = E_MouseActiveState::ActiveLeft;
	}
	// 右クリックを長押ししていたら。
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
		// どっちも押していない場合

		// まずトリガーを取る
		if (m_MouseActiveState == E_MouseActiveState::ActiveLeft)
			m_MouseTriggerState = E_MouseTriggerState::TriggerOFFLeft;
		if (m_MouseActiveState == E_MouseActiveState::ActiveRight)
			m_MouseTriggerState = E_MouseTriggerState::TriggerOFFRight;

		// 最後にアクティブをリセット
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
static BOOL IsMouseLeftPressed(void);      // 左クリックした状態
static BOOL IsMouseLeftTriggered(void);    // 左クリックした瞬間
static BOOL IsMouseRightPressed(void);     // 右クリックした状態
static BOOL IsMouseRightTriggered(void);   // 右クリックした瞬間
static BOOL IsMouseCenterPressed(void);    // 中クリックした状態
static BOOL IsMouseCenterTriggered(void);  // 中クリックした瞬間
*/

// レイを作ってメッシュフィールドオブジェクトとの衝突点をm_Positionにセットする。衝突していなかったら指定した分先の座標が返る
D3DXVECTOR3 Brush::CheckRayCollisionMeshFieldObject(const float& maxlength, const float& nohitlength, const D3DXVECTOR3& MouseWorldOrigin, const D3DXVECTOR3& MouseWorldDirection)
{
	Scene* scene = Manager::GetScene();

	//float MaxLength = 30.0f;	// Rayの最大長さ
	int	  MaxDiv = 100;			// 分割数		(MaxLength / MaxDiv)がPanelSizeを下回らなければ大丈夫
	D3DXVECTOR3 DivValue = (maxlength / MaxDiv) * MouseWorldDirection;	// 1分割の大きさ。これを足していく。

	//m_Position = MouseWorldOrigin + MaxLength * MouseWorldDirection;

	// 方向が上方向か下方向に進んでいるかを決めておく。
	bool DirectUp = false; {}
	if (MouseWorldDirection.y > 0)
		DirectUp = true;

	// メッシュフィールドとの当たり判定を手前からとっていく。Ray。
	// その時のxzを使って、そのブロックの高さを求めて、yを比較してRayの方が低い(高い)位置にいたら衝突。
	D3DXVECTOR3 nowPos = MouseWorldOrigin;
	bool col = false;
	for (int div = 0; div <= MaxDiv; div++)
	{
		// メッシュフィールド高さ取得
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

		// 最後に次に向けて座標を加算する
		nowPos += DivValue;
	}

	// 衝突していたらそこの座標を。ひっとしていなかったら設定した遠さの座標を返す
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
		m_TempPaintBrushNow = m_PaintBrushNow;		// 添付に保存して消しゴムに変える
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