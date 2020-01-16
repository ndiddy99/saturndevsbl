#include <SEGA_MTH.H>
#include "blocks.h"
#include "scroll.h"

Uint8 tile2_slope[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16};
Uint8 tile3_slope[] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};
Uint8 tile4_slope[] = {8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15};
Uint8 *block_slopes[] = {NULL, NULL, tile2_slope, tile3_slope, tile4_slope, NULL, NULL, NULL, NULL, tile2_slope, tile3_slope, tile4_slope};

Uint8 block_get(int tile, int offset) {
    Uint8 *slope_ptr = block_slopes[(tile & 0x1ff) >> 1];
    if (((tile & 0x1ff) >> 1) >= sizeof(block_slopes)/sizeof(Uint8*) || slope_ptr == NULL) {
        return 0;
    }
    if (tile & SCROLL_HMIRROR) {
        return slope_ptr[15 - offset];
    }
    else {
        return slope_ptr[offset];
    }
}

inline int block_check(int tile) {
    if (((tile & 0x1ff) >> 1) < sizeof(block_slopes)/sizeof(Uint8*) && block_slopes[(tile & 0x1ff) >> 1] != NULL) {
        return 1;
    }
    return 0;
}

inline int block_spike(Fixed32 x, Fixed32 y) {
    if ((scroll_get(SCROLL_PLAYFIELD, x >> 20, y >> 20) & 0x1ff) >> 1 == BLOCK_SPIKE) {
        return 1;
    }
    return 0;
}
