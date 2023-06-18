#include "lasermanager.h"

#include "../game/space.h"
#include "camera.h"
#include "consts.h"
#include "explosionmanager.h"
#include "laser.h"

#include <vector>

#include <QVector3D>

LaserManager::LaserManager()
{

}

void LaserManager::render(Camera *camera){
    for(int i = 0; i < lasers.size(); i++)
        lasers[i].first->render(camera);
}

void LaserManager::update(float deltaTime){
    currentTime += deltaTime;
    for(int i = lasers.size() - 1; i >= 0; i--){
        if(lasers[i].second <= currentTime){
            removeLaser(i);
        }else{
            lasers[i].first->update(deltaTime);
        }
    }
}

void LaserManager::addLaser(QVector3D position, QVector3D direction, float baseSpeed){
    lasers.push_back(std::make_pair(new Laser(direction, position, baseSpeed + LASER_SPEED), currentTime + LASER_LIFETIME));
}

void LaserManager::removeLaser(Laser *laser){
    for(int i = 0; i < lasers.size(); i++){
        if(lasers[i].first == laser){
            removeLaser(i);
            return;
        }
    }
}

void LaserManager::removeLaser(int id){
    swap(lasers[id], lasers.back());
    delete lasers.back().first;
    lasers.pop_back();
}

void LaserManager::checkCollisions(Space *space, ExplosionManager *explosionManager){
    for(int i = lasers.size() - 1; i >= 0; i--){
        int type = lasers[i].first->checkCollisions(space);
        if(type != 0){
            removeLaser(i);
            if(type == ASTEROID_COLLISION){
                space->deleteAsteroid(lasers[i].first->getCenter());
                explosionManager->addExplosion();
            }
        }
    }
}
