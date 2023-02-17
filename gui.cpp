
#include "stdafx.h"

// Comboをstringで簡単に扱うための自作関数
static bool ImGuiComboUI(const std::string& caption, std::string& current_item, int& current_itemNum,
	const std::vector<std::string>& items) 
{
	// const をつけるとこの関数内で書き換えることができなくなる。

	bool changed = false;

	if (ImGui::BeginCombo(caption.c_str(), current_item.c_str())) 
	{
		for (int n = 0; n < items.size(); n++) 
		{
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n].c_str(), is_selected)) 
			{
				current_item = items[n];
				current_itemNum = n;
				changed = true;
			}
			if (is_selected) {
				// Set the initial focus when opening the combo (scrolling + for
				// keyboard navigation support in the upcoming navigation branch)
				//ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	return changed;
}

void Gui::TestGUI(const std::string& caption, std::string& current_item,
	std::vector<std::string>& items) noexcept
{
	ImGui::Begin("TestGUI");

	Scene* scene = Manager::GetScene();
	Savedata* savedata = Manager::GetSavedata();

	// 自分がどのオブジェクトタイプかを選ぶところ----Start

	// ゲット用
	std::vector<std::string> ObjectTypeNameList;
	std::vector<int> DefaultResponseList;
	int ObjectTypeNum;

	savedata->Get_Collision_ObjectTypeSaveListData(&ObjectTypeNameList, &DefaultResponseList);
	ObjectTypeNum = ObjectTypeNameList.size();	// オブジェクトタイプの種類数

	// 表示用。初期設定
	ImGui::Text("Object Type"); ImGui::SameLine();
	static std::string Ccaption = "##";
	static int		   Ccurrent_itemNum = 0;
	static std::string Ccurrent_item = ObjectTypeNameList[Ccurrent_itemNum];

	ImGuiComboUI(Ccaption, Ccurrent_item, Ccurrent_itemNum, ObjectTypeNameList);

	// 自分がどのオブジェクトタイプかを選ぶところ----End

	// スペースを空ける
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::Text("Ignore"); ImGui::SameLine();
	ImGui::Text("Overlap"); ImGui::SameLine();
	ImGui::Text("Block");

	// オブジェクトタイプに対する反応の設定---Start
	for (int n = 0; n < ObjectTypeNum; n++)
	{
		static int mode = 0;

		enum {
			MODE_1,
			MODE_2,
			MODE_3,
		};

		
		ImGui::RadioButton("##", &mode, MODE_1); ImGui::SameLine(0.0f, 35.0f);
		ImGui::RadioButton("##", &mode, MODE_2); ImGui::SameLine(0.0f, 35.0f);
		ImGui::RadioButton("##", &mode, MODE_3); ImGui::SameLine();
		ImGui::Text(ObjectTypeNameList[n].c_str());




	}

	ImGui::End();

}



void Gui::DebugStorage() noexcept
{
	//// ウィンドウの色を決める
	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	//-----ウィンドウの場所を決める
	ImGui::SetNextWindowPos({ 0, 0 });
	//-----ウィンドウのサイズを決める, 第2引数になんか入れないとサイズ変わってくれないんだけど、まだよくわからん
	//ImGui::SetNextWindowSize({ WIDTH, HEIGHT });	
	ImGui::SetNextWindowSize({ 500, 500 });

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		//-----ウィンドウ1つ作る,名前決めれるよ
		ImGui::Begin("DebugVV");

		ImVec2 aaa =  ImGui::GetWindowSize();

		//ImGui::Text("This is some useful text.");
		//ImGui::Checkbox("Demo Window", &show_demo_window);
		//ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		//ImGui::ColorEdit3("clear color", (float*)&clear_color);

		//if (ImGui::Button("Button"))
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//-----このウィンドウは終わり
		ImGui::End();

		//-----またウィンドウ作りたかったらこんな感じに増やしてく
		//ImGui::Begin("aaa");
		//ImGui::End();

	}
}

void Gui::DestroyImGui() noexcept
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}




float Gui::DebugA() noexcept
{
	//gl::clear(Color(0, 0, 0));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	//ImGui::SetNextWindowPos(ImVec2(20, 20), 0);	// ImGuiSetCond_Once
	//ImGui::SetNextWindowSize(ImVec2(280, 300), 0); // ImGuiSetCond_Once

	ImGui::Begin("config 1BFGEG");

	ImVec2 bbb = ImGui::GetWindowPos();
	ImGui::Text("GetWindowPose %f, %f", bbb.x, bbb.y);

	static std::vector<float> items(10, 1.0f);	// 初期で配列10個、初期値全部1.0f

	if (ImGui::Button("add")) {
		items.push_back(1.0f);
	}
	if (ImGui::Button("remove")) {
		if (items.empty() == false) {
			items.pop_back();
		}
	}

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
	for (int i = 0; i < items.size(); ++i) {
		char name[16];
		sprintf_s(name, "item %d", i);
		ImGui::SliderFloat(name, &items[i], 0.1f, 10.0f);
	}
	ImGui::EndChild();

	ImGui::End();

	return items[0];

	//ImGui::PopStyleColor();
	//ImGui::PopStyleColor();
}


std::string Gui::ObjectChoiceGUI(std::list<std::string> LoadedObjectNameList) noexcept
{
	//gl::clear(Color(0, 0, 0));

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	//ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(280, 300), ImGuiSetCond_Once);

	ImGui::Begin("config 2");

	static int mode = 0;


	
	// 選択するチェックボックスの表示
	int mode_count = 0;
	for (auto& objname : LoadedObjectNameList)
	{
		ImGui::RadioButton(objname.c_str(), &mode, mode_count);
		mode_count++;
	}

	// 選択しているのはこれだよの表示
	int mode_count_result = 0;
	for (auto& objname : LoadedObjectNameList)
	{
		if (mode == mode_count_result)
		{
			std::string display = "Selecting : " + objname;
			ImGui::Text(display.c_str());
			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			return objname;
		}
		mode_count_result++;
	}

	

	/*ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();*/
}


