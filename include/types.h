#pragma once

#include <cstdint>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

using s64 = int64_t;
using s32 = int32_t;
using s16 = int16_t;
using s8 =  int8_t;
struct Vec2f {
    Vec2f() { x = 0; y = 0; }
    Vec2f(float nX, float nY) { x = nX; y = nY; }
    float x;
    float y;
};


struct RGBA {
    RGBA() { r = 255; g = 255; b = 255; a = 255; }
    RGBA(u8 newR, u8 newG, u8 newB, u8 newA) {
        r = newR; g = newG; b = newB; a = newA;
    }
    RGBA(unsigned int val) {
        r = (val >> 24) & 0xFF;
        g = (val >> 16) & 0xFF;
        b = (val >> 8) & 0xFF;
        a = val & 0xFF;
    }
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

/* colors */

const RGBA PATH_COLOR = RGBA(0x80ed99FF);
const RGBA GIMMICK_COLOR_SELECT = RGBA(0x7F96FFFF);
const RGBA GIMMICK_COLOR = RGBA(0x7F96FF7F);
const RGBA WALL_COLOR = RGBA(0xFFFFFFFF);
const RGBA ENEMY_COLOR = RGBA(0xFFFF557F);
const RGBA ENEMY_COLOR_SELECT = RGBA(0xFFFF55FF);

struct Vec3f { float x; float y; float z; };