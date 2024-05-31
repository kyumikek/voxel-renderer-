#pragma once
#define blockSize 2
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <iostream>
enum BLOCK_TYPES {
    GRASS,
    WATER,
    STONE,
    SAND,
    DIRT,
};
struct BlockData {
    int x;
    int y;
    int z;
    bool obstructed;
    bool frontObscured;
    bool backObscured;
    bool leftObscured;
    bool rightObscured;
    bool topObscured;
    bool bottomObscured;
    BLOCK_TYPES type;
};
struct VoxelModel {
    Mesh _mesh;
    int vertCount;
    int texCount;
};
Vector3 castVector3(int x, int y, int z) {
    Vector3 newVec = { x,y,z };
    return newVec;
}
int SetVertex(VoxelModel *_model, int x, int y, int z, float tx1, float tx2) {
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
int addCubeMesh(VoxelModel *_model, int x, int y, int z, float tx1, float tx2, BlockData _block) {

    // Front face
    if (!_block.frontObscured) {
        SetVertex(_model, x, y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, z, tx1, tx2);
        SetVertex(_model, x, y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, z, tx1, tx2);
        SetVertex(_model, x, blockSize + y, z, tx1, tx2);
    }

    // Back face
    if (!_block.backObscured) {
        SetVertex(_model, x, y, blockSize + z, tx1, tx2);
        SetVertex(_model, blockSize + x, y, blockSize + z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, blockSize + z, tx1, tx2);
        SetVertex(_model, x, y, blockSize + z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, blockSize + z, tx1, tx2);
        SetVertex(_model, x, blockSize + y, blockSize + z, tx1, tx2);
    }

    // Left face
    if (!_block.leftObscured) {
        SetVertex(_model, x, y, z, tx1, tx2);
        SetVertex(_model, x, blockSize + y, z, tx1, tx2);
        SetVertex(_model, x, blockSize + y, blockSize + z, tx1, tx2);
        SetVertex(_model, x, y, z, tx1, tx2);
        SetVertex(_model, x, blockSize + y, blockSize + z, tx1, tx2);
        SetVertex(_model, x, y, blockSize + z, tx1, tx2);
    }

    // Right face
    if (!_block.rightObscured) {
        SetVertex(_model, blockSize + x, y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, blockSize + z, tx1, tx2);
        SetVertex(_model, blockSize + x, y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, blockSize + z, tx1, tx2);
        SetVertex(_model, blockSize + x, y, blockSize + z, tx1, tx2);
    }

    // Top face
    if (!_block.topObscured) {
        SetVertex(_model, x, y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, y, blockSize + z, tx1, tx2);
        SetVertex(_model, x, y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, y, blockSize + z, tx1, tx2);
        SetVertex(_model, x, y, blockSize + z, tx1, tx2);
    }

    // Bottom face
    if (!_block.bottomObscured) {
        SetVertex(_model, x, blockSize + y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, blockSize + z, tx1, tx2);
        SetVertex(_model, x, blockSize + y, z, tx1, tx2);
        SetVertex(_model, blockSize + x, blockSize + y, blockSize + z, tx1, tx2);
        SetVertex(_model, x, blockSize + y, blockSize + z, tx1, tx2);
    }

    return 0;
}
int addCubeType(VoxelModel * _model, int x, int y, int z, BLOCK_TYPES block, BlockData _data) {
    // (0,0) - green
    // (0.26,0.26) - brown
    // (0.52,0.52) - yellow
    // (0.78,0.78) - blue 
    // (1.04,1.04) - gray
    switch (block) {
    case GRASS:
        addCubeMesh(_model, x, y, z, 0, 0, _data);
        break;
    case DIRT:
        addCubeMesh(_model, x, y, z, 0.25, 0.25, _data);
        break;
    case STONE:
        addCubeMesh(_model, x, y, z, 0.9, 0.9, _data);
        break;
    case WATER:
        addCubeMesh(_model, x, y, z, 0.75, 0.75, _data);
        break;
    case SAND:
        addCubeMesh(_model, x, y, z, 0.5, 0.5, _data);
        break;
    }
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
    
    for (const BlockData _block : _blockList) {
        addCubeType(&_model, _block.x * 2, _block.y * 2, _block.z * 2, _block.type, _block);
    }
    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&_model._mesh, false);
    Model mod = LoadModelFromMesh(_model._mesh);
    return mod;
}