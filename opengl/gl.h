#ifndef GL_H
#define GL_H

class QOpenGLFunctions_3_3_Core;
class Shader;

class GL{
public:
    static QOpenGLFunctions_3_3_Core funcs;
    static Shader *terrainShader;
    static Shader *skyboxShader;
};

#endif // GL_H
