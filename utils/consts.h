#ifndef CONSTS_H
#define CONSTS_H

#define DEFAULT_RENDER_DISTNACE 180
#define DEFAULT_PLANET_QUALITY 3

constexpr int MAX_LOD_DIFFERENCE = {
    #include "../play/saves/planetquality.dat"
};
constexpr int PATCH_QUADS = (1 << MAX_LOD_DIFFERENCE);
constexpr int PATCH_VERTS = (PATCH_QUADS + 1);

constexpr int NORMAL_MAP_SIZE = (10 * PATCH_QUADS);

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

#define WAVES_AMOUNT 10

#define WATER_CUTOFF 0.9
#define TERRAIN_CUTOFF 0.8

#define COLOR_TEXTURE_RESOLUTION 512

#define MAX_UPDATES_PER_FRAME 400

#define LASER_WIDTH 0.02
#define LASER_LENGTH 0.5

constexpr int RENDER_DIST = {
    #include "../play/saves/renderdistance.dat"
};
constexpr int DISTincrement = 50;
constexpr int hashRange = 1e9;

int combine_dirs(int x, int cw);
int opposite_dir(int dir);

#endif // CONSTS_H
