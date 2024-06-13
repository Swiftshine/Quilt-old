#pragma once


const int CIRCLE_SEGMENT_COUNT = 32;
const float SQUARE_SIZE = 8.0f;
const int MAX_SLOTS = 4;
const float GRID_CELL_SIZE = 4.0f;

#include <cstring>
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>


#include <nfd.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "mapdata.h"
#include "parse.h"
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

    static void ScrollCallback(GLFWwindow* win, double xOffs, double yOffs);

private:
    void UpdateCamera();
    void UpdateNodes();

    void SetupFile();
    void SaveFile();

    void RenderFile();
    void RenderGrid();

    void HandleMenu();
    void HandleTabs();
    void HandleFileDropdown();
    void HandleViewport();
    void HandleSettings();
    void HandleParameters();
    void HandleGimmickParameters();
    void HandleEnemyParameters();
    void ClearMapdata();
    void ClearNodes();

    void SetupDocking();

private:
    GLFWwindow* window;
    bool        running;
    bool        dockSetup;
    std::string folderPath;
    std::string folderName;

    std::vector<std::string> filenames;
    int selectedFileIndex;
    std::string selectedFile;
    bool        open;
    bool        saved;

    std::vector<GmkNode> gmkNodes;
    std::vector<EnNode> enNodes;

    NodeBase* selectedNode;


    std::vector<Colbin::Entry> walls;
    std::vector<Mapdata::Mapbin::Path> paths;
    std::vector<std::vector<Vec2f>> lines;
    std::vector<Mapdata::Mapbin::Gimmick> gimmicks;
    std::vector<Mapdata::Enbin::EnemyEntry> enemies;
    
    bool renderWalls;
    bool renderGimmicks;
    bool renderPaths;
    bool renderEnemies;
    bool renderGrid;
    
    std::vector<GMKParameter> cachedParams;
    std::string cachedParamName;
};