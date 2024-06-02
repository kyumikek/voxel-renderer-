#include <cmath>
#include "noise.hpp"
float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = (h < 8) ? x : y;
    float v = (h < 4) ? y : (h == 12 || h == 14) ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float perlin(float x, float y, float z, int permutation_table[256]) {
    int X = ((int)std::floor(x)) & 255;
    int Y = ((int)std::floor(y)) & 255;
    int Z = ((int)std::floor(z)) & 255;

    float fx = x - std::floor(x);
    float fy = y - std::floor(y);
    float fz = z - std::floor(z);

    float u = fade(fx);
    float v = fade(fy);
    float w = fade(fz);

    int A = permutation_table[X] + Y;
    int AA = permutation_table[A] + Z;
    int AB = permutation_table[A + 1] + Z;
    int B = permutation_table[X + 1] + Y;
    int BA = permutation_table[B] + Z;
    int BB = permutation_table[B + 1] + Z;

    return lerp(w,
        lerp(v,
            lerp(u, grad(permutation_table[AA], fx, fy, fz), grad(permutation_table[BA], fx - 1, fy, fz)),
            lerp(u, grad(permutation_table[AB], fx, fy - 1, fz), grad(permutation_table[BB], fx - 1, fy - 1, fz))
        ),
        lerp(v,
            lerp(u, grad(permutation_table[AA + 1], fx, fy, fz - 1), grad(permutation_table[BA + 1], fx - 1, fy, fz - 1)),
            lerp(u, grad(permutation_table[AB + 1], fx, fy - 1, fz - 1), grad(permutation_table[BB + 1], fx - 1, fy - 1, fz - 1))
        )
    );
}