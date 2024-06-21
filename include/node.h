#pragma once

#include <memory>
#include "mapdata.h"

#include "quilt.h"
/* Base */
class NodeBase {
public:
    static enum NodeType {
        Base = 0,
        Wall,
        Wall2,
        CommonGimmick,
        Gimmick,
        Zone,
    };
public:
	NodeBase() { }
	~NodeBase() { }

	virtual void Update() = 0;

    bool IsSelected() { return isSelected; }
    void Select() { isSelected = true;  }
    void Deselect() { isSelected = false; }
    NodeType GetType() { return type; }
protected:
    bool isSelected = false;
    bool isDragOffsInited = false;
    Vec2f dragOffs = Vec2f();
    NodeType type = NodeType::Base;
};

/* Common Gimmick */
class CmnGmkNode : public NodeBase {
public:
    CmnGmkNode(Mapdata::Mapbin::CommonGimmick& newCmnGmk) {
        cmnGmk = std::make_shared<Mapdata::Mapbin::CommonGimmick>();
        *cmnGmk = newCmnGmk;
        type = NodeType::CommonGimmick;
    }

    ~CmnGmkNode() { }

    virtual void Update() override;
public:
    std::shared_ptr<Mapdata::Mapbin::CommonGimmick> cmnGmk;
};
/* Gimmick */
class GmkNode : public NodeBase {
public:
	GmkNode(Mapdata::Mapbin::Gimmick& newGmk) {
		gmk = std::make_shared<Mapdata::Mapbin::Gimmick>();
		*gmk = newGmk;
        type = NodeType::Gimmick;
	}

	~GmkNode() { }

    virtual void Update() override;

public:
	std::shared_ptr<Mapdata::Mapbin::Gimmick> gmk;
};

class ZoneNode : public NodeBase {
public:
    ZoneNode(Mapdata::Mapbin::Zone& newZone) {
        type = NodeType::Zone;
        zone = std::make_shared < Mapdata::Mapbin::Zone>();
        *zone = newZone;
    }

    ~ZoneNode() { }

    virtual void Update() override;
public:
    std::shared_ptr<Mapdata::Mapbin::Zone> zone;
};