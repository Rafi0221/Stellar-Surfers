#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
    Camera();

    QMatrix4x4 GetViewMatrix();

    void SetPosition(QVector3D position);
    QVector3D GetPosition();

    QVector3D GetFront();

    void SetYaw(float yaw);
    float GetYaw();

    void SetPitch(float pitch);
    float GetPitch();

    void SetRoll(float roll);
    float GetRoll();
private:
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;

    float Yaw;
    float Pitch;
    float Roll;

    void UpdateCameraVectors();
};

#endif // CAMERA_H
