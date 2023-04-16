#ifndef TERRAINTYPE_H
#define TERRAINTYPE_H

class QVector3D;

class TerrainType
{
public:
    TerrainType(float radius);

    QVector3D cubeToSphere(QVector3D position);
    virtual float terrainHeight(QVector3D position) = 0;
private:
    float radius;
};

#endif // TERRAINTYPE_H
