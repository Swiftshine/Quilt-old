#pragma once


const int CIRCLE_SEGMENT_COUNT = 32;
const float SQUARE_SIZE = 4.0f;
const int MAX_SLOTS = 3;
const float GRID_CELL_SIZE = 4.0f;

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <iomanip>

#include <rapidxml.hpp>

#include <nfd.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "mapdata.h"
#include "parameter.h"
#include "node.h"


/* util */

// endianness swap functions
static u16 Swap16(u16 value) {
    return (value << 8) | (value >> 8);
}

static u32 Swap32(u32 value) {
    return ((value >> 24) & 0x000000FF) |
        ((value >> 8) & 0x0000FF00) |
        ((value << 8) & 0x00FF0000) |
        ((value << 24) & 0xFF000000);
}

static u64 Swap64(u64 value) {
    return ((value << 56) & 0xFF00000000000000ULL) |
        ((value << 40) & 0x00FF000000000000ULL) |
        ((value << 24) & 0x0000FF0000000000ULL) |
        ((value << 8) & 0x000000FF00000000ULL) |
        ((value >> 8) & 0x00000000FF000000ULL) |
        ((value >> 24) & 0x0000000000FF0000ULL) |
        ((value >> 40) & 0x000000000000FF00ULL) |
        ((value >> 56) & 0x00000000000000FFULL);
}

static float SwapF32(float value) {
    u32 intval;
    std::memcpy(&intval, &value, sizeof(float));
    intval = Swap32(intval);
    float result;
    std::memcpy(&result, &intval, sizeof(float));
    return result;
}

static double SwapF64(double value) {
    u64 intval;
    std::memcpy(&intval, &value, sizeof(double));
    intval = Swap64(intval);
    double result;
    std::memcpy(&result, &intval, sizeof(double));
    return result;
}

/* editor */

const float FONT_SIZE = 13.0f;

class Editor {
public:
    Editor();
    ~Editor();

    bool Setup();
    void Cleanup();
    void Run();
    void SetSelectedNode(NodeBase* node);
    void ClearSelectedNode();
    NodeBase* GetSelectedNode();
    Vec2f GetWindowDimensions();
    GLFWwindow* GetGLFWWindow();

    static std::vector<char> HexToBytes(std::string hex);
    static std::string BytesToHex(char* bytes, size_t len);

    static void DrawLine(Vec2f p1, Vec2f p2, RGBA col);
    static void DrawLine(Vec2f p1, Vec2f p2, u32 col);
    static void DrawRect(Vec2f pos, float w, float h, RGBA col);
    static void DrawRect(Vec2f pos, float w, float h, u32 col);
    static void DrawText(Vec2f pos, std::string str);
    static void DrawCircle(Vec2f pos, float radius, RGBA col);
    static void DrawCircle(Vec2f pos, float radius, u32 col);
    static void DrawTooltip(std::string str);

    static bool RectHover(Vec2f pos, float w, float h);
    static bool RectClick(Vec2f pos, float w, float h);

private:
    void UpdateCamera();
    void UpdateNodes();

    void SetupFile();
    void SaveFile();

    void LoadParameters();
    void ReloadParameters();
    void LoadTranslations();
    void ReloadTranslations();
    void SetupCommonGimmickNames();

    Translation* GetTranslationByHex(std::string hex);
    Translation* GetTranslationByQID(std::string qid); // QID -> Quilt IDentifier
    std::string  GetQIDByName(const std::string& name);
    void RenderFile();
    void RenderGrid();

    void HandleMenu();
    void HandleTabs();
    void HandleFileDropdown();
    void HandleCommonGimmickList();
    void HandleViewport();
    void HandleSettings();
    void HandleParameters();
    void HandleCommonGimmickParameters();
    void HandleGimmickParameters();
    void HandleControllerParameters();
    void HandleEnemyParameters();
    void ClearMapdata();
    void ClearNodes();

    void SetupDocking();

private:
    GLFWwindow* window;
    bool        running;
    bool        dockSetup;
    ImFont*     font;
    std::string folderPath;
    std::string folderName;

    Mapdata::Mapbin::Header mapHeader;
    Mapdata::Enbin::Header enHeader;

    std::vector<std::string> filenames;
    int selectedFileIndex;
    std::string selectedFile;
    bool        open;
    bool        saved;

    std::vector<GmkNode> gmkNodes;
    std::vector<EnNode> enNodes;
    std::vector<ContNode> contNodes;
    std::vector<CmnGmkNode> cmnGmkNodes;

    NodeBase* selectedNode;


    std::vector<Colbin::Entry> walls;
    std::vector<Mapdata::Mapbin::DataSegLabel> dataSegLabels;
    std::vector<Mapdata::Mapbin::CommonGimmick> commonGimmicks;
    std::vector<Mapdata::Mapbin::Gimmick> gimmicks;
    std::vector<Mapdata::Mapbin::Path> paths;
    std::vector<std::vector<Vec2f>> lines;
    std::vector<Mapdata::Mapbin::Controller> controllers;
    std::vector<Mapdata::Mapbin::CourseInfo> courseInfos;


    std::vector<std::string> rawCommonGimmickNames;
    std::vector<std::string> commonGimmickNameBytes;
    std::vector<std::string> commonGimmickNames;
    std::vector<std::string> colbinTypes;
    std::vector<std::string> dataSegLabelNames;

    std::vector<Mapdata::Enbin::EnemyEntry> enemies;
    std::vector<Mapdata::Enbin::FooterEntry> enFooterEntries;

    bool renderWalls;
    bool renderActionPoints;
    bool renderGimmicks;
    bool renderControllers;
    bool renderPaths;
    bool renderEnemies;
    bool renderGrid;

    std::string parameterXMLContents;
    std::string translationXMLContents;

    std::string cachedParamName;

    std::vector<std::pair<std::string, std::vector<GmkParamInfo>>> gmkParams;
    std::vector<std::pair<std::string, std::vector<GmkParamInfo>>> contParams;
    std::vector<std::pair<std::string, std::vector<CmnGmkParamInfo>>> cmnGmkParams;

    std::unordered_map<std::string, Translation> translations;
};