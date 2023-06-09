#include <cstdlib>
#include <vector>

// THIS CLASS IS A TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT PRESENT IN THE ORIGINAL IMPLEMENTATION)

#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include "noise.h"

class PerlinNoise : public Noise {
    // The permutation vector
    std::vector<int> p;
public:
    // Initialize with the reference values for the permutation vector
    PerlinNoise();
    ~PerlinNoise();
    // Generate a new permutation vector based on the value of seed
    PerlinNoise(unsigned int seed = rand());
    // Get a noise value, for 2D images z can have any value
    virtual double getValue(double x, double y, double z);

    unsigned int getPermutationTexture();
private:
    unsigned int textureID;

    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
    void createPermutationTexture();
};

#endif
