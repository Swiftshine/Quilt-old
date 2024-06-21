#include "editor.h"

void Editor::ClearNodes() {
	DeselectNode();
	nodes_cmnGmk.clear();
	nodes_gmk.clear();
	nodes_zone.clear();
}

void Editor::UpdateNodes() {
	if (ImGui::IsKeyDown(ImGuiKey_Escape)) DeselectNode();
	if (ImGui::IsKeyDown(ImGuiKey_Delete)) DeleteNode();

	if (showCommon) {
		for (auto& node : nodes_cmnGmk) {
			node.Update();
		}
	} else {
		DeselectNode();
	}

	if (showGimmicks) {
		for (auto& node : nodes_gmk) {
			node.Update();
		}
	} else {
		DeselectNode();
	}

	if (showZones) {
		for (auto& node : nodes_zone) {
			node.Update();
		}
	}
	else {
		DeselectNode();
	}
}

void Editor::SelectNode(NodeBase* node) {
	if (!node || selected_node == node) return;
	selected_node = node;
	selected_node->Select();

}

void Editor::DeselectNode() {
	if (!selected_node) return;
	selected_node->Deselect();
	selected_node = nullptr;
}

void Editor::CreateNode(NodeBase::NodeType type) {
	switch (type) {
		case NodeBase::NodeType::CommonGimmick: {
			DeselectNode();
			Mapdata::Mapbin::CommonGimmick cmnGmk = { 0 };
			Vec2f pos = GetWorldMousePosition().GetSwap();
			cmnGmk.position = Vec3f(pos.x, pos.y, 0);

			std::string name = query_selected_label;
			u32 name_index = GetIndexFromName(name);
			cmnGmk.nameIndex = Swap32(name_index);
			nodes_cmnGmk.emplace_back(cmnGmk);
			size_t index = nodes_cmnGmk.size() - 1;
			SelectNode(&nodes_cmnGmk[index]);
			break;
		}

		case NodeBase::NodeType::Gimmick: {
			DeselectNode();
			Mapdata::Mapbin::Gimmick gmk = { 0 };
			std::string temp = "NONE";
			std::copy(temp.data(), temp.data() + 4, gmk.name);
			Vec2f pos = GetWorldMousePosition().GetSwap();
			gmk.position = Vec3f(pos.x, pos.y, 0);
			nodes_gmk.emplace_back(gmk);
			size_t index = nodes_gmk.size() - 1;
			SelectNode(&nodes_gmk[index]);
			break;
		}

	}
}

void Editor::DeleteNode() {
	if (!selected_node) return;

	for (auto it = nodes_cmnGmk.begin(); it != nodes_cmnGmk.end(); it++) {
		if (&(*it) == selected_node) {
			nodes_cmnGmk.erase(it);
			selected_node = nullptr;
			return;
		}
	}

	for (auto it = nodes_gmk.begin(); it != nodes_gmk.end(); it++) {
		if (&(*it) == selected_node) {
			nodes_gmk.erase(it);
			selected_node = nullptr;
			return;
		}
	}
}