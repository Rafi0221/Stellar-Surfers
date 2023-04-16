#include "sphericalterrain.h"

#include <QVector3D>

SphericalTerrain::SphericalTerrain(float radius) : TerrainType(radius)
{

}


float SphericalTerrain::terrainHeight(QVector3D position){
    return 1.0f;
}
