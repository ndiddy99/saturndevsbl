#include <SEGA_MTH.H>
#include "collision.h"
#include "blocks.h"
#include "print.h"
#include "sprite.h"
#include "scroll.h"

#define TO_TILE(fixed) ((MTH_FixedToInt(fixed)) >> 4)

inline int collision_check_up(SPRITE_INFO *sprite) {
    //top: top left or top right pixel hits something
    if (scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos)) ||
        scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos))) {
        return 1;
    }
    return 0;
}

inline int collision_check_down(SPRITE_INFO *sprite) {
    //bottom: bottom left or bottom right pixel hits something
    if (scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos + (sprite->ySize - MTH_FIXED(1)))) ||
        scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos + (sprite->ySize - MTH_FIXED(1))))) {
        return 1;
    }
    return 0;
}

inline int collision_check_below(SPRITE_INFO *sprite) {
    //bottom: bottom left or bottom right pixel hits something
    if (scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos + (sprite->ySize + MTH_FIXED(1)))) ||
        scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos + (sprite->ySize + MTH_FIXED(1))))) {
        return 1;
    }
    return 0;
}

inline int collision_check_left(SPRITE_INFO *sprite) {
    //left: top left + 8px and bottom left - 8px hits something
    Uint16 top_left, bottom_left;
    top_left = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos + MTH_FIXED(8)));
    bottom_left = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos + (sprite->ySize - MTH_FIXED(1)) - MTH_FIXED(8)));
    if (top_left || bottom_left) {
        // if (block_slopes[bottom_left >> 1] != NULL) {
        //     return 0;
        // }
        return 1;
    }
    return 0;
}

inline int collision_check_right(SPRITE_INFO *sprite) {
    // //right: top right + 8px and bottom right - 8px hits something
    Uint16 top_right, bottom_right;
    top_right = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos + MTH_FIXED(8)));
    bottom_right = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos + (sprite->ySize - MTH_FIXED(1)) - MTH_FIXED(8)));
    if (top_right || bottom_right) {
        // if (block_slopes[bottom_right >> 1] != NULL) {
        //     return 0;
        // }
        return 1;
    }
    return 0;
}

void collision_check(SPRITE_INFO *sprite) {
    Uint16 collision = 0;

    if (collision_check_up(sprite)) {
        collision |= COLLISION_UP;
    }
    if (collision_check_down(sprite)) {
        collision |= COLLISION_DOWN;
    }
    if (collision_check_left(sprite)) {
        collision |= COLLISION_LEFT;
    }
    if (collision_check_right(sprite)) {
        collision |= COLLISION_RIGHT;
    }
    sprite->collision = collision;
}

void collision_eject_vert(SPRITE_INFO *sprite) {
    if (sprite->dy < 0) {
        while (collision_check_up(sprite)) {
            sprite->dy = 0;
            sprite->yPos += MTH_FIXED(1);
        }
    }
    if (sprite->dy > 0) {
        while (collision_check_down(sprite)) {
            sprite->dy = 0;
            sprite->yPos -= MTH_FIXED(1);
        }  
    }
}

void collision_eject_horiz(SPRITE_INFO *sprite) {
    if (sprite->dx < 0) {
        while (collision_check_left(sprite)) {
            sprite->dx = 0;
            // sprite->xPos &= 0xffff0000;
            sprite->xPos += MTH_FIXED(1);
        }
    }
    if (sprite->dx > 0) {
        while (collision_check_right(sprite)) {
            sprite->dx = 0;
            // sprite->xPos &= 0xffff0000;
            sprite->xPos -= MTH_FIXED(1);
        }
    }
}
