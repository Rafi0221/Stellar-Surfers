#ifndef LAYEREDPERLINNOISE_H
#define LAYEREDPERLINNOISE_H

#include <stdlib.h>

#include "noise.h"

class PerlinNoise;

class LayeredPerlinNoise : public Noise
{
private:
    PerlinNoise *perlin;

    int octaves;
    double baseFrequency;
    double persistance;
    double lacunarity;

    double multiplier;
public:
    LayeredPerlinNoise(unsigned int seed = rand(), int octaves = 15, double maxValue = 1.0f,
                       double baseFrequency = 0.6f, double persistance = 0.5f, double lacunarity = 2.0f);
    virtual double getValue(double x, double y, double z);
    double getMultiplier();
    PerlinNoise *getPerlinNoise();
};

#endif // LAYEREDPERLINNOISE_H
