#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

class Camera;
class ExplosionManager;
class LaserManager;
class Space;

class CollisionManager
{
private:
    Space *space;
    LaserManager *laserManager;
    Camera *camera;
    ExplosionManager *explosionManager;
public:
    CollisionManager(Camera *camera, Space *space, LaserManager *laserManager, ExplosionManager *explosionManager);
    void update();
};

#endif // COLLISIONMANAGER_H
