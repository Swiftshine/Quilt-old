#pragma once

#include "mapdata.h"
#include "types.h"

class NodeBase {
public:
    static enum DrawTypes : u32 {
        DrawType_Square,
        DrawType_Line,
        DrawType_Circle,
        DrawType_Triangle,
    };

    static enum NodeType : u32 {
        NodeType_Base = 0,
        NodeType_CommonGimmick,
        NodeType_Gimmick,
        NodeType_Controller,
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
    Vec2f       size;
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

class CmnGmkNode : public NodeBase {
public:
    CmnGmkNode();
    ~CmnGmkNode();

    void Configure(Mapdata::Mapbin::CommonGimmick* cmnGmk);
public:
    int nameIndex;
    std::string qid;
    Mapdata::Mapbin::CommonGimmickParameters params;
};

class ContNode : public NodeBase {
public:
    ContNode();
    ~ContNode();

    void Configure(Mapdata::Mapbin::Controller* controller);
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
    std::string pathName;
    std::string beadType;
    std::string beadColor;
    std::string direction;
    std::string orientation;
};