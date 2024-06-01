#pragma once
#include "mesher.hpp"
class Block {
public:
    BlockData _data;
    bool alive = false;
    void checkObscures(Block(&_blocks)[16][256][16]);
    void greed(Block(&_block)[16][256][16]);
    void setup(int x, int y, int z, BLOCK_TYPES type);
};

class Chunk {
public:
    Block _blocks[16][256][16];
    Model _model;
    Vector3 _chunkPos;
    Texture2D _textureMap;
    std::vector<BlockData> _blockData;
    void generateModel();
    void spawnTree(uint8_t tx, uint8_t ty, uint8_t tz);
    void setup(Texture2D _texture, int cx, int cy, int cz, int permutation_table[256]);
    void update();

};