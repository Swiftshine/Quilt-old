#pragma once

#include <cstdint>

#include "colbin.h"
#include "enbin.h"

#include "types.h"

#pragma pack(push, 1)
namespace Mapdata {
    namespace Mapbin {
        struct Header {
            float _0;
            Vec2f boundsStart;
            Vec2f boundsEnd;
            u32 wallCount; // colbin
            u32 wallOffset;
            u32 dataSegLabelCount; // no struct yet
            u32 dataSegLabelOffset;
            u32 commonGimmickCount;
            u32 commonGimmickOffset;
            u32 gimmickCount;
            u32 gimmickOffset;
            u32 pathCount;
            u32 pathOffset;
            u32 controllerCount;
            u32 controllerOffset;
            u32 structs5Count;
            u32 structs5Offset;
            u32 commonGimmickHeaderOffset;
            u32 colbinFooterOffset;
            u32 colbinFooterEnd;
        };

        static_assert(sizeof(Mapdata::Mapbin::Header) == 0x58, "header is wrong size");

        // it's really more like "Data Seg WITH Label"
        // but i'm using official names wherever possible
        struct DataSegLabel {
            Vec2f point1;
            Vec2f point2;
            Vec2f _10;
            u32 index;
            u32 collisionTypeIndex;
            u32 _20;
        };

        static_assert(sizeof(Mapdata::Mapbin::DataSegLabel) == 0x24, "data seg label is wrong size");

        struct GimmickParameters {
            int ints[3];
            float floats[3];
            char strings[3][64];
        };


        // size - 0x124
        struct Gimmick {
            char name[0x30];
            u8 _30[0x10];
            Vec3f position;
            GimmickParameters params;
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

        struct Controller {
            char name[0x20];
            char _20[0x20];
            GimmickParameters params;
            Vec2f boundsStart;
            Vec2f boundsEnd;
        };

        static_assert(sizeof(Mapdata::Mapbin::Controller) == 296, "controller size is incorrect");
    }
}
#pragma pack(pop)