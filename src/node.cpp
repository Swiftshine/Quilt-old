#include "node.h"
#include "editor.h"

void CmnGmkNode::Update() {
    // drag
    cmnGmk->position.Swap();

    // start - check if i got clicked
    if (Quilt::Util::IsRectHovered(cmnGmk->position, GIMMICK_SQUARE_SIZE, GIMMICK_SQUARE_SIZE)) {
        int index = Swap32(cmnGmk->nameIndex);
        std::string str = Editor::Instance()->GetCommonGimmickNameFromIndex(index);
        Quilt::Util::DrawTooltip(str, false);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            Editor::Instance()->SelectNode(this);
            
            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            // calculate drag offs
            if (!isDragOffsInited) {
                dragOffs.x = worldMouse.x - cmnGmk->position.x;
                dragOffs.y = worldMouse.y - cmnGmk->position.y;
                isDragOffsInited = true;
            }
        }
    }

    // middle - change position if im being dragged
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && isDragOffsInited) {
        Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

        cmnGmk->position.x = worldMouse.x - dragOffs.x;
        cmnGmk->position.y = worldMouse.y - dragOffs.y;
    }

    // end - reset
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        isDragOffsInited = false;
    }

    cmnGmk->position.Swap();

    RGBA drawCol = COMMON_GIMMICK_COLOR_INACTIVE;
    if (isSelected) {
        drawCol = COMMON_GIMMICK_COLOR_ACTIVE;
    }

    // draw
    Vec2f p = cmnGmk->position.GetSwap();
    Quilt::Util::DrawRect(p, COMMON_GIMMICK_SQUARE_SIZE, COMMON_GIMMICK_SQUARE_SIZE, drawCol);
}

void GmkNode::Update() {
    // drag
    gmk->position.Swap();

    // start - check if i got clicked
    if (Quilt::Util::IsRectHovered(gmk->position, GIMMICK_SQUARE_SIZE, GIMMICK_SQUARE_SIZE)) {
        Quilt::Util::DrawTooltip(gmk->name, false);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            Editor::Instance()->SelectNode(this);
            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            // calculate drag offs
            if (!isDragOffsInited) {
                dragOffs.x = worldMouse.x - gmk->position.x;
                dragOffs.y = worldMouse.y - gmk->position.y;
                isDragOffsInited = true;
            }
        }
    }

    // middle - change position if im being dragged
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && isDragOffsInited) {
        Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

        gmk->position.x = worldMouse.x - dragOffs.x;
        gmk->position.y = worldMouse.y - dragOffs.y;
    }

    // end - reset
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        isDragOffsInited = false;
    }

    gmk->position.Swap();

    RGBA drawCol = GIMMICK_COLOR_INACTIVE;
    if (isSelected) {
        drawCol = GIMMICK_COLOR_ACTIVE;
    }

    // draw
    Vec2f p = gmk->position.GetSwap();
    Quilt::Util::DrawRect(p, GIMMICK_SQUARE_SIZE, GIMMICK_SQUARE_SIZE, drawCol);
}

void ZoneNode::Update() {
    zone->boundsStart.Swap();
    zone->boundsEnd.Swap();

    if (Quilt::Util::IsRectHovered(zone->boundsStart, zone->boundsEnd)) {
        Quilt::Util::DrawTooltip(zone->name, false);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            Editor::Instance()->SelectNode(this);
            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            if (!isDragOffsInited) {
                dragOffs.x = worldMouse.x - zone->boundsStart.x;
                dragOffs.y = worldMouse.y - zone->boundsStart.y;
                isDragOffsInited = true;
            }
        }
    }
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && isDragOffsInited) {
        Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

        Vec2f newBoundsStart = Vec2f(worldMouse.x - dragOffs.x, worldMouse.y - dragOffs.y);
        Vec2f offset = newBoundsStart - zone->boundsStart;
        zone->boundsStart = newBoundsStart;
        zone->boundsEnd = zone->boundsEnd + offset;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        isDragOffsInited = false;
    }

    zone->boundsStart.Swap();
    zone->boundsEnd.Swap();

    RGBA drawCol = ZONE_COLOR_INACTIVE;
    if (isSelected) {
        drawCol = ZONE_COLOR_ACTIVE;
    }

    Quilt::Util::DrawRect(zone->boundsStart.GetSwap(), zone->boundsEnd.GetSwap(), drawCol);
}
