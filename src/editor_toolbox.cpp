#include "editor.h"

void Editor::Toolbox_Files() {
	if (filenames.empty()) {
		ImGui::Text("No files available.");
		ImGui::End();
		return;
	}

	std::vector<const char*> cstrs;
	for (const auto& name : filenames) { cstrs.push_back(name.c_str()); }

	if (ImGui::Combo("Change File", &selected_file_index, cstrs.data(), cstrs.size())) {
		selected_file = filenames[selected_file_index];
		SetupFile();
	}
}

void Editor::Toolbox_Visibility() {
	bool b = showBounds;
	if (ImGui::Checkbox("Show Stage Bounds", &b)) showBounds = b;

	b = showWalls;
	if (ImGui::Checkbox("Show Walls", &b)) showWalls = b;

	b = showCommon;
	if (ImGui::Checkbox("Show Common Gimmicks", &b)) showCommon = b;

	b = showGimmicks;
	if (ImGui::Checkbox("Show Gimmicks", &b)) showGimmicks = b;

	b = showPaths;
	if (ImGui::Checkbox("Show Paths", &b)) showPaths = b;

	b = showZones;
	if (ImGui::Checkbox("Show Zones", &b)) showZones = b;

	b = showEnemies;
	if (ImGui::Checkbox("Show Enemies", &b)) showEnemies = b;

	b = showRCInfo;
	if (ImGui::Checkbox("Show Race Course Information", &b)) showRCInfo = b;


}

void Editor::Toolbox_StringSearch() {
	ImGui::Begin("Search");

	if (ImGui::CollapsingHeader("Common Gimmicks")) {
		StringSearch_CommonGimmick();
	}

	if (ImGui::CollapsingHeader("Enemies")) {
		StringSearch_Enemy();
	}

	

	/* enemy search */

	/* (assume i write the code here) */
	ImGui::End();

	if (search_cmnGmk_index != -1) {
		if (search_cmnGmk_index > search_cmnGmk_filtered_items.size() - 1) {
			search_cmnGmk_index = search_cmnGmk_filtered_items.size() - 1;
		}
		search_cmnGmk_label = search_cmnGmk_filtered_items[search_cmnGmk_index];
	}

	if (search_enemy_index != -1) {
		if (search_enemy_index > search_enemy_filtered_items.size() - 1) {
			search_enemy_index = search_enemy_filtered_items.size() - 1;
		}
		search_enemy_label = search_enemy_filtered_items[search_enemy_index];
	}
}