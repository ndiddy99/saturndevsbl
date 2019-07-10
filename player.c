#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "player.h"
#include "print.h"
#include "scroll.h"
#include "vblank.h"

#define PLAYER_SPEED (MTH_FIXED(2))
//square root of 1/2, normalizes diagonal speed
#define DIAGONAL_MULTIPLIER (MTH_FIXED(0.7071))
#define PLAYER_SPRITE_X (MTH_FIXED(144))
#define PLAYER_SPRITE_Y (MTH_FIXED(96))
#define PLAYER_TOP (MTH_FIXED(0))
#define PLAYER_SIDE (MTH_FIXED(15))
#define PLAYER_BOTTOM (MTH_FIXED(31))
#define PLAYER_WIDTH (MTH_FIXED(31))

#define WALKABLE(TILE) (TILE <= 24 && floor_tiles[TILE])

//maps to the D-Pad bitmap provided by the Saturn hardware
//what state the character should be set to given the d-pad's input
const Uint16 states[] = {
	                  // RLDU
	STATE_NULL,       // 0000
	STATE_UP,         // 0001
	STATE_DOWN,  	  // 0010
	STATE_NULL,       // 0011
	STATE_LEFT,       // 0100
	STATE_UPLEFT,     // 0101
	STATE_DOWNLEFT,   // 0110
	STATE_LEFT,       // 0111
	STATE_RIGHT,      // 1000 
	STATE_UPRIGHT,    // 1001
	STATE_DOWNRIGHT,  // 1010
	STATE_RIGHT,      // 1011 
	STATE_NULL,       // 1100
	STATE_UP,         // 1101
	STATE_DOWN,       // 1110
	STATE_NULL        // 1111
};
//which points on the sprite to use for collision
const Uint16 player_down[] = {11, 10, 12, 10}; //frames for when the player's walking down
const Uint16 player_up[] = {14, 13, 15, 13};
const Uint16 player_side[] = {17, 16, 18, 16};
//if a tile is walkable or not
const int floor_tiles[] = {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1};
int anim_cursor = 0;
SPRITE_INFO player;

static inline Uint16 get_tile(Fixed32 x, Fixed32 y);
static void collision_detect_up(Fixed32 *x, Fixed32 *y);
static void collision_detect_down(Fixed32 *x, Fixed32 *y);
static void collision_detect_left(Fixed32 *x, Fixed32 *y);
static void collision_detect_right(Fixed32 *x, Fixed32 *y);

void player_init() {
	make_sprite(0, PLAYER_SPRITE_X, PLAYER_SPRITE_Y, &player);
	player.char_num = 10;
	set_scroll(0, MTH_FIXED(48), MTH_FIXED(16));
}

//gets tile number for given coordinates
static inline Uint16 get_tile(Fixed32 x, Fixed32 y) {
	return get_map_val(0, MTH_FixedToInt(x) >> 4, MTH_FixedToInt(y) >> 4);
}

static void collision_detect_up(Fixed32 *x, Fixed32 *y) {
	int walkable_ul = WALKABLE(get_tile(*x, *y));
	int walkable_ur = WALKABLE(get_tile(*x + PLAYER_WIDTH, *y));
	if (!walkable_ul && !walkable_ur) {
		*y &= 0xffff0000; //take off the decimal point
		while (!walkable_ul && !walkable_ur) {
			*y += MTH_FIXED(1);
			walkable_ul = WALKABLE(get_tile(*x, *y));
			walkable_ur = WALKABLE(get_tile(*x + PLAYER_WIDTH, *y));			
		}
	}
	// else if (!walkable_ul) {
	// 	*x &= 0xffff0000;
	// 	while (!walkable_ul) {
	// 		*x += MTH_FIXED(1);
	// 	}
	// }
	// else if (!walkable_ur) {
	// 	*x &= 0xffff0000;
	// 	while (!walkable_ur) {
	// 		*x -= MTH_FIXED(1);
	// 	}
	// }
}

static void collision_detect_down(Fixed32 *x, Fixed32 *y) {
	int walkable_ll = WALKABLE(get_tile(*x, *y + PLAYER_BOTTOM));
	int walkable_lr = WALKABLE(get_tile(*x + PLAYER_WIDTH, *y + PLAYER_BOTTOM));
	if (!walkable_ll && !walkable_lr) {
		*y &= 0xffff0000; //take off the decimal point
		while (!walkable_ll && !walkable_lr) {
			*y -= MTH_FIXED(1);
			walkable_ll = WALKABLE(get_tile(*x, *y + PLAYER_BOTTOM));
			walkable_lr = WALKABLE(get_tile(*x + PLAYER_WIDTH, *y + PLAYER_BOTTOM));			
		}
	}
	// else if (!walkable_ll) {
	// 	*x &= 0xffff0000;
	// 	while (!walkable_ll) {
	// 		*x += MTH_FIXED(1);
	// 	}
	// }
	// else if (!walkable_lr) {
	// 	*x &= 0xffff0000;
	// 	while (!walkable_lr) {
	// 		*x -= MTH_FIXED(1);
	// 	}
	// }
}

