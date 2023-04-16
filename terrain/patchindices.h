#ifndef PATCHINDICES_H
#define PATCHINDICES_H

#include "../utils/consts.h"

class PatchIndices
{
private:
    static PatchIndices* topologies[MAX_LOD_DIFFERENCE+1][MAX_LOD_DIFFERENCE+1][MAX_LOD_DIFFERENCE+1][MAX_LOD_DIFFERENCE+1];

    unsigned int EBO;
    unsigned int size;
public:
    PatchIndices(int dNorth, int dEast, int dSouth, int dWest);

    static void initialize();

    static PatchIndices* getIndices(int dNorth, int dEast, int dSouth, int dWest);

    unsigned int getEBO();
    unsigned int getSize();
};

#endif // PATCHINDICES_H
