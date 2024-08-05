#include "node.h"
#include "editor.h"

void WallNode::Update() {
    wall->point1.Swap();

    // handle the first point
    if (Quilt::Util::IsRectHovered(wall->point1, WALL_POINT_SIZE, WALL_POINT_SIZE)) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            Editor::Instance()->SelectNode(this);

            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            // calculate drag offs
            if (!isDragOffsInited) {
                dragOffs1.x = worldMouse.x - wall->point1.x;
                dragOffs1.y = worldMouse.y - wall->point1.y;
                isDragOffsInited1 = true;
            }
        }
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && isDragOffsInited1) {
        Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

        wall->point1.x = worldMouse.x - dragOffs1.x;
        wall->point1.y = worldMouse.y - dragOffs1.y;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        isDragOffsInited1 = false;
    }

    wall->point1.Swap();

    // second point
    wall->point2.Swap();;
    if (Quilt::Util::IsRectHovered(wall->point2, WALL_POINT_SIZE, WALL_POINT_SIZE)) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            Editor::Instance()->SelectNode(this);

            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            // calculate drag offs
            if (!isDragOffsInited) {
                dragOffs2.x = worldMouse.x - wall->point2.x;
                dragOffs2.y = worldMouse.y - wall->point2.y;
                isDragOffsInited2 = true;
            }
        }
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && isDragOffsInited2) {
        Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

        wall->point2.x = worldMouse.x - dragOffs2.x;
        wall->point2.y = worldMouse.y - dragOffs2.y;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        isDragOffsInited2 = false;
    }

    wall->point2.Swap();

    RGBA drawCol = COLOR_WALL_INACTIVE;
    if (isSelected) drawCol = COLOR_WALL_ACTIVE;

    Vec2f p1 = wall->point1.GetSwap();
    Quilt::Util::DrawRect(p1, WALL_POINT_SIZE, WALL_POINT_SIZE, drawCol, isSelected);
    Vec2f p2 = wall->point2.GetSwap();
    Quilt::Util::DrawRect(p2, WALL_POINT_SIZE, WALL_POINT_SIZE, drawCol, isSelected);
    Quilt::Util::DrawLine(p1, p2, drawCol);
    if (isSelected) {
        Quilt::Util::DrawText(Vec2f(p1.x, p1.y + 1.0), "1");
        Quilt::Util::DrawText(Vec2f(p2.x, p2.y + 1.0), "2");
    }
}

void LabeledWallNode::Update() {
    wall->point1.Swap();

    // handle the first point
    if (Quilt::Util::IsRectHovered(wall->point1, WALL_POINT_SIZE, WALL_POINT_SIZE)) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            Editor::Instance()->SelectNode(this);

            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            // calculate drag offs
            if (!isDragOffsInited) {
                dragOffs1.x = worldMouse.x - wall->point1.x;
                dragOffs1.y = worldMouse.y - wall->point1.y;
                isDragOffsInited1 = true;
            }
        }
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && isDragOffsInited1) {
        Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

        wall->point1.x = worldMouse.x - dragOffs1.x;
        wall->point1.y = worldMouse.y - dragOffs1.y;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        isDragOffsInited1 = false;
    }

    wall->point1.Swap();

    // second point
    wall->point2.Swap();;
    if (Quilt::Util::IsRectHovered(wall->point2, WALL_POINT_SIZE, WALL_POINT_SIZE)) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            Editor::Instance()->SelectNode(this);

            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            // calculate drag offs
            if (!isDragOffsInited) {
                dragOffs2.x = worldMouse.x - wall->point2.x;
                dragOffs2.y = worldMouse.y - wall->point2.y;
                isDragOffsInited2 = true;
            }
        }
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && isDragOffsInited2) {
        Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

        wall->point2.x = worldMouse.x - dragOffs2.x;
        wall->point2.y = worldMouse.y - dragOffs2.y;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        isDragOffsInited2 = false;
    }

    wall->point2.Swap();

    RGBA drawCol = COLOR_WALL_LABEL_INACTIVE;
    if (isSelected) drawCol = COLOR_WALL_LABEL_ACTIVE;;

    Vec2f p1 = wall->point1.GetSwap();
    Quilt::Util::DrawRect(p1, WALL_POINT_SIZE, WALL_POINT_SIZE, drawCol, isSelected);
    Vec2f p2 = wall->point2.GetSwap();
    Quilt::Util::DrawRect(p2, WALL_POINT_SIZE, WALL_POINT_SIZE, drawCol, isSelected);
    Quilt::Util::DrawLine(p1, p2, drawCol);
    if (isSelected) {
        Quilt::Util::DrawText(Vec2f(p1.x, p1.y + 1.0), "1");
        Quilt::Util::DrawText(Vec2f(p2.x, p2.y + 1.0), "2");
    }
}

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

    RGBA drawCol = COLOR_GIMMICK_COMMON_INACTIVE;
    if (isSelected) {
        drawCol = COLOR_GIMMICK_COMMON_ACTIVE;
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

    RGBA drawCol = COLOR_GIMMICK_UNIQUE_INACTIVE;
    if (isSelected) {
        drawCol = COLOR_GIMMICK_UNIQUE_ACTIVE;
    }

    // draw
    Vec2f p = gmk->position.GetSwap();
    Quilt::Util::DrawRect(p, GIMMICK_SQUARE_SIZE, GIMMICK_SQUARE_SIZE, drawCol);
}


