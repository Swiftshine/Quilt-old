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

}

void Editor::Toolbox_StringSearch() {
	ImGui::Begin("Common Gimmick Search");

	static std::vector<std::string> filtered_items;

	ImGui::InputText("Search ##cmnGmkSearch", cmnGmkQuery, sizeof(cmnGmkQuery));

	filtered_items.clear();

	for (const auto& t : translations) {
		std::string lower = Quilt::Util::ToLower(t.name);
		if (lower.find(Quilt::Util::ToLower(cmnGmkQuery)) != std::string::npos) {
			filtered_items.push_back(t.name);
		}
	}

	if (filtered_items.empty()) {
		ImGui::Text("Common Gimmick name not found.");
	}
	else {

		for (auto i = 0; i < filtered_items.size(); i++) {
			std::string item = filtered_items[i];

			ImGui::PushID(i);
			bool selected = query_selected_index == i;

			if (ImGui::Selectable(item.c_str(), selected) && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				query_selected_index = i;
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("DRAG_DROP_ITEM", item.c_str(), item.length() + 1);
				query_selected_index = i;
				ImGui::Text(item.c_str());
				ImGui::EndDragDropSource();
			}

			ImGui::PopID();
		}
	}
	ImGui::End();

	if (query_selected_index != -1) {
		if (query_selected_index > filtered_items.size() - 1) {
			query_selected_index = filtered_items.size() - 1;
		}
		query_selected_label = filtered_items[query_selected_index];
	}
}