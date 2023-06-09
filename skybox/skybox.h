#ifndef SKYBOX_H
#define SKYBOX_H

class PerlinNoise;

class SkyBox
{
private:
    unsigned int VAO[6], VBO[6];

    unsigned int textures[6];

public:
    SkyBox(PerlinNoise *noise);

    void render();
};

#endif // SKYBOX_H
