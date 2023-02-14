#pragma once

class Brush;
enum class E_CreateStageScene_Mode;
enum class E_UseSculptBrush;
enum class E_UseSettingOfSculptBrush;

class Gui
{
private:

public:
	void testGui();

	void DebugStorage() noexcept;

	void DestroyImGui() noexcept;

	void TestGUI(const std::string& caption, std::string& current_item,
		std::vector<std::string>& items) noexcept;
	float DebugA() noexcept;
	std::string ObjectChoiceGUI(std::list<std::string> LoadedObjectNameList) noexcept;
	bool CreateNewObjectGUI(GameObject* gameobject) noexcept;
	bool SelectLoadObject(GameObject* gameobject) noexcept;

	GameObject* CreateNewStageGUI(E_CreateStageScene_Mode* mode, bool* pOut_backTitle, std::string* pOut_pickupname, int* pOut_objectnum, GameObject* pickupobject) noexcept;
	bool SelectingObjectStateGUI(GameObject* object, std::string displayname) noexcept;
	GameObject* AddObjectToStage() noexcept;
	bool AddMeshFieldObjectToStage() noexcept;

	bool SelectLoadStage(std::string* return_name) noexcept;
	void CollisionResponseGUI(Collision* collision) noexcept;
	void SaveData_CollisionResponseGUI() noexcept;
	bool StageSelectScene_StageSelect(std::string* return_name) noexcept;
	void CreateNewStage_SculptBrushOptionGUI(Brush* brush) noexcept;
	void CreateNewStage_PaintBrushOptionGUI(Brush* brush) noexcept;
	void CreateNewStage_PickupObjectGUI(GameObject* pickupobject, const std::string& pickupname, bool* pOutDestroy) noexcept;
	void GameSettingGUI(const bool& useclose, Camera* camera, bool* pOutClose, bool* pOutBackTitle) noexcept;

	void Debug_String(const std::string& string) noexcept;
	void Debug_Vec3(const D3DXVECTOR3& vec) noexcept;

	int Title_SelectSeaneGUI() noexcept;

};