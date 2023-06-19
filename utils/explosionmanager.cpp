#include "explosionmanager.h"

#include "consts.h"
#include "explosion.h"
#include "obj_loader.h"

#include <QVector3D>

ExplosionManager::ExplosionManager()
{

}

void ExplosionManager::addExplosion(QVector3D position){
    explosions.push_back({new Explosion(position), EXPLOSION_LIFETIME + currentTime});
}

void ExplosionManager::removeExplosion(int id){
    swap(explosions[id], explosions.back());
    delete explosions.back().first;
    explosions.pop_back();
}

void ExplosionManager::update(Camera *camera, float deltaTime){
    currentTime += deltaTime;
    for(int i = explosions.size() - 1; i >= 0;i--){
        if(explosions[i].second <= currentTime){
            removeExplosion(i);
        }else{
            explosions[i].first->update(camera, deltaTime);
        }
    }

}

void ExplosionManager::render(){
    for(int i = 0; i < explosions.size(); i++){
        explosions[i].first->render();
    }
}
