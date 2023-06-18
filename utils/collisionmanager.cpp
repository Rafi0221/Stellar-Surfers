#include "collisionmanager.h"

#include "../game/space.h"
#include "camera.h"
#include "consts.h"
#include "explosionmanager.h"
#include "lasermanager.h"

CollisionManager::CollisionManager(Camera *camera, Space *space, LaserManager *laserManager, ExplosionManager* explosionManager)
{
    this->camera = camera;
    this->space = space;
    this->laserManager = laserManager;
    this->explosionManager = explosionManager;
}

void CollisionManager::update(){
    // camera collisions
    if(space->checkCollision(camera->getPosition())) {
        int collisionType;
        if(space->checkCollisionPlanet(camera->getPosition()))
            collisionType = PLANET_COLLISION;
        else
            collisionType = ASTEROID_COLLISION;
        camera->notifyCollision(collisionType);

        if(collisionType == ASTEROID_COLLISION && camera->getSpeed() > 0.007){
            space->deleteAsteroid(camera->getPosition());
            explosionManager->addExplosion();
            // eXPloSiOn !!!
        }
    }


    // laser collisions
    laserManager->checkCollisions(space, explosionManager);
}
