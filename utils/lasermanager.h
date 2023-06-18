#ifndef LASERMANAGER_H
#define LASERMANAGER_H

#include <QVector3D>
#include <vector>

class Camera;
class ExplosionManager;
class Laser;
class Space;

class LaserManager
{
private:
    std::vector<std::pair<Laser*, float>> lasers;
    float currentTime = 0;
    void removeLaser(int id);
public:
    LaserManager();

    void update(float deltaTime);
    void render(Camera *camera);
    void addLaser(QVector3D position, QVector3D direction, float baseSpeed);
    void removeLaser(Laser *laser);
    bool checkCollisions(Space *space, ExplosionManager *explosionManager);
};

#endif // LASERMANAGER_H
