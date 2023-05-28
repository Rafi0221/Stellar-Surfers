#include "colors.h"

#include <algorithm>
#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "consts.h"

// source: https://stackoverflow.com/questions/7530627/hcl-color-to-rgb-and-backward

Color::Color(float r, float g, float b) {
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}

float rgb255(float v) {
    return std::min(255.0f, std::max(v, 0.0f)) / 255.0;
}

float b1(float v) {
    if (v > 0.0031308) return pow(v, 0.416667) * 269.025 - 14.025;
    return v * 3294.6;
}

float b2(float v) {
    if (v > 0.2068965) return pow(v, 3);
    return (v - 0.137931) * 0.1284185;
}

Color HCLtoRGB(float h, float c, float l) {
    l = (l + 16.0) / 116.0;
    h *= M_PI / 180.0;
    float x = b2(l + (c / 500.0) * cos(h));
    float y = b2(l);
    float z = b2(l - (c / 200.0) * sin(h));
    return Color(
        rgb255(b1(x * 3.021973625 - y * 1.617392459 - z * 0.404875592)),
        rgb255(b1(x * -0.943766287 + y * 1.916279586 + z * 0.027607165)),
        rgb255(b1(x * 0.069407491 - y * 0.22898585 + z * 1.159737864))
    );
}

void genTwoColorScheme(float* colorData) {
    float h0 = rand() % 360;
    for(int i = 0; i < COLOR_TEXTURE_RESOLUTION; i++){
        float t = float(i) / COLOR_TEXTURE_RESOLUTION;
        float h = h0 + (180 * t);
        if (h > 360) h -= 360;
        float c = 5 + t * 10.0;
        float l = 10 + t * 45.0;
        Color color = HCLtoRGB(h, c, l);
        float alpha = std::min(1.0f, std::max(0.0f, 4.0f - 5.0f * t));
        for (int j = 0; j < 3; j++)
            colorData[i * 3 + j] = color.rgb[j];
    }
}

void genMountainousScheme(float* colorData) {
    int hue = rand() % 360;
    Color dark = HCLtoRGB(hue, 2.0, 26.0);
    Color depression = HCLtoRGB(hue, 2.0, 27.0);
    Color baseColor = HCLtoRGB(hue, 20.0, 27.0);
    Color baseColorDarker = HCLtoRGB(hue, 10.0, 25.0);
    Color greyish = HCLtoRGB(hue, 2.0, 30.0);
    Color white = HCLtoRGB(hue, 0.0, 60.0);
    Color colors[] = {dark, depression, baseColor, baseColor, baseColorDarker, dark, greyish, white};
    float th[] = {0.0, 0.40, 0.5, 0.6, 0.72, 0.75, 0.78, 1.0}; // thresholds
    for(int i = 0; i < COLOR_TEXTURE_RESOLUTION; i++){
        float t = float(i) / COLOR_TEXTURE_RESOLUTION;
        for (int j = 1; j <= 7; j++) {
            if (t < th[j]) {
                float alpha = (t - th[j - 1]) / (th[j] - th[j - 1]);
                if (j == 7) alpha = pow(alpha, 2);
                for (int k = 0; k < 3; k++)
                    colorData[i * 3 + k] =
                        colors[j - 1].rgb[k] * (1 - alpha) +
                        colors[j].rgb[k] * alpha;
                break;
            }
        }
    }
}

void genYoloScheme(float* colorData) {
    float hue = rand() % 360;
    Color baseColor = HCLtoRGB(hue, 100.0, 50.0);
    Color colors[] = {baseColor, baseColor};
    float th[] = {0.0, 0.8}; // thresholds
    for(int i = 0; i < COLOR_TEXTURE_RESOLUTION; i++){
        float t = float(i) / COLOR_TEXTURE_RESOLUTION;
        for (int j = 1; j <= 1; j++) {
            if (t < th[j]) {
                float alpha = (t - th[j - 1]) / (th[j] - th[j - 1]);
                for (int k = 0; k < 3; k++)
                    colorData[i * 3 + k] =
                        colors[j - 1].rgb[k] * (1 - alpha) +
                        colors[j].rgb[k] * alpha;
                break;
            }
        }
    }
}
