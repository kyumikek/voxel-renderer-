#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#define playerTextureWidth 1200
#define playerTextureHeight 800

class Player {
public:
    RenderTexture _renderTexture;
    Vector3 _direction;
    Camera _cam = { { 800, 100.0f, 800 }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 90.0f, CAMERA_PERSPECTIVE };
    void setup(int x, int y, int z);
    void update();
};
