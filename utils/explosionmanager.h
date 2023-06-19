#ifndef EXPLOSIONMANAGER_H
#define EXPLOSIONMANAGER_H

#include <QVector3D>

#include <vector>

class Camera;
class Explosion;

class ExplosionManager
{
private:
    float currentTime = 0;
    std::vector<std::pair<Explosion*, float>> explosions;
    void removeExplosion(int id);
public:
    ExplosionManager();
    void addExplosion(QVector3D position);
    void update(Camera *camera, float deltaTime);
    void render();
};

#endif // EXPLOSIONMANAGER_H
