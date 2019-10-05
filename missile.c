#include <sega_mth.h>

#include "missile.h"
#include "sprite.h"
#include "player.h"

#define MISSILE_CHARNUM (47)
//max # of degrees missile can rotate in a frame
#define MISSILE_ROTATE_SPEED (MTH_FIXED(1.2)) 
#define MISSILE_SPEED (0)

void missile_make(Fixed32 x, Fixed32 y) {
    SPRITE_INFO *missile = sprite_next();
    sprite_make(MISSILE_CHARNUM, x, y, missile);
    missile->speed = MISSILE_SPEED;
    missile->iterate = &missile_move;
}

void missile_move(SPRITE_INFO *missile) {
    Fixed32 x_diff = (player.xPos + (player.xSize >> 1)) - (missile->xPos + (missile->xSize >> 1));
    Fixed32 y_diff = (player.yPos + (player.ySize >> 1)) - (missile->yPos + (missile->ySize >> 1));
    Fixed32 target_angle = MTH_Atan(y_diff, x_diff);
    //handle when angle "wraps around"
    if (missile->angle > MTH_FIXED(90) && target_angle < MTH_FIXED(-90)) {
        missile->angle += MISSILE_ROTATE_SPEED;
    }
    else if (missile->angle < MTH_FIXED(-90) && target_angle > MTH_FIXED(90)) {
        missile->angle -= MISSILE_ROTATE_SPEED;
    }
    else if (target_angle - missile->angle < MISSILE_ROTATE_SPEED && target_angle - missile->angle > -MISSILE_ROTATE_SPEED) {
        missile->angle = target_angle;
    }
    else if (target_angle > missile->angle) {
        missile->angle += MISSILE_ROTATE_SPEED;
    }
    else {
        missile->angle -= MISSILE_ROTATE_SPEED;
    }
    
    if (missile->angle > MTH_FIXED(180)) {
        missile->angle -= MTH_FIXED(360);
    }
    else if (missile->angle < MTH_FIXED(-180)) {
        missile->angle += MTH_FIXED(360);
    }
}

