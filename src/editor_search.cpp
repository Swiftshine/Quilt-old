#include "editor.h"

void Editor::StringSearch_CommonGimmick() {
	ImGui::InputText("Search ##cmnGmkSearch", search_cmnGmk_query, sizeof(search_cmnGmk_query));

	search_cmnGmk_filtered_items.clear();

	for (const auto& t : translations) {
		std::string lower = Quilt::Util::ToLower(t.name);
		if (lower.find(Quilt::Util::ToLower(search_cmnGmk_query)) != std::string::npos) {
			search_cmnGmk_filtered_items.push_back(t.name);
		}
	}

	if (search_cmnGmk_filtered_items.empty()) {
		ImGui::Text("Common Gimmick name not found.");
	}
	else {
		for (auto i = 0; i < search_cmnGmk_filtered_items.size(); i++) {
			std::string item = search_cmnGmk_filtered_items[i];

			ImGui::PushID(i);
			bool selected = search_cmnGmk_index == i;

			if (ImGui::Selectable(item.c_str(), selected) && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				search_cmnGmk_index = i;
				search_type = SearchType::CommonGimmick;
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("DRAG_DROP_ITEM", item.c_str(), item.length() + 1);
				search_cmnGmk_index = i;
				ImGui::Text(item.c_str());
				ImGui::EndDragDropSource();
			}

			ImGui::PopID();
		}
	}
}

void Editor::StringSearch_Enemy() {
	ImGui::InputText("Search ##enemySearch", search_enemy_query, sizeof(search_enemy_query));

	search_enemy_filtered_items.clear();

	for (const auto& t : enemy_translations) {
		std::string lower = Quilt::Util::ToLower(t.name);

		if (lower.find(Quilt::Util::ToLower(search_enemy_query)) != std::string::npos) {
			search_enemy_filtered_items.push_back(t.name);
		}
	}

	if (search_enemy_filtered_items.empty()) {
		ImGui::Text("Enemy name not found.");
	} else {
		for (auto i = 0; i < search_enemy_filtered_items.size(); i++) {
			std::string item = search_enemy_filtered_items[i];

			ImGui::PushID(i);
			bool selected = search_enemy_index == i;

			if (ImGui::Selectable(item.c_str(), selected) && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				search_enemy_index = i;
				search_type = SearchType::Enemy;
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("DRAG_DROP_ITEM", item.c_str(), item.length() + 1);
				search_enemy_index = i;
				ImGui::Text(item.c_str());
				ImGui::EndDragDropSource();
			}
			ImGui::PopID();
		}
	}
}