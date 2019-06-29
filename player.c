#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "player.h"
#include "print.h"
#include "scroll.h"
#include "vblank.h"

#define PLAYER_SPEED (MTH_FIXED(2))
#define DIAGONAL_MULTIPLIER (MTH_FIXED(0.7071)) //square root of 1/2, normalizes diagonal speed
#define PLAYER_SPRITE_X (MTH_FIXED(144))
#define PLAYER_SPRITE_Y (MTH_FIXED(96))
#define PLAYER_TOP (MTH_FIXED(3))
#define PLAYER_SIDE (MTH_FIXED(15))
#define PLAYER_BOTTOM (MTH_FIXED(31))
#define PLAYER_WIDTH (MTH_FIXED(31))

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
int anim_cursor = 0;
SPRITE_INFO player;

static inline Uint16 get_tile(Fixed32 x, Fixed32 y);

void player_init() {
	make_sprite(0, PLAYER_SPRITE_X, PLAYER_SPRITE_Y, &player);
	player.char_num = 10;
	set_scroll(0, MTH_FIXED(48), MTH_FIXED(16));
}

//gets tile number for given sprite coordinates
static inline Uint16 get_tile(Fixed32 x, Fixed32 y) {
	return get_map_val(0, MTH_FixedToInt(x) >> 4, MTH_FixedToInt(y) >> 4);
}

void player_input() {
	Fixed32 scrollDX = 0;
	Fixed32 scrollDY = 0;
	Uint16 PadData1EW = PadData1E;
	PadData1E = 0;
	//if state changed, reset animation
	if (player.state != states[PadData1 >> 12]) {
		player.state = states[PadData1 >> 12];
		player.animTimer = 0;
	}
	Uint16 curr_tile;
	switch (player.state) {
		case STATE_UP:
			scrollDX = 0;
			scrollDY = -PLAYER_SPEED;
			//check top left corner
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X, scrolls_y[0] + PLAYER_SPRITE_Y + scrollDY + PLAYER_TOP);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
			//check top right corner
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + PLAYER_WIDTH, scrolls_y[0] + PLAYER_SPRITE_Y + scrollDY + PLAYER_TOP);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}			
		break;
		case STATE_DOWN:
			scrollDX = 0;
			scrollDY = PLAYER_SPEED;
			//check bottom left corner
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X, scrolls_y[0] + PLAYER_SPRITE_Y + scrollDY + PLAYER_BOTTOM);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
			//check bottom right corner
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + PLAYER_WIDTH, scrolls_y[0] + PLAYER_SPRITE_Y + scrollDY + PLAYER_BOTTOM);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
		break;
		case STATE_LEFT:
			scrollDX = -PLAYER_SPEED;
			scrollDY = 0;
			//check top left corner
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
			//check bottom left corner
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
		break;
		case STATE_RIGHT:
			scrollDX = PLAYER_SPEED;
			scrollDY = 0;
			//top right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + scrollDX + PLAYER_WIDTH, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
			//bottom right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + scrollDX + PLAYER_WIDTH, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
		break;
		//for diagonals, first check x axis, then check y axis
		case STATE_UPLEFT:
			scrollDX = MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			//top left
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
			//bottom left
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}		
			scrollDY = MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			//top left
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP + scrollDY);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
			//top right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + PLAYER_WIDTH, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP + scrollDY);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
		break;
		case STATE_UPRIGHT:
			scrollDX = MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			//top right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + PLAYER_WIDTH + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
			//bottom right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + PLAYER_WIDTH + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
			scrollDY = MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			//top left
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP + scrollDY);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
			//top right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + PLAYER_WIDTH, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP + scrollDY);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
		break;
		case STATE_DOWNLEFT:
			scrollDX = MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			//top left
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
			//bottom left
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}	
			scrollDY = MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			//bottom left
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM + scrollDY);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
			//bottom right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM + scrollDY);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
		break;
		case STATE_DOWNRIGHT:
			scrollDX = MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			//top right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + PLAYER_WIDTH + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_TOP);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
			//bottom right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X + PLAYER_WIDTH + scrollDX, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDX = 0;
			}
			scrollDY = MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			//bottom left
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM + scrollDY);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
			//bottom right
			curr_tile = get_tile(scrolls_x[0] + PLAYER_SPRITE_X, scrolls_y[0] + PLAYER_SPRITE_Y + PLAYER_BOTTOM + scrollDY);
			if (curr_tile != 2 && curr_tile != 4 && curr_tile != 22 && curr_tile != 24) {
				scrollDY = 0;
			}
		break;
	}

	move_scroll(0, scrollDX, scrollDY);
	player_animate(&player);
	print_num(scrolls_x[0] >> 16, 0, 0);
	print_num(scrolls_y[0] >> 16, 1, 0);
	print_num(get_tile(scrolls_x[0] + PLAYER_SPRITE_X, scrolls_y[0] + PLAYER_SPRITE_Y), 2, 0);
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
