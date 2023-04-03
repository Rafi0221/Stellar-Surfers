#ifndef TERRAINTYPE_H
#define TERRAINTYPE_H

class QVector3D;

class TerrainType
{
public:
    TerrainType();

    QVector3D CubeToSphere(QVector3D position);
    virtual float TerrainHeight(QVector3D position) = 0;
};

#endif // TERRAINTYPE_H
