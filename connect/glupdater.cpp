
#include "glupdater.h"

#include "../opengl/gl.h"

GLUpdater::GLUpdater()
{
}


void GLUpdater::setToDefault() {
    setCameraXYZ(0.0f, 0.0f, 0.0f);
    setAcceleration(0.0f);
}

float GLUpdater::scaleAngle(const float a) {
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

void GLUpdater::smoothenAngles(float &x, float &y, float &z) {
    static const float C = 0.1f;
    avX = C * x + (1.0f-C) * avX;
    avY = C * y + (1.0f-C) * avY;
    avZ = C * z + (1.0f-C) * avZ;
    x = avX, y = avY, z = avZ;
}

void GLUpdater::setCameraXYZ(float x, float y, float z) {
    x = scaleAngle(x);
    y = scaleAngle(y);
    z = scaleAngle(z);
    smoothenAngles(x, y, z);
    GL::rotation.setX(x);
    GL::rotation.setY(y);
    GL::rotation.setZ(z);
}

void GLUpdater::setAcceleration(float acceleration) {
    GL::acceleration = acceleration;
}

void GLUpdater::notifyShoot(float x, float y){
    GL::shoot = true;
    //should set some coordinates
}
