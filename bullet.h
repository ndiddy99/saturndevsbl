#ifndef BULLET_H
#define BULLET_H

#include "sprite.h"

//adds a bullet to the screen
void bullet_make(Fixed32 x, Fixed32 y, Uint16 direction);
//moves the bullet (runs every frame from the sprite draw function)
void bullet_move(SPRITE_INFO *bullet);

#endif
