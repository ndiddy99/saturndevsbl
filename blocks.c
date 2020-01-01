#include <SEGA_MTH.H>

Uint8 tile2_slope[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16};
Uint8 tile3_slope[] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};
Uint8 tile4_slope[] = {8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15};
Uint8 *block_slopes[] = {NULL, NULL, tile2_slope, tile3_slope, tile4_slope};
