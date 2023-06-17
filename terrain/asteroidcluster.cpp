
#include "asteroidcluster.h"
#include "../utils/consts.h"

#include <random>

//static const QVector3D spheresCenters[6] = {
//    {-6.3625, 40.662, 51.736},
//    {-27.32, 82.978, -58.953},
//    {29.88, 108.95, 3.7949},
//    {1.4422, 88.771, 27.336},
//    {27.447, 64.488, -12.116},
//    {-28.464, 37.002, -2.9935}
//};

static const int SPHERES_NUM = 9;

static const QVector3D spheresCenters[SPHERES_NUM] = {
    {-8.5356, 42.251, 62.327},
    {18.541, 88.537, 46.017},
    {-23.135, 56.536, 9.2953},
    {35.959, 113.83, 8.4014},
    {-7.301, 88.517, 18.659},
    {-10.507, 88.136, -10.522},
    {-16.736, 97.783, -61.542},
    {-41.649, 75.022, -54.506},
    {9.7718, 69.786, -4.6312}
};

//static const float radii[6] = {38.211, 42.315, 43.334, 39.314, 33.606, 31.038};
static const float radii[SPHERES_NUM] = {45.454, 34.825, 56.476, 50.140, 43.361, 48.868, 55.867, 43.057, 57.667};
const int amount = 80;
const int cluserRadius = 3 * DISTincrement;

AsteroidCluster::AsteroidCluster(int seed, QVector3D coordinates) {
    this->seed = seed;
    this->position = coordinates;

    std::mt19937 gen(seed);

    for(int i = 0; i < amount - numOfDeleted; i++) {
        QMatrix4x4 matrix;

        matrix.translate(position.x() + gen()%cluserRadius - cluserRadius/2,
                         position.y() + gen()%cluserRadius - cluserRadius/2,
                         position.z() + gen()%cluserRadius - cluserRadius/2);
        float scale = (gen()%100+100.0)/10000;
        //float scale = 1;
        scales.push_back(scale);
        matrix.scale(scale);
        matrix.rotate(gen()%360, 1, 0, 0);
        matrix.rotate(gen()%360, 0, 1, 0);
        matrix.rotate(gen()%360, 0, 0, 1);
        rotationMatrices.push_back(matrix);

        const int rspeed = 1e5;
        speeds.push_back(0.00002f/rspeed);
        movementVectors.push_back(QVector3D(gen()%rspeed - rspeed/2, gen()%rspeed - rspeed/2, gen()%rspeed - rspeed/2));
    }
}


QVector3D AsteroidCluster::getPosition() {
    return position;
}

int AsteroidCluster::getNumberOfAsteroids() {
    return rotationMatrices.size();
}

QVector<QMatrix4x4> AsteroidCluster::getRotationMatrices() {
    return rotationMatrices;
}

bool AsteroidCluster::checkCollision(QVector3D cameraPosition) {
    if(cameraPosition.distanceToPoint(position) > cluserRadius + 1)
        return false;

    for(int i = 0; i < amount - numOfDeleted; i++) {
        for(int j = 0; j < SPHERES_NUM; j++) {
            QVector3D center = rotationMatrices[i].map(spheresCenters[j]);
            float radius = scales[i] * radii[j];

            if(cameraPosition.distanceToPoint(center) <= radius) {
                // qDebug() << center << radius << position << cameraPosition;
                // qDebug() << "COLLISION WITH ASTEROID!";
                return true;
            }
        }
    }

    return false;
}

void AsteroidCluster::update() {
    for(int i = 0; i < amount - numOfDeleted; i++) {
        rotationMatrices[i].translate(movementVectors[i] * speeds[i]);
    }
}

bool AsteroidCluster::collisionAhead(QVector3D position, QVector3D direction, float distance, float width) {
    for(int i = 0; i < amount - numOfDeleted; i++) {
        for(int j = 0; j < SPHERES_NUM; j++) {
            QVector3D center = rotationMatrices[i].map(spheresCenters[j]);
            float radius = scales[i] * radii[j];

            if(center.distanceToLine(position, direction) > radius + width)
                continue;

            float projection = QVector3D::dotProduct(direction, center - position);
            if(projection >= 0 && projection <= distance) {
                return true;
            }
        }
    }
    return false;
}

QVector3D AsteroidCluster::getCollisionPoint(QVector3D position, QVector3D direction, float distance) {
    QVector3D result(1e9, 1e9, 1e9);
    for(int i = 0; i < amount - numOfDeleted; i++) {
        for(int j = 0; j < SPHERES_NUM; j++) {
            QVector3D center = rotationMatrices[i].map(spheresCenters[j]);
            float radius = scales[i] * radii[j];

            if(center.distanceToLine(position, direction) > radius)
                continue;

            float projection = QVector3D::dotProduct(direction, center - position);
            if(projection >= 0 && projection <= distance) {
                QVector3D candidate = direction * projection + position;
                if(position.distanceToPoint(candidate) < position.distanceToPoint(result))
                    result = candidate;
            }
        }
    }
    return result;
}

void AsteroidCluster::deleteAsteroid(QVector3D asteroidPosition) {
    for(int i = 0; i < amount - numOfDeleted; i++) {
        for(int j = 0; j < SPHERES_NUM; j++) {
            QVector3D center = rotationMatrices[i].map(spheresCenters[j]);
            float radius = scales[i] * radii[j];

            if(asteroidPosition.distanceToPoint(center) <= radius) {
                // qDebug() << center << radius << position << asteroidPosition;
                 qDebug() << "DELETING ASTEROID!";
                rotationMatrices.remove(i);
                scales.remove(i);
                movementVectors.remove(i);
                speeds.remove(i);
                numOfDeleted++;
                return;
            }
        }
    }
}
