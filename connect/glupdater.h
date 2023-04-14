
#ifndef GLUPDATER_H
#define GLUPDATER_H


#include <QObject>


class GLUpdater : public QObject
{
    Q_OBJECT

public:
    GLUpdater();

    void setCameraXYZ(const float &x, const float &y, const float &z);

public slots:
    void setToDefault();

private:
    float scaleAngle(const float &a);
};

#endif // GLUPDATER_H