bool Gui::CreateNewObjectGUI(GameObject* gameobject) noexcept
{
	Scene* scene = Manager::GetScene();
	Savedata* save = Manager::GetSavedata();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("CreateNewObjectGUI", nullptr, ImGuiWindowFlags_MenuBar);

	// タイトルに戻るボタン
	ImVec2 winSize = ImGui::GetWindowSize();
	float CloseStartX = winSize.x - 110.0f;
	if (CloseStartX < 0.0f)
		CloseStartX = 0.0f;
	ImGui::SameLine(CloseStartX);
	if (ImGui::Button("Back to Title"))
	{
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		return true;
	}

	static bool Loading = false;		// static にすることでグローバル変数のように、変数の内容が保持される

	// メニューバー
	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) 
			{

				char objname[64];
				gameobject->GetObjectNameChar64(objname);
				save->OutputErrorMessages(objname);

				if (save->CheckSaveObjectName(objname))
				{
					save->OutputErrorMessages(" True ");

					// セーブしようとしたときに同じものがあった場合、上書きする。
					GameObject* loadedobj = save->GetLoadedObjectWithName(objname);
					if (loadedobj == nullptr)
					{
						ImGui::End();
						return true;
					}

					// 一度コンポーネントなどをリセットする
					save->OutputErrorMessages(" DesCompListS ");
					loadedobj->DestroyComponentList();
					save->OutputErrorMessages(" DesCompListE ");


					// 現在のものをコピーする
					save->OutputErrorMessages(" CpyGObjS ");
					loadedobj->CopyGameObject(gameobject);
					save->OutputErrorMessages(" CpyGObjE ");

					save->OutputErrorMessages(" CpyCompS ");
					loadedobj->CopyComponentListToAdd(gameobject);
					save->OutputErrorMessages(" CpyCompE ");


					save->SaveObject();
				}
				else
				{
					save->OutputErrorMessages(" False ");

					// なかった場合。新規で追加。
					GameObject* addObj = save->AddLoadedObject<LoadedObject>(1);
					int fr = save->GetLoadedObjectNum();


					addObj->CopyGameObject(gameobject);
					addObj->CopyComponentListToAdd(gameobject);

					char addname[64];
					addObj->GetObjectNameChar64(addname);
					//savedata->CheckSaveObjectList(addObj);

					//
					save->SaveObject();

					//savedata->UnLoad_LoadedObjectList();
					//savedata->LoadObject();
				}
				save->OutputErrorMessages(" End\n");

			}

			if (ImGui::MenuItem("Load")) 
			{
				Loading = true;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// オブジェクトの名前の表示と更新
	char ObjectName[64] = "";
	gameobject->GetObjectNameChar64(ObjectName);
	ImGui::InputText("ObjectName##", ObjectName, sizeof(ObjectName));
	gameobject->SetObjectName(ObjectName);


	// バウンディングボックスの大きさの設定
	{
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		float dragspeed = 0.005f;
		ImGui::Text("Simple BoundingBox3D");
		float tradius = gameobject->GetSimpleBoundingBox3DRadius();
		ImGui::DragFloat("Size", &tradius, dragspeed);
		gameobject->SetSimpleBoundingBox3DRadius(tradius);

		// スペースを空ける
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}


	// コンポーネントの表示
	ImGui::Text("Component");

	int cmpcount = 0;
	std::list<CComponent*> compList = gameobject->GetComponentList();
	for (auto comp : compList)
	{
		
		std::string componentName;

		// コリジョンの場合
		Collision* collision = dynamic_cast<Collision*>(comp);
		if (collision != nullptr)
		{
			componentName = "Collision " + to_string(cmpcount);
			if (ImGui::TreeNode(componentName.c_str()))
			{

				float dragspeed = 0.005f;

				// コリジョンタイプの設定
				int SelectCollisionType = collision->GetCollisionType();
				ImGui::Combo("Collision Model Type", &SelectCollisionType, "Box\0Capsule\0Sphere\0\0");
				collision->SetCollisionType(SelectCollisionType);

				// Offsetの設定
				if (ImGui::TreeNode("Offset"))
				{
					D3DXVECTOR3 Offset = collision->GetColInitOffset();
					D3DXVECTOR3 tempOffset = Offset;
					ImGui::DragFloat("Offset X", &tempOffset.x, dragspeed);
					ImGui::DragFloat("Offset Y", &tempOffset.y, dragspeed);
					ImGui::DragFloat("Offset Z", &tempOffset.z, dragspeed);
					collision->SetColInitOffset(tempOffset);

					ImGui::TreePop();
				}
				// スケールの設定
				if (ImGui::TreeNode("Scale"))
				{
					D3DXVECTOR3 Scale = collision->GetColInitScale();
					D3DXVECTOR3 tempScale = Scale;
					if (SelectCollisionType == BOX_COLLISION)
					{
						ImGui::DragFloat("Scale X", &tempScale.x, dragspeed);
						ImGui::DragFloat("Scale Y", &tempScale.y, dragspeed);
						ImGui::DragFloat("Scale Z", &tempScale.z, dragspeed);
					}
					else if (SelectCollisionType == CAPSULE_COLLISION)
					{
						ImGui::DragFloat("Radius", &tempScale.x, dragspeed);
						ImGui::DragFloat("Height", &tempScale.y, dragspeed);
						if (tempScale.x > tempScale.y + 0.1f)
							tempScale.x = tempScale.y;
						tempScale.z = tempScale.x;
					}
					else if (SelectCollisionType == SPHERE_COLLISION)
					{
						ImGui::DragFloat("Radius", &tempScale.x, dragspeed);
						tempScale.y = tempScale.x;
						tempScale.z = tempScale.x;
					}
					collision->SetColInitScale(tempScale);

					ImGui::TreePop();
				}
				// 回転の設定
				if (ImGui::TreeNode("Rotation"))
				{
					D3DXVECTOR3 Rotation = collision->GetColInitRotation();
					D3DXVECTOR3 tempRotation = Rotation;
					ImGui::DragFloat("Rotation X", &tempRotation.x, dragspeed);
					ImGui::DragFloat("Rotation Y", &tempRotation.y, dragspeed);
					ImGui::DragFloat("Rotation Z", &tempRotation.z, dragspeed);
					collision->SetColInitRotation(tempRotation);

					ImGui::TreePop();
				}
				
				// コリジョンのレスポンスの設定
				CollisionResponseGUI(collision);

				// スペースを空ける
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				std::string destroyname = "Destroy " + to_string(cmpcount);
				if (ImGui::Button(destroyname.c_str()))
				{
					gameobject->DestroyComponent(cmpcount);

					ImGui::TreePop();
					goto for_exit;
				}

				ImGui::TreePop();
			}
			cmpcount++;
			// スペースを空ける
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			continue;
		}

		Rigidbody* rigidbody = dynamic_cast<Rigidbody*>(comp);
		if (rigidbody != nullptr)
		{
			componentName = "Rigidbody " + to_string(cmpcount);
			if (ImGui::TreeNode(componentName.c_str()))
			{
				// スペースを空ける
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				std::string destroyname = "Destroy " + to_string(cmpcount);
				if (ImGui::Button(destroyname.c_str()))
				{
					gameobject->DestroyComponent(cmpcount);

					ImGui::TreePop();
					goto for_exit;
				}
				ImGui::TreePop();
			}
			cmpcount++;
			// スペースを空ける
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			continue;
		}

		// モデルコンポーネントの場合
		ModelComponent* modelcomp = dynamic_cast<ModelComponent*>(comp);
		if (modelcomp != nullptr)
		{
			componentName = "Model " + to_string(cmpcount);
			if (ImGui::TreeNode(componentName.c_str()))
			{
				// セーブデータから読み込んでいるモデルのリストをもらう
				Savedata* savedata = Manager::GetSavedata();
				std::list<Model*> modellist = savedata->GetLoadedModelList();
				int modelnum = savedata->GetLoadedModelNum();
				char items[100][64]{};		// モデルを最大100個まで読み込めるようにしている。本当はモデルの数文指定したかった

				int modelcount = 0;
				for (auto model : modellist)
				{
					char tempName[64];	// モデルの名前がchar型で入る
					std::char_traits<char>::copy(tempName, model->GetModelName().c_str(), model->GetModelName().size() + 1);

					strcat_s(items[modelcount], tempName);

					modelcount++;
				}

				/*char getmodelname[64];
				gameobject->GetModelName(getmodelname);*/
				std::string modelname = modelcomp->GetModelName();
				
				static std::string previewValue = modelname;	// クリックしたアイテム名が入る
				bool selected[100]{};
				if (ImGui::BeginCombo("Model Name", modelname.c_str()))
				{
					// 表示数は100までいけるが、nullptrになってしまうのでmodelcountの分だけやる
					for (size_t i = 0; i < modelcount; i++)
					{
						selected[i] = false;	// 毎回リセット
						ImGui::Selectable(items[i], &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_None);
						if (selected[i])
						{
							// クリックしたなら(クリックした瞬間のみ実行される)
							previewValue = items[i];	// 現在のアイテムの更新

							/*char newmodelname[64];
							StringToChar64(previewValue, newmodelname);
							gameobject->SetModelName(newmodelname);*/				// モデル名の更新
							modelcomp->SetModelName(previewValue);
						}
					}
					ImGui::EndCombo();
				}

				// モデルのテクスチャーに使う色の設定
				{
					bool usecolor = modelcomp->GetSynthesizeColorUse();
					ImGui::Checkbox("Add Color to Texture", &usecolor);
					modelcomp->SetSynthesizeColorUse(usecolor);
					if (usecolor)
					{
						D3DXVECTOR4 color = modelcomp->GetSynthesizeColor();
						color = color;
						float col[4]{ color.x , color.y, color.z, color.w};
						ImGui::ColorPicker4("color picker", col);
						//ImGui::ColorPicker3("color picker", col);
						//ImGui::DragFloat("A : ", &col[3], 1.0f);
						//if (col[3] < 0.0)
						//	col[3] = 0.0;
						//if (col[3] > 255.0)
						//	col[3] = 255.0;
						color.x = col[0];
						color.y = col[1];
						color.z = col[2];
						color.w = col[3];
						modelcomp->SetSynthesizeColor(color);
					}
				}

				float dragspeed = 0.005f;

				// Offsetの設定
				if (ImGui::TreeNode("Offset"))
				{
					D3DXVECTOR3 Offset = modelcomp->GetModelOffset();
					D3DXVECTOR3 tempOffset = Offset;
					ImGui::DragFloat("Offset X", &tempOffset.x, dragspeed);
					ImGui::DragFloat("Offset Y", &tempOffset.y, dragspeed);
					ImGui::DragFloat("Offset Z", &tempOffset.z, dragspeed);

					// カメラの方向に応じて座標変えれるバージョン
					{
						ImGui::Text("CameraDirection Offset");

						// yの移動を無視するかどうか
						static bool IgnoreUp = true;
						ImGui::Checkbox("Ignore Y", &IgnoreUp);
						ImGui::SameLine();
						// 軸に平行して移動するかどうか
						static bool ParallelToAxis = false;
						ImGui::Checkbox("Parallel to axis", &ParallelToAxis);

						Scene* scene = Manager::GetScene();
						Camera* camera = scene->GetCameraObject();
						D3DXVECTOR3 CameraForward;
						D3DXVECTOR3 CameraRight;
						if (ParallelToAxis)
						{
							IgnoreUp = true;
							CameraForward = camera->GetCameraForwardParallelToAxis();
							CameraRight = camera->GetCameraRightParallelToAxis();
						}
						else
						{
							CameraForward = camera->GetCameraForward();
							CameraRight = camera->GetCameraRight();
						}
						if (IgnoreUp)
						{
							CameraForward.y = 0.0f;
							CameraRight.y = 0.0f;
							D3DXVec3Normalize(&CameraForward, &CameraForward);
							D3DXVec3Normalize(&CameraRight, &CameraRight);
						}
						float addForwardScale = 0.0f;
						float addRightScale = 0.0f;
						ImGui::DragFloat("ADD Forward", &addForwardScale, dragspeed);
						ImGui::DragFloat("ADD Right", &addRightScale, dragspeed);
						D3DXVECTOR3 addForward = CameraForward * addForwardScale;
						D3DXVECTOR3 addRight = CameraRight * addRightScale;
						tempOffset += addForward;
						tempOffset += addRight;
					}
					modelcomp->SetModelOffset(tempOffset);

					ImGui::TreePop();
				}
				// スケールの設定
				if (ImGui::TreeNode("Scale"))
				{
					D3DXVECTOR3 Scale = modelcomp->GetModelScale();
					D3DXVECTOR3 tempScale = Scale;
					ImGui::DragFloat("Scale X", &tempScale.x, dragspeed);
					ImGui::DragFloat("Scale Y", &tempScale.y, dragspeed);
					ImGui::DragFloat("Scale Z", &tempScale.z, dragspeed);
					modelcomp->SetModelScale(tempScale);

					ImGui::TreePop();
				}
				// 回転の設定
				if (ImGui::TreeNode("Rotation"))
				{
					D3DXVECTOR3 Rotation = modelcomp->GetModelRotation();
					D3DXVECTOR3 tempRotation = Rotation;
					ImGui::DragFloat("Rotation X", &tempRotation.x, dragspeed);
					ImGui::DragFloat("Rotation Y", &tempRotation.y, dragspeed);
					ImGui::DragFloat("Rotation Z", &tempRotation.z, dragspeed);
					modelcomp->SetModelRotation(tempRotation);

					ImGui::TreePop();
				}

				// スペースを空ける
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				std::string destroyname = "Destroy " + to_string(cmpcount);
				if (ImGui::Button(destroyname.c_str()))
				{
					gameobject->DestroyComponent(cmpcount);

					ImGui::TreePop();
					goto for_exit;
				}
				// スペースを空ける
				ImGui::Dummy(ImVec2(0.0f, 20.0f));

				ImGui::TreePop();
			}
			cmpcount++;
			// スペースを空ける
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			continue;
		}

		// スペースを空ける
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		cmpcount++;
	}
for_exit:

	// スペースを空ける
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	
	// コンポーネントの追加ボタン
	ImGui::Text("Add Component");
	const char* itemsComp[] = { "Please Select", "Collision", "RigidBody", "Model" };
	static int itemsComp_current = 0;	// static にして選択情報を保持
	ImGui::Combo("Select", &itemsComp_current, itemsComp, IM_ARRAYSIZE(itemsComp));

	ImGui::SameLine();
	if (ImGui::Button("Add Component Button")) 
	{
		if (itemsComp_current == 0)
		{ }
		else if (itemsComp_current == 1)
		{
			Collision* collision = gameobject->AddComponent<Collision>();

			// コリジョンの場合最初にリストを追加しなければならない。Initでは上書きされるのでダメ
			// コリジョンレスポンスのvectorのサイズの整合性をとる。初期化。
			std::vector<std::string> ObjectTypeNameList;
			std::vector<int> DefaultResponseList;
			int ObjectTypeNum;

			Manager::GetSavedata()->Get_Collision_ObjectTypeSaveListData(&ObjectTypeNameList, &DefaultResponseList);
			ObjectTypeNum = ObjectTypeNameList.size();	// オブジェクトタイプの種類数
			for (int n = 0; n < ObjectTypeNum; n++)
			{
				collision->AddResponseObjectList(DefaultResponseList[n]);
			}
			// コリジョンが見えるかどうかを現在のシーンの設定に合わせる
			collision->SetVisibility(scene->GetCollision_Visility());

		}
		else if (itemsComp_current == 2)
			gameobject->AddComponent<Rigidbody>();
		else if (itemsComp_current == 3)
			gameobject->AddComponent<ModelComponent>();
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	if (Loading)
	{
		if (SelectLoadObject(gameobject))
		{
			// 選択されたらロードの表示を消す
			Loading = false;

		}
	}
	return false;
}


// オブジェクト追加シーンのとこのLoadを推したときに出てくるやつ
bool Gui::SelectLoadObject(GameObject* gameobject) noexcept
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("Select  Load Object");

	// 閉じるボタン
	ImVec2 winSize = ImGui::GetWindowSize();
	float CloseStartX = winSize.x - 60.0f;
	if (CloseStartX < 0.0f)
		CloseStartX = 0.0f;
	ImGui::SameLine(CloseStartX);
	if (ImGui::Button("Close"))
	{
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return true;
	}

	// スペースを空ける
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	ImGui::Text("Object List");

	Savedata* savedata = Manager::GetSavedata();
	//savedata->SaveObject();

	bool result;
	std::list<GameObject*> loadedobjectlist = savedata->GetLoadedObjectList(&result);
	if (!result)
	{
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return true;
	}

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
	for (auto loadedobject : loadedobjectlist)
	{
		if (loadedobject == nullptr)
			int ddd = 4;
		char objnameC[64];
		loadedobject->GetObjectNameChar64(objnameC);
		if (ImGui::Button(objnameC))
		{
			gameobject->DestroyComponentList();		// 中身を更新するために一度中身を消す

			gameobject->CopyGameObject(loadedobject);
			gameobject->CopyComponentListToAdd(loadedobject);


			ImGui::EndChild();
			ImGui::End();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			return true;
		}
	}
	ImGui::EndChild();




	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	return false;
}




