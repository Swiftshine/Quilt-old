#pragma once
#include <cstdint>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8  = uint8_t;

using s64 = int64_t;
using s32 = int32_t;
using s16 = int16_t;
using s8  = int8_t;

using f32 = float;

using string32 = char[0x20];

static u16 Swap16(u16 value) {
    return (value << 8) | (value >> 8);
}

static u32 Swap32(u32 value) {
    return ((value >> 24) & 0x000000FF) |
        ((value >> 8) & 0x0000FF00) |
        ((value << 8) & 0x00FF0000) |
        ((value << 24) & 0xFF000000);
}

static u64 Swap64(u64 value) {
    return ((value << 56) & 0xFF00000000000000ULL) |
        ((value << 40) & 0x00FF000000000000ULL) |
        ((value << 24) & 0x0000FF0000000000ULL) |
        ((value << 8) & 0x000000FF00000000ULL) |
        ((value >> 8) & 0x00000000FF000000ULL) |
        ((value >> 24) & 0x0000000000FF0000ULL) |
        ((value >> 40) & 0x000000000000FF00ULL) |
        ((value >> 56) & 0x00000000000000FFULL);
}

static f32 SwapF32(float value) {
    u32 intval;
    std::memcpy(&intval, &value, sizeof(float));
    intval = Swap32(intval);
    float result;
    std::memcpy(&result, &intval, sizeof(float));
    return result;
}

struct Vec3f {
    Vec3f() { x = 0; y = 0; z = 0; }
    Vec3f(float nX, float nY, float nZ) { x = nX; y = nY; z = nZ; }

    void Swap() {
        x = SwapF32(x);
        y = SwapF32(y);
        z = SwapF32(z);
    }

    Vec3f GetSwap() {
        return Vec3f(SwapF32(x), SwapF32(y), SwapF32(z));
    }

    float x;
    float y;
    float z;
};

struct Vec2f {
    Vec2f() { x = 0; y = 0; }
    Vec2f(float nX, float nY) { x = nX; y = nY; }
    Vec2f(Vec3f src) { x = src.x; y = src.y; }

    Vec2f operator+(Vec2f& other) {
        return Vec2f(x + other.x, y + other.y);
    }

    Vec2f operator-(Vec2f& other) {
        return Vec2f(x - other.x, y - other.y);
    }
    void Swap() {
        x = SwapF32(x);
        y = SwapF32(y);
    }

    Vec2f GetSwap() {
        return Vec2f(SwapF32(x), SwapF32(y));
    }

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
