#ifndef COLORS_H
#define COLORS_H

#include <math.h>

struct Color {
    float rgb[3];
    Color(float r, float g, float b);
};

struct ColorPalette {
    Color color[3];
};

float b1(float v);
float b2(float v);
Color HCLtoRGB(float h, float c, float l);

void genTwoColorScheme(float* colorData);
void genMountainousScheme(float* colorData);
void genYoloScheme(float* colorData);

#endif // COLORS_H