// ステージ追加モードのやつ。返り値はタイトルに戻るボタン
GameObject* Gui::CreateNewStageGUI(E_CreateStageScene_Mode* mode, bool* pOut_backTitle, std::string* pOut_pickupname, int* pOut_objectnum, GameObject* pickupobject) noexcept
{
	Scene* scene = Manager::GetScene();
	Savedata* save = Manager::GetSavedata();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("CreateNewStageGUI", nullptr, ImGuiWindowFlags_MenuBar);

	static bool LoadingStage = false;		// static にすることでグローバル変数のように、変数の内容が保持される
	//static GameObject* SelectingObject = nullptr;
	static std::string SelectingObjectNameS;
	static std::string StageName = "Input StageName";


	// タイトルに戻るボタン
	ImVec2 winSize = ImGui::GetWindowSize();
	float CloseStartX = winSize.x - 110.0f;
	if (CloseStartX < 0.0f)
		CloseStartX = 0.0f;
	ImGui::SameLine(CloseStartX);
	if (ImGui::Button("Back to Title"))
	{
		LoadingStage = false;
		//pickupobject = nullptr;
		//SelectingObject = nullptr;
		StageName = "Input StageName";
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		*pOut_backTitle = true;
		return nullptr;
	}



	// メニューバー
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{

				// ステージ名が空っぽだったら名前を設定して保存する
				if (StageName.empty())
				{
					StageName = "Input StageName";
					//ImGui::EndMenu();
					//ImGui::EndMenuBar();
					//goto Menu_Fin;
				}

				save->SaveStage(StageName);


				//if (savedata->CheckSaveObjectName(objname))
				//{
				//	// セーブしようとしたときに同じものがあった場合、上書きする。
				//	GameObject* loadedobj = savedata->GetLoadedObjectWithName(objname);

				//	// 一度コンポーネントなどをリセットする
				//	loadedobj->DestroyComponentList();

				//	// 現在のものをコピーする
				//	loadedobj->CopyGameObject(gameobject);
				//	loadedobj->CopyComponentListToAdd(gameobject);
				//}
				//else
				//{
				//	// なかった場合。新規で追加。
				//	GameObject* addObj = savedata->AddLoadedObject<LoadedObject>(1);
				//	int fr = savedata->GetLoadedObjectNum();


				//	addObj->CopyGameObject(gameobject);
				//	addObj->CopyComponentListToAdd(gameobject);

				//	char addname[64];
				//	addObj->GetObjectNameChar64(addname);
				//	//savedata->CheckSaveObjectList(addObj);

				//	//
				//	savedata->SaveObject();

				//	//savedata->UnLoad_LoadedObjectList();
				//	//savedata->LoadObject();
				//}
			}
			if (ImGui::MenuItem("Load"))
			{
				// まず現在配置されているオブジェクトを全部消す
				

				LoadingStage = true;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	Menu_Fin:

	std::vector<GameObject*> gameobjectList = scene->GetAllGameObjects(1);

	char StageNameC[64];
	StringToChar64(StageName, StageNameC);
	// ステージの名前の表示と更新
	ImGui::InputText("StageName", StageNameC, sizeof(StageNameC));
	StageName = StageNameC;

	// モードの選択
	{
		ImGui::Text("Mode: Object  Sculpt  Paint");
		ImGui::Dummy(ImVec2(40.0f, 0.0f));
		ImGui::SameLine();

		int nowmode;
		if (*mode == E_CreateStageScene_Mode::Object)
			nowmode = 1;
		else if (*mode == E_CreateStageScene_Mode::Sculpt)
			nowmode = 2;
		else if (*mode == E_CreateStageScene_Mode::Paint)
			nowmode = 3;
		ImGui::RadioButton("##mode1", &nowmode, 1);
		ImGui::SameLine(); 
		ImGui::Dummy(ImVec2(20.0f, 0.0f));
		ImGui::SameLine();
		ImGui::RadioButton("##mode2", &nowmode, 2);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(20.0f, 0.0f));
		ImGui::SameLine();
		ImGui::RadioButton("##mode3", &nowmode, 3);

		if (nowmode == 1)
			*mode = E_CreateStageScene_Mode::Object;
		else if (nowmode == 2)
			*mode = E_CreateStageScene_Mode::Sculpt;
		else if (nowmode == 3)
			*mode = E_CreateStageScene_Mode::Paint;
	}


	bool addbutton = false;
	GameObject* tempobject = nullptr;
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
	int count = 0;
	for (auto gameobject : gameobjectList)
	{
		// 既にデストロイの準備をしているものは表示しない
		if (gameobject->GetDestroyState())
			continue;

		// セーブするオブジェクトじゃないもの(フィールドとか)は表示しない
		if (!gameobject->GetDoSave())
			continue;

		if (gameobject == pickupobject)
		{
			*pOut_pickupname = "_" + to_string(count);
		}

		char objectname[64];
		gameobject->GetObjectNameChar64(objectname);
		std::string nameS = objectname;
		nameS = nameS + "_" + to_string(count);
		// 読み込み失敗しているなら表示するときNullをつける
		if(!gameobject->GetLoadSuccess())
		{
			nameS += "(Null)";
		}
		if (ImGui::Button(nameS.c_str()))
		{
			addbutton = true;
			SelectingObjectNameS = nameS;
			tempobject = gameobject;
			*pOut_pickupname = "_" + to_string(count);
			//SelectingObject = gameobject;
		}
		count++;
	}
	ImGui::EndChild();
	*pOut_objectnum = count;

	//// 選択しているならば、選択したオブジェクトの情報のUIを表示する
	//if (SelectingObject != nullptr/* || !SelectingObject->GetDestroyState()*/)
	//{
	//	if (SelectingObjectStateGUI(SelectingObject, SelectingObjectNameS))
	//	{
	//		SelectingObject = nullptr;
	//		SelectingObjectNameS = "";
	//	}
	//}







	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	if (addbutton)
	{
		// モードもオブジェクトモードに変える
		*mode = E_CreateStageScene_Mode::Object;
		return tempobject;
	}
	else
	{
		//char objectname[64];
		//tempobject->GetObjectNameChar64(objectname);
		//std::string nameS = objectname;
		//nameS = nameS + "_" + to_string(count);
		//SelectingObjectNameS = nameS;
		//*pOut_pickupname = "_" + to_string(count);
	}


	if (LoadingStage)
	{
		// StageNameはstaticなのでアドレス渡して向こうで中身を変えられないので。
		std::string GetNewStageName;
		if (SelectLoadStage(&GetNewStageName))
		{
			// ステージ名の表示の更新
			StageName = GetNewStageName;		// こうやって変数作ってここでコピーする
			// 選択されたらロードの表示を消す
			LoadingStage = false;

		}
	}

	return nullptr;

}

