#ifndef SKYBOX_H
#define SKYBOX_H


class SkyBox
{
private:
    unsigned int VAO, VBO;
public:
    SkyBox();

    void Render();
};

#endif // SKYBOX_H
