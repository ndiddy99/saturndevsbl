#include <sega_mth.h>

#include "bullet.h"
#include "sprite.h"

#define BULLET_CHARNUM 19
#define BULLET_SPEED MTH_FIXED(5)

void bullet_make(SPRITE_INFO *parent) {
    SPRITE_INFO *bullet = sprite_next();
    sprite_make(BULLET_CHARNUM, parent->xPos, parent->yPos, bullet);
    bullet->state = parent->facing;
    bullet->iterate = &bullet_move;
}

void bullet_move(SPRITE_INFO *bullet) {
    switch(bullet->state) {
        case STATE_UP:
            bullet->yPos -= BULLET_SPEED;
        break;
        case STATE_DOWN:
            bullet->yPos += BULLET_SPEED;
        break;
        case STATE_LEFT:
            bullet->xPos -= BULLET_SPEED;
        break;
        case STATE_RIGHT:
            bullet->xPos += BULLET_SPEED;
        break;
        case STATE_UPLEFT:
            bullet->xPos -= BULLET_SPEED;
            bullet->yPos -= BULLET_SPEED;
        break;
        case STATE_UPRIGHT:
            bullet->xPos += BULLET_SPEED;
            bullet->yPos -= BULLET_SPEED;
        break;
        case STATE_DOWNLEFT:
            bullet->xPos -= BULLET_SPEED;
            bullet->yPos += BULLET_SPEED;
        break;
        case STATE_DOWNRIGHT:
            bullet->xPos += BULLET_SPEED;
            bullet->yPos += BULLET_SPEED;
        break;
        
    }
}
