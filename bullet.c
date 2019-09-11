#include <sega_mth.h>

#include "bullet.h"
#include "collision.h"
#include "scroll.h"
#include "sprite.h"

#define BULLET_CHARNUM 19
#define BULLET_SPEED MTH_FIXED(5)

void bullet_make(SPRITE_INFO *parent) {
    SPRITE_INFO *bullet = sprite_next();
    sprite_make(BULLET_CHARNUM, parent->xPos + MTH_Div(parent->xSize, MTH_FIXED(2)),
                                parent->yPos + MTH_Div(parent->ySize, MTH_FIXED(2)), bullet);
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
    Fixed32 x_diff = bullet->xPos - scrolls_x[0];
    Fixed32 y_diff = bullet->yPos - scrolls_y[0];
    //delete bullet if it's offscreen or has hit a wall
    if (x_diff < MTH_FIXED(0) || x_diff > MTH_FIXED(SCREEN_WIDTH)  ||
        y_diff < MTH_FIXED(0) || y_diff > MTH_FIXED(SCREEN_HEIGHT) ||
        !WALKABLE(get_tile(bullet->xPos, bullet->yPos))) {
            sprite_delete(bullet);
    }
}
