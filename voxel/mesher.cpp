#include "mesher.hpp"
#include "raylib.h"
#include "rlgl.h"
#include <iostream>
#include <vector>
#define blockSize 1
Vector3 castVector3(int x, int y, int z) {
    Vector3 newVec = { x,y,z };
    return newVec;
}
int SetVertex(VoxelModel *_model, uint16_t x, uint16_t y, uint16_t z, float tx1, float tx2) {
    //printf("\n %d", id);
    
    _model->_mesh.vertices[_model->vertCount] = x;
    _model->vertCount++;
    _model->_mesh.vertices[_model->vertCount] = y;
    _model->vertCount++;
    _model->_mesh.vertices[_model->vertCount] = z;
    _model->vertCount++;
    _model->_mesh.texcoords[_model->texCount] = tx1;
    _model->texCount++;
    _model->_mesh.texcoords[_model->texCount] = tx2;
    _model->texCount++;
    return 0;
}

int addCubeMesh(VoxelModel* _model, uint16_t x, uint16_t y, uint16_t z,
    float length, float width, float height,
    float tx1, float tx2, BlockData _block) {

    // Front face
    if (!_block.frontObscured) {
        SetVertex(_model, x, y, z, tx1, tx2);           // Bottom-left
        SetVertex(_model, x + length, y, z, tx1, tx2);  // Bottom-right
        SetVertex(_model, x + length, y + height, z, tx1, tx2); // Top-right
        SetVertex(_model, x, y, z, tx1, tx2);           // Bottom-left
        SetVertex(_model, x + length, y + height, z, tx1, tx2); // Top-right
        SetVertex(_model, x, y + height, z, tx1, tx2);  // Top-left
    }

    // Back face
    if (!_block.backObscured) {
        SetVertex(_model, x, y, z + width, tx1, tx2);            // Bottom-left
        SetVertex(_model, x + length, y, z + width, tx1, tx2);   // Bottom-right
        SetVertex(_model, x + length, y + height, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x, y, z + width, tx1, tx2);            // Bottom-left
        SetVertex(_model, x + length, y + height, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x, y + height, z + width, tx1, tx2);   // Top-left
    }

    // Left face
    if (!_block.leftObscured) {
        SetVertex(_model, x, y, z, tx1, tx2);            // Bottom-left
        SetVertex(_model, x, y + height, z, tx1, tx2);   // Top-left
        SetVertex(_model, x, y + height, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x, y, z, tx1, tx2);            // Bottom-left
        SetVertex(_model, x, y + height, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x, y, z + width, tx1, tx2);    // Bottom-right
    }

    // Right face
    if (!_block.rightObscured) {
        SetVertex(_model, x + length, y, z, tx1, tx2);            // Bottom-left
        SetVertex(_model, x + length, y + height, z, tx1, tx2);   // Top-left
        SetVertex(_model, x + length, y + height, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x + length, y, z, tx1, tx2);            // Bottom-left
        SetVertex(_model, x + length, y + height, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x + length, y, z + width, tx1, tx2);    // Bottom-right
    }

    // Top face
    if (!_block.topObscured) {
        SetVertex(_model, x, y + height, z, tx1, tx2);            // Bottom-left
        SetVertex(_model, x + length, y + height, z, tx1, tx2);   // Bottom-right
        SetVertex(_model, x + length, y + height, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x, y + height, z, tx1, tx2);            // Bottom-left
        SetVertex(_model, x + length, y + height, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x, y + height, z + width, tx1, tx2);    // Top-left
    }

    // Bottom face
    if (!_block.bottomObscured) {
        SetVertex(_model, x, y, z, tx1, tx2);            // Bottom-left
        SetVertex(_model, x + length, y, z, tx1, tx2);   // Bottom-right
        SetVertex(_model, x + length, y, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x, y, z, tx1, tx2);            // Bottom-left
        SetVertex(_model, x + length, y, z + width, tx1, tx2); // Top-right
        SetVertex(_model, x, y, z + width, tx1, tx2);    // Top-left
    }

    return 0;
}
int addCubeType(VoxelModel * _model, int x, int y, int z, uint8_t width, uint8_t height, uint8_t length, uint8_t block, BlockData _data) {
    // (0,0) - green
    // (0.26,0.26) - brown
    // (0.52,0.52) - yellow
    // (0.78,0.78) - blue 
    // (1.04,1.04) - gray
    switch (block) {
    case GRASS:
        addCubeMesh(_model, x, y, z, width, height, length, 0, 0, _data);
        break;
    case DIRT:
        addCubeMesh(_model, x, y, z, width, height, length, 0.25, 0.25, _data);
        break;
    case STONE:
        addCubeMesh(_model, x, y, z, width, height, length, 0.9, 0.9, _data);
        break;
    case WATER:
        addCubeMesh(_model, x, y, z, width, height, length, 0.75, 0.75, _data);
        break;
    case SAND:
        addCubeMesh(_model, x, y, z, width, height, length, 0.5, 0.5, _data);
        break;
    }
    // a a down
    return 0;
}
// Generate a simple triangle mesh from code
Model GenMeshCustom(std::vector<BlockData> _blockList)
{
    VoxelModel _model;
    _model._mesh = { 0 };
    _model.vertCount = 0;
    _model.texCount = 0;
    int triangles = 0;
    for (const BlockData _block : _blockList) {
        if (!_block.backObscured)  triangles += 2;
        if (!_block.frontObscured)  triangles += 2;
        if (!_block.leftObscured)  triangles += 2;
        if (!_block.rightObscured)  triangles += 2;
        if (!_block.topObscured)  triangles += 2;
        if (!_block.bottomObscured)  triangles += 2;
    }
    _model._mesh.triangleCount = triangles;
    _model._mesh.vertexCount = _model._mesh.triangleCount*3;
    _model._mesh.vertices = (float*)MemAlloc(_model._mesh.vertexCount * 3 * sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    _model._mesh.texcoords = (float*)MemAlloc(_model._mesh.vertexCount * 2 * sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    _model._mesh.normals = (float*)MemAlloc(_model._mesh.vertexCount * 3 * sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)
    BLOCK_TYPES types[4] = { GRASS,DIRT,STONE,WATER };
    int r = 0;
    for (const BlockData _block : _blockList) {
        if (r > 4) {
            r = 0;
        }
        addCubeType(&_model, _block.x * blockSize, _block.y * blockSize, _block.z * blockSize, _block.width, _block.length,1, _block.type, _block);
        r += 1;

    }
    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&_model._mesh, false);
    Model mod = LoadModelFromMesh(_model._mesh);
    return mod;
}