// 返り値のboolは、オブジェクトをデストロイボタンが押されたかどうか
// ステージ追加モードでオブジェクトを選択している時のやつ
bool Gui::SelectingObjectStateGUI(GameObject* object, std::string displayname) noexcept
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("Selecting Object State", nullptr, ImGuiWindowFlags_MenuBar);

	if (object == nullptr/* || object->GetDestroyState()*/)
	{
		ImGui::Text("No Selected");
	}
	else
		ImGui::Text(displayname.c_str());

	float dragspeed = 0.005f;

	// Positionの設定
	if (ImGui::TreeNode("Position"))
	{
		D3DXVECTOR3* pPosition = object->GetpPosition();
		ImGui::DragFloat("Position X", &pPosition->x, dragspeed);
		ImGui::DragFloat("Position Y", &pPosition->y, dragspeed);
		ImGui::DragFloat("Position Z", &pPosition->z, dragspeed);

		ImGui::TreePop();
	}
	// スケールの設定
	if (ImGui::TreeNode("Scale"))
	{
		D3DXVECTOR3* pScale = object->GetpScaleRate();
		ImGui::DragFloat("Scale X", &pScale->x, dragspeed);
		ImGui::DragFloat("Scale Y", &pScale->y, dragspeed);
		ImGui::DragFloat("Scale Z", &pScale->z, dragspeed);

		ImGui::TreePop();
	}
	// 回転の設定
	if (ImGui::TreeNode("Rotation"))
	{
		D3DXVECTOR3* pRotation = object->GetpRotation();
		ImGui::DragFloat("Rotation X", &pRotation->x, dragspeed);
		ImGui::DragFloat("Rotation Y", &pRotation->y, dragspeed);
		ImGui::DragFloat("Rotation Z", &pRotation->z, dragspeed);

		ImGui::TreePop();
	}

	// スペースを空ける
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	if (ImGui::Button("Destroy"))
	{
		object->SetDestroy();

		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return true;

	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	return false;
}



