#ifndef GL_H
#define GL_H

class QOpenGLFunctions_3_3_Core;

#include <QVector3D>

class GL{
public:
    static QOpenGLFunctions_3_3_Core funcs;
    static QVector3D rotation;
};

#endif // GL_H
