#include <sega_mth.h>

#include "circle.h"
#include "sprite.h"
#include "player.h"

#define CIRCLE_CHARNUM (46)
#define CIRCLE_SPEED MTH_FIXED(0.5)

void circle_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *circle = sprite_next();
    sprite_make(CIRCLE_CHARNUM, x, y, circle);
    circle->speed = CIRCLE_SPEED;
    circle->iterate = &circle_move;
}

void circle_move(SPRITE_INFO *circle) {
    circle->state = 0;
    if (circle->xPos > player.xPos) {
        circle->state |= SPRITE_LEFT;
    }
    else if (circle->xPos < player.xPos) {
        circle->state |= SPRITE_RIGHT;
    }
    if (circle->yPos > player.yPos) {
        circle->state |= SPRITE_UP;
    }
    else if (circle->yPos < player.yPos) {
        circle->state |= SPRITE_DOWN;
    }
    sprite_move(circle, 1);
}
