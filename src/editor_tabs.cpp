#include "editor.h"

void Editor::Tabs() {

	Tabs_Settings();
	Tabs_Quilt();
	Tabs_Canvas();
}

void Editor::Tabs_Canvas() {
	ImGui::Begin("Canvas");
	if (!open) {
		ImGui::Text("Open a stage folder to get started.");
		ImGui::End();
		return;
	}

	ImGuiID dockspace_id = ImGui::GetID("Canvas_Dockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), 0);
	
	Canvas_Toolbox();
	Canvas_Viewport();

	ImGui::End(); 
}

void Editor::Tabs_Settings() {
	ImGui::Begin("Settings");

	ImGui::End();
}

void Editor::Tabs_Quilt() {
	ImGui::Begin("Quilt");
	ImGui::Text("Quilt - Kirby's Epic Yarn level editor");
	ImGui::Text("Swiftshine 2024");
	ImGui::Text("https://github.com/Swiftshine/Quilt");
	ImGui::End();
}