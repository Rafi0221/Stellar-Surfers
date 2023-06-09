#include "gl.h"

#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>

QOpenGLFunctions_3_3_Core GL::funcs;

// values in [-1, 1]
QVector3D GL::rotation;

// currently 0 or 1
float GL::acceleration;

float GL::time;

int GL::drawCount;