// 追加したらピックアップオブジェクトに設定する。引き数でセットしてもダメなので返り値で渡す
// ステージ追加シーンのとこの新規オブジェクトを追加するやつ。
GameObject* Gui::AddObjectToStage() noexcept
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("AddObject List");

	ImGui::Text("Click to Add Object to Stage");

	Scene* scene = Manager::GetScene();
	Savedata* savedata = Manager::GetSavedata();

	bool result;
	std::list<GameObject*> loadedobjectlist = savedata->GetLoadedObjectList(&result);
	if (!result)
	{
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return nullptr;
	}

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
	for (auto loadedobject : loadedobjectlist)
	{
		char objnameC[64];
		loadedobject->GetObjectNameChar64(objnameC);
		// ボタンを押すとそのオブジェクトが追加される
		if (ImGui::Button(objnameC))
		{
			//OriginalBlock* object = scene->AddGameObject<OriginalBlock>(1);
			GameObject* object = scene->AddGameObject<OriginalBlock>(1);

			//object->DestroyComponentList();		// 中身を更新するために一度中身を消す

			object->CopyGameObject(loadedobject);
			object->CopyComponentListToAdd(loadedobject);


			ImGui::EndChild();
			ImGui::End();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			return object;
		}
	}
	ImGui::EndChild();




	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	return nullptr;
}


// ステージ追加シーンのとこの新規オブジェクトを追加するやつ
bool Gui::AddMeshFieldObjectToStage() noexcept
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("Add MeshField List");

	Scene* scene = Manager::GetScene();
	Savedata* savedata = Manager::GetSavedata();

	{
		// カメラ座標
		D3DXVECTOR3 cPos = scene->GetCameraObject()->GetPosition();
		std::string cPosS;
		cPosS = "X:" + std::to_string(cPos.x);
		cPosS += " Y:" + std::to_string(cPos.y);
		cPosS += " Z:" + std::to_string(cPos.z);
		ImGui::Text("Camera Position");
		ImGui::Text(cPosS.c_str());
		ImGui::Dummy(ImVec2(0.0f, 10.0f));


		// カメラ座標のエリアブロック位置
		ImGui::Text("Camera AreaBlock");
		Int2   areablockI = GetAreaBlockInt2(cPos);
		std::string areablockS;
		areablockS = "X:" + std::to_string(areablockI.x);
		areablockS += " Z:" + std::to_string(areablockI.y);
		ImGui::Text(areablockS.c_str());
		ImGui::Dummy(ImVec2(0.0f, 20.0f));


	}

	{
		ImGui::Text("Click to Add MeshField to Stage");

		static Int2 AreaBlock;

		std::string strx, stry;
		strx = std::to_string(AreaBlock.x);
		stry = std::to_string(AreaBlock.y);

		char cx[64], cy[64];
		StringToChar64(strx, cx);
		StringToChar64(stry, cy);

		int pushx = 0, pushy = 0;

		ImGui::Text("X:");
		ImGui::SameLine();
		if (ImGui::Button("<##x"))
			pushx = -1;
		ImGui::SameLine();
		ImGui::InputText("##AreaBlock.x", cx, sizeof(cx));
		ImGui::SameLine();
		if (ImGui::Button(">##x"))
			pushx = 1;

		ImGui::Text("Z:");
		ImGui::SameLine();
		if (ImGui::Button("<##z"))
			pushy = -1;
		ImGui::SameLine();
		ImGui::InputText("##AreaBlock.y", cy, sizeof(cy));
		ImGui::SameLine();
		if (ImGui::Button(">##z"))
			pushy = 1;

		AreaBlock.x = atoi(cx);
		AreaBlock.y = atoi(cy);

		AreaBlock.x += pushx;
		AreaBlock.y += pushy;


		if (ImGui::Button("Add MeshField"))
		{
			if (!scene->CheckMeshFieldObjectUsed(AreaBlock))
				scene->AddMeshFieldObject(AreaBlock);
		}
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	return false;
}


