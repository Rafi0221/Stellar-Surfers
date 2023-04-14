#include "gl.h"

#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>

QOpenGLFunctions_3_3_Core GL::funcs;

// values in [0,1]
QVector3D GL::rotation;
