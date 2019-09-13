#include <sega_mth.h>

#include "collision.h"
#include "scroll.h"

                  // 0  1  2  3  4  5  6  7  8  9 10 11 12 
int floor_tiles[] = {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1};

//gets tile number for given coordinates
inline Uint16 collision_get_tile(Fixed32 x, Fixed32 y) {
	return scroll_get(0, MTH_FixedToInt(x) >> 4, MTH_FixedToInt(y) >> 4);
}

int over_air(SPRITE_INFO *sprite) {
	return (collision_get_tile(sprite->xPos, sprite->yPos) == 10 &&
			collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos) == 10 &&
			collision_get_tile(sprite->xPos, sprite->yPos + sprite->ySize - 1) == 10 &&
			collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos + sprite->ySize - 1) == 10); 
}

void collision_detect_up(SPRITE_INFO *sprite, int adjust) {
	int walkable_ul = WALKABLE(collision_get_tile(sprite->xPos, sprite->yPos));
	int walkable_ur = WALKABLE(collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos));
	if (!walkable_ul && !walkable_ur) {
		sprite->yPos &= 0xffff0000; //take off the decimal point
		while (!walkable_ul && !walkable_ur) {
			sprite->yPos += MTH_FIXED(1);
			walkable_ul = WALKABLE(collision_get_tile(sprite->xPos, sprite->yPos));
			walkable_ur = WALKABLE(collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos));	
		}
	}
	else if (adjust && !walkable_ul) {
		sprite->xPos += sprite->speed;
		collision_detect_right(sprite, 0);
	}
	else if (adjust && !walkable_ur) {
		sprite->xPos -= sprite->speed;
		collision_detect_left(sprite, 0);
	}
}

void collision_detect_down(SPRITE_INFO *sprite, int adjust) {
	int walkable_ll = WALKABLE(collision_get_tile(sprite->xPos, sprite->yPos + sprite->ySize - 1));
	int walkable_lr = WALKABLE(collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos + sprite->ySize - 1));
	if (!walkable_ll && !walkable_lr) {
		sprite->yPos &= 0xffff0000; //take off the decimal point
		while (!walkable_ll && !walkable_lr) {
			sprite->yPos -= MTH_FIXED(1);
			walkable_ll = WALKABLE(collision_get_tile(sprite->xPos, sprite->yPos + sprite->ySize -1));
			walkable_lr = WALKABLE(collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos + sprite->ySize - 1));			
		}
	}
	else if (adjust && !walkable_ll) {
		sprite->xPos += sprite->speed;
		collision_detect_right(sprite, 0);
	}
	else if (adjust && !walkable_lr) {
		sprite->xPos -= sprite->speed;
		collision_detect_left(sprite, 0);
	}
}

void collision_detect_left(SPRITE_INFO *sprite, int adjust) {
	int walkable_ul = WALKABLE(collision_get_tile(sprite->xPos, sprite->yPos));
	int walkable_ll = WALKABLE(collision_get_tile(sprite->xPos, sprite->yPos + sprite->ySize - 1));
	if (!walkable_ul && !walkable_ll) {
		sprite->xPos &= 0xffff0000; //take off the decimal point
		while (!walkable_ul && !walkable_ll) {
			sprite->xPos += MTH_FIXED(1);
			walkable_ul = WALKABLE(collision_get_tile(sprite->xPos, sprite->yPos));
			walkable_ll = WALKABLE(collision_get_tile(sprite->xPos, sprite->yPos + sprite->ySize - 1));		
		}
	}
	else if (adjust && !walkable_ul) {
		sprite->yPos += sprite->speed;
		collision_detect_down(sprite, 0);
	}
	else if (adjust && !walkable_ll) {
		sprite->yPos -= sprite->speed;
		collision_detect_up(sprite, 0);
	}
}

void collision_detect_right(SPRITE_INFO *sprite, int adjust) {
	int walkable_ur = WALKABLE(collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos));
	int walkable_lr = WALKABLE(collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos + sprite->ySize - 1));
	if (!walkable_ur && !walkable_lr) {
		sprite->xPos &= 0xffff0000; //take off the decimal point
		while (!walkable_ur && !walkable_lr) {
			sprite->xPos -= MTH_FIXED(1);
			walkable_ur = WALKABLE(collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos));
			walkable_lr = WALKABLE(collision_get_tile(sprite->xPos + sprite->xSize - 1, sprite->yPos + sprite->ySize - 1));
		}
	}
	else if (adjust && !walkable_ur) {
		sprite->yPos += sprite->speed;
		collision_detect_down(sprite, 0);
	}
	else if (adjust && !walkable_lr) {
		sprite->yPos -= sprite->speed;
		collision_detect_up(sprite, 0);
	}
}

inline void collision_detect_up_left(SPRITE_INFO *sprite) {
	collision_detect_up(sprite, 0);
	collision_detect_left(sprite, 1);
}

inline void collision_detect_up_right(SPRITE_INFO *sprite) {
	collision_detect_up(sprite, 0);
	collision_detect_right(sprite, 1);
}

inline void collision_detect_down_left(SPRITE_INFO *sprite) {
	collision_detect_down(sprite, 0);
	collision_detect_left(sprite, 1);
}

inline void collision_detect_down_right(SPRITE_INFO *sprite) {
	collision_detect_down(sprite, 0);
	collision_detect_right(sprite, 1);
}