void PathNode::Update() {
    RGBA drawCol = COLOR_PATH_INACTIVE;
    if (isSelected) drawCol = COLOR_PATH_ACTIVE;
    for (auto i = 0; i < points.size(); i++) {
        auto& point = points[i];
        point.Swap();

        if (Quilt::Util::IsRectHovered(point, WALL_POINT_SIZE, WALL_POINT_SIZE)) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                Editor::Instance()->SelectNode(this);

                Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

                if (!dragOffsInited[i]) {
                    dragOffsets[i].x = worldMouse.x - point.x;
                    dragOffsets[i].y = worldMouse.y - point.y;
                    dragOffsInited[i] = true;
                }
            }
        }

        if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && dragOffsInited[i]) {
            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            point.x = worldMouse.x - dragOffsets[i].x;
            point.y = worldMouse.y - dragOffsets[i].y;
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            dragOffsInited[i] = false;
        }

        point.Swap();

        Vec2f p = point.GetSwap();
        Quilt::Util::DrawRect(p, WALL_POINT_SIZE, WALL_POINT_SIZE, drawCol, isSelected);
    }

    Quilt::Util::DrawLines(points, drawCol, true);
}

void RaceCourseInfoNode::Update() {
    // drag
    rcinfo->position.Swap();

    // start - check if i got clicked
    if (Quilt::Util::IsRectHovered(rcinfo->position, GIMMICK_SQUARE_SIZE, GIMMICK_SQUARE_SIZE)) {
        Quilt::Util::DrawTooltip(rcinfo->name, false);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            Editor::Instance()->SelectNode(this);
            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            // calculate drag offs
            if (!isDragOffsInited) {
                dragOffs.x = worldMouse.x - rcinfo->position.x;
                dragOffs.y = worldMouse.y - rcinfo->position.y;
                isDragOffsInited = true;
            }
        }
    }

    // middle - change position if im being dragged
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && isDragOffsInited) {
        Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

        rcinfo->position.x = worldMouse.x - dragOffs.x;
        rcinfo->position.y = worldMouse.y - dragOffs.y;
    }

    // end - reset
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        isDragOffsInited = false;
    }

    rcinfo->position.Swap();

    RGBA drawCol = COLOR_RACE_COURSE_INFO_INACTIVE;
    if (isSelected) {
        drawCol = COLOR_RACE_COURSE_INFO_ACTIVE;
    }

    // draw
    Vec2f p = rcinfo->position.GetSwap();
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

    RGBA drawCol = COLOR_ZONE_INACTIVE;
    if (isSelected) {
        drawCol = COLOR_ZONE_ACTIVE;
    }

    Quilt::Util::DrawRect(zone->boundsStart.GetSwap(), zone->boundsEnd.GetSwap(), drawCol);
}

void EnemyNode::Update() {
    // drag
    enemy->position1.Swap();

    // start - check if i got clicked
    if (Quilt::Util::IsRectHovered(enemy->position1, GIMMICK_SQUARE_SIZE, GIMMICK_SQUARE_SIZE)) {
        Quilt::Util::DrawTooltip(Editor::Instance()->GetEnemyNameFromID(enemy->name), false);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            Editor::Instance()->SelectNode(this);
            Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

            // calculate drag offs
            if (!isDragOffsInited) {
                dragOffs.x = worldMouse.x - enemy->position1.x;
                dragOffs.y = worldMouse.y - enemy->position1.y;
                isDragOffsInited = true;
            }
        }
    }

    // middle - change position if im being dragged
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && isDragOffsInited) {
        Vec2f worldMouse = Editor::Instance()->GetWorldMousePosition();

        enemy->position1.x = worldMouse.x - dragOffs.x;
        enemy->position1.y = worldMouse.y - dragOffs.y;
    }

    // end - reset
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        isDragOffsInited = false;
    }

    enemy->position1.Swap();

    RGBA drawCol = COLOR_ENEMY_INACTIVE;
    if (isSelected) {
        drawCol = COLOR_ENEMY_ACTIVE;
    }

    // draw
    Vec2f p = enemy->position1.GetSwap();
    Quilt::Util::DrawRect(p, GIMMICK_SQUARE_SIZE, GIMMICK_SQUARE_SIZE, drawCol);
}