
#include "raylib.h"
#include "rlgl.h"
#include "stdio.h"
#define blockSize 0.5
static Mesh GenMeshCustom(void);

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - mesh generation");

    Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    Model model = LoadModelFromMesh(GenMeshCustom());
    rlDisableBackfaceCulling();

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawModel(model, (Vector3){0,0,0}, 1.0f, WHITE);
        DrawGrid(10, 1.0);

        EndMode3D();
        DrawFPS(0, 0);
        EndDrawing();
    }

    

    CloseWindow();
    
    return 0;
}
int SetVertex(Mesh* mesh, int x, int y, int z, int id) {
    printf("\n %d", id);
    mesh->vertices[id * 3] = x;
    mesh->vertices[id * 3+1] = y;
    mesh->vertices[id * 3+2] = z;

}
void addCube(Mesh* mesh, int x, int y, int z, int id) {
    // Front face
    SetVertex(mesh, -blockSize + x, -blockSize + y, -blockSize + z, 0 + id * 36);
    SetVertex(mesh, blockSize + x, -blockSize + y, -blockSize + z, 1 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, -blockSize + z, 2 + id * 36);
    SetVertex(mesh, -blockSize + x, -blockSize + y, -blockSize + z, 3 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, -blockSize + z, 4 + id * 36);
    SetVertex(mesh, -blockSize + x, blockSize + y, -blockSize + z, 5 + id * 36);

    // Back face
    SetVertex(mesh, -blockSize + x, -blockSize + y, blockSize + z, 6 + id * 36);
    SetVertex(mesh, blockSize + x, -blockSize + y, blockSize + z, 7 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, blockSize + z, 8 + id * 36);
    SetVertex(mesh, -blockSize + x, -blockSize + y, blockSize + z, 9 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, blockSize + z, 10 + id * 36);
    SetVertex(mesh, -blockSize + x, blockSize + y, blockSize + z, 11 + id * 36);

    // Left face
    SetVertex(mesh, -blockSize + x, -blockSize + y, -blockSize + z, 12 + id * 36);
    SetVertex(mesh, -blockSize + x, blockSize + y, -blockSize + z, 13 + id * 36);
    SetVertex(mesh, -blockSize + x, blockSize + y, blockSize + z, 14 + id * 36);
    SetVertex(mesh, -blockSize + x, -blockSize + y, -blockSize + z, 15 + id * 36);
    SetVertex(mesh, -blockSize + x, blockSize + y, blockSize + z, 16 + id * 36);
    SetVertex(mesh, -blockSize + x, -blockSize + y, blockSize + z, 17 + id * 36);

    // Right face
    SetVertex(mesh, blockSize + x, -blockSize + y, -blockSize + z, 18 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, -blockSize + z, 19 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, blockSize + z, 20 + id * 36);
    SetVertex(mesh, blockSize + x, -blockSize + y, -blockSize + z, 21 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, blockSize + z, 22 + id * 36);
    SetVertex(mesh, blockSize + x, -blockSize + y, blockSize + z, 23 + id * 36);

    // Top face
    SetVertex(mesh, -blockSize + x, -blockSize + y, -blockSize + z, 24 + id * 36);
    SetVertex(mesh, blockSize + x, -blockSize + y, -blockSize + z, 25 + id * 36);
    SetVertex(mesh, blockSize + x, -blockSize + y, blockSize + z, 26 + id * 36);
    SetVertex(mesh, -blockSize + x, -blockSize + y, -blockSize + z, 27 + id * 36);
    SetVertex(mesh, blockSize + x, -blockSize + y, blockSize + z, 28 + id * 36);
    SetVertex(mesh, -blockSize + x, -blockSize + y, blockSize + z, 29 + id * 36);

    // Bottom face
    SetVertex(mesh, -blockSize + x, blockSize + y, -blockSize + z, 30 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, -blockSize + z, 31 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, blockSize + z, 32 + id * 36);
    SetVertex(mesh, -blockSize + x, blockSize + y, -blockSize + z, 33 + id * 36);
    SetVertex(mesh, blockSize + x, blockSize + y, blockSize + z, 34 + id * 36);
    SetVertex(mesh, -blockSize + x, blockSize + y, blockSize + z, 35 + id * 36);
}
// Generate a simple triangle mesh from code
static Mesh GenMeshCustom(void)
{
    Mesh mesh = { 0 };
    mesh.triangleCount = 6*4096;
    mesh.vertexCount = mesh.triangleCount * 6;
    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float*)MemAlloc(mesh.vertexCount * 2 * sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)
    for (int i = 0; i < 4096; i += 1) {
        addCube(&mesh, GetRandomValue(0, 10), GetRandomValue(0, 10),GetRandomValue(0,10), i);

    }
    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

    return mesh;
}