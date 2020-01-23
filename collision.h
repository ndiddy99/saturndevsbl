#ifndef COLLISION_H
#define COLLISION_H
#include "scroll.h"
#include "sprite.h"

#define COLLISION_UP (1 << 0)
#define COLLISION_DOWN (1 << 1)
#define COLLISION_LEFT (1 << 2)
#define COLLISION_RIGHT (1 << 3)

void collision_check(SPRITE_INFO *sprite);
inline int collision_check_up(SPRITE_INFO *sprite);
inline int collision_check_down(SPRITE_INFO *sprite);
inline int collision_check_below(SPRITE_INFO *sprite);
inline int collision_check_left(SPRITE_INFO *sprite);
inline int collision_check_right(SPRITE_INFO *sprite);
//returns 1 if the sprite's head or foot hits a spike, 0 otherwise
int collision_spikes(SPRITE_INFO *sprite);
//ejects the sprite vertically
void collision_eject_vert(SPRITE_INFO *sprite);
//ejects the sprite horizontally
void collision_eject_horiz(SPRITE_INFO *sprite);
//returns 1 if the sprite is touching a player, zero otherwise
int collision_player(SPRITE_INFO *sprite);
#endif
