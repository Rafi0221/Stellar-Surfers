
#include "space.h"

#include <QVector3D>
#include <cmath>
#include <random>
#include <QDebug>
#include <functional>

const int DIST = 80;
const int DISTincrement = 5;
static int hashRange = 1e9;

// for QHash takenCoords
inline uint qHash(const QVector3D &v, uint seed)
{
    #define ROTL10(x) (((x) << 10) | (((x) >> 22) & 0x000000ff))
    #define ROTL20(x) (((x) << 20) | (((x) >> 12) & 0x0000ffff))
    return qHash(v.x()) ^ ROTL10(qHash(v.y())) ^ ROTL20(qHash(v.z()));
}


Space::Space(int seed)
{
    this->seed = seed;
}

int Space::hashCoordinates(const QVector3D & coordinates) {
    static int hashBase = 1e8+7;
    long long hash = seed;
    int hashX = std::hash<int>{}(coordinates.x())^seed;
    int hashY = std::hash<int>{}(coordinates.y())^seed;
    int hashZ = std::hash<int>{}(coordinates.z())^seed;
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

void Space::checkAddPlanet(const QVector3D & coordinates) {
    const int hashCutoff = 4000000;
    if(hashCoordinates(coordinates) < hashCutoff && !takenCoords[coordinates]) {
        qDebug() << hashCoordinates(coordinates) << coordinates;
        Planet* planet = new Planet(3.0f, hashCoordinates(coordinates));
        planet->setPosition(coordinates);
        planets.push_back(planet);
        takenCoords[coordinates] = true;
        qDebug() << "adding planet, number of planets" << planets.size();
    }
}


void Space::initialize() {
    for(int x = -DIST; x <= DIST; x += DISTincrement) {
        for(int y = -DIST; y <= DIST; y += DISTincrement) {
            for(int z = -DIST; z <= DIST; z += DISTincrement) {
                checkAddPlanet(QVector3D(x, y, z));
            }
        }
    }
    qDebug() << "number of planets" << planets.size();

    position = QVector3D(0, 0, 0);
}

bool Space::isInRange(const QVector3D & pos, const QVector3D & coordinates) {
    return  pos.x()-DIST <= coordinates.x() && coordinates.x() <= pos.x()+DIST
            && pos.y()-DIST <= coordinates.y() && coordinates.y() <= pos.y()+DIST
            && pos.z()-DIST <= coordinates.z() && coordinates.z() <= pos.z()+DIST;
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

    int oldX, newX;
    if(cameraPosition.x() < position.x())
        oldX = position.x()-DIST, newX = cameraPosition.x()-DIST;
    else
        oldX = position.x()+DIST, newX = cameraPosition.x()+DIST;
    if(oldX != newX && newX % DISTincrement == 0) {
        for(int y = cameraPosition.y()-DIST; y <= cameraPosition.y()+DIST; y += DISTincrement) {
            for(int z = cameraPosition.z()-DIST; z <= cameraPosition.z()+DIST; z += DISTincrement){
                checkAddPlanet(QVector3D(newX, y, z));
            }
        }
    }

    int oldY, newY;
    if(cameraPosition.y() < position.y())
        oldY = position.y()-DIST, newY = cameraPosition.y()-DIST;
    else
        oldY = position.y()+DIST, newY = cameraPosition.y()+DIST;
    if(oldY != newY && newY % DISTincrement == 0) {
        for(int x = cameraPosition.x()-DIST; x <= cameraPosition.x()+DIST; x += DISTincrement) {
            for(int z = cameraPosition.z()-DIST; z <= cameraPosition.z()+DIST; z += DISTincrement){
                checkAddPlanet(QVector3D(x, newY, z));
            }
        }
    }

    int oldZ, newZ;
    if(cameraPosition.z() < position.z())
        oldZ = position.z()-DIST, newZ = cameraPosition.z()-DIST;
    else
        oldZ = position.z()+DIST, newZ = cameraPosition.z()+DIST;
    if(oldZ != newZ && newZ % DISTincrement == 0) {
        for(int x = cameraPosition.x()-DIST; x <= cameraPosition.x()+DIST; x += DISTincrement) {
            for(int y = cameraPosition.y()-DIST; y <= cameraPosition.y()+DIST; y += DISTincrement){
                checkAddPlanet(QVector3D(x, y, newZ));
            }
        }
    }

    position = cameraPosition;

    for(Planet* planet: planets) {
        planet->update(cameraPosition);
    }
}


void Space::render() {
    for(Planet* planet: planets) {
        planet->render();
    }
}

