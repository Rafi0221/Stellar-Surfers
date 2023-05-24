#ifndef CONSTS_H
#define CONSTS_H

#define MAX_LOD_DIFFERENCE (4)
#define PATCH_QUADS ((1 << 4))
#define PATCH_VERTS (PATCH_QUADS + 1)

#define NORMAL_MAP_SIZE 160

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define NORTH_WEST 0
#define NORTH_EAST 1
#define SOUTH_EAST 2
#define SOUTH_WEST 3

#define NEAR_ 0
#define LEFT 1
#define FAR_ 2
#define RIGHT 3
#define UP 4
#define DOWN 5

#define WAVES_AMOUNT 3

#define WATER_CUTOFF 0.0
#define TERRAIN_CUTOFF 0.8

int combine_dirs(int x, int cw);
int opposite_dir(int dir);

#endif // CONSTS_H
