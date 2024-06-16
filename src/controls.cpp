#include "quilt.h"

bool Editor::RectHover(Vec2f pos, float w, float h) {
	ImVec2 mousePos = ImGui::GetMousePos();
	Vec2f topLeft = Vec2f((pos.x * camera.zoom) + camera.x, (pos.y * camera.zoom) + camera.y);
	Vec2f bottomRight = Vec2f(topLeft.x + (w * camera.zoom), topLeft.y + (h * camera.zoom));

	return (mousePos.x >= topLeft.x && mousePos.x <= bottomRight.x &&
		mousePos.y >= topLeft.y && mousePos.y <= bottomRight.y);
}

bool Editor::RectClick(Vec2f pos, float w, float h) {
	return RectHover(pos, w, h) && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
}

void Editor::UpdateCamera() {
	Vec2f dimensions = GetWindowDimensions();

	camera.w = static_cast<float>(dimensions.x);
	camera.h = static_cast<float>(dimensions.y);

	if (!(ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))) return;

	// this part is just for the relatively hacky node dragging
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) return;

	if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) camera.y = (camera.y + camera.speed);
	if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) camera.x = (camera.x + camera.speed);
	if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) camera.y = (camera.y - camera.speed);
	if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) camera.x = (camera.x - camera.speed);

	if (ImGui::IsKeyDown(ImGuiKey_R)) {
		camera.x = 0.0f;
		camera.y = 0.0f;
		camera.zoom = 1.0f;
	}

	ImGuiIO& io = ImGui::GetIO();

	if (io.MouseWheel > 0.0f) {
		camera.zoom += 0.1f;
	}
	else if (io.MouseWheel < 0.0f) {
		camera.zoom -= 0.1f;

		if (camera.zoom < 1.0f) camera.zoom = 1.0f;
	}
}

Vec2f Editor::GetWindowDimensions() {
	int curW, curH;
	glfwGetFramebufferSize(window, &curW, &curH);

	return Vec2f((float)curW, (float)curH);
}