static void collision_detect_left(Fixed32 *x, Fixed32 *y) {
	int walkable_ul = WALKABLE(get_tile(*x, *y));
	int walkable_ll = WALKABLE(get_tile(*x, *y + PLAYER_BOTTOM));
	if (!walkable_ul && !walkable_ll) {
		*x &= 0xffff0000; //take off the decimal point
		while (!walkable_ul && !walkable_ll) {
			*x += MTH_FIXED(1);
			walkable_ul = WALKABLE(get_tile(*x, *y));
			walkable_ll = WALKABLE(get_tile(*x, *y + PLAYER_BOTTOM));			
		}
	}
	// else if (!walkable_ul) {
	// 	*y &= 0xffff0000;
	// 	while (!walkable_ul) {
	// 		*y += MTH_FIXED(1);
	// 	}
	// }
	// else if (!walkable_ll) {
	// 	*y &= 0xffff0000;
	// 	while (!walkable_ll) {
	// 		*y  -= MTH_FIXED(1);
	// 	}
	// }
}

static void collision_detect_right(Fixed32 *x, Fixed32 *y) {
	int walkable_ur = WALKABLE(get_tile(*x + PLAYER_WIDTH, *y));
	int walkable_lr = WALKABLE(get_tile(*x + PLAYER_WIDTH, *y + PLAYER_BOTTOM));
	if (!walkable_ur && !walkable_lr) {
		*x &= 0xffff0000; //take off the decimal point
		while (!walkable_ur && !walkable_lr) {
			*x -= MTH_FIXED(1);
			walkable_ur = WALKABLE(get_tile(*x + PLAYER_WIDTH, *y));
			walkable_lr = WALKABLE(get_tile(*x + PLAYER_WIDTH, *y + PLAYER_BOTTOM));			
		}
	}
	// else if (!walkable_ur) {
	// 	*y &= 0xffff0000;
	// 	while (!walkable_ur) {
	// 		*y += MTH_FIXED(1);
	// 	}
	// }
	// else if (!walkable_lr) {
	// 	*y &= 0xffff0000;
	// 	while (!walkable_lr) {
	// 		*y -= MTH_FIXED(1);
	// 	}
	// }
}

void player_input() {
	Fixed32 player_x = scrolls_x[0] + PLAYER_SPRITE_X;
	Fixed32 player_y = scrolls_y[0] + PLAYER_SPRITE_Y;

	Uint16 PadData1EW = PadData1E;
	PadData1E = 0;
	//if state changed, reset animation
	if (player.state != states[PadData1 >> 12]) {
		player.state = states[PadData1 >> 12];
		player.animTimer = 0;
	}
	switch (player.state) {
		case STATE_UP:
			player_y -= PLAYER_SPEED;
			collision_detect_up(&player_x, &player_y);
		break;
		case STATE_DOWN:
			player_y += PLAYER_SPEED;
			collision_detect_down(&player_x, &player_y);		
		break;
		case STATE_LEFT:
			player_x -= PLAYER_SPEED;
			collision_detect_left(&player_x, &player_y);
		break;
		case STATE_RIGHT:
			player_x += PLAYER_SPEED;
			collision_detect_right(&player_x, &player_y);
		break;
		// //for diagonals, first check x axis, then check y axis
		// case STATE_UPLEFT:
		// 	scrollDX = MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
		// 	scrollDY = MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
		// break;
		// case STATE_UPRIGHT:
		// 	scrollDX = MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
		// 	scrollDY = MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
		// break;
		// case STATE_DOWNLEFT:
		// 	scrollDX = MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
		// 	scrollDY = MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
		// break;
		// case STATE_DOWNRIGHT:
		// 	scrollDX = MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
		// 	scrollDY = MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
		// break;
	}

	

	set_scroll(0, player_x - PLAYER_SPRITE_X, player_y - PLAYER_SPRITE_Y);
	player_animate(&player);
	print_num(scrolls_x[0] >> 16, 0, 0);
	print_num(scrolls_y[0] >> 16, 1, 0);
	print_num(get_tile(player_x, player_y), 2, 0);
}

void player_animate(SPRITE_INFO *player) {
	if (player->animTimer > 0) player->animTimer--;
	
	if (player->state == STATE_LEFT) {
		player->mirror = MIRROR_HORIZ;
	}
	else if (player->state != STATE_NULL) {
		player->mirror = 0;
	}
	
	if (player->animTimer == 0) {
		player->animTimer = 10;
		switch (player->state) {
			case STATE_UP:
			case STATE_UPLEFT:
			case STATE_UPRIGHT:
				player->char_num = player_up[anim_cursor];
			break;
			case STATE_DOWN:
			case STATE_DOWNLEFT:
			case STATE_DOWNRIGHT:
				player->char_num = player_down[anim_cursor];
			break;
			case STATE_LEFT:
				player->char_num = player_side[anim_cursor];
			break;
			case STATE_RIGHT:
				player->char_num = player_side[anim_cursor];
			break;
		}
		anim_cursor == 3 ? anim_cursor = 0 : anim_cursor++;
	}
}
