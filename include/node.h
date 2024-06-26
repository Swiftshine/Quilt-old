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
        LabeledWall,
        CommonGimmick,
        Gimmick,
        Path,
        Zone,
        Enemy,
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

/* Walls */

// wall nodes are...a little different in that
// they aren't one, point, but they're two.
class WallNode : public NodeBase {
public:
    WallNode(Colbin::Entry& newWall) {
        wall = std::make_shared<Colbin::Entry>();
        *wall = newWall;
        type = NodeType::Wall;
    }

    ~WallNode() { }

    virtual void Update() override;
public:
    std::shared_ptr<Colbin::Entry> wall;
private:
    bool isDragOffsInited1 = false;
    bool isDragOffsInited2 = false;
    Vec2f dragOffs1 = Vec2f();
    Vec2f dragOffs2 = Vec2f();
};

/* Labeled Wall */

class LabeledWallNode : public NodeBase {
public:
    LabeledWallNode(Mapdata::Mapbin::LabeledWall& newWall) {
        wall = std::make_shared<Mapdata::Mapbin::LabeledWall>();
        *wall = newWall;
        type = NodeType::LabeledWall;
    }

    ~LabeledWallNode() { }

    virtual void Update() override;
public:
    std::shared_ptr<Mapdata::Mapbin::LabeledWall> wall;
private:
    bool isDragOffsInited1 = false;
    bool isDragOffsInited2 = false;
    Vec2f dragOffs1 = Vec2f();
    Vec2f dragOffs2 = Vec2f();
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

/* Path */
class PathNode : public NodeBase {
public:
    PathNode(Mapdata::Mapbin::Path& newPath, std::vector<Vec2f>& newPoints) {
        path = std::make_shared<Mapdata::Mapbin::Path>();
        *path = newPath;
        points = newPoints;
        type = NodeType::Path;

        dragOffsInited.resize(points.size(), false);
        dragOffsets.resize(points.size(), Vec2f());
    }

    ~PathNode() { }

    virtual void Update() override;

    void AddPoint() {
        
    }

    void RemovePoint() {

    }

public:
    std::shared_ptr<Mapdata::Mapbin::Path> path;
private:
    std::vector<Vec2f> points;
    std::vector<bool> dragOffsInited;
    std::vector<Vec2f> dragOffsets;
};

/* Zone */
class ZoneNode : public NodeBase {
public:
    ZoneNode(Mapdata::Mapbin::Zone& newZone) {
        type = NodeType::Zone;
        zone = std::make_shared<Mapdata::Mapbin::Zone>();
        *zone = newZone;
    }

    ~ZoneNode() { }

    virtual void Update() override;
public:
    std::shared_ptr<Mapdata::Mapbin::Zone> zone;
};

/* Enemy */
class EnemyNode : public NodeBase {
public:
    EnemyNode(Mapdata::Enbin::EnemyEntry& newEnemy) {
        enemy = std::make_shared<Mapdata::Enbin::EnemyEntry>();
        type = NodeType::Enemy;
        *enemy = newEnemy;
    }

    ~EnemyNode() { }

    virtual void Update() override;
public:
    std::shared_ptr<Mapdata::Enbin::EnemyEntry> enemy;
};