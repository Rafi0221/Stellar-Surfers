#ifndef MODEL_H
#define MODEL_H

#include <string>

class Shader;

class Model
{
private:
    unsigned int VAO;
    unsigned int size;
    unsigned int diffuseTexture = -1, specularTexture = -1, normalTexture = -1;

public:
    Model(std::string path, std::string diffusePath, std::string specularPath, std::string normalPath);
    unsigned int getVAO();
    unsigned int getSize();
    void setupShader(Shader *shader);
};

#endif // MODEL_H
