#define _CRT_SECURE_NO_WARNINGS
#include "gui.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <tchar.h>

void gui::DebugStorage() noexcept
{
	//�E�B���h�E�̐F�����߂�
	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	////-----�E�B���h�E�̏ꏊ�����߂�
	//ImGui::SetNextWindowPos({ 0, 0 });
	////-----�E�B���h�E�̃T�C�Y�����߂�, ��2�����ɂȂ񂩓���Ȃ��ƃT�C�Y�ς���Ă���Ȃ��񂾂��ǁA�܂��悭�킩���
	//ImGui::SetNextWindowSize({ WIDTH, HEIGHT });	

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		//-----�E�B���h�E1���,���O���߂���
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

		//-----���̃E�B���h�E�͏I���
		ImGui::End();

		//-----�܂��E�B���h�E��肽�������炱��Ȋ����ɑ��₵�Ă�
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