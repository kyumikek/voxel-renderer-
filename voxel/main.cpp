#include <stdio.h>
#include <vector>
#include "raylib.h"
#include "chunk.hpp"
#include <iostream>
#include <map>
#include <string>
#include "rlgl.h"
#include "noise.hpp"
#define playerTextureWidth 1200
#define playerTextureHeight 800
class Player {
public:
    RenderTexture _renderTexture;
    Vector3 _pos = { 5.0f,5.0f,5.0f };
    Camera _cam = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 90.0f, CAMERA_PERSPECTIVE };
    int setup(int x, int y, int z) {
        this->_cam.position.x = x;
        this->_cam.position.y = y;
        this->_cam.position.z = z;
        this->_renderTexture = LoadRenderTexture(playerTextureWidth, playerTextureHeight);
        return 0;
    }
    int update() {

        UpdateCamera(&_cam, CAMERA_FIRST_PERSON);
        if (IsKeyDown(KEY_SPACE)) {
            this->_cam.target.y += 1;
            this->_cam.position.y += 1;
        }
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            this->_cam.target.y -= 1;
            this->_cam.position.y -= 1;
            std::cout << "a";
        }
        Vector3 _cube = { _cam.position.x,-4,_cam.position.z };
        DrawCube(_cube, 8, 8, 8, RED);
        return 0;
    }
};
class Game {
private:
    Texture2D _textureGraph;
    std::map<std::string, Chunk>_chunks;
    Player _player;
    int permutation_table[256];
    int renderPlayer(Player* _player) {
        //BeginTextureMode(_player->_renderTexture);
        ClearBackground(SKYBLUE);
        _player->update();
        rlEnableWireMode();
        BeginMode3D(_player->_cam);
        
        //int cx = _player->_cam.position.x / (8*7);
        //int cz = _player->_cam.position.z / (8*7);
        Vector2 pos;
        pos.y = (int)(_player->_cam.position.z / 16);
        pos.x = (int)(_player->_cam.position.x / 16);
        int cx = pos.x;
        int cz = pos.y;
        for (int x = cx-8; x < 8 + cx; x += 1) {
            for (int z = cz-8; z < 8 + cz; z += 1) {
                this->_chunks[std::to_string(x) + " " + std::to_string(z)].update();
            }
        }
        rlDisableWireMode();

        EndMode3D();
        DrawRectangle(pos.x*8, pos.y*8, 8, 8, RED);
        DrawFPS(0, 0);
        //EndTextureMode();
        return 0;
    }
public:
    Game() {
        InitWindow(1200, 800, "voxel engine");
        Image img = GenImageColor(5, 1, WHITE);
        ImageDrawPixel(&img, 0, 0, GREEN);
        ImageDrawPixel(&img, 1, 0, BROWN);
        ImageDrawPixel(&img, 2, 0, YELLOW);
        ImageDrawPixel(&img, 3, 0, BLUE);
        ImageDrawPixel(&img, 4, 0, GRAY);
        _player.setup(5, 5, 5);
        this->_textureGraph = LoadTextureFromImage(img);
        UnloadImage(img);
        for (int i = 0; i < 256; i += 1) {
            this->permutation_table[i] = GetRandomValue(0, 255);
        }
        for (int x = 0; x < 64; x += 1) {
            for (int z = 0; z < 64; z += 1) {
                this->_chunks[std::to_string(x) + " " + std::to_string(z)].setup(this->_textureGraph, x * 16 , 0, z * 16, this->permutation_table);
            }
        }
        
    }
    int update() {
        rlDisableBackfaceCulling();

        DisableCursor();
        //SetTargetFPS(60);

        Rectangle splitScreenRect = { 0.0f, 0.0f, 1200, -800 };
        Vector2 pos = { 0,0 };
        while (!WindowShouldClose())
        {
            this->renderPlayer(&this->_player);
            BeginDrawing();

            DrawTextureRec(this->_player._renderTexture.texture, splitScreenRect, pos, WHITE);
            
            EndDrawing();
        }
        return 0;
    }
};
int main(void)
{
    Game _game;
    _game.update();
    return 0;
}
