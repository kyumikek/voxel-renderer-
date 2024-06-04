#pragma once
#include "raylib.h"
#include "chunk.hpp"
#include "rlgl.h"
#include "entities.hpp"
#include <string>
#include <vector>
#include <map>
class World {
private:

    Texture2D _textureGraph;
    std::map<std::string, Chunk>_chunks;
    Player _player;
    int permutation_table[256];
    void renderPlayer(Player* _player);
public:
    void setup();
    void genWorld();
    void update();
};
