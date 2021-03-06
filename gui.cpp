#define _CRT_SECURE_NO_WARNINGS
#include "gui.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <tchar.h>
#include <vector>

void gui::DebugStorage() noexcept
{
	//// ウィンドウの色を決める
	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	//-----ウィンドウの場所を決める
	ImGui::SetNextWindowPos({ 0, 0 });
	//-----ウィンドウのサイズを決める, 第2引数になんか入れないとサイズ変わってくれないんだけど、まだよくわからん
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });	

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		//-----ウィンドウ1つ作る,名前決めれるよ
		ImGui::Begin("Debug");



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

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}




float gui::DebugA() noexcept
{
	//gl::clear(Color(0, 0, 0));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	//ImGui::SetNextWindowPos(ImVec2(20, 20), 0);	// ImGuiSetCond_Once
	//ImGui::SetNextWindowSize(ImVec2(280, 300), 0); // ImGuiSetCond_Once

	ImGui::Begin("config 1");

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
		sprintf(name, "item %d", i);
		ImGui::SliderFloat(name, &items[i], 0.1f, 10.0f);
	}
	ImGui::EndChild();

	ImGui::End();

	return items[0];

	//ImGui::PopStyleColor();
	//ImGui::PopStyleColor();
}