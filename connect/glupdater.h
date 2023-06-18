
#ifndef GLUPDATER_H
#define GLUPDATER_H


#include <QObject>


class GLUpdater : public QObject
{
    Q_OBJECT

public:
    GLUpdater();

    void setCameraXYZ(float x, float y, float z);
    void setAcceleration(float accel);
    void notifyShoot(float x, float y);
    void notifyGameOver();

public slots:
    void setToDefault();

private:
    float scaleAngle(float a);
    void smoothenAngles(float &x, float &y, float &z);

    float avX = 0.0f, avY = 0.0f, avZ = 0.0f;
};

#endif // GLUPDATER_H
