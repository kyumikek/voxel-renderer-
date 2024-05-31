#include <stdio.h>
#include <vector>
#include "raylib.h"
#include "mesher.h"
#include <iostream>
#include <map>
#include <string>
class Block {
public:
    BlockData _data;
    bool alive = false;
    void checkObscures(Block _blocks[16][256][16], int x, int y, int z) {

        
        _data.frontObscured = (z-1 > -1 && _blocks[x][y][z - 1].alive); // Front
        _data.backObscured = (z+1 < 16 && _blocks[x][y][z + 1].alive); // Back
        _data.leftObscured = (x-1 > -1 && _blocks[x - 1][y][z].alive); // Left
        _data.rightObscured = (x+1 < 16 && _blocks[x + 1][y][z].alive); // Right
        _data.bottomObscured = (y+1 < 256 && _blocks[x][y + 1][z].alive); // Top
        _data.topObscured = (y-1 > -1 && _blocks[x][y - 1][z].alive); // Bottom
    }

    int setup(int x, int y, int z, BLOCK_TYPES type) {
        this->_data.x = x;
        this->_data.y = y;
        this->_data.z = z;
        this->_data.type = type;
        this->alive = true;
        return 0;
    }
};

class Chunk {
public:
    Block _blocks[16][256][16];
    Model _model;
    Vector3 _chunkPos;
    Texture2D _textureMap;
    std::vector<BlockData> _blockData;
    int generateModel() {

        for (int x = 0; x < 16; x += 1) {
            for (int y = 0; y < 256; y += 1) {
                for (int z = 0; z < 16; z += 1) {
                    _blocks[x][y][z].checkObscures(_blocks,x,y,z);
                }
            }
        }
        _blockData.clear();
        for (int x = 0; x < 16; x += 1) {
            for (int y = 0; y < 256; y += 1) {
                for (int z = 0; z < 16; z += 1) {
                    if (_blocks[x][y][z].alive) {
                        _blockData.push_back(_blocks[x][y][z]._data);
                    }
                }
            }
        }

        this->_model = GenMeshCustom(_blockData);
        this->_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = this->_textureMap;
        return 0;
    }
    int setup(Texture2D _texture, int cx, int cy, int cz) {
        this->_textureMap = _texture;
        this->_chunkPos.x = cx;
        this->_chunkPos.y = cy;
        this->_chunkPos.z = cz;
        for (int x = 0; x < 16; x += 1) {
            for (int z = 0; z < 16; z += 1) {
                for (int y = 0; y < 128; y += 1) {
                    if (y == 0) this->_blocks[x][y][z].setup(x + this->_chunkPos.x, y + this->_chunkPos.y, z + this->_chunkPos.z, GRASS);
                    else if (y < 10) this->_blocks[x][y][z].setup(x + this->_chunkPos.x, y + this->_chunkPos.y, z + this->_chunkPos.z, DIRT);
                    else this->_blocks[x][y][z].setup(x + this->_chunkPos.x, y + this->_chunkPos.y, z + this->_chunkPos.z, STONE);
                }
                
            }
        }
        this->generateModel();
        return 0;
    }
    int update() {
        DrawModel(this->_model, this->_chunkPos, 1, WHITE);
        
        return 0;
    }
    
};
class Game {
public:
    Texture2D _textureGraph;
    Camera _cam;
    std::map<std::string, Chunk>_chunks;
    Game() {
        InitWindow(1200, 800, "voxel engine");
        Image img = GenImageColor(5, 1, WHITE);
        ImageDrawPixel(&img, 0, 0, GREEN);
        ImageDrawPixel(&img, 1, 0, BROWN);
        ImageDrawPixel(&img, 2, 0, YELLOW);
        ImageDrawPixel(&img, 3, 0, BLUE);
        ImageDrawPixel(&img, 4, 0, GRAY);

        this->_textureGraph = LoadTextureFromImage(img);
        UnloadImage(img);
        this->_cam = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
        for (int x = 0; x < 8; x += 1) {
            for (int z = 0; z < 8; z += 1) {
                this->_chunks[std::to_string(x)+" "+std::to_string(z)].setup(this->_textureGraph, x * 11, 0, z * 11);
            }
        }
        
    }
    int update() {
        rlDisableBackfaceCulling();
        DisableCursor();
        //SetTargetFPS(60);
        while (!WindowShouldClose())
        {
            UpdateCamera(&this->_cam, CAMERA_FREE);

            BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(this->_cam);
            for (int x = 0; x < 8; x += 1) {
                for (int z = 0; z < 8; z += 1) {
                    this->_chunks[std::to_string(x) + " " + std::to_string(z)].update();
                }
            }

            EndMode3D();

            DrawFPS(0, 0);
            EndDrawing();
        }
        return 0;
    }
};
Game _game;
int main(void)
{
    
    //GenImageColor(16, 16, WHITE);
    //Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    _game.update();
    return 0;
}
