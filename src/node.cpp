#include "quilt.h"


NodeBase::NodeBase() {
	name = "";
	position = Vec2f();
	drawPosition = Vec2f();
	size = 1.0f;
	color = RGBA(0xFFFFFFFF);
	selectionColor = RGBA(color.r, color.g, color.b, color.a / 2);
	drawType = DrawType_Square;
	isSelected = false;
	nodeType = NodeType_Base;
}

NodeBase::~NodeBase() {

}


bool NodeBase::RectHover() {
	return Editor::RectHover(drawPosition, size, size);
}

bool NodeBase::RectClick() {
	return Editor::RectClick(drawPosition, size, size);
}


void NodeBase::Draw() {
	if (RectHover() && !name.empty()) {
		Editor::DrawTooltip(name.c_str());
	}
	float drawX = position.x * camera.zoom + camera.x;
	float drawY = camera.h - (position.y * camera.zoom) + camera.y;
	drawY -= size;
	drawPosition = Vec2f(drawX, drawY);

	RGBA drawCol = color;
	if (RectHover() || isSelected) drawCol = selectionColor;


	switch (drawType) {
		case DrawType_Circle:
			break;
		

		case DrawType_Square: {
			Editor::DrawRect(drawPosition, size, size, drawCol);
			break;
		}

		case DrawType_Triangle: {

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

	position = Vec2f(SwapF32(gimmick->position.x), SwapF32(gimmick->position.y));

	float drawX = SwapF32(gimmick->position.x) * camera.zoom + camera.x;
	float drawY = camera.h - (SwapF32(gimmick->position.y) * camera.zoom) + camera.y;

	size = SQUARE_SIZE;
	drawY -= size;
	drawPosition = Vec2f(drawX, drawY);
	name = gimmick->name;
	params = gimmick->params;
}

EnNode::EnNode() {
	behavior = "";
	unk1 = "";
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

	float drawX = SwapF32(enemy->position1.x) * camera.zoom + camera.x;
	float drawY = camera.h - (SwapF32(enemy->position1.y) * camera.zoom) + camera.y;

	size = SQUARE_SIZE;
	drawY -= size;
	drawPosition = Vec2f(drawX, drawY);

	name = enemy->name;

	behavior = enemy->behavior;
	unk1 = enemy->_40;
	beadType = enemy->beadType;
	beadColor = enemy->beadColor;
	direction = enemy->direction;
	orientation = enemy->orientation;
}