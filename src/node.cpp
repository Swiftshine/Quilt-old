#include "quilt.h"


NodeBase::NodeBase() {
	name = "";
	position = Vec2f();
	drawPosition = Vec2f();
	size = Vec2f();
	color = RGBA(0xFFFFFFFF);
	selectionColor = RGBA(color.r, color.g, color.b, color.a / 2);
	drawType = DrawType_Square;
	isSelected = false;
	draggable = false;
	paramsShown = false;
	nodeType = NodeType_Base;
}

NodeBase::~NodeBase() {

}

void NodeBase::Update() {
	HandleDragging();
	Draw();
}

bool NodeBase::RectHover() {
	return Editor::RectHover(drawPosition, size.x, size.y);
}

bool NodeBase::RectClick() {
	if (Editor::RectClick(drawPosition, size.x, size.y)) {
		EnableDragging();
		return true;
	}

	return false;
}

// dragging kinda sucks atm
// so ignore this hacky solution
void NodeBase::HandleDragging() {
	ImGuiIO& io = ImGui::GetIO();

	if (draggable && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		position.x += (io.MouseDelta.x / camera.zoom) / camera.zoom;
		position.y -= (io.MouseDelta.y / camera.zoom) / camera.zoom;
	}
	else {
		DisableDragging();
	}
}


void NodeBase::EnableDragging() {
	draggable = true;
}
void NodeBase::DisableDragging() { draggable = false; }

void NodeBase::Draw() {
	if (RectHover() && !name.empty()) Editor::DrawTooltip(name.c_str());

	float drawX = position.x * camera.zoom + camera.x;
	float drawY = camera.h - (position.y * camera.zoom) + camera.y;
	drawY -= size.y;
	drawPosition = Vec2f(drawX, drawY);

	RGBA drawCol = color;
	if (RectHover() || isSelected) drawCol = selectionColor;


	switch (drawType) {
		case DrawType_Circle: break;

		case DrawType_Square: {
			Editor::DrawRect(drawPosition, size.x, size.y, drawCol);
			break;
		}

		case DrawType_Triangle: break;

		case DrawType_Line: {
			Editor::DrawLine(drawPosition, size, drawCol);
			break;
		}
	}
}

GmkNode::GmkNode() { params = { 0 }; }

GmkNode::~GmkNode() { }

void GmkNode::Configure(Mapdata::Mapbin::Gimmick* gimmick) {
	nodeType = NodeType_Gimmick;
	color = GIMMICK_COLOR;
	selectionColor = GIMMICK_COLOR_SELECT;

	EnableDragging();

	position = Vec2f(SwapF32(gimmick->position.x), SwapF32(gimmick->position.y));

	float drawX = SwapF32(gimmick->position.x) * camera.zoom + camera.x;
	float drawY = camera.h - (SwapF32(gimmick->position.y) * camera.zoom) + camera.y;

	size = Vec2f(SQUARE_SIZE, SQUARE_SIZE);
	drawY -= size.y;
	drawPosition = Vec2f(drawX, drawY);
	name = gimmick->name;
	params = gimmick->params;
}

CmnGmkNode::CmnGmkNode() { nameIndex = -1; params = { 0 }; }
CmnGmkNode::~CmnGmkNode() { }

void CmnGmkNode::Configure(Mapdata::Mapbin::CommonGimmick* cmnGmk) {
	nodeType = NodeType_CommonGimmick;
	color = COMMON_GIMMICK_COLOR;
	selectionColor = COMMON_GIMMICK_COLOR_SELECT;

	EnableDragging();

	position = Vec2f(SwapF32(cmnGmk->position.x), SwapF32(cmnGmk->position.y));

	params = cmnGmk->params;

	nameIndex = Swap32(cmnGmk->nameIndex);

	float drawX = SwapF32(cmnGmk->position.x) * camera.zoom + camera.x;
	float drawY = camera.h - (SwapF32(cmnGmk->position.y) * camera.zoom) + camera.y;
	
	size = Vec2f(SQUARE_SIZE / 2, SQUARE_SIZE / 2);
	drawY -= size.y;
	drawPosition = Vec2f(drawX, drawY);
}

ContNode::ContNode() { params = { 0 }; }

ContNode::~ContNode() { }

void ContNode::Configure(Mapdata::Mapbin::Controller* controller) {
	nodeType = NodeType_Controller;
	color = CONTROLLER_COLOR;
	selectionColor = CONTROLLER_COLOR_SELECT;

	EnableDragging();

	position = Vec2f(SwapF32(controller->boundsStart.x), SwapF32(controller->boundsStart.y));

	float drawX = SwapF32(controller->boundsStart.x) * camera.zoom + camera.x;
	float drawY = camera.h - (SwapF32(controller->boundsStart.y) * camera.zoom) + camera.y;

	
	size.x = SwapF32(controller->boundsEnd.x) - SwapF32(controller->boundsStart.x);
	size.y = SwapF32(controller->boundsEnd.y) - SwapF32(controller->boundsStart.y);
																					   
	drawY -= size.y;
	drawPosition = Vec2f(drawX, drawY);
	name = controller->name;
	params = controller->params;
}

EnNode::EnNode() {
	behavior = "";
	pathName = "";
	beadType = "";
	beadColor = "";
	direction = "";
	orientation = "";
}

EnNode::~EnNode() { }

void EnNode::Configure(Mapdata::Enbin::EnemyEntry* enemy) {
	nodeType = NodeType_Enemy;
	position = Vec2f(SwapF32(enemy->position1.x), SwapF32(enemy->position1.y));

	color = ENEMY_COLOR;
	selectionColor = ENEMY_COLOR_SELECT;

	EnableDragging();

	float drawX = SwapF32(enemy->position1.x) * camera.zoom + camera.x;
	float drawY = camera.h - (SwapF32(enemy->position1.y) * camera.zoom) + camera.y;

	size = Vec2f(SQUARE_SIZE, SQUARE_SIZE);
	drawY -= size.y;
	drawPosition = Vec2f(drawX, drawY);

	name = enemy->name;

	behavior = enemy->behavior;
	pathName = enemy->pathName;
	beadType = enemy->beadType;
	beadColor = enemy->beadColor;
	direction = enemy->direction;
	orientation = enemy->orientation;
}