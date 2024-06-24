#include "editor.h"

void Editor::UpdateCamera() {
	Vec2f dim = GetWindowDimensions();

	Camera::Instance()->h = dim.x;
	Camera::Instance()->w = dim.y;

	if (!ImGui::IsWindowFocused()) return;

	if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) Camera::Instance()->y = (Camera::Instance()->y + Camera::Instance()->speed);
	if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) Camera::Instance()->x = (Camera::Instance()->x + Camera::Instance()->speed);
	if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) Camera::Instance()->y = (Camera::Instance()->y - Camera::Instance()->speed);
	if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) Camera::Instance()->x = (Camera::Instance()->x - Camera::Instance()->speed);


	if (ImGui::IsKeyDown(ImGuiKey_R)) {
		Camera::Instance()->x = 0;
		Camera::Instance()->y = 0;
		Camera::Instance()->zoom = 1.0f;
	}

	if (!ImGui::IsWindowHovered()) return;

	ImGuiIO& io = ImGui::GetIO();

	if (io.MouseWheel > 0.0f) {
		Camera::Instance()->zoom += 0.2f;
	}
	else if (io.MouseWheel < 0.0f) {
		Camera::Instance()->zoom -= 0.2f;
		if (Camera::Instance()->zoom < CAMERA_ZOOM_MINIMUM) Camera::Instance()->zoom = CAMERA_ZOOM_MINIMUM;
	}
}

Vec2f Editor::GetWindowDimensions() {
	int curW, curH;
	glfwGetFramebufferSize(window, &curW, &curH);
	return Vec2f(float(curW), float(curH));
}

// imgui's mouse is often innacurate. as such, it's unreliable.
Vec2f Editor::GetWorldMousePosition() {
	double mXd;
	double mYd;

	glfwGetCursorPos(window, &mXd, &mYd);

	float mX = static_cast<float>(mXd);
	float mY = static_cast<float>(mYd);

	const float zoom = Camera::Instance()->zoom;
	const float cam_x = Camera::Instance()->x;
	const float cam_y = Camera::Instance()->y;

	mX = (mX - cam_x) / zoom;
	mY = -(mY - cam_y) / zoom;

	return Vec2f(mX, mY);
}