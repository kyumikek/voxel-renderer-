#pragma once

#include <vector>
#include "raylib.h"

enum BLOCK_TYPES {
    GRASS,
    WATER,
    STONE,
    SAND,
    DIRT,
};
struct BlockData {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t width;
    uint8_t length;
    bool obstructed;
    bool frontObscured;
    bool backObscured;
    bool leftObscured;
    bool rightObscured;
    bool topObscured;
    bool bottomObscured;
    uint8_t  type;
};

struct VoxelModel {
    Mesh _mesh;
    uint_fast32_t vertCount;
    uint_fast32_t texCount;
};

// Declarations
Vector3 castVector3(int x, int y, int z);
int SetVertex(VoxelModel* _model, uint16_t x, uint16_t y, uint16_t z, float tx1, float tx2);
int addCubeMesh(VoxelModel* _model, uint16_t x, uint16_t y, uint16_t z, float length, float width, float height,float tx1, float tx2, BlockData _block);
int addCubeType(VoxelModel* _model, int x, int y, int z, BLOCK_TYPES block, BlockData _data);
Model GenMeshCustom(std::vector<BlockData> _blockList);