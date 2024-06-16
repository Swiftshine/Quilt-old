#pragma once

#include "types.h"


namespace Colbin {
    struct Header {
        float multiplier; // i think? i know it's a float though
        u32 numEntries;
        u32 entryOffset;
        u32 footerOffset;
    };

    struct Entry {
        Vec2f point1;
        Vec2f point2;
        Vec2f _10; //
        u32 index;
        u32 collisionTypeIndex;
    };

    struct Footer {
        u32 numCollisionTypes;
        /* present is a Colbin::Descriptor[numCollisionTypes] */

        /* padding until next multiple of 0x20 */
    };
};