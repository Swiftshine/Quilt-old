#pragma once

#include "types.h"

class Camera {
public:
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    static Camera* Instance() {
        static Camera inst;
        return &inst;
    }

public:
    f32 w = 0.0f;
    f32 h = 0.0f;
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 zoom = 1.0f;
    f32 speed = 3.0f;
private:
    Camera() { }
    ~Camera() { }
};