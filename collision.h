#ifndef COLLISION_H
#define COLLISION_H
#include "sprite.h"

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
