#include "gl.h"

#include "../utils/shader.h"

#include <QOpenGLFunctions_3_3_Core>

QOpenGLFunctions_3_3_Core GL::funcs;
Shader *GL::terrainShader;
