#include "quilt.h"

std::string Quilt::Util::BytesToHex(char* bytes, size_t len) {
	std::stringstream ss;

	for (auto i = 0; i < len; i++) {
		// using uppercase for arbitrary reasons
		ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(bytes[i]) & 0xFF);
	}

	return ss.str();
}

std::string Quilt::Util::HexToBytes(std::string hex) {
	std::string bytes;

	if (hex.length() % 2 != 0) {
		// must be even. blame the person who wrote the xml for messing up
		throw std::invalid_argument("Hex string must have an even number of characters.");
	}

	for (auto i = 0; i < hex.length(); i += 2) {
		std::string byteStr = hex.substr(i, 2);
		char byte = (char)strtol(byteStr.c_str(), NULL, 16);
		bytes += byte;
	}
	return bytes;
}

std::string Quilt::Util::ToLower(const std::string& input) {
	std::string lower = input;
	std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {return std::tolower(c);  });
	return lower;
}


Vec2f Quilt::Util::ToWorld(Vec2f src) {
	Vec2f result;
	const float zoom = Camera::Instance()->zoom;
	const float cam_x = Camera::Instance()->x;
	const float cam_y = Camera::Instance()->y;

	result.x = result.x * zoom + cam_x;
	result.y = (-result.y * zoom) + cam_y;
	return result;
}

ImVec2 Quilt::Util::ToWorldImVec2(Vec2f src) {
	ImVec2 result;
	const float zoom = Camera::Instance()->zoom;
	const float cam_x = Camera::Instance()->x;
	const float cam_y = Camera::Instance()->y;

	result.x = src.x * zoom + cam_x;
	result.y = (-src.y * zoom) + cam_y;
	return result;
}
void Quilt::Util::DrawLine(Vec2f p1, Vec2f p2, RGBA col) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 point1 = ToWorldImVec2(p1);
	ImVec2 point2 = ToWorldImVec2(p2);

	draw_list->AddLine(point1, point2, IM_COL32(col.r, col.g, col.b, col.a), 1.0f);
}

void Quilt::Util::DrawLines(std::vector<Vec2f>& points, RGBA col) {
	if (points.size() < 2) return; // not enough points to draw lines


	for (auto i = 0; i < points.size() - 1; i++) {
		Quilt::Util::DrawLine(points[i], points[i + 1], col);
	}
}

void Quilt::Util::DrawRect(Vec2f pos, float w, float h, RGBA col) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	const float zoom = Camera::Instance()->zoom;
	const float cam_x = Camera::Instance()->x;
	const float cam_y = Camera::Instance()->y;

	ImVec2 p1 = ImVec2(pos.x * zoom + cam_x, (-pos.y * zoom) + cam_y);

	ImVec2 p2 = ImVec2((pos.x + w) * zoom + cam_x, (-pos.y * zoom) + cam_y);
	ImVec2 p3 = ImVec2((pos.x + w) * zoom + cam_x, (-(pos.y + h) * zoom) + cam_y);
	ImVec2 p4 = ImVec2(pos.x * zoom + cam_x, (-(pos.y + h) * zoom) + cam_y);

	draw_list->AddQuad(p1, p2, p3, p4, IM_COL32(col.r, col.g, col.b, col.a));
}

void Quilt::Util::DrawRect(Vec2f pos1, Vec2f pos2, RGBA col) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();


	ImVec2 world1 = ToWorldImVec2(pos1);
	ImVec2 world2 = ToWorldImVec2(pos2);

	ImVec2 topLeft = ImVec2(std::min(world1.x, world2.x), std::min(world1.y, world2.y));
	ImVec2 bottomRight = ImVec2(std::max(world1.x, world2.x), std::max(world1.y, world2.y));

	ImVec2 topRight = ImVec2(bottomRight.x, topLeft.y);
	ImVec2 bottomLeft = ImVec2(topLeft.x, bottomRight.y);

	draw_list->AddQuad(topLeft, topRight, bottomRight, bottomLeft, IM_COL32(col.r, col.g, col.b, col.a));
}

void Quilt::Util::DrawText(Vec2f pos, std::string str) {
	const float zoom = Camera::Instance()->zoom;
	const float cam_x = Camera::Instance()->x;
	const float cam_y = Camera::Instance()->y;

	ImVec2 origPos = ImGui::GetCursorPos();

	ImGui::SetCursorPos(ToWorldImVec2(pos));
	ImGui::Text(str.c_str());
	ImGui::SetCursorPos(origPos);
}

void Quilt::Util::DrawCircle(Vec2f pos, float radius, RGBA col) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	const float zoom = Camera::Instance()->zoom;
	const float cam_x = Camera::Instance()->x;
	const float cam_y = Camera::Instance()->y;

	ImVec2 center = ToWorldImVec2(pos);
	ImU32 color = IM_COL32(col.r, col.g, col.b, col.a);

	draw_list->AddCircle(center, radius, color, CIRCLE_SEGMENT_COUNT);
}

void Quilt::Util::DrawTooltip(std::string str, bool itemCheck) {
	if (itemCheck) {
		if (!ImGui::IsItemHovered() || str.empty()) return;
	}

	ImVec2 mousePos = ImGui::GetMousePos();

	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	if (mousePos.x >= windowPos.x && mousePos.y >= windowPos.y &&
		mousePos.x < windowPos.x + windowSize.x && mousePos.y < windowPos.y + windowSize.y) {
		ImGui::SetTooltip("%s", str.c_str());
	}
}

bool Quilt::Util::IsRectHovered(Vec2f pos, float w, float h) {
	if (!ImGui::IsWindowHovered() || !ImGui::IsWindowFocused()) return false;
	const float zoom = Camera::Instance()->zoom;
	const float cam_x = Camera::Instance()->x;
	const float cam_y = Camera::Instance()->y;

	const ImVec2 mousePos = ImGui::GetMousePos();
	const Vec2f bottomLeft = Vec2f((pos.x * zoom) + cam_x, -(pos.y * zoom) + cam_y);
	const Vec2f topRight = Vec2f(bottomLeft.x + (w * zoom), bottomLeft.y - (h * zoom));

	// POSITIVE Y IS DOWN!

	// conditions to check if the mouse is...

	// to the left of my right wall
	bool a = mousePos.x < topRight.x;

	// to the right of my left wall
	bool b = mousePos.x > bottomLeft.x;

	// above my floor
	bool c = mousePos.y < bottomLeft.y;

	// below my ceiling
	bool d = mousePos.y > topRight.y;

	return a && b && c && d;
}

bool Quilt::Util::IsRectHovered(Vec2f p1, Vec2f p2) {
	if (!ImGui::IsWindowHovered() || !ImGui::IsWindowFocused()) return false;
	const ImVec2 mousePos = ImGui::GetMousePos();

	ImVec2 world1 = ToWorldImVec2(p1);
	ImVec2 world2 = ToWorldImVec2(p2);


	// calculate the bounding box
	float minX = std::min(world1.x, world2.x);
	float maxX = std::max(world1.x, world2.x);
	float minY = std::min(world1.y, world2.y);
	float maxY = std::max(world1.y, world2.y);

	bool isHovered = (mousePos.x >= minX && mousePos.x <= maxX &&
		mousePos.y >= minY && mousePos.y <= maxY);

	return isHovered;
}


bool Quilt::Util::IsRectClicked(Vec2f pos, float h, float w) {
	return Quilt::Util::IsRectHovered(pos, h, w) && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
}

bool Quilt::Util::IsRectClicked(Vec2f p1, Vec2f p2) {
	return Quilt::Util::IsRectHovered(p1, p2) && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
}

Vec2f Quilt::Util::GetMouseWorldPos() {
	ImVec2 mousePos = ImGui::GetIO().MousePos;

	return ToWorld(Vec2f(mousePos.x, mousePos.y));
}