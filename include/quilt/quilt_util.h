#pragma once

#include <cstring>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "types.h"



namespace Quilt::Util {
    std::string BytesToHex(char* bytes, size_t len);
    std::string HexToBytes(std::string hex);

    std::string ToLower(const std::string& input);

    void DrawLine(Vec2f p1, Vec2f p2, RGBA col);
    void DrawLines(std::vector<Vec2f>& points, RGBA col, bool swap = false);
    void DrawRect(Vec2f pos, float w, float h, RGBA col, bool fill = false);
    void DrawRect(Vec2f pos1, Vec2f pos2, RGBA col, bool fill = false);
    void DrawText(Vec2f pos, std::string str);
    void DrawCircle(Vec2f pos, float radius, RGBA col);
    void DrawTooltip(std::string str, bool itemCheck = true);

    bool IsRectHovered(Vec2f pos, float w, float h);
    bool IsRectHovered(Vec2f p1, Vec2f p2);
    bool IsRectClicked(Vec2f pos, float w, float h);
    bool IsRectClicked(Vec2f p1, Vec2f p2);

    bool IsCircleHovered(Vec2f center, float radius);
    bool IsCircleClicked(Vec2f center, float radius);
    Vec2f GetMouseWorldPos();
    Vec2f ToWorld(Vec2f src);
    ImVec2 ToWorldImVec2(Vec2f src);
}