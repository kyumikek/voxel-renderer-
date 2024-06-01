#include "chunk.hpp"
#include <iostream>
#include "noise.hpp"
void Block::checkObscures(Block(&_blocks)[16][256][16]) {
    _data.frontObscured = (_data.z - 1 > -1 && _blocks[_data.x][_data.y][_data.z - 1].alive); // Front
    _data.backObscured = (_data.z + 1 < 16 && _blocks[_data.x][_data.y][_data.z + 1].alive); // Back
    _data.leftObscured = (_data.x - 1 > -1 && _blocks[_data.x - 1][_data.y][_data.z].alive); // Left
    _data.rightObscured = (_data.x + 1 < 16 && _blocks[_data.x + 1][_data.y][_data.z].alive); // Right
    _data.bottomObscured = (_data.y + 1 < 256 && _blocks[_data.x][_data.y + 1][_data.z].alive); // Top
    _data.topObscured = (_data.y - 1 > -1 && _blocks[_data.x][_data.y - 1][_data.z].alive); // Bottom
}
void Block::setup(int x, int y, int z, BLOCK_TYPES type) {
    this->_data.x = x;
    this->_data.y = y;
    this->_data.z = z;
    this->_data.length = 1;
    this->_data.width = 1;
    this->_data.type = type;
    this->alive = true;
}
void Block::greed(Block(&_blocks)[16][256][16]) {
    int width = 1, length = 1;
    bool extendWidth = true, extendLength = true;

    // Extend width
    while (extendWidth && _data.x + width < 16) {
        for (int z = 0; z < length; ++z) {
            if (!_blocks[_data.x + width][_data.y][_data.z + z].alive ||
                _blocks[_data.x + width][_data.y][_data.z + z]._data.type != _data.type) {
                extendWidth = false;
                break;
            }
        }
        if (extendWidth) {
            for (int z = 0; z < length; ++z) {
                _blocks[_data.x + width][_data.y][_data.z + z].alive = false;
            }
            ++width;
        }
    }
    // Extend length
    while (extendLength && _data.z + length < 16) {
        for (int x = 0; x < width; ++x) {
            if (!_blocks[_data.x + x][_data.y][_data.z + length].alive ||
                _blocks[_data.x + x][_data.y][_data.z + length]._data.type != _data.type) {
                extendLength = false;
                break;
            }
        }
        if (extendLength) {
            for (int x = 0; x < width; ++x) {
                _blocks[_data.x + x][_data.y][_data.z + length].alive = false;
            }
            ++length;
        }
    }

    _data.width = width;
    _data.length = length;
    
    
}
void Chunk::generateModel() {


    for (int x = 0; x < 16; x += 1) {
        for (int y = 0; y < 256; y += 1) {
            for (int z = 0; z < 16; z += 1) {
                _blocks[x][y][z].checkObscures(_blocks);
                if (_blocks[x][y][z]._data.backObscured == false ||
                    _blocks[x][y][z]._data.leftObscured == false ||
                    _blocks[x][y][z]._data.frontObscured == false ||
                    _blocks[x][y][z]._data.rightObscured == false ||
                    _blocks[x][y][z]._data.topObscured == false ||
                    _blocks[x][y][z]._data.bottomObscured == false
                ) _blocks[x][y][z].greed(_blocks);
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
}
void Chunk::spawnTree(uint8_t tx, uint8_t ty, uint8_t tz) {
    for (uint8_t i = 0; i < 3; i += 1) {
        this->_blocks[tx][ty+i][tz].setup(tx, ty+i, tz, DIRT);
    }
    for (int x = -2; x < 3; x += 1) {
        for (int y = 0; y < 3; y += 1) {
            for (int z = -2; z < 3; z += 1) {
                if (tx+x>-1 && tx+x < 16 && tz+z>-1 && tz+z < 16) this->_blocks[tx + x][ty + 3 + y][tz + z].setup(tx + x, ty + 3 + y, tz + z, GRASS);
            }
        }
    }
}
void Chunk::setup(Texture2D _texture, int cx, int cy, int cz, int permutation_table[256]) {
    this->_textureMap = _texture;
    this->_chunkPos.x = cx;
    this->_chunkPos.y = cy;
    this->_chunkPos.z = cz;
    for (uint8_t x = 0; x < 16; x += 1) {
        for (uint8_t z = 0; z < 16; z += 1) {
            uint8_t maxY = GetRandomValue(100, 101);
            for (uint8_t y = 0; y < maxY; y++) {
                if (y == 99) this->_blocks[x][y][z].setup(x, y, z, GRASS);
                
                else if (y > 90) this->_blocks[x][y][z].setup(x, y, z, DIRT);
                else this->_blocks[x][y][z].setup(x, y, z, STONE);
            }
            if (GetRandomValue(0, 100) == 5) {
                this->spawnTree(x, maxY, z);
            }
        }
    }
    this->generateModel();
}
void Chunk::update() {
    DrawModel(this->_model, this->_chunkPos, 1, WHITE);

}