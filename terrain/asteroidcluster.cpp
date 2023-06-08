
#include "asteroidcluster.h"
#include "../utils/consts.h"

#include <random>

static const QVector3D spheresCenters[6] = {
    {-6.3625, 51.736, 40.662},
    {-27.32, -58.953, 82.978},
    {29.88, 3.7949, 108.95},
    {1.4422, 27.336, 88.771},
    {27.447, -12.116, 64.488},
    {-28.464, -2.9935, 37.002}
};
static const float radii[6] = {38.211, 42.315, 43.334, 39.314, 33.606, 31.038};
const int amount = 5;
const int cluserRadius = 4 * DISTincrement;

AsteroidCluster::AsteroidCluster(int seed, QVector3D coordinates) {
    this->seed = seed;
    this->position = coordinates;

    std::mt19937 gen(seed);

    for(int i = 0; i < amount; i++) {
        QMatrix4x4 matrix;

        matrix.translate(position.x() + gen()%cluserRadius - cluserRadius/2,
                         position.y() + gen()%cluserRadius - cluserRadius/2,
                         position.z() + gen()%cluserRadius - cluserRadius/2);
        float scale = (gen()%100+100.0)/2000;
        //float scale = 1;
        scales.push_back(scale);
        matrix.scale(scale);
//        matrix.rotate(gen()%360, 1, 0, 0);
//        matrix.rotate(gen()%360, 0, 1, 0);
//        matrix.rotate(gen()%360, 0, 0, 1);
        rotationMatrices.push_back(matrix);
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

    for(int i = 0; i < amount; i++) {
        for(int j = 0; j < 6; j++) {
            QVector3D center = rotationMatrices[i].map(spheresCenters[j]);
            float radius = scales[i] * radii[j];

            if(cameraPosition.distanceToPoint(center) <= radius) {
                qDebug() << center << radius << position << cameraPosition;
                qDebug() << "COLLISION WITH ASTEROID!";
                return true;
            }
        }
    }

    return false;
}