// ステージ追加シーンのとこのLoadを推したときに出てくるやつ
bool Gui::SelectLoadStage(std::string* return_name) noexcept
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("Select  Load Stage");


	// 閉じるボタン
	ImVec2 winSize = ImGui::GetWindowSize();
	float CloseStartX = winSize.x - 60.0f;
	if (CloseStartX < 0.0f)
		CloseStartX = 0.0f;
	ImGui::SameLine(CloseStartX);
	if (ImGui::Button("Close"))
	{
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return true;
	}

	// スペースを空ける
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	ImGui::Text("Stage List");

	Savedata* savedata = Manager::GetSavedata();
	//savedata->SaveObject();

	std::vector<string> stagenamelist = savedata->GetStageNameListForFolder();


	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
	for (auto &stagename : stagenamelist)
	{
		// ステージ名のボタンの表示
		if (ImGui::Button(stagename.c_str()))
		{
			// まずセーブデータの現在読み込んでいるステージデータの更新+配置されているオブジェクトの削除
			savedata->LoadStage(stagename);

			// データを反映させる。配置する。
			savedata->AddObjectForLoadedStage();

			std::string returnname = stagename;
			// ステージ名を渡す
			*return_name = returnname;

			ImGui::EndChild();
			ImGui::End();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			return true;
		}
	}
	ImGui::EndChild();




	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	return false;
}


// Collisionの設定のGUIの途中で使う補助関数
void Gui::CollisionResponseGUI(Collision* collision) noexcept
{
	if (ImGui::TreeNode("Response ObjectType"))
	{
		bool bemoved = collision->GetBeMoved();
		ImGui::Checkbox("BeMoved", &bemoved);
		collision->SetBeMoved(bemoved);

		// スペースを空ける
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		Scene* scene = Manager::GetScene();
		Savedata* savedata = Manager::GetSavedata();

		// 自分がどのオブジェクトタイプかを選ぶところ----Start

		// ゲット用
		std::vector<std::string> ObjectTypeNameList;
		std::vector<int> DefaultResponseList;
		int ObjectTypeNum;

		savedata->Get_Collision_ObjectTypeSaveListData(&ObjectTypeNameList, &DefaultResponseList);
		ObjectTypeNum = ObjectTypeNameList.size();	// オブジェクトタイプの種類数

		// オブジェクトタイプの設定
		ImGui::Text("Object Type"); ImGui::SameLine();
		std::string Ccaption = "##Object Type";
		// 取得
		int		   Ccurrent_itemNum = collision->GetObjectType();
		std::string Ccurrent_item = ObjectTypeNameList[Ccurrent_itemNum];
		// 表示
		ImGuiComboUI(Ccaption, Ccurrent_item, Ccurrent_itemNum, ObjectTypeNameList);
		// 更新
		collision->SetObjectType(Ccurrent_itemNum);


		// 自分がどのオブジェクトタイプかを選ぶところ----End

		// スペースを空ける
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Text("Ignore"); ImGui::SameLine();
		ImGui::Text("Overlap"); ImGui::SameLine();
		ImGui::Text("Block");

		// このコリジョンの設定をロード
		//std::vector<int> ResponseObjectList = collision->GetResponseObjectList();
		std::vector<int> ResponseObjectList(ObjectTypeNum);		// vectorの場合先にサイズ指定したから
		ResponseObjectList = collision->GetResponseObjectList();// じゃないとコピーできない。エラー出る

		std::vector<int> NewResponseList(ObjectTypeNum);	// temp
		// オブジェクトタイプに対する反応の設定---Start
		for (int n = 0; n < ObjectTypeNum; n++)
		{
			// 読み込み
			int mode = ResponseObjectList[n];

			enum {
				MODE_1,
				MODE_2,
				MODE_3,
			};

			std::string lavelname = "##" + to_string(n);	// ラベル名が同じだとダメなので
			ImGui::RadioButton((lavelname + "a").c_str(), &mode, MODE_1); ImGui::SameLine(0.0f, 35.0f);
			ImGui::RadioButton((lavelname + "b").c_str(), &mode, MODE_2); ImGui::SameLine(0.0f, 35.0f);
			ImGui::RadioButton((lavelname + "c").c_str(), &mode, MODE_3); ImGui::SameLine();
			ImGui::Text(ObjectTypeNameList[n].c_str());

			NewResponseList.push_back(mode);
			// 書き込み
			collision->SetResponseObject(n, mode);
		}
		//collision->SetResponseObjectList(NewResponseList);

		ImGui::TreePop();
	}
}



// コリジョンレスポンスの設定。セーブデータ用
void Gui::SaveData_CollisionResponseGUI() noexcept
{
	ImGui::Begin("CollisionResponse Option");


	// セーブボタン
	ImVec2 winSize = ImGui::GetWindowSize();
	float SaveStartX = winSize.x - 60.0f;
	if (SaveStartX < 0.0f)
		SaveStartX = 0.0f;
	ImGui::SameLine(SaveStartX);
	if (ImGui::Button("Save"))
	{
		Manager::GetSavedata()->SaveCollisionResponse();
		ImGui::End();
		return;
	}




	ImGui::Text("Default Response Option");
	ImGui::Text("Ignore"); ImGui::SameLine();
	ImGui::Text("Overlap"); ImGui::SameLine();
	ImGui::Text("Block");

	Scene* scene = Manager::GetScene();
	Savedata* savedata = Manager::GetSavedata();


	// 現在のセーブデータにある設定を取得
	std::vector<std::string> namelist;
	std::vector<int> responselist;
	savedata->Get_Collision_ObjectTypeSaveListData(&namelist, &responselist);
	int objectnum = responselist.size();

	// オブジェクトタイプに対する反応の表示と設定
	for (int n = 0; n < objectnum; n++)
	{
		std::string lavelname = "##" + to_string(n);	// ラベル名が同じだとダメなので
		

		// 読み込み
		int mode = responselist[n];

		enum {
			MODE_1,
			MODE_2,
			MODE_3,
		};

		ImGui::RadioButton((lavelname + "a").c_str(), &mode, MODE_1); ImGui::SameLine(0.0f, 35.0f);
		ImGui::RadioButton((lavelname + "b").c_str(), &mode, MODE_2); ImGui::SameLine(0.0f, 35.0f);
		ImGui::RadioButton((lavelname + "c").c_str(), &mode, MODE_3); ImGui::SameLine();

		// オブジェクトタイプの名前の設定
		std::string objtypename = namelist[n];
		char objtypenameC[64];
		StringToChar64(objtypename, objtypenameC);
		ImGui::InputText((lavelname + "objtypename").c_str(), objtypenameC, sizeof(objtypenameC));
		objtypename = objtypenameC;

		// 更新・保存する
		savedata->Set_Collision_ObjectTypeSaveNum(n, objtypename, mode);

		ImGui::SameLine();
		std::string displayDelete = "Delete##" + to_string(n);
		if (ImGui::Button(displayDelete.c_str()))
		{
			savedata->Delete_Collision_ObjectTypeSaveList(n);
			Manager::GetSavedata()->SaveCollisionResponse();
			goto endpoint;
		}

	}

	// 追加
	// スペースを空ける
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::Text("Add Object Type");
	ImGui::Text("Default Response Option");
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::Text("Ignore"); ImGui::SameLine();
	ImGui::Text("Overlap"); ImGui::SameLine();
	ImGui::Text("Block");

	// 読み込み
	static int mode = 0;

	enum {
		MODE_1,
		MODE_2,
		MODE_3,
	};

	ImGui::RadioButton("##aa", &mode, MODE_1); ImGui::SameLine(0.0f, 35.0f);
	ImGui::RadioButton("##bb", &mode, MODE_2); ImGui::SameLine(0.0f, 35.0f);
	ImGui::RadioButton("##cc", &mode, MODE_3); ImGui::SameLine();

	// オブジェクトタイプの名前の設定
	static std::string objtypename = "ObjectType Name";
	char objtypenameC[64];
	StringToChar64(objtypename, objtypenameC);
	ImGui::InputText("##objtypename", objtypenameC, sizeof(objtypenameC));
	objtypename = objtypenameC;

	if (ImGui::Button("Add"))
	{
		savedata->Add_Collision_ObjectTypeSaveList(objtypename, mode);
		objtypename = "ObjectType Name";

		Manager::GetSavedata()->SaveCollisionResponse();
		goto endpoint;
	}

	endpoint:
	ImGui::End();
}




