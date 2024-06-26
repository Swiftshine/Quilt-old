#include "editor.h"

void Editor::ClearNodes() {
	DeselectNode();
	nodes_wall.clear();
	nodes_labeledWall.clear();
	nodes_cmnGmk.clear();
	nodes_gmk.clear();
	nodes_path.clear();
	nodes_zone.clear();
	nodes_enemy.clear();
}

void Editor::UpdateNodes() {
	if (ImGui::IsKeyDown(ImGuiKey_Escape)) DeselectNode();
	if (ImGui::IsKeyDown(ImGuiKey_Delete)) DeleteNode();

	if (showWalls) {
		for (auto& node : nodes_wall) {
			node.Update();
		}

		for (auto& node : nodes_labeledWall) {
			node.Update();
		}
	}

	if (showCommon) {
		for (auto& node : nodes_cmnGmk) {
			node.Update();
		}
	} else {
		if (selected_node) {
			if (selected_node->GetType() == NodeBase::NodeType::CommonGimmick) {
				DeselectNode();
			}
		}
	}

	if (showGimmicks) {
		for (auto& node : nodes_gmk) {
			node.Update();
		}
	} else {
		if (selected_node) {
			if (selected_node->GetType() == NodeBase::NodeType::Gimmick) {
				DeselectNode();
			}
		}
	}

	if (showPaths) {
		for (auto& node : nodes_path) {
			node.Update();
		}
	} else {
		if (selected_node) {
			if (selected_node->GetType() == NodeBase::NodeType::Path) {
				DeselectNode();
			}
		}
	}
	if (showZones) {
		for (auto& node : nodes_zone) {
			node.Update();
		}
	}
	else {
		if (selected_node) {
			if (selected_node->GetType() == NodeBase::NodeType::Zone) {
				DeselectNode();
			}
		}
	}

	if (showEnemies) {
		for (auto& node : nodes_enemy) {
			node.Update();
		}
	} else {
		if (selected_node) {
			if (selected_node->GetType() == NodeBase::NodeType::Enemy) {
				DeselectNode();
			}
		}
	}
}

void Editor::SelectNode(NodeBase* node) {
	if (!node || selected_node == node) return;
	if (selected_node) DeselectNode();

	selected_node = node;
	selected_node->Select();

}

void Editor::DeselectNode() {
	if (!selected_node) return;
	selected_node->Deselect();
	selected_node = nullptr;
}

void Editor::CreateNode(NodeBase::NodeType type) {
	DeselectNode();
	switch (type) {
		case NodeBase::NodeType::Wall: {
			Colbin::Entry wall = { 0 };
			wall.point1 = GetWorldMousePosition().GetSwap();
			wall.point2 = GetWorldMousePosition();
			wall.point2.x += 8.0f;
			wall.point2.Swap();

			nodes_wall.emplace_back(wall);
			SelectNode(&nodes_wall.back());
			break;
		}

		case NodeBase::NodeType::LabeledWall: {
			Mapdata::Mapbin::DataSegLabel wall = { 0 };
			wall.point1 = GetWorldMousePosition().GetSwap();
			wall.point2 = GetWorldMousePosition();
			wall.point2.x += 8.0f;
			wall.point2.Swap();

			nodes_labeledWall.emplace_back(wall);
			SelectNode(&nodes_labeledWall.back());
			break;
		}

		case NodeBase::NodeType::CommonGimmick: {
			Mapdata::Mapbin::CommonGimmick cmnGmk = { 0 };
			Vec2f pos = GetWorldMousePosition().GetSwap();
			cmnGmk.position = Vec3f(pos.x, pos.y, 0);

			std::string name = search_cmnGmk_label;
			u32 name_index = GetIndexFromName(name);
			cmnGmk.nameIndex = Swap32(name_index);
			nodes_cmnGmk.emplace_back(cmnGmk);
			SelectNode(&nodes_cmnGmk.back());
			break;
		}

		case NodeBase::NodeType::Gimmick: {
			Mapdata::Mapbin::Gimmick gmk = { 0 };
			std::string temp = "NONE";
			std::copy(temp.data(), temp.data() + 4, gmk.name);
			Vec2f pos = GetWorldMousePosition().GetSwap();
			gmk.position = Vec3f(pos.x, pos.y, 0);
			nodes_gmk.emplace_back(gmk);
			SelectNode(&nodes_gmk.back());
			break;
		}

		
		case NodeBase::NodeType::Zone: {
			Mapdata::Mapbin::Zone zone = { 0 };
			std::string temp = "NONE";
			std::copy(temp.data(), temp.data() + 4, zone.name);
			zone.boundsStart = GetWorldMousePosition();
			zone.boundsEnd.x = zone.boundsStart.x + 4.0f;
			zone.boundsEnd.y = zone.boundsStart.y + 4.0f;
			zone.boundsStart.Swap();
			zone.boundsEnd.Swap();
			nodes_zone.emplace_back(zone);
			SelectNode(&nodes_zone.back());
			break;
		}

		case NodeBase::NodeType::Enemy: {
			Mapdata::Enbin::EnemyEntry enemy = { 0 };
			Vec2f pos = GetWorldMousePosition().GetSwap();
			enemy.position1 = Vec3f(pos.x, pos.y, 0);

			std::string id = GetEnemyIDFromName(search_enemy_label);
			std::copy(id.data(), id.data() + id.length(), enemy.name);
			nodes_enemy.emplace_back(enemy);
			SelectNode(&nodes_enemy.back());
			break;
		}
	}
}

void Editor::DeleteNode() {
	if (!selected_node) return;


	switch (selected_node->GetType()) {
		case NodeBase::NodeType::Wall: {
			for (auto it = nodes_wall.begin(); it != nodes_wall.end(); it++) {
				if (&(*it) == selected_node) {
					nodes_wall.erase(it);
					selected_node = nullptr;
					return;
				}
			}
			break;
		}

		case NodeBase::NodeType::LabeledWall: {
			for (auto it = nodes_labeledWall.begin(); it != nodes_labeledWall.end(); it++) {
				if (&(*it) == selected_node) {
					nodes_labeledWall.erase(it);
					selected_node = nullptr;
					return;
				}
			}
			break;
		}

		case NodeBase::NodeType::CommonGimmick: {
			for (auto it = nodes_cmnGmk.begin(); it != nodes_cmnGmk.end(); it++) {
				if (&(*it) == selected_node) {
					nodes_cmnGmk.erase(it);
					selected_node = nullptr;
					return;
				}
			}

			break;
		}

		case NodeBase::NodeType::Gimmick: {
			for (auto it = nodes_gmk.begin(); it != nodes_gmk.end(); it++) {
				if (&(*it) == selected_node) {
					nodes_gmk.erase(it);
					selected_node = nullptr;
					return;
				}
			}
			break;
		}

		case NodeBase::NodeType::Zone: {
			for (auto it = nodes_zone.begin(); it != nodes_zone.end(); it++) {
				if (&(*it) == selected_node) {
					nodes_zone.erase(it);
					selected_node = nullptr;
					return;
				}
			}
			break;
		}

		case NodeBase::NodeType::Enemy: {
			for (auto it = nodes_enemy.begin(); it != nodes_enemy.end(); it++) {
				if (&(*it) == selected_node) {
					nodes_enemy.erase(it);
					selected_node = nullptr;
					return;
				}
			}
			break;
		}
	}
}