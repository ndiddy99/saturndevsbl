#include <SEGA_MTH.H>
#include "collision.h"
#include "blocks.h"
#include "player.h"
#include "print.h"
#include "sprite.h"
#include "scroll.h"

#define TO_TILE(fixed) ((fixed) >> 20)

inline int collision_check_point(Fixed32 x, Fixed32 y) {
    return scroll_get(SCROLL_PLAYFIELD, TO_TILE(x), TO_TILE(y));
}

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
    Uint16 bottom_left = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos + (sprite->ySize - MTH_FIXED(1))));
    Uint16 bottom_right = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos + (sprite->ySize - MTH_FIXED(1))));
    if (block_check(bottom_left)) {
        bottom_left = 0;
    }
    if (block_check(bottom_right)) {
        bottom_right = 0;
    }    
    if (bottom_left || bottom_right) {
        Uint16 bottom = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + (sprite->xSize >> 1)), TO_TILE(sprite->yPos + sprite->ySize - MTH_FIXED(1)));
        if (block_check(bottom)) {
            return 0;
        }
        return 1;
    }
    return 0;
}

inline int collision_check_below(SPRITE_INFO *sprite) {
    // //bottom: bottom left or bottom right pixel hits something
    // if (scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos + (sprite->ySize + MTH_FIXED(1)))) ||
    //     scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos + (sprite->ySize + MTH_FIXED(1))))) {
    //     return 1;
    // } b
    // return 0; 
    Uint16 bottom_left = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos + (sprite->ySize + MTH_FIXED(1))));
    Uint16 bottom_right = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos + (sprite->ySize + MTH_FIXED(1))));
    if (block_check(bottom_left)) {
        bottom_left = 0;
    }
    if (block_check(bottom_right)) {
        bottom_right = 0;
    }    
    if (bottom_left || bottom_right) {
        Uint16 bottom = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + (sprite->xSize >> 1)), TO_TILE(sprite->yPos + sprite->ySize - MTH_FIXED(1)));
        if (block_check(bottom)) {
            return 0;
        }
        return 1;
    }
    return 0;    
}

inline int collision_check_left(SPRITE_INFO *sprite) {
    //left: top left + 8px and bottom left - 8px hits something
    Uint16 top_left, bottom_left;
    top_left = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos + MTH_FIXED(4)));
    //if we're on a slope, don't check bottom left to avoid colliding with the solid wall next to it
    if (sprite->options & OPTION_SLOPE) {
        bottom_left = 0;
    }
    else {
        bottom_left = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos), TO_TILE(sprite->yPos + (sprite->ySize - MTH_FIXED(1)) - MTH_FIXED(8)));
    }
    if (top_left || bottom_left) {
        if (block_check(bottom_left)) {
            return 0;
        }
        return 1;
    }
    return 0;
}

inline int collision_check_right(SPRITE_INFO *sprite) {
    // //right: top right + 8px and bottom right - 8px hits something
    Uint16 top_right, bottom_right;
    
    top_right = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos + MTH_FIXED(4)));
    bottom_right = 0;
    if (sprite->options & OPTION_SLOPE) {
        bottom_right = 0;
    }
    else {
        bottom_right = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + sprite->xSize - MTH_FIXED(1)), TO_TILE(sprite->yPos + (sprite->ySize - MTH_FIXED(1)) - MTH_FIXED(8)));
    }
    if (top_right || bottom_right) {
        if (block_check(bottom_right)) {
            return 0;
        }
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

int collision_spikes(SPRITE_INFO *sprite) {
    if (sprite->dy > 0) { //if sprite is falling
        if (block_spike(sprite->xPos, sprite->yPos + sprite->ySize) || //bottom left
            block_spike(sprite->xPos + sprite->xSize - MTH_FIXED(1), sprite->yPos + sprite->ySize)) { //bottom right
            return 1;
        }
    }
    else if (sprite->dy < 0) { //if sprite is rising
        if (block_spike(sprite->xPos, sprite->yPos) || //top left
            block_spike(sprite->xPos + sprite->xSize - MTH_FIXED(1), sprite->yPos)) { //top right
            return 1;
        }
    }
    // print_string("nospi", 8, 0);
    return 0;
}

void collision_eject_vert(SPRITE_INFO *sprite) {
    //reset slope bit
    sprite->options &= ~OPTION_SLOPE;

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
        Uint16 bottom = scroll_get(SCROLL_PLAYFIELD, TO_TILE(sprite->xPos + (sprite->xSize >> 1)), TO_TILE(sprite->yPos + sprite->ySize - MTH_FIXED(1)));
        if (block_check(bottom)) {
            //if we're on a tile boundary, don't need to do anything to the position.
            //if we're not (have been moved vertically by the slope tile), set the position to the
            //bottom of the tile before modifying it with the heightmap
            if (sprite->yPos & 0xf0000) {
                sprite->yPos &= 0xfff00000;
                sprite->yPos += MTH_FIXED(16);
            }
            int block_index = ((sprite->xPos + (sprite->xSize >> 1)) >> 16) & 0xf;
            sprite->yPos -= block_get(bottom, block_index) << 16;//block_arr[block_index] << 16;
            sprite->options |= OPTION_SLOPE;
        }
    }
}

void collision_eject_horiz(SPRITE_INFO *sprite) {
    if (sprite->dx < 0) {
        while (collision_check_left(sprite)) {
            sprite->dx = 0;
            sprite->xPos &= 0xffff0000;
            sprite->xPos += MTH_FIXED(1);
        }
    }
    if (sprite->dx > 0) {
        while (collision_check_right(sprite)) {
            sprite->dx = 0;
            sprite->xPos &= 0xffff0000;
            sprite->xPos -= MTH_FIXED(1);
        }
    }
}

int collision_player(SPRITE_INFO *sprite) {
    return ((player.xPos <= sprite->xPos + sprite->xSize) &&
        (player.xPos + player.xSize >= sprite->xPos) &&
        (player.yPos <= sprite->yPos + sprite->ySize) && 
        (player.yPos + player.ySize >= sprite->yPos));
}
