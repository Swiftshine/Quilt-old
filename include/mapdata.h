#pragma once

#include "types.h"

#pragma pack(push, 1)

namespace Colbin {
    struct Header {
        f32 _0;
        u32 entryCount;
        u32 entryOffs;
        u32 footerOffs;
    };

    struct Entry {
        Vec2f point1;
        Vec2f point2;
        Vec2f _10;
        u32 index;
        u32 colbinTypeIndex;
    };

    struct Footer {
        u32 nameCount;
        /* present is a char[nameCount][0x20] */
    };
}


namespace Mapdata {
namespace Mapbin {
	struct Header {
		f32 _0;
		Vec2f boundsStart;
		Vec2f boundsEnd;
		u32 wallCount;
		u32 wallOffs;
		u32 wall2Count;
		u32 wall2Offs;
		u32 commonGmkCount;
		u32 commonGmkOffs;
		u32 gmkCount;
		u32 gmkOffs;
		u32 pathCount;
		u32 pathOffs;
		u32 zoneCount;
		u32 zoneOffs;
		u32 courseInfoCount;
		u32 courseInfoOffs;
		u32 commonGmkNamesOffs;
		u32 colbinTypeOffs;
		u32 wall2LabelOffs;
	};

	static_assert(sizeof(Mapdata::Mapbin::Header) == 0x58, "header is wrong size");

    // it's really more like "Data Seg WITH Label"
    // but i'm using official names wherever possible
    struct DataSegLabel : public Colbin::Entry {
        u32 labelIndex;
    };

    static_assert(sizeof(Mapdata::Mapbin::DataSegLabel) == 0x24, "data seg label is wrong size");

    struct CommonParam {
        int ints[3];
        float floats[3];
        char strings[3][64];
    };


    // size - 0x124
    struct Gimmick {
        char name[0x30];
        u8 _30[0x10];
        Vec3f position;
        CommonParam params;
    };
    static_assert(sizeof(Mapdata::Mapbin::Gimmick) == 0x124, "gimmick is wrong size");

    struct CommonGimmickParameters {
        s32 ints1[3];
        float floats1[3];
        s32 ints2[4];
        float floats2[3];
        float floats3[3];
        char string1[0x20];
        u8 rest[0x120];
    };

    struct CommonGimmick {
        u32 nameIndex;
        Vec3f position;
        CommonGimmickParameters params;
    };

    static_assert(sizeof(Mapdata::Mapbin::CommonGimmick) == 400, "common gimmick is wrong size");

    // variable length
    struct Path {
        char name[0x20];
        u8 _20[0xD8];
        char type[0x20];
        u32 numPoints;
        /* present is a Vec2f[numPoints] */
    };

    struct Zone {
        char name[0x20];
        char _20[0x20];
        CommonParam params;
        Vec2f boundsStart;
        Vec2f boundsEnd;
    };

    static_assert(sizeof(Mapdata::Mapbin::Zone) == 296, "zone size is incorrect");


    struct CourseInfo {
        char name[0x20];
        char _20[0x20];
        CommonParam params;
        Vec3f position;
    };

    static_assert(sizeof(Mapdata::Mapbin::CourseInfo) == 292, "course info size is incorrect");
}

namespace Enbin {
    struct Header {
        char magic[4]; // GFES
        u32 _4;
        u32 _8;
        u32 _C;
        u32 footerOffs;
        u32 numEntries;
    };

    struct EnemyEntry {
        char name[0x20];
        char behavior[0x20];
        char pathName[0x20];
        char beadType[0x10];
        char beadColor[0x10];
        char direction[0x10];
        char orientation[0x10];
        Vec3f position1;
        Vec3f position2;
        Vec3f position3;
        Vec3f _C4;
        u32 _D0;
        u32 _D4;
        s32 _D8;
        Vec3f _DC;
        u32 _E8;
        u32 _EC;
        u32 _F0;
        u32 _F4;
        u32 _F8;
        u32 _FC;
        u32 _100;
        u32 _104;
        s32 _108;
        u32 _10C;
        u32 _110;
        u32 _114;
        u32 _118;
        u32 _11C;
        u32 _120;
        u32 _124;
        u32 _128;
        u32 _12C;
        u32 _130;
        u32 _134;
        u32 _138;
        u32 _13C;
        u32 _140;
        u32 _144;
        u32 _148;
        u32 _14C;
        u32 _150;
        u32 _154;
        u32 _158;
        u32 _15C;
        u32 _160;
        u32 _164;
        u32 _168;
        u32 _16C;
        u32 _170;
    };

    struct Footer {
        u32 numFooterEntries;
    };

    struct FooterEntry {
        u32 _0;
        u32 _4;
        Vec2f _8;
        Vec2f _10;
    };
}
}


#pragma pack(pop)