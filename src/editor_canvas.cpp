#include "editor.h"

void Editor::Canvas_Viewport() {
	ImGui::Begin("Viewport");
	ImGui::BeginChild("Viewport_Drop");
	
	UpdateCamera();

	Viewport_ClickMenu();
	RenderMapdata();
	UpdateNodes();


	if (selected_node) {
		switch (selected_node->GetType()) {
			case NodeBase::NodeType::LabeledWall: {
				Param_LabeledWall();
				break;
			}

			case NodeBase::NodeType::Gimmick: {
				Param_Gimmick();
				break;
			}

			case NodeBase::NodeType::Path: {
				Param_Path();
				break;
			}
			case NodeBase::NodeType::CommonGimmick: {
				Param_CommonGimmick();
				break;
			}

			case NodeBase::NodeType::Zone: {
				Param_Zone();
				break;
			}

			case NodeBase::NodeType::Enemy: {
				Param_Enemy();
				break;
			}

			case NodeBase::NodeType::RaceCourseInfo: {
				Param_RaceCourseInfo();
				break;
			}
		}
	}
	
	ImGui::EndChild();
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_DROP_ITEM")) {

			if (SearchType::CommonGimmick == search_type) {
				CreateNode(NodeBase::NodeType::CommonGimmick);
			}
			if (SearchType::Enemy == search_type) {
				CreateNode(NodeBase::NodeType::Enemy);
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::End();
}

void Editor::Canvas_Toolbox() {
	ImGui::Begin("Toolbox");

	Toolbox_Files();
	Toolbox_Visibility();
	Toolbox_StringSearch();

	ImGui::End();
}

void Editor::Viewport_ClickMenu() {
	if (ImGui::BeginPopupContextWindow("RightClickMenu")) {
		if (ImGui::BeginMenu("Add...")) {
			if (ImGui::MenuItem("Wall")) {
				CreateNode(NodeBase::NodeType::Wall);
			}

			if (ImGui::MenuItem("Labeled Wall")) {
				CreateNode(NodeBase::NodeType::LabeledWall);
			}

			if (ImGui::MenuItem("Gimmick")) {
				CreateNode(NodeBase::NodeType::Gimmick);
			}

			if (ImGui::MenuItem("Race Course Info")) {
				CreateNode(NodeBase::NodeType::RaceCourseInfo);
			}

			if (ImGui::MenuItem("Zone")) {
				CreateNode(NodeBase::NodeType::Zone);
			}

			ImGui::EndMenu();
		}

		if (selected_node) {
			if (ImGui::MenuItem("Deselect Node")) {
				DeselectNode();
			}
			if (ImGui::MenuItem("Delete Node")) {
				DeleteNode();
			}
		}

		ImGui::EndPopup();
	}
}