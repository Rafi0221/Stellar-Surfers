#include "patchindices.h"

#include <algorithm>

#include <QDebug>
#include <qopenglfunctions_3_3_core.h>

#include "../opengl/gl.h"

PatchIndices* PatchIndices::topologies[MAX_LOD_DIFFERENCE+1][MAX_LOD_DIFFERENCE+1][MAX_LOD_DIFFERENCE+1][MAX_LOD_DIFFERENCE+1];

namespace {
    unsigned int toID(int x, int y){
        return x * PATCH_VERTS + y;
    }

    void mapCoordinatesAndPush(int x, int y, int side, int &counter, unsigned int *indices){
        switch(side){
            case NORTH:
                break;
            case EAST:
                std::swap(x,y);
                x = PATCH_VERTS - 1 - x;
                break;
            case SOUTH:
                x = PATCH_VERTS - 1 - x;
                y = PATCH_VERTS - 1 - y;
                break;
            case WEST:
                std::swap(x,y);
                y = PATCH_VERTS - 1 - y;
                break;
        }
        indices[counter++] = toID(x,y);
    }
}

void buildEdge(int diff, int side, int &counter, unsigned int *indices){
    int skip = (1 << diff);
    for(int x = 1; x < PATCH_VERTS - 2; x++){
        int c1 = x - x%skip;
        int c2 = c1 + skip;
        int v;
        if(c2 - x <= x - c1){
            v = c2;
        }else{
            v = c1;
        }
        mapCoordinatesAndPush(x,1,side,counter,indices);
        mapCoordinatesAndPush(x+1,1,side,counter,indices);
        mapCoordinatesAndPush(v,0,side,counter,indices);
    }
    for(int x = 0; x < PATCH_VERTS - 1; x+=skip){
        if(diff == 0){
            int v = (x + (skip+1)/2);
            if(x == PATCH_VERTS - 2){
                v--;
            }
            mapCoordinatesAndPush(x,0,side,counter,indices);
            mapCoordinatesAndPush(v,1,side,counter,indices);
            mapCoordinatesAndPush(x+skip,0,side,counter,indices);
        }else{
            int v = (x + skip/2);
            mapCoordinatesAndPush(x,0,side,counter,indices);
            mapCoordinatesAndPush(v,1,side,counter,indices);
            mapCoordinatesAndPush(x+skip,0,side,counter,indices);
        }
    }
}

PatchIndices::PatchIndices(int dNorth, int dEast, int dSouth, int dWest)
{
    unsigned int indicesCount = 2 * (PATCH_VERTS - 3) * (PATCH_VERTS - 3);
    indicesCount += 4*(PATCH_VERTS-3);
    
    indicesCount += (PATCH_VERTS - 1) / (1 << dNorth);
    indicesCount += (PATCH_VERTS - 1) / (1 << dEast);
    indicesCount += (PATCH_VERTS - 1) / (1 << dSouth);
    indicesCount += (PATCH_VERTS - 1) / (1 << dWest);
    
    indicesCount *= 3;
    unsigned int *indices = new unsigned int[indicesCount];
    
    int counter = 0;
    for(int x = 1; x < PATCH_VERTS - 2; x++){
        for(int y = 1; y < PATCH_VERTS - 2; y++){
            indices[counter++] = toID(x,y);
            indices[counter++] = toID(x,y+1);
            indices[counter++] = toID(x+1,y+1);

            indices[counter++] = toID(x,y);
            indices[counter++] = toID(x+1,y+1);
            indices[counter++] = toID(x+1,y);
        }
    }
    buildEdge(dNorth,NORTH,counter,indices);
    buildEdge(dEast,EAST,counter,indices);
    buildEdge(dSouth,SOUTH,counter,indices);
    buildEdge(dWest,WEST,counter,indices);

    GL::funcs.glGenBuffers(1,&EBO);
    GL::funcs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    GL::funcs.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * counter, indices, GL_STATIC_DRAW);
    GL::funcs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    this->size = counter;

    delete[] indices;
}

void PatchIndices::initialize(){
    for(int dNorth = 0; dNorth <= MAX_LOD_DIFFERENCE; dNorth++){
        for(int dEast = 0; dEast <= MAX_LOD_DIFFERENCE; dEast++){
            for(int dSouth = 0; dSouth <= MAX_LOD_DIFFERENCE; dSouth++){
                for(int dWest = 0; dWest <= MAX_LOD_DIFFERENCE; dWest++){
                    topologies[dNorth][dEast][dSouth][dWest] = new PatchIndices(dNorth, dEast, dSouth, dWest);
                }
            }
        }
    }
}

PatchIndices* PatchIndices::getIndices(int dNorth, int dEast, int dSouth, int dWest){
    return topologies[dNorth][dEast][dSouth][dWest];
}

unsigned int PatchIndices::getEBO(){
    return EBO;
}

unsigned int PatchIndices::getSize(){
    return size;
}
