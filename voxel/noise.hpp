#pragma once
float fade(float t);
float lerp(float t, float a, float b);
float grad(int hash, float x, float y, float z);
float perlin(float x, float y, float z, int permutation_table[256]);