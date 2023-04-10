#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "shader.h"

class ShaderManager{
private:
    static std::map<std::string, Shader*> shaders;
public:
    static void initialize();

    static Shader* getShader(std::string name);
};

#endif // SHADERMANAGER_H
