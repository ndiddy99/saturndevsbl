#ifndef BLOCKS_H
#define BLOCKS_H
#include <SEGA_MTH.H>

extern Uint8 *block_slopes[];

#define BLOCK_SPIKE (5)
//get the height to add for a slope.
//tile- the tile number.
//offset- the player's position within the tile
Uint8 block_get(int tile, int offset);
//checks if a tile is a slope or not.
//tile- the tile number
//returns 1 if it is a slope, 0 if not.
inline int block_check(int tile);
//checks if the block at a given x/y pos is a spike or not.
//returns 1 if yes, 0 if no
inline int block_spike(Fixed32 x, Fixed32 y);
#endif
