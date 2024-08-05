#pragma once

#include "quilt.h"
#include "mapdata.h"
#include "node.h"
#include "param.h"

using NameNotePair = std::pair<std::string, std::string>;
using QIDNotePair = std::pair<std::string, std::string>;

class Editor {
public:
    static enum SearchType : int {
        DefaultSearchType,
        CommonGimmick,
        Enemy,
    };

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
    std::string GetEnemyNameFromID(std::string id);
    std::string GetEnemyIDFromName(std::string name);
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
    // -> -> -> String Search
    void StringSearch_CommonGimmick();
    void StringSearch_Enemy();

    // -> Other
    void Tabs_Settings();
    void Tabs_Quilt();

    // -> Parameter handling
    void LoadParams();
    void LoadTranslations();
    void LoadEnemyTranslations();
    void ClearParams();
    void Param_CommonGimmick();
    void Param_Gimmick();
    void Param_Zone();
    void Param_RaceCourseInfo();
    void Param_Enemy();
    void Param_Path();
    void Param_LabeledWall();
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
    std::string enemy_translation_xml_contents;
    
    // i call this "general" because other structures use the same parameter pack
    std::vector<std::pair<NameNotePair, std::vector<Param>>> params_general;
    std::vector<std::pair<QIDNotePair, std::vector<Param>>> params_cmnGmk;
    // -> visibility options
    bool showBounds = true;
    bool showWalls = true;
    bool showCommon = true;
    bool showGimmicks = true;
    bool showPaths = true;
    bool showRCInfo = true;
    bool showZones = true;
    bool showEnemies = true;

    // -> toolbox
    char search_cmnGmk_query[128] = { 0 };
    char search_enemy_query[128] = { 0 };
    // -> string search
    std::vector<std::string> search_cmnGmk_filtered_items;
    std::vector<std::string> search_enemy_filtered_items;
    int search_cmnGmk_index = -1;
    int search_enemy_index = -1;
    std::string search_cmnGmk_label;
    std::string search_enemy_label;
    SearchType search_type;

    // Mapdata
    Mapdata::Mapbin::Header* mapHeader = new Mapdata::Mapbin::Header;
    Mapdata::Enbin::Header* enHeader = new Mapdata::Enbin::Header;


    // -> strings
    std::vector<Translation> translations;
    std::vector<std::string> bytes_cmnGmk;
    std::vector<EnemyTranslation> enemy_translations;

    // -> nodes
    NodeBase* selected_node = nullptr;

    std::vector<WallNode> nodes_wall;
    std::vector<LabeledWallNode> nodes_labeledWall;
    std::vector<CmnGmkNode> nodes_cmnGmk;
    std::vector<GmkNode> nodes_gmk;
    std::vector<PathNode> nodes_path;
    std::vector<RaceCourseInfoNode> nodes_rcinfo;
    std::vector<ZoneNode> nodes_zone;
    std::vector<EnemyNode> nodes_enemy;

    // debug (everything below this point must be erased at the end)
    
};