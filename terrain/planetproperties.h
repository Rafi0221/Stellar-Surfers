#ifndef PLANETPROPERTIES_H
#define PLANETPROPERTIES_H

#include "../opengl/gl.h"
#include "../utils/consts.h"

#include <QOpenGLFunctions_3_3_Core>
#include <random>

struct Wave{
    float amplitude;
    float steepness;
    float frequency;
    float speed;
    float direction[4];
};

class PlanetProperties{
public:
    PlanetProperties(int seed, float radius){
        this->seed = seed;
        this->radius = radius;

        std::mt19937 gen(seed); // seed the generator
        std::uniform_real_distribution<> distr(0, 1);

        if(distr(gen) <= WATER_CUTOFF){
            hasWater = true;

            Wave *waves = (Wave*)malloc(sizeof(Wave) * WAVES_AMOUNT);
            for(int i = 0; i < WAVES_AMOUNT; i++){
//                waves[i].amplitude = distr(gen) / 50.0f + 0.01f;
                waves[i].amplitude = 0.007f;
        //        waves[i].frequency = rand() % 150 + 150;
                waves[i].frequency = (i * 3) * (0.5 + (rand()%100 / 100.0f));
                waves[i].speed = 0.5;
                waves[i].steepness = 0.6;
                for(int j = 0; j < 3; j++){
                    waves[j].direction[j] = (rand()%1000 + 1) / 500.0f - 1.0f;
                }
            }
            GL::funcs.glGenBuffers(1, &wavesBuffer);
            GL::funcs.glBindBuffer(GL_UNIFORM_BUFFER, wavesBuffer);
            GL::funcs.glBufferData(GL_UNIFORM_BUFFER, sizeof(Wave) * WAVES_AMOUNT, waves, GL_STATIC_DRAW);
            GL::funcs.glBindBuffer(GL_UNIFORM_BUFFER, 0);
            delete[] waves;
        }

        if(distr(gen) <= TERRAIN_CUTOFF || !hasWater){
            hasTerrain = true;

            float colorData[COLOR_TEXTURE_RESOLUTION * 3];

            for(int i = 0; i < COLOR_TEXTURE_RESOLUTION; i++){
                if(i > 256){
                    if(i > 300){
                        if(i > 400){
                            colorData[i * 3] = 0.35 + (1.0 - 0.35) * (i-300.0) / (512.0 - 300.0);
                            colorData[i * 3 + 1] = 0.30 + (0.97 - 0.30) * (i-300.0) / (512.0 - 300.0);
                            colorData[i * 3 + 2] = 0.25 + (0.97 - 0.25) * (i-300.0) / (512.0 - 300.0);
                        }else{
                            colorData[i * 3] = 0.0 + (0.35 - 0.0) * (i-300.0) / (512.0 - 300.0);
                            colorData[i * 3 + 1] = 0.6 + (0.30 - 0.6) * (i-300.0) / (512.0 - 300.0);
                            colorData[i * 3 + 2] = 0.1 + (0.25 - 0.1) * (i-300.0) / (512.0 - 300.0);
                        }
                    }else{
                        colorData[i * 3] = 0.76 + (0.0 - 0.76) * (i-256.0) / (300.0 - 256.0);
                        colorData[i * 3 + 1] = 0.7 + (0.6 - 0.7) * (i-256.0) / (300.0 - 256.0);
                        colorData[i * 3 + 2] = 0.5 + (0.1 - 0.5) * (i-256.0) / (300.0 - 256.0);
                    }
                }else{
                    colorData[i * 3] = 0.76;
                    colorData[i * 3 + 1] = 0.7;
                    colorData[i * 3 + 2] = 0.5;
                }
//                colorData[i * 3] = (float)i / (COLOR_TEXTURE_RESOLUTION-1);
//                colorData[i * 3 + 1] = (float)i / (COLOR_TEXTURE_RESOLUTION-1);
//                colorData[i * 3 + 2] = (float)i / (COLOR_TEXTURE_RESOLUTION-1);
            }

            GL::funcs.glGenTextures(1, &colorMapTexture);
            GL::funcs.glBindTexture(GL_TEXTURE_1D, colorMapTexture);

            GL::funcs.glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, COLOR_TEXTURE_RESOLUTION, 0, GL_RGB, GL_FLOAT, colorData);
            GL::funcs.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            GL::funcs.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            GL::funcs.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            GL::funcs.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GL::funcs.glBindTexture(GL_TEXTURE_1D, 0);

        }
    }

    int seed;
    float radius;

    bool hasWater;
    bool hasTerrain;

    unsigned int wavesBuffer;

    unsigned int colorMapTexture;
};

#endif // PLANETPROPERTIES_H