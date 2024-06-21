#pragma once

#include "quilt.h"
#include "mapdata.h"
#include "node.h"
#include "param.h"

using NameNotePair = std::pair<std::string, std::string>;
using QIDNotePair = std::pair<std::string, std::string>;

class Editor {
public:
    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;

    static Editor* Instance() {
        static Editor inst;
        return &inst;
    }
    
    // Primary
    bool Setup();
    void Run();
    void Cleanup();

    // Editor
    void SelectNode(NodeBase* node);
    void DeselectNode();

    // Utility
    Vec2f GetWindowDimensions();
    Vec2f GetWorldMousePosition();

    std::string GetCommonGimmickNameFromIndex(size_t index);
    std::string GetQIDFromIndex(size_t index);
    size_t GetIndexFromHex(std::string hex);
    size_t GetIndexFromName(std::string name);

private:
    Editor() { };
    ~Editor() { };

    // Quilt
    void Menu();
    void Tabs();
    // -> Canvas
    void Tabs_Canvas();
    void Canvas_Viewport();
    void Canvas_Toolbox();
    // -> -> Viewport
    void Viewport_ClickMenu();
    // -> -> Toolbox
    void Toolbox_Files();
    void Toolbox_Visibility();
    void Toolbox_StringSearch();

    // -> Other
    void Tabs_Settings();
    void Tabs_Quilt();

    void Process();

    // -> Parameter handling
    void LoadParams();
    void LoadTranslations();
    void ClearParams();
    void Param_CommonGimmick();
    void Param_Gimmick();
    void Param_Zone();
    
    // Camera
    void UpdateCamera();

    // -> File
    void OpenFolder();
    void SaveFile();
    void ClearFilenames();
    void SetupFile();

    // Mapdata
    void ClearMapdata();
    void RenderMapdata();

    // -> Nodes
    void UpdateNodes();
    void ClearNodes();
    void CreateNode(NodeBase::NodeType type);
    void DeleteNode();
    

private:
    // General
    bool running = false;

    // OpenGL
    GLFWwindow* window = nullptr;

    // ImGui
    ImFont* font = nullptr;

    // Quilt
    bool open = false;

    // -> file
    std::string folder_path = "";
    std::string folder_name = "";
    std::string selected_file = "";
    std::vector<std::string> filenames;
    int selected_file_index = 0;

    // -> params
    std::string param_xml_contents;
    std::string translation_xml_contents;

    // i call this "general" because other structures use the same parameter pack
    std::vector<std::pair<NameNotePair, std::vector<GmkParam>>> params_general;
    std::vector<std::pair<QIDNotePair, std::vector<GmkParam>>> params_cmnGmk;

    // -> visibility options
    bool showBounds = true;
    bool showWalls = true;
    bool showCommon = true;
    bool showGimmicks = true;
    bool showPaths = true;
    bool showZones = true;

    // -> toolbox
    char cmnGmkQuery[128] = { 0 };
    int query_selected_index = -1;
    std::string query_selected_label;

    // Mapdata
    Mapdata::Mapbin::Header* mapHeader = new Mapdata::Mapbin::Header;
    Mapdata::Enbin::Header* enHeader = new Mapdata::Enbin::Header;


    // -> strings
    std::vector<Translation> translations;
    std::vector<std::string> bytes_cmnGmk;

    // -> nodes
    NodeBase* selected_node = nullptr;

    // for some reason my wall node code got irreversibly messed up
    // so i'll just use these for now until i can rewrite it proper
    std::vector<Colbin::Entry> walls;
    std::vector<Mapdata::Mapbin::Path> paths;
    std::vector<std::vector<Vec2f>> lines_path;

    std::vector<CmnGmkNode> nodes_cmnGmk;
    std::vector<GmkNode> nodes_gmk;
    std::vector<ZoneNode> nodes_zone;

    // debug (everything below this point must be erased at the end)
    
};