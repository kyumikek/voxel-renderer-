#include "manager.hpp"
void World::renderPlayer(Player* _player) {
    BeginTextureMode(_player->_renderTexture);
    ClearBackground(SKYBLUE);
    _player->update();
    BeginMode3D(_player->_cam);

    //int cx = _player->_cam.position.x / (8*7);
    //int cz = _player->_cam.position.z / (8*7);
    Vector2 pos;
    pos.y = (int)(_player->_cam.position.z / 16);
    pos.x = (int)(_player->_cam.position.x / 16);

    int cx = pos.x;
    int cz = pos.y;

    for (int x = cx - 8; x < 8 + cx; x += 1) {
        for (int z = cz - 8; z < 8 + cz; z += 1) {
            if (x > -1 && x<64 && z>-1 && z < 64) {
                if (this->_chunks[std::to_string(x) + " " + std::to_string(z)]._needToGenerate) {
                    this->_chunks[std::to_string(x) + " " + std::to_string(z)].setup(this->_textureGraph, x * 16, 0, z * 16, this->permutation_table);
                }
                this->_chunks[std::to_string(x) + " " + std::to_string(z)].update();

            }
        }
    }
    EndMode3D();
    //DrawRectangle(pos.x * 8, pos.y * 8, 8, 8, RED);
    DrawFPS(0, 0);
    EndTextureMode();
}
void World::setup() {
    Image img = GenImageColor(5, 1, WHITE);
    ImageDrawPixel(&img, 0, 0, GREEN);
    ImageDrawPixel(&img, 1, 0, BROWN);
    ImageDrawPixel(&img, 2, 0, YELLOW);
    ImageDrawPixel(&img, 3, 0, BLUE);
    ImageDrawPixel(&img, 4, 0, GRAY);
    this->_player.setup(5, 5, 5);
    rlEnableBackfaceCulling();
    //rlEnableWireMode();
    this->_textureGraph = LoadTextureFromImage(img);
    UnloadImage(img);
    for (int i = 0; i < 256; i += 1) {
        this->permutation_table[i] = GetRandomValue(0, 255);
    }
}
void World::genWorld() {
    for (int x = 0; x < 64; x += 1) {
        for (int z = 0; z < 64; z += 1) {
            this->_chunks[std::to_string(x) + " " + std::to_string(z)].setup(this->_textureGraph, x * 16, 0, z * 16, this->permutation_table);
        }
    }
}
void World::update() {

    DisableCursor();
    rlDisableBackfaceCulling();
    SetTargetFPS(60);
    Rectangle splitScreenRect = { 0.0f, 0.0f, 1200, -800 };
    Vector2 pos = { 0,0 };
    while (!WindowShouldClose())
    {
        this->renderPlayer(&this->_player);
        BeginDrawing();

        DrawTextureRec(this->_player._renderTexture.texture, splitScreenRect, pos, WHITE);

        EndDrawing();
    }
}