// ステージ追加シーンのとこのLoadを推したときに出てくるやつ
bool Gui::StageSelectScene_StageSelect(std::string* return_name) noexcept
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("Select Stage");


	// 閉じるボタン
	ImVec2 winSize = ImGui::GetWindowSize();
	float CloseStartX = winSize.x - 80.0f;
	if (CloseStartX < 0.0f)
		CloseStartX = 0.0f;
	ImGui::SameLine(CloseStartX);
	if (ImGui::Button("Back to Title"))
	{
		std::string returnname = "BackTitle";
		// BackTitleという名前をステージ名に入れて終了する
		*return_name = returnname;

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return true;
	}

	// スペースを空ける
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	ImGui::Text("Stage List");

	Savedata* savedata = Manager::GetSavedata();

	std::vector<string> stagenamelist = savedata->GetStageNameListForFolder();


	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
	for (auto& stagename : stagenamelist)
	{
		// ステージ名のボタンの表示
		if (ImGui::Button(stagename.c_str()))
		{
			// まずセーブデータの現在読み込んでいるステージデータの更新+配置されているオブジェクトの削除
			savedata->LoadStage(stagename);

			//// データを反映させる。配置する。
			//savedata->AddObjectForLoadedStage();	// それぞれのシーンのInitで読み込むようにしている

			std::string returnname = stagename;
			// ステージ名を渡す
			*return_name = returnname;

			ImGui::EndChild();
			ImGui::End();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			return true;
		}
	}
	ImGui::EndChild();




	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	return false;
}



void Gui::testGui()
{
	// Our state
	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}






// スカルプトブラシのオプションGUI
void Gui::CreateNewStage_SculptBrushOptionGUI(Brush* brush) noexcept
{
	// 変数を貰って作っておく
	E_UseSettingOfSculptBrush* usesettingofsculptbrush = brush->GetpUseSettingOfSculptBrush();
	E_UseSculptBrush* pusesculptbrush = brush->GetpUseSculptBrush();
	PassedSculptBrushState* passedsculptbrushstate = brush->GetpPassedSculptBrushState(*pusesculptbrush);




	ImGui::Begin("Sculpt Option");


	//ImGui::Text("Overlap"); 
	//ImGui::SameLine();
	
	// まずブラシのもこもこか減らすかの設定
	{
		int items_current = static_cast<int>(*usesettingofsculptbrush);

		ImGui::RadioButton("##+", &items_current, static_cast<int>(E_UseSettingOfSculptBrush::Setting001));
		ImGui::SameLine(0.0f, 35.0f);
		ImGui::RadioButton("##-", &items_current, static_cast<int>(E_UseSettingOfSculptBrush::Setting002));

		*usesettingofsculptbrush = static_cast<E_UseSettingOfSculptBrush>(items_current);
	}


	// どのブラシを使うかの設定
	{
		enum {
			Brush_1,
			Brush_2,
			Brush_3,
			Brush_4
		};
		int UseSculptBrush = 0;


		// コンポーネントの追加ボタン
		ImGui::Text("Use Brush");
		const char* itemsComp[] = { "Brush001", "Brush002" , "Brush003" /*, "Brush004"*/ };
		int itemsComp_current = static_cast<int>(*pusesculptbrush);
		ImGui::Combo("Select", &itemsComp_current, itemsComp, IM_ARRAYSIZE(itemsComp));
		*pusesculptbrush = static_cast<E_UseSculptBrush>(itemsComp_current);
	}


	// 現在使っているブラシの半径と強さの設定
	{
		float dragspeed = 0.005f;
		ImGui::DragFloat("Radius", &passedsculptbrushstate->Radius, dragspeed);
		ImGui::DragFloat("Strength", &passedsculptbrushstate->Strength, dragspeed);
	}


	ImGui::End();

	return;
}

// ペイントブラシのオプションGUI	現在セットリスト0にしか対応してない。
void Gui::CreateNewStage_PaintBrushOptionGUI(Brush* brush) noexcept
{
	// 変数を貰って作っておく
	int* pPaintBrushSetList = brush->GetPaintBrushSetList();	// [10][4]の配列
	int nowListNum = brush->GetPaintBrushNowList();
	int nowNum = brush->GetPaintBrushNow();
	PassedPaintBrushState* passedpaintbrushstate = brush->GetpPassedPaintBrushState(nowListNum, nowNum);




	ImGui::Begin("Paint Option");


	//ImGui::Text("Overlap"); 
	//ImGui::SameLine();

	// どのブラシを使うかの設定
	{
		enum {
			Brush_1,
			Brush_2,
			Brush_3
		};
		int UsePaintBrush = 0;


		// コンポーネントの追加ボタン
		ImGui::Text("Use Brush");
		const char* itemsComp[] = { "Eraser", "Ground001", "Grass001" /*, "Brush003"*/ };
		int itemsComp_current = nowNum + 1;
		if (nowNum == PaintBrushSetMax - 1)		// 消しゴムだったらPaintBrushSetMax(5)-1を渡す
			itemsComp_current = 0;
		ImGui::Combo("Select", &itemsComp_current, itemsComp, IM_ARRAYSIZE(itemsComp));
		// どのブラシを選んだかをセットする。現在セットリストは対応していない。
		if (itemsComp_current == 0)		// 消しゴムだったらPaintBrushSetMax(5)-1を渡す
			brush->SetPaintBrushNow(PaintBrushSetMax - 1);
		else
			brush->SetPaintBrushNow(itemsComp_current - 1);
		
	}


	// 現在使っているブラシの半径と強さの設定
	{
		float dragspeed = 0.005f;
		ImGui::DragFloat("Radius", &passedpaintbrushstate->Radius, dragspeed);
		ImGui::DragFloat("Strength", &passedpaintbrushstate->Strength, dragspeed);
	}


	ImGui::End();

	return;
}

