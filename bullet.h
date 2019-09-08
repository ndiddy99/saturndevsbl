#ifndef BULLET_H
#define BULLET_H

#include "sprite.h"

//adds a bullet to the screen at the parent's location
void bullet_make(SPRITE_INFO *parent);
//moves the bullet (runs every frame from the sprite draw function)
void bullet_move(SPRITE_INFO *bullet);

#endif
