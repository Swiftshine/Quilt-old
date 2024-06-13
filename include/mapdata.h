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
            u32 structs1Count; // undocumented
            u32 structs1Offset;
            u32 gimmickCount;
            u32 gimmickOffset;
            u32 pathCount;
            u32 pathOffset;
            u32 structs4Count;
            u32 structs4Offset;
            u32 controllerCount;
            u32 controllerOffset;
            u32 paramsOffset;
            u32 colbinFooterOffset;
            u32 colbinFooterEnd;
        };

        static_assert(sizeof(Mapdata::Mapbin::Header) == 0x58, "header is wrong size");

        struct GimmickParameters {
            int ints[3];
            float floats[3];
        };

        // size - 0x124
        struct Gimmick {
            char name[0x30];
            u8 _30[0x10];
            Vec3f position;
            GimmickParameters params;
            u8 _60[0xD0 - 0x10];
        };
        static_assert(sizeof(Mapdata::Mapbin::Gimmick) == 0x124, "gimmick is wrong size");


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
            int intVal1;
            int intVal2;
            int intVal3;
            float floatVal1;
            float floatVal2;
            float floatVal3;
            u8 _58[192];
            Vec2f _118;
            Vec2f _120;
        };
    }
}
#pragma pack(pop)