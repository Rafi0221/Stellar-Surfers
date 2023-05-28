
#include "asteroidcluster.h"
#include "../utils/consts.h"

#include <random>

AsteroidCluster::AsteroidCluster(int seed, QVector3D coordinates) {
    this->seed = seed;
    this->position = coordinates;

    const int amount = 25;
    const int radius = 4 * DISTincrement;
    std::mt19937 gen(seed);

    for(int i = 0; i < amount; i++) {
        QMatrix4x4 matrix;

        matrix.translate(position.x() + gen()%radius - radius/2,
                         position.y() + gen()%radius - radius/2,
                         position.z() + gen()%radius - radius/2);
        matrix.scale((gen()%100+100.0)/20000);
        matrix.rotate(gen()%360, 1, 0, 0);
        matrix.rotate(gen()%360, 0, 1, 0);
        matrix.rotate(gen()%360, 0, 0, 1);
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