void Gui::CreateNewStage_PickupObjectGUI(GameObject* pickupobject, const std::string& pickupname, bool* pOutDestroy) noexcept
{

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("Pickup Object State", nullptr, ImGuiWindowFlags_MenuBar);

	if (pickupobject == nullptr)
	{
		ImGui::Text("No Selected");

		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return;
	}
	else
	{
		assert(pickupobject);
		//pickupobject->GetObjectNameString();
		//ImGui::Text(pickupobject->GetObjectNameString().c_str());

		char objectname[64];
		pickupobject->GetObjectNameChar64(objectname);
		std::string nameS = objectname;
		nameS += pickupname;
		ImGui::Text(nameS.c_str());
	}

	float dragspeed = 0.005f;

	// Positionの設定
	if (ImGui::TreeNode("Position"))
	{
		D3DXVECTOR3* pPosition = pickupobject->GetpPosition();
		ImGui::DragFloat("Position X", &pPosition->x, dragspeed);
		ImGui::DragFloat("Position Y", &pPosition->y, dragspeed);
		ImGui::DragFloat("Position Z", &pPosition->z, dragspeed);

		// カメラの方向に応じて座標変えれるバージョン
		{
			ImGui::Text("CameraDirection movement");

			// yの移動を無視するかどうか
			static bool IgnoreUp = true;
			ImGui::Checkbox("Ignore Y", &IgnoreUp);
			ImGui::SameLine();
			// 軸に平行して移動するかどうか
			static bool ParallelToAxis = false;
			ImGui::Checkbox("Parallel to axis", &ParallelToAxis);

			Scene* scene = Manager::GetScene();
			Camera* camera = scene->GetCameraObject();
			D3DXVECTOR3 CameraForward;
			D3DXVECTOR3 CameraRight;
			if (ParallelToAxis)
			{
				IgnoreUp = true;
				CameraForward = camera->GetCameraForwardParallelToAxis();
				CameraRight = camera->GetCameraRightParallelToAxis();
			}
			else
			{
				CameraForward = camera->GetCameraForward();
				CameraRight = camera->GetCameraRight();
			}
			if (IgnoreUp)
			{
				CameraForward.y = 0.0f;
				CameraRight.y = 0.0f;
				D3DXVec3Normalize(&CameraForward, &CameraForward);
				D3DXVec3Normalize(&CameraRight, &CameraRight);
			}
			float addForwardScale = 0.0f;
			float addRightScale = 0.0f;
			ImGui::DragFloat("ADD Forward", &addForwardScale, dragspeed);
			ImGui::DragFloat("ADD Right", &addRightScale, dragspeed);
			D3DXVECTOR3 addForward = CameraForward * addForwardScale;
			D3DXVECTOR3 addRight = CameraRight * addRightScale;
			pickupobject->AddPosition(addForward);
			pickupobject->AddPosition(addRight);
		}


		ImGui::TreePop();
	}
	// スケールの設定
	if (ImGui::TreeNode("Scale"))
	{
		D3DXVECTOR3* pScale = pickupobject->GetpScaleRate();
		ImGui::DragFloat("Scale X", &pScale->x, dragspeed);
		ImGui::DragFloat("Scale Y", &pScale->y, dragspeed);
		ImGui::DragFloat("Scale Z", &pScale->z, dragspeed);

		ImGui::TreePop();
	}
	// 回転の設定
	if (ImGui::TreeNode("Rotation"))
	{
		D3DXVECTOR3* pRotation = pickupobject->GetpRotation();
		ImGui::DragFloat("Rotation X", &pRotation->x, dragspeed);
		ImGui::DragFloat("Rotation Y", &pRotation->y, dragspeed);
		ImGui::DragFloat("Rotation Z", &pRotation->z, dragspeed);

		ImGui::TreePop();
	}

	// スペースを空ける
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	// デストロイ処理。SetDestroy()した後にステージ作成シーンのピックアップオブジェクトもnullptrにしてあげる
	if (ImGui::Button("Destroy"))
	{
		pickupobject->SetDestroy();
		*pOutDestroy = true;
		//pickupobject = nullptr;

		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return;

	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//return false;


	return;
}



void Gui::GameSettingGUI(const bool& useclose, Camera* camera, bool* pOutClose, bool* pOutBackTitle) noexcept
{

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	ImGui::Begin("Game Setting GUI");

	// 閉じるボタン
	{
		if (useclose)
		{
			if (ImGui::Button("Close"))
			{
				ImGui::End();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				*pOutClose = true;
				return;
			}
		}
	}

	ImGui::SameLine();
	// タイトルに戻るボタン
	{
		if (ImGui::Button("Back to Title"))
		{

			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			*pOutBackTitle = true;
			return;
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 30.0f));
	// マウス感度設定
	{
		float dragspeed = 0.003f;
		float sensiMin = 0.0010f;
		static float sensi = camera->m_FPSCamera.sensitivity;
		ImGui::DragFloat("Mouse Sensitivity", &sensi, dragspeed);
		if (sensi < sensiMin)
			sensi = sensiMin;
		camera->m_FPSCamera.SetSensitivity(sensi);
	}

	if (useclose)
	{
		ImGui::Dummy(ImVec2(0.0f, 30.0f));
		// カメラの近さ設定
		{
			float dragspeed = 0.005f;
			static float dist = camera->GetCameraDistance();
			ImGui::DragFloat("Camera Distance", &dist, dragspeed);
			camera->SetCameraDistance(dist);
		}
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	return;
}


// デバッグ用
void Gui::Debug_String(const std::string& string) noexcept
{
	ImGui::Begin("Debug String");

	ImGui::Text(string.c_str());

	ImGui::End();
	return;
}
void Gui::Debug_Vec3(const D3DXVECTOR3& vec) noexcept
{
	ImGui::Begin("Debug String");

	float x = vec.x;
	float y = vec.y;
	float z = vec.z;
	ImGui::DragFloat("x", &x);
	ImGui::DragFloat("y", &y);
	ImGui::DragFloat("z", &z);

	ImGui::End();
	return;
}



// タイトル画面のシーンを選択するところ
int Gui::Title_SelectSeaneGUI() noexcept
{
	ImGui::Begin("Please Select Seane");

	int re = 0;


	if (ImGui::Button("Create Object")) 
	{
		re = 1;
	}
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	if (ImGui::Button("Create Stage"))
	{
		re = 2;
	}
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	if (ImGui::Button("Play Game"))
	{
		re = 3;
	}
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::Dummy(ImVec2(0.0f, 80.0f));


	if (ImGui::Button("Exit"))
	{
		re = 4;
	}

	ImGui::End();
	return re;
}