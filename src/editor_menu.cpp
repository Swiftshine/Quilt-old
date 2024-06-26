#include "editor.h"

void Editor::Menu() {
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Open")) {
			OpenFolder();
		}

		if (ImGui::MenuItem("Save")) {
			SaveFile();
		}

		if (ImGui::MenuItem("Reload Parameters")) {
			LoadParams();
		}

		if (ImGui::MenuItem("Reload Translations")) {
			LoadTranslations();
			LoadEnemyTranslations();
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}