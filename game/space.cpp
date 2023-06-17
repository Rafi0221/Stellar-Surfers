
#include "space.h"

#include "../terrain/noisedpatchfactory.h"
#include "../terrain/planetproperties.h"
#include "../terrain/planet.h"
#include "../utils/consts.h"
#include "../utils/frustum.h"

#include <QVector3D>
#include <cmath>
#include <random>
#include <QDebug>
#include <functional>


// for QHash takenCoords
inline uint qHash(const QVector3D &v, uint seed)
{
    #define ROTL10(x) (((x) << 10) | (((x) >> 22) & 0x000000ff))
    #define ROTL20(x) (((x) << 20) | (((x) >> 12) & 0x0000ffff))
    return qHash(v.x()) ^ ROTL10(qHash(v.y())) ^ ROTL20(qHash(v.z()));
}

unsigned long long hashLL(long long u) {
    unsigned long long v = u * 3935559000370003845 + 2691343689449507681;
    v ^= v >> 21;
    v ^= v << 37;
    v ^= v >>  4;
    v *= 4768777513237032717;
    v ^= v << 20;
    v ^= v >> 41;
    v ^= v <<  5;
    return v;
}


Space::Space(int seed)
{
    this->seed = seed;
}

int Space::hashCoordinates(const QVector3D & coordinates) {
    static int hashBase = 1e8+7;
    long long hash = seed;
    int hashX = hashLL(coordinates.x())^seed;
    int hashY = hashLL(coordinates.y())^seed;
    int hashZ = hashLL(coordinates.z())^seed;
    //qDebug() << hashX << hashY << hashZ;
    hash = (hash * hashBase + hashX) % hashRange;
    hash = (hash * hashBase + hashY) % hashRange;
    hash = (hash * hashBase + hashZ) % hashRange;
    //hash = (hash * hashBase + seed) % hashRange;
    if(hash < 0)
        hash += hashRange;
//    std::mt19937 gen(hash);
//    return (float)gen() / (float)gen.max();
    return hash;
}

void Space::checkAddObject(const QVector3D & coordinates) {
    const int planetHashCutoff = 2500000;
    const int asteroidHashCutoff = 4400000;

    if(takenCoords[coordinates])
        return;

    int hash = hashCoordinates(coordinates);
    if(hash < planetHashCutoff) {
        //qDebug() << hashCoordinates(coordinates) << coordinates;
        //we might need a new hash for planet seed
        Planet* planet = new Planet(hash, hash%5 + 20.0f);
//        PlanetLayer* planet = new PlanetLayer(new NoisedPatchFactory(hashCoordinates(coordinates)),
//                                              new PlanetProperties{hashCoordinates(coordinates),3.0f});
        planet->setPosition(coordinates);
        planets.push_back(planet);
        takenCoords[coordinates] = true;
        // qDebug() << "adding planet, number of planets" << planets.size();
    }
    else if(hash < asteroidHashCutoff) {
        AsteroidCluster* asteroidCluster = new AsteroidCluster(hash, coordinates);
        asteroidClusters.push_back(asteroidCluster);
        takenCoords[coordinates] = true;
        // qDebug() << "adding asteroid, number of asteroids" << asteroidClusters.size();
    }
}


void Space::initialize() {
    asteroidModel = new Model("media/rock.obj", "media/rock_diffuse.png", "media/rock_specular.png", "media/rock_normal.png");

//    models = new float[10000 * 16];
//    int amount = 1000;
//    for(int i = 0; i < amount; i++){
//        QMatrix4x4 tmp;
//        tmp.translate(rand()%100 - 50, rand()%100 - 50, rand()%100 + 5);
//        tmp.scale(0.01);
//        memcpy(models + i * 16, tmp.data(), 16 * sizeof(float));
//    }

    GL::funcs.glGenBuffers(1, &buffer);
    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, buffer);
//    GL::funcs.glBufferData(GL_ARRAY_BUFFER, 16 * amount * sizeof(float), &models[0], GL_STATIC_DRAW);

    unsigned int VAO = asteroidModel->getVAO();
    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glEnableVertexAttribArray(3);
    GL::funcs.glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(4);
    GL::funcs.glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(5);
    GL::funcs.glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(6);
    GL::funcs.glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));

    GL::funcs.glVertexAttribDivisor(3, 1);
    GL::funcs.glVertexAttribDivisor(4, 1);
    GL::funcs.glVertexAttribDivisor(5, 1);
    GL::funcs.glVertexAttribDivisor(6, 1);

    GL::funcs.glBindVertexArray(0);
    
    for(int x = -RENDER_DIST; x <= RENDER_DIST; x += DISTincrement) {
        for(int y = -RENDER_DIST; y <= RENDER_DIST; y += DISTincrement) {
            for(int z = -RENDER_DIST; z <= RENDER_DIST; z += DISTincrement) {
                checkAddObject(QVector3D(x, y, z));
            }
        }
    }
    // qDebug() << "number of planets" << planets.size();
    // qDebug() << "number of asteroids" << asteroidClusters.size();

    position = QVector3D(0, 0, 0);
}

bool Space::isInRange(const QVector3D & pos, const QVector3D & coordinates) {
    static const int dist = RENDER_DIST + 2*DISTincrement;
    return  pos.x()-dist <= coordinates.x() && coordinates.x() <= pos.x()+dist
            && pos.y()-dist <= coordinates.y() && coordinates.y() <= pos.y()+dist
            && pos.z()-dist <= coordinates.z() && coordinates.z() <= pos.z()+dist;
}

void Space::update(QVector3D cameraPosition) {
    QVector<Planet*> planetsUpdated;
    for(Planet* planet: planets) {
        if(isInRange(cameraPosition, planet->getPosition()))
            planetsUpdated.push_back(planet);
        else {
            takenCoords[planet->getPosition()] = false;
            delete planet;
        }
    }
    planets = planetsUpdated;

    QVector<AsteroidCluster*> asteroidsUpdated;
    for(AsteroidCluster* asteroidCluster: asteroidClusters) {
        if(isInRange(cameraPosition, asteroidCluster->getPosition()))
            asteroidsUpdated.push_back(asteroidCluster);
        else {
            takenCoords[asteroidCluster->getPosition()] = false;
            delete asteroidCluster;
        }
    }
    asteroidClusters = asteroidsUpdated;

    int oldX, newX;
    if(cameraPosition.x() < position.x())
        oldX = position.x()-RENDER_DIST, newX = cameraPosition.x()-RENDER_DIST;
    else
        oldX = position.x()+RENDER_DIST, newX = cameraPosition.x()+RENDER_DIST;
    if(oldX != newX && newX % DISTincrement == 0) {
        for(int y = cameraPosition.y()-RENDER_DIST; y <= cameraPosition.y()+RENDER_DIST; y += DISTincrement) {
            for(int z = cameraPosition.z()-RENDER_DIST; z <= cameraPosition.z()+RENDER_DIST; z += DISTincrement){
                checkAddObject(QVector3D(newX, y, z));
            }
        }
    }

    int oldY, newY;
    if(cameraPosition.y() < position.y())
        oldY = position.y()-RENDER_DIST, newY = cameraPosition.y()-RENDER_DIST;
    else
        oldY = position.y()+RENDER_DIST, newY = cameraPosition.y()+RENDER_DIST;
    if(oldY != newY && newY % DISTincrement == 0) {
        for(int x = cameraPosition.x()-RENDER_DIST; x <= cameraPosition.x()+RENDER_DIST; x += DISTincrement) {
            for(int z = cameraPosition.z()-RENDER_DIST; z <= cameraPosition.z()+RENDER_DIST; z += DISTincrement){
                checkAddObject(QVector3D(x, newY, z));
            }
        }
    }

    int oldZ, newZ;
    if(cameraPosition.z() < position.z())
        oldZ = position.z()-RENDER_DIST, newZ = cameraPosition.z()-RENDER_DIST;
    else
        oldZ = position.z()+RENDER_DIST, newZ = cameraPosition.z()+RENDER_DIST;
    if(oldZ != newZ && newZ % DISTincrement == 0) {
        for(int x = cameraPosition.x()-RENDER_DIST; x <= cameraPosition.x()+RENDER_DIST; x += DISTincrement) {
            for(int y = cameraPosition.y()-RENDER_DIST; y <= cameraPosition.y()+RENDER_DIST; y += DISTincrement){
                checkAddObject(QVector3D(x, y, newZ));
            }
        }
    }

    position = cameraPosition;

    for(Planet* planet: planets) {
        planet->update(cameraPosition);
    }

    for(AsteroidCluster* asteroidCluster: asteroidClusters) {
        asteroidCluster->update();
    }
}


void Space::render(Shader *asteroidShader, Frustum *frustum) {
    asteroidModel->setupShader(asteroidShader);
    GL::funcs.glBindVertexArray(asteroidModel->getVAO());

    int amount = 0;
    for(AsteroidCluster* asteroidCluster: asteroidClusters) {
        amount += asteroidCluster->getNumberOfAsteroids();
    }
    models.resize(16 * amount);

    int cnt = 0;
    for(AsteroidCluster* asteroidCluster: asteroidClusters) {
        QVector<QMatrix4x4> rotationMatrices = asteroidCluster->getRotationMatrices();
        for(QMatrix4x4 rotMatrix: rotationMatrices) {
            memcpy(models.data() + cnt * 16, rotMatrix.data(), 16 * sizeof(float));
            cnt++;
        }
    }
//    for(int i = 0; i < amount; i++){
//        QMatrix4x4 tmp;
//        tmp.translate(rand()%100 - 50, rand()%100 - 50, rand()%100 + 5);
//        //        tmp.translate(0,0,100);
//        tmp.scale(0.01);
//        memcpy(models + i * 16, tmp.data(), 16 * sizeof(float));
//    }

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //GL::funcs.glBufferData(GL_ARRAY_BUFFER, 16 * amount * sizeof(float), &models[0], GL_STATIC_DRAW);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, 16 * amount * sizeof(float), models.constData(), GL_STATIC_DRAW);
    GL::funcs.glDrawArraysInstanced(GL_TRIANGLES, 0, asteroidModel->getSize(), amount);

    for(Planet* planet: planets) {
        planet->render(frustum);
    }
}

bool Space::checkCollision(QVector3D cameraPosition) {
    for(Planet* planet: planets) {
        if(cameraPosition.distanceToPoint(planet->getPosition()) <= 2*planet->getRadius()) {
            if(planet->checkCollision(cameraPosition))
                return true;
        }
    }

    for(AsteroidCluster* asteroidCluster: asteroidClusters) {
        if(asteroidCluster->checkCollision(cameraPosition))
            return true;
    }

    return false;
}

bool Space::collisionAheadPlanet(QVector3D position, QVector3D direction, float distance) {
    direction.normalize();
    for(Planet* planet: planets) {
        if(planet->collisionAhead(position, direction, distance))
            return true;
    }
    return false;
}

bool Space::collisionAheadAsteroid(QVector3D position, QVector3D direction, float distance) {
    direction.normalize();
    for(AsteroidCluster* asteroidCluster: asteroidClusters) {
        if(asteroidCluster->collisionAhead(position, direction, distance))
            return true;
    }
    return false;
}

QVector3D Space::getCollisionPointPlanet(QVector3D position, QVector3D direction, float distance) {
    direction.normalize();
    QVector3D result(1e9, 1e9, 1e9);
    for(Planet* planet: planets) {
        QVector3D candidate = planet->getCollisionPoint(position, direction, distance);
        if(position.distanceToPoint(candidate) < position.distanceToPoint(result))
            result = candidate;
    }
    return result;
}

QVector3D Space::getCollisionPointAsteroid(QVector3D position, QVector3D direction, float distance) {
    direction.normalize();
    QVector3D result(1e9, 1e9, 1e9);
    for(AsteroidCluster* asteroidCluster: asteroidClusters) {
        QVector3D candidate = asteroidCluster->getCollisionPoint(position, direction, distance);
        if(position.distanceToPoint(candidate) < position.distanceToPoint(result))
            result = candidate;
    }
    return result;
}

void Space::deleteAsteroid(QVector3D asteroidPosition) {
    for(AsteroidCluster* asteroidCluster: asteroidClusters) {
        asteroidCluster->deleteAsteroid(asteroidPosition);
    }
}

