#ifndef COLLISION_H
#define COLLISION_H
#include "sprite.h"

#define PLAYER_SPEED (MTH_FIXED(2))

extern int floor_tiles[];
//if a tile is walkable or not
#define WALKABLE(TILE) (TILE <= 12 && floor_tiles[TILE])


Uint16 get_tile(Fixed32 x, Fixed32 y);
int over_air(SPRITE_INFO *sprite);
//adjust should be 1 if we want to move the sprite up/down on corners
void collision_detect_up(SPRITE_INFO *sprite, int adjust);
void collision_detect_down(SPRITE_INFO *sprite, int adjust);
void collision_detect_left(SPRITE_INFO *sprite, int adjust);
void collision_detect_right(SPRITE_INFO *sprite, int adjust);
inline void collision_detect_up_left(SPRITE_INFO *sprite);
inline void collision_detect_up_right(SPRITE_INFO *sprite);
inline void collision_detect_down_left(SPRITE_INFO *sprite);
inline void collision_detect_down_right(SPRITE_INFO *sprite);

#endif
