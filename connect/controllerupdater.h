
#ifndef CONTROLLERUPDATER_H
#define CONTROLLERUPDATER_H


#include "connectmanager.h"

#include <QObject>


class ControllerUpdater : public QObject
{
    Q_OBJECT
public:
    ControllerUpdater(ConnectManager* connectManager);
    void update(QVector3D position, float speed);

private:
    ConnectManager* connectManager;
};

#endif // CONTROLLERUPDATER_H
