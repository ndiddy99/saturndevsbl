#include <sega_mth.h>

#include "circle.h"
#include "sprite.h"
#include "player.h"

#define CIRCLE_CHARNUM (20)
#define CIRCLE_SPEED MTH_FIXED(2)

void circle_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *circle = sprite_next();
    sprite_make(CIRCLE_CHARNUM, x, y, circle);
    circle->iterate = &circle_move;
}

void circle_move(SPRITE_INFO *circle) {
    if (circle->xPos > player.xPos) {
        circle->state 
    }
    else if (circle->xPos < player.xPos) {

    }
    if (circle->yPos > player.yPos) {

    }
    else if (circle->yPos < player.yPos) {

    }
}