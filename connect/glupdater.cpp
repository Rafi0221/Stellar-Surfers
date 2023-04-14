
#include "glupdater.h"

#include "../opengl/gl.h"

GLUpdater::GLUpdater()
{
}


void GLUpdater::setToDefault() {
    setCameraXYZ(0.0f, 0.0f, 0.0f);
}

float GLUpdater::scaleAngle(const float &a) {
    static const float Deadzone = 0.05f;
    static const float MaxAngle = 0.4f;

    if(abs(a) < Deadzone)
        return 0.0f;
    if(a > MaxAngle)
        return 1.0f;
    if(a < -MaxAngle)
        return -1.0f;

    if(a > 0)
        return (a - Deadzone)/(MaxAngle - Deadzone);
    else
        return (a + Deadzone)/(MaxAngle - Deadzone);
}

void GLUpdater::setCameraXYZ(const float &x, const float &y, const float &z) {
    GL::rotation.setX(scaleAngle(x));
    GL::rotation.setY(scaleAngle(y));
    GL::rotation.setZ(scaleAngle(z));
}
