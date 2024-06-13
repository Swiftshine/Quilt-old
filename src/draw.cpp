#include "quilt.h"


void Editor::DrawLine(Vec2f p1, Vec2f p2, RGBA col) {

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 point1 = ImVec2(p1.x * camera.zoom + camera.x, p1.y * camera.zoom + camera.y);
	ImVec2 point2 = ImVec2(p2.x * camera.zoom + camera.x, p2.y * camera.zoom + camera.y);

	draw_list->AddLine(point1, point2, IM_COL32(col.r, col.g, col.b, col.a), 1.0f);
}

void Editor::DrawLine(Vec2f p1, Vec2f p2, u32 col) {
	DrawLine(p1, p2, RGBA(col));
}

void Editor::DrawRect(Vec2f pos, float w, float h, RGBA col) {

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 topLeft = ImVec2((pos.x * camera.zoom) + camera.x, (pos.y * camera.zoom) + camera.y);
	ImVec2 bottomRight = ImVec2(topLeft.x + (w * camera.zoom), topLeft.y + (h * camera.zoom));

	draw_list->AddRect(topLeft, bottomRight, IM_COL32(col.r, col.g, col.b, col.a), 0.0f, 0, 1.0f);
}

void Editor::DrawRect(Vec2f pos, float w, float h, u32 col) {
	DrawRect(pos, w, h, RGBA(col));
}

void Editor::DrawText(Vec2f pos, std::string str) {
	ImVec2 origPos = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2((pos.x * camera.zoom) + camera.x, (pos.y * camera.zoom) + camera.y));
	ImGui::Text(str.c_str());
	ImGui::SetCursorPos(origPos);
}

void Editor::DrawCircle(Vec2f pos, float radius, RGBA col) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();


	ImVec2 center((pos.x * camera.zoom) + camera.x, (pos.y * camera.zoom) + camera.y);
	ImU32 color = IM_COL32(col.r, col.g, col.b, col.a);

	draw_list->AddCircle(center, radius, color, CIRCLE_SEGMENT_COUNT);
}

void Editor::DrawCircle(Vec2f pos, float radius, u32 col) {
	Editor::DrawCircle(pos, radius, RGBA(col));
}

void Editor::DrawTooltip(std::string str) {
	ImVec2 mousePos = ImGui::GetMousePos();

	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	if (mousePos.x >= windowPos.x && mousePos.y >= windowPos.y &&
		mousePos.x < windowPos.x + windowSize.x && mousePos.y < windowPos.y + windowSize.y) {
		ImGui::SetTooltip("%s", str.c_str());
	}
}

void Editor::RenderGrid() {
	if (!renderGrid) return;


	ImVec2 position = ImGui::GetCursorScreenPos();
	ImVec2 size = ImGui::GetContentRegionAvail();

	float cellSize = GRID_CELL_SIZE * camera.zoom;

	ImU32 color = IM_COL32(255, 255, 255, 0x10);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	int vertical_lines = static_cast<int>(size.x / cellSize);
	int horizontal_lines = static_cast<int>(size.y / cellSize);

	for (int i = 0; i < vertical_lines; i++) {
		float x = position.x + i * cellSize;
		draw_list->AddLine(ImVec2(x, position.y), ImVec2(x, position.y + size.y), color);
	}

	for (int i = 0; i <= horizontal_lines; i++) {
		float y = position.y + i * cellSize;
		draw_list->AddLine(ImVec2(position.x, y), ImVec2(position.x + size.x, y), color);
	}
}