#include <filesystem>
#include <set>

#include "quilt.h"

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
    ImGui::DockBuilderDockWindow("Common Gimmick Parameters", dock_RightID);
    ImGui::DockBuilderDockWindow("Controller Parameters", dock_RightID);
    ImGui::DockBuilderDockWindow("Enemy Parameters", dock_RightID);
    ImGui::DockBuilderFinish(dockspaceID);
}


void Editor::HandleTabs() {
    int curW, curH;
    glfwGetFramebufferSize(window, &curW, &curH);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBringToFrontOnFocus;
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
            UpdateCamera();
            HandleFileDropdown();
            HandleCommonGimmickList();
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

void Editor::HandleCommonGimmickList() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowPos(ImVec2(320, 100));
    ImGui::SetNextWindowSize(ImVec2(500, 200));
    ImGui::Begin("Common Gimmick List", nullptr, flags);

    for (auto const& name : commonGimmickNames) {
        ImGui::Text(name.c_str());
    }

    ImGui::End();
}


void Editor::HandleSettings() {
    if (ImGui::BeginTabItem("Settings")) {

        bool b;

        b = renderWalls;
        if (ImGui::Checkbox("Render Walls", &b)) renderWalls = b;

        b = renderActionPoints;
        if (ImGui::Checkbox("Render Action Points", &b)) renderActionPoints = b;

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
        cachedParamName.clear();
        return;
    }

    switch (GetSelectedNode()->nodeType) {
        case NodeBase::NodeType_CommonGimmick:
            HandleCommonGimmickParameters(); break;
        case NodeBase::NodeType_Gimmick:
            HandleGimmickParameters(); break;
        case NodeBase::NodeType_Enemy:
            HandleEnemyParameters(); break;
        case NodeBase::NodeType_Controller:
            HandleControllerParameters(); break;
    }
}

void Editor::HandleGimmickParameters() {
    GmkNode* node = static_cast<GmkNode*>(GetSelectedNode());

    ImGui::Begin("Gimmick Parameters");
    
    ImGui::InputText("Name", node->name.data(), 0x20);
    ImGui::InputFloat("X Position", &node->position.x, 0.1f, 1.f);
    ImGui::InputFloat("Y Position", &node->position.y, 0.1f, 1.0f);

    cachedParamName = node->name;

    for (auto& [gimmick_name, params] : gmkParams) {
        if (gimmick_name != node->name) continue;

        for (auto& info : params) {
            // slot needs to be valid
            if (info.slot >= MAX_SLOTS || info.slot < 0) continue;

            if ("int" == info.data_type) {
                int val = Swap32(node->params.ints[info.slot]);

                ImGui::InputInt(info.title.c_str(), &val, 1, 10);
                if (ImGui::IsItemHovered() && !info.description.empty()) {
                    Editor::DrawTooltip(info.description);
                }
            }
            else if ("float" == info.data_type) {
                float val = SwapF32(node->params.floats[info.slot]);

                ImGui::InputFloat(info.title.c_str(), &val, 0.1f, 1.0f);
                if (ImGui::IsItemHovered() && !info.description.empty()) {
                    Editor::DrawTooltip(info.description);
                }
            }
            else if ("string" == info.data_type) {
                ImGui::InputText(info.title.c_str(), node->params.strings[info.slot], 64);
                if (ImGui::IsItemHovered() && !info.description.empty()) {
                    Editor::DrawTooltip(info.description);
                }
            }
        }
    }
    ImGui::End();
}

void Editor::HandleCommonGimmickParameters() {
    CmnGmkNode* node = static_cast<CmnGmkNode*>(GetSelectedNode());
    ImGui::Begin("Common Gimmick Parameters");
    
    ImGui::InputText("Name", node->name.data(), 0x20);
    if (ImGui::IsItemHovered()) {
        Editor::DrawTooltip(node->name);
    }
    ImGui::InputFloat("X Position", &node->position.x, 0.1f, 1.f);
    ImGui::InputFloat("Y Position", &node->position.y, 0.1f, 1.0f);

    // instead of using a normal name (because we can't),
    // we'll use the quilt id

    std::string qid = GetQIDByName(node->name);

    cachedParamName = qid;
    if (cachedParamName.empty()) {
        ImGui::End();
        return;
    }

    std::string workingName = node->name;
    if ("0x" == workingName.substr(0, 2)) workingName = workingName.substr(2);

    for (auto& pair : cmnGmkParams) {
        if (pair.first != qid) continue;

        for (auto& info : pair.second) {
            // double-check qid
            if (info.quilt_identifier != qid) continue;

            // at the moment, we can't take a lot of parameters (such as slot)
            // into account because that part of the format
            // is still unknown

            if ("string" == info.data_type) {
                ImGui::InputText(info.title.c_str(), node->params.string1, 0x20);
                if (ImGui::IsItemHovered() && !info.description.empty()) {
                    Editor::DrawTooltip(info.description);
                }
            }
        }
    }

    ImGui::End();
}


void Editor::HandleControllerParameters() {
    ContNode* node = static_cast<ContNode*>(GetSelectedNode());

    ImGui::Begin("Controller Parameters");

    ImGui::InputText("Name", node->name.data(), 0x20);
    ImGui::InputFloat("X Position", &node->position.x, 0.1f, 1.f);
    ImGui::InputFloat("Y Position", &node->position.y, 0.1f, 1.0f);

    cachedParamName = node->name;

    for (auto& [controller_name, params] : contParams) {
        if (controller_name != node->name) continue;

        for (auto& info : params) {
            // slot needs to be valid
            if (info.slot >= MAX_SLOTS || info.slot < 0) continue;

            if ("int" == info.data_type) {
                int val = Swap32(node->params.ints[info.slot]);

                ImGui::InputInt(info.title.c_str(), &val, 1, 10);
                if (ImGui::IsItemHovered() && !info.description.empty()) {
                    Editor::DrawTooltip(info.description);
                }
            }
            else if ("float" == info.data_type) {
                float val = SwapF32(node->params.floats[info.slot]);

                ImGui::InputFloat(info.title.c_str(), &val, 0.1f, 1.0f);
                if (ImGui::IsItemHovered() && !info.description.empty()) {
                    Editor::DrawTooltip(info.description);
                }
            }
            else if ("string" == info.data_type) {
                ImGui::InputText(info.title.c_str(), node->params.strings[info.slot], 64);
                if (ImGui::IsItemHovered() && !info.description.empty()) {
                    Editor::DrawTooltip(info.description);
                }
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
    ImGui::InputText("Path Name",    node->pathName.data(), 0x20);
    ImGui::InputText("Bead Type",    node->beadType.data(), 0x20);
    ImGui::InputText("Bead Color",    node->beadColor.data(), 0x20);
    ImGui::InputText("Direction",    node->direction.data(), 0x20);
    ImGui::InputText("Orientation",    node->orientation.data(), 0x20);
    ImGui::End();
}


void Editor::HandleMenu() {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open")) {
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

        if (ImGui::MenuItem("Save")) {
            SaveFile();
        }

        if (ImGui::MenuItem("Reload Parameters")) {
            ReloadParameters();
        }

        if (ImGui::MenuItem("Reload Translations")) {
            ReloadTranslations();
        }

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}
