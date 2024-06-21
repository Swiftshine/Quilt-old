#include "editor.h"

void Editor::Canvas_Viewport() {

	ImGui::Begin("Viewport");
	ImGui::BeginChild("Viewport_Drop");
	
	Viewport_ClickMenu();
	RenderMapdata();
	UpdateNodes();


	if (selected_node) {
		switch (selected_node->GetType()) {
		case NodeBase::NodeType::Gimmick: {
			Param_Gimmick();
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
		}
	}
	
	ImGui::EndChild();
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_DROP_ITEM")) {
			CreateNode(NodeBase::NodeType::CommonGimmick);
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
			if (ImGui::MenuItem("Gimmick")) {
				CreateNode(NodeBase::NodeType::Gimmick);
			}

			if (ImGui::MenuItem("Zone")) {

			}

			if (ImGui::MenuItem("Enemy")) {

			}

			ImGui::EndMenu();
		}

		if (selected_node) {
			if (ImGui::MenuItem("Delete Node")) {
				DeleteNode();
			}
		}

		ImGui::EndPopup();
	}
}