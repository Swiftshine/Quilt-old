#include <filesystem>
#include <set>

#include "quilt.h"
#include "parse.h"

namespace fs = std::filesystem;

GLFWwindow* Editor::GetGLFWWindow() {
    return window;
}

void Editor::SetupDocking() {
    ImGuiID dockspaceID = ImGui::GetID("Quilt_Editor_DockSpace");

    ImGui::DockBuilderRemoveNode(dockspaceID);
    ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);
    

    // central node
    ImGuiID dock_MainID = dockspaceID;
    // right node
    ImGuiID dock_RightID = ImGui::DockBuilderSplitNode(dock_MainID, ImGuiDir_Right, 0.25f, nullptr, &dock_MainID);

    ImGui::DockBuilderDockWindow("Viewport", dock_MainID);
    ImGui::DockBuilderDockWindow("Gimmick Parameters", dock_RightID);
    ImGui::DockBuilderDockWindow("Enemy Parameters", dock_RightID);
    ImGui::DockBuilderFinish(dockspaceID);
}


void Editor::HandleTabs() {
    int curW, curH;
    glfwGetFramebufferSize(window, &curW, &curH);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_DockNodeHost | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::SetNextWindowPos(ImVec2(0, 18));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(curW), static_cast<float>(curH)));

    std::string title = "Quilt - ";
    if (folderPath.empty() || !open) title += "No folder open";
    else title += folderName;


    ImGui::Begin(title.c_str(), NULL, window_flags);
    if (ImGui::BeginTabBar("QuiltTabBar")) {
        HandleViewport();
        HandleSettings();
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void Editor::HandleViewport() {
    if (ImGui::BeginTabItem("Editor")) {
        if (!open) {
            ImGui::EndTabItem();
            return;
        }

        if (!dockSetup) {
            SetupDocking();
            dockSetup;
        }

        ImGuiID dockspace_id = ImGui::GetID("Quilt_Editor_DockSpace");
        ImGui::BeginChild("Quilt_Editor_DockSpace", ImVec2(0, 0), 0, ImGuiWindowFlags_NoMove);
        ImGui::DockSpace(dockspace_id);

        if (ImGui::Begin("Viewport")) {
            HandleFileDropdown();
            RenderGrid();
            RenderFile();
            HandleParameters();
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) ClearSelectedNode();
            UpdateNodes();
            ImGui::End();
        }
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}


void Editor::HandleFileDropdown() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::SetNextWindowPos(ImVec2(10, 100));

    ImGui::Begin("Files", nullptr, flags);

    if (filenames.empty()) {
        ImGui::Text("No files available.");
        ImGui::End();
        return;
    }

    std::vector<const char*> cstrs;
    for (const auto& name : filenames) { cstrs.push_back(name.c_str()); }

    if (ImGui::Combo("Change File", &selectedFileIndex, cstrs.data(), cstrs.size())) {
        // the file selection was changed
        selectedFile = filenames[selectedFileIndex];
        SetupFile();
    }

    ImGui::End();
}

void Editor::HandleSettings() {
    if (ImGui::BeginTabItem("Settings")) {

        bool b;

        b = renderWalls;
        if (ImGui::Checkbox("Render Walls", &b)) renderWalls = b;

        b = renderGimmicks;
        if (ImGui::Checkbox("Render Gimmicks", &b)) renderGimmicks = b;

        b = renderPaths;
        if (ImGui::Checkbox("Render Paths", &b)) renderPaths = b;

        b = renderEnemies;
        if (ImGui::Checkbox("Render Enemies", &b)) renderEnemies = b;

        b = renderGrid;
        if (ImGui::Checkbox("Render Grid*", &b)) renderGrid = b;
        if (ImGui::IsItemHovered()) DrawTooltip("The grid is not always accurate.");
        ImGui::EndTabItem();
    }
}

void Editor::HandleParameters() {
    if (!GetSelectedNode()) {
        if (!cachedParamName.empty()) cachedParamName = "";
        if (0 != cachedParams.size()) cachedParams.clear();
        return;
    }


    switch (GetSelectedNode()->nodeType) {
        case NodeBase::NodeType_Gimmick:
            HandleGimmickParameters(); break;
        case NodeBase::NodeType_Enemy:
            HandleEnemyParameters(); break;
    }
}

void Editor::HandleGimmickParameters() {
    GmkNode* node = static_cast<GmkNode*>(GetSelectedNode());

    std::string title = "Gimmick Parameters";
    ImGui::Begin(title.c_str());
    
    
    ImGui::InputText("Name", node->name.data(), 0x20);
    ImGui::InputFloat("X Position", &node->position.x, 0.1f, 1.f);
    ImGui::InputFloat("Y Position", &node->position.y, 0.1f, 1.0f);

    if (node->name != cachedParamName) {
        cachedParams.clear();
        cachedParamName = node->name;
        auto ptr = parser.OpenGMK(node->name);

        if (!ptr) {
            cachedParams.clear();
            parser.Close();
            ImGui::End();
            return;
        }


        cachedParams = *ptr;
        parser.Close();
    }

    int numParams = cachedParams.size();

    for (const GMKParameter& param : cachedParams) {
        if (param.slot >= MAX_SLOTS) continue;

        if (GetSelectedNode()->nodeType != NodeBase::NodeType::NodeType_Gimmick) break;

        GmkNode* gmkNode = static_cast<GmkNode*>(GetSelectedNode());

        if ("int" == param.dataType) {

            int val = Swap32(gmkNode->params.ints[param.slot - 1]);

            ImGui::InputInt(param.name.c_str(), &val, 1, 10);
            if (ImGui::IsItemHovered()) {
                Editor::DrawTooltip(param.description);
            }
        }
        else if ("float" == param.dataType) {
            float val = SwapF32(gmkNode->params.floats[param.slot - 1]);

            ImGui::InputFloat(param.name.c_str(), &val, 0.1f, 1.0f);
            if (ImGui::IsItemHovered()) {
                Editor::DrawTooltip(param.description);
            }
        }
    }
    ImGui::End();
}

void Editor::HandleEnemyParameters() {
    EnNode* node = static_cast<EnNode*>(GetSelectedNode());
    ImGui::Begin("Enemy Parameters");
    ImGui::InputText("Name", node->name.data(), 0x20);
    ImGui::InputFloat("X Position", &node->position.x, 0.1f, 1.f);
    ImGui::InputFloat("Y Position", &node->position.y, 0.1f, 1.0f);
    ImGui::InputText("Behavior", node->behavior.data(), 0x20);
    ImGui::InputText("Target",    node->unk1.data(), 0x20);
    ImGui::InputText("Bead Type",    node->beadType.data(), 0x20);
    ImGui::InputText("Bead Color",    node->beadColor.data(), 0x20);
    ImGui::InputText("Direction",    node->direction.data(), 0x20);
    ImGui::InputText("Orientation",    node->orientation.data(), 0x20);
    ImGui::End();
}


void Editor::HandleMenu() {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open", NULL, false)) {
            nfdchar_t* outPath = nullptr;
            nfdresult_t result = NFD_PickFolder(nullptr, &outPath);
            
            if (NFD_OKAY == result) {
                folderPath = outPath;
                free(outPath);
            }
            else if (NFD_CANCEL == result) {
                // the user cancelled
                ImGui::EndMenu();
                ImGui::EndMainMenuBar();
                return;
            }
            else {
                // idk
                printf("Warning - NFD failed to open folder. Error: %s\n", NFD_GetError());
                ImGui::EndMenu();
                ImGui::EndMainMenuBar();
                return;
            }

            selectedFileIndex = 0;
            selectedFile = "";
        
            folderName = folderPath.substr(folderPath.find_last_of("/\\") + 1);
            folderPath += '/';

            std::set<std::string> uniqueNames;

            for (const auto& entry : fs::directory_iterator(folderPath)) {
                if (!entry.is_regular_file()) continue;

                fs::path path = entry.path();

                std::string extension = path.extension().string();

                if (!(".mapbin" == extension || ".enbin" == extension)) continue;

                uniqueNames.insert(path.stem().string());
            }

            filenames.assign(uniqueNames.begin(), uniqueNames.end());
            if (!filenames.empty()) {
                selectedFile = filenames[0];
                SetupFile();
            }

        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}
