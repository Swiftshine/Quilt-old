#pragma once

const float CAMERA_SPEED = 0.7f;
struct Camera {
    Camera() : x(0), y(0), w(0), h(0), zoom(1.0f), speed(CAMERA_SPEED) {}

    float x, y, w, h;
    float zoom;
    float speed;
};

extern Camera camera;
