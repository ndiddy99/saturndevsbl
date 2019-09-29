#include <sega_mth.h>

#include "bullet.h"
#include "collision.h"
#include "scroll.h"
#include "sprite.h"

#define BULLET_CHARNUM (45)
#define BULLET_SPEED MTH_FIXED(5)

void bullet_make(Fixed32 x, Fixed32 y, Uint16 direction) {
    SPRITE_INFO *bullet = sprite_next();
    sprite_make(BULLET_CHARNUM, x, y, bullet);
    bullet->speed = BULLET_SPEED;
    bullet->state = direction;
    bullet->iterate = &bullet_move;
}

void bullet_move(SPRITE_INFO *bullet) {
    sprite_move(bullet, 0);
    Fixed32 x_diff = bullet->xPos - scrolls_x[0];
    Fixed32 y_diff = bullet->yPos - scrolls_y[0];
    //delete bullet if it's offscreen or has hit a wall
    if (x_diff < MTH_FIXED(0) || x_diff > MTH_FIXED(SCREEN_WIDTH)  ||
        y_diff < MTH_FIXED(0) || y_diff > MTH_FIXED(SCREEN_HEIGHT) ||
        !WALKABLE(collision_get_tile(bullet->xPos, bullet->yPos))) {
            sprite_delete(bullet);
    }
    //delete bullet if it hits another sprite
    int i;
    for (i = 0; i < SPRITE_LIST_SIZE; i++) {
        if (i != bullet->index && sprites[i].xSize != NODISP &&
            collision_point_sprite(bullet, &sprites[i])) {
            sprite_delete(bullet);
            sprite_delete(&sprites[i]);
            break;
        }
    }
}
