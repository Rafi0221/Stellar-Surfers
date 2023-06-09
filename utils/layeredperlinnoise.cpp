#include "layeredperlinnoise.h"

#include "perlinnoise.h"

LayeredPerlinNoise::LayeredPerlinNoise(unsigned int seed, int octaves, double maxValue,
                                       double baseFrequency, double persistance, double lacunarity){
    this->perlin = new PerlinNoise(seed);

    this->octaves = octaves;
    this->baseFrequency = baseFrequency;
    this->persistance = persistance;
    this->lacunarity = lacunarity;


    double realMaxValue = 0;
    double amplitude = 1.0f;
    for(int i = 0; i < this->octaves; i++){
        realMaxValue += amplitude;
        amplitude *= this->persistance;
    }
    realMaxValue *= 0.707;
    this->multiplier = maxValue/realMaxValue;
}

LayeredPerlinNoise::~LayeredPerlinNoise(){
    delete perlin;
}

double LayeredPerlinNoise::getValue(double x, double y, double z){
    double value = 0.0f;
    double amplitude = 1.0f;
    double frequency = baseFrequency;
    for(int i = 0; i < octaves; i++){
        value += perlin->getValue(x * frequency, y * frequency, z * frequency) * amplitude;
        frequency *= lacunarity;
        amplitude *= persistance;
    }
    return value * multiplier;
}

double LayeredPerlinNoise::getMultiplier(){
    return multiplier;
}

PerlinNoise* LayeredPerlinNoise::getPerlinNoise(){
    return perlin;
}
