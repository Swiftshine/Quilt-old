#pragma once

#include "mapdata.h"
#include "types.h"

class NodeBase {
public:
    static enum DrawTypes : u32 {
        DrawType_Circle,
        DrawType_Square,
        DrawType_Triangle,
    };

    static enum NodeType : u32 {
        NodeType_Base = 0,
        NodeType_Gimmick,
        NodeType_Enemy,
    };
public:
    NodeBase();
    ~NodeBase();

    bool RectHover();
    bool RectClick();
    void HandleDragging();
    void EnableDragging();
    void DisableDragging();
    void Draw();
    void Update();
public:
    std::string name;
    Vec2f       position;
    Vec2f       drawPosition;
    float       size;
    RGBA        color;
    RGBA        selectionColor;
    NodeType    nodeType;
    DrawTypes   drawType;
    bool        isSelected;
    bool        paramsShown;
private:
    bool        draggable;
};

class GmkNode : public NodeBase {
public:
    GmkNode();
    ~GmkNode();

    void Configure(Mapdata::Mapbin::Gimmick* gmk);
public:
    Mapdata::Mapbin::GimmickParameters params;
};

class EnNode : public NodeBase {
public:
    EnNode();
    ~EnNode();

    void Configure(Mapdata::Enbin::EnemyEntry* entry);

public:
    std::string behavior;
    std::string unk1;
    std::string beadType;
    std::string beadColor;
    std::string direction;
    std::string orientation;
};