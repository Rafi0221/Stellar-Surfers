#include "consts.h"

/* cw - clockwise
 * combine_dirs(SOUTH, 1) = SOUTH_WEST */
int combine_dirs(int x, int cw) {
    return (x + cw) & 3;
}

/* can be used for both N, E, S, W and NE, SE, SW, NW */
int opposite_dir(int dir) {
    return (dir + 2) & 3;
}
