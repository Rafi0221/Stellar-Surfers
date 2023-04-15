#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
    Camera();

    QMatrix4x4 getViewMatrix();

    void setPosition(QVector3D position);
    QVector3D getPosition();

    QVector3D getFront();

    void move(float distance);

    void setYaw(float yaw);
    float getYaw();

    void setPitch(float pitch);
    float getPitch();

    void setRoll(float roll);
    float getRoll();

    void updateCameraVectors();
private:
    QVector3D position;
    QVector3D front;
    QVector3D up;
    QVector3D right;

    float yaw;
    float pitch;
    float roll;
};

#endif // CAMERA_H
