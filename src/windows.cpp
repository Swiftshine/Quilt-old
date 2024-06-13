#include <filesystem>
#include <set>

#include "quilt.h"
#include "parse.h"

namespace fs = std::filesystem;

GLFWwindow* Editor::GetGLFWWindow() {
    return window;
}

void Editor::SetupDocking() {
    ImGuiID dockspaceID = ImGui::GetID("Quilt_Viewport_DockSpace");

    ImGui::DockBuilderRemoveNode(dockspaceID);
    ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);
    

    // central node
    ImGuiID dock_MainID = dockspaceID;
    // right node
    ImGuiID dock_RightID = ImGui::DockBuilderSplitNode(dock_MainID, ImGuiDir_Right, 0.2f, nullptr, &dock_MainID);

    ImGui::DockBuilderDockWindow("Rendering", dock_MainID);
    ImGui::DockBuilderDockWindow("Gimmick Parameters", dock_RightID);
    ImGui::DockBuilderDockWindow("Enemy Parameters", dock_RightID);
    ImGui::DockBuilderFinish(dockspaceID);
}


void Editor::HandleTabs() {
    
    int curW, curH;
    glfwGetFramebufferSize(window, &curW, &curH);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
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

static bool dock_is_setup = false;
void Editor::HandleViewport() {

    if (ImGui::BeginTabItem("Viewport")) {
        if (!open) {
            ImGui::EndTabItem();
            return;
        }

        if (!dock_is_setup) {
            SetupDocking();
            dock_is_setup = true;
        }
        ImGuiID dockspace_id = ImGui::GetID("Quilt_Viewport_DockSpace");
        ImGui::BeginChild("Quilt_Viewport_DockSpace", ImVec2(0, 0), 0, ImGuiWindowFlags_NoMove);
        ImGui::DockSpace(dockspace_id);

        HandleFileDropdown();

        if (ImGui::Begin("Rendering")) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) ClearSelectedNode();
            RenderFile();
            HandleParameters();
            UpdateNodes();
            ImGui::End();
        }
        ImGui::EndChild();

        ImGui::EndTabItem();
    }
}


void Editor::HandleFileDropdown() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove;
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
    ImGui::InputText("unk 1",    node->unk1.data(), 0x20);
    ImGui::InputText("Bead Type",    node->beadType.data(), 0x20);
    ImGui::InputText("Bead Color",    node->beadColor.data(), 0x20);
    ImGui::InputText("Direction",    node->direction.data(), 0x20);
    ImGui::InputText("Orientation",    node->orientation.data(), 0x20);
    ImGui::End();
}


void Editor::HandleMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", NULL, false)) {
                // ImGuiFileDialog doesn't have an option to open a folder,
                // so the user will open a mapdata file instead. any one will do
                ImGuiFileDialog::Instance()->OpenDialog("ChooseMapdataFile", "Choose a mapdata file. The editor will open the folder its in.", ".mapbin,.enbin");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Editor::HandleFile() {
    if (ImGuiFileDialog::Instance()->Display("ChooseMapdataFile")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            selectedFileIndex = 0;
            selectedFile = "";
            filenames.clear();

            // ImGuiFileDialog's string functions seem to work differently
            // for the first file in a directory, so let's work around it
            std::string file_name = ImGuiFileDialog::Instance()->GetFilePathName();

            // if the file is the first one, cut the rest of the path out
            file_name = fs::path(file_name).filename().string();

            // the filename is now correct and we can continue

            std::string folder_path = ImGuiFileDialog::Instance()->GetCurrentPath();


            // next we need to get the folder's base name, not path
            // unfortunately, std::filesystem doesn't seem to have a function
            // that does this for folder names, so we'll have to do it ourselves
            auto lastSlash = folder_path.find_last_of("/\\");
            std::string folder_name = folder_path.substr(lastSlash + 1);

            // okay, we finally have a normal string name, we're done with it for now
            folderName = folder_name;
            folderPath = folder_path +  '/'; // using a forward slash because every OS supports one

            std::set<std::string> unique_names;

            for (const auto& entry : fs::directory_iterator(folderPath)) {
                if (!entry.is_regular_file()) continue;

                fs::path path = entry.path();

                std::string extension = path.extension().string();

                if (!(".mapbin" == extension || ".enbin" == extension)) continue;

                unique_names.insert(path.stem().string());
            }

            filenames.assign(unique_names.begin(), unique_names.end());
            selectedFile = filenames[0];
            if (!selectedFile.empty()) SetupFile();
        }

        ImGuiFileDialog::Instance()->Close();
    }
}