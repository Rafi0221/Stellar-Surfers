#version 330 core

uniform sampler1D permutation;

float fade(float t){
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b){
    return a + t * (b - a);
}

float grad(int hash, float x, float y, float z){
    int h = hash & 15;
    float u = h < 8 ? x : y,
           v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float perlin(vec3 point){
    float x = point.x;
    float y = point.y;
    float z = point.z;

    int X = int(floor(x)) & 255;
    int Y = int(floor(y)) & 255;
    int Z = int(floor(z)) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    int A = int(round(texture(permutation, (X) / 512.0).a)) + Y;
    int AA = int(round(texture(permutation, (A) / 512.0).a)) + Z;
    int AB = int(round(texture(permutation, (A + 1) / 512.0).a)) + Z;
    int B = int(round(texture(permutation, (X + 1) / 512.0).a)) + Y;
    int BA = int(round(texture(permutation, (B) / 512.0).a)) + Z;
    int BB = int(round(texture(permutation, (B + 1) / 512.0).a)) + Z;

    int PAA = int(round(texture(permutation, (AA) / 512.0).a));
    int PAB = int(round(texture(permutation, (AB) / 512.0).a));
    int PBA = int(round(texture(permutation, (BA) / 512.0).a));
    int PBB = int(round(texture(permutation, (BB) / 512.0).a));

    int PAA1 = int(round(texture(permutation, (AA + 1) / 512.0).a));
    int PAB1 = int(round(texture(permutation, (AB + 1) / 512.0).a));
    int PBA1 = int(round(texture(permutation, (BA + 1) / 512.0).a));
    int PBB1 = int(round(texture(permutation, (BB + 1) / 512.0).a));

    float res = lerp(w, lerp(v, lerp(u, grad(PAA, x, y, z), grad(PBA, x-1, y, z)), lerp(u, grad(PAB, x, y-1, z), grad(PBB, x-1, y-1, z))), lerp(v, lerp(u, grad(PAA1, x, y, z-1), grad(PBA1, x-1, y, z-1)), lerp(u, grad(PAB1, x, y-1, z-1), grad(PBB1, x-1, y-1, z-1))));
    return (res + 1.0)/2.0;
}
