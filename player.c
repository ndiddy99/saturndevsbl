#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "collision.h"
#include "player.h"
#include "print.h"
#include "scroll.h"
#include "sprite.h"
#include "vblank.h"

#define PLAYER_SPEED (MTH_FIXED(2))
//normalize diagonal speed
#define DIAGONAL_MULTIPLIER (MTH_FIXED(0.8))
#define PLAYER_TOP (MTH_FIXED(0))
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

void player_init() {
	sprite_make(10, MTH_FIXED(48) + PLAYER_SPRITE_X, MTH_FIXED(16) + PLAYER_SPRITE_Y, &player);
	scroll_set(0, MTH_FIXED(48), MTH_FIXED(16));
}

void player_input() {
	Uint16 PadData1EW = PadData1E;
	PadData1E = 0;
	//if state changed, reset animation
	if (player.state != states[PadData1 >> 12]) {
		player.state = states[PadData1 >> 12];
		player.animTimer = 0;
	}
	switch (player.state) {
		case STATE_UP:
			player.yPos -= PLAYER_SPEED;
			collision_detect_up(&player, 1);
		break;
		case STATE_DOWN:
			player.yPos += PLAYER_SPEED;
			collision_detect_down(&player, 1);
		break;
		case STATE_LEFT:
			player.xPos -= PLAYER_SPEED;
			collision_detect_left(&player, 1);
		break;
		case STATE_RIGHT:
			player.xPos += PLAYER_SPEED;
			collision_detect_right(&player, 1);
		break;
		case STATE_UPLEFT:
			player.xPos += MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			player.yPos += MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			collision_detect_up_left(&player);
		break;
		case STATE_UPRIGHT:
			player.xPos += MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			player.yPos += MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			collision_detect_up_right(&player);
		break;
		case STATE_DOWNLEFT:
			player.xPos += MTH_Mul(-PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			player.yPos += MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			collision_detect_down_left(&player);
		break;
		case STATE_DOWNRIGHT:
			player.xPos += MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			player.yPos += MTH_Mul(PLAYER_SPEED, DIAGONAL_MULTIPLIER);
			collision_detect_down_right(&player);
		break;
	}
	if (over_air(&player)) {
		scroll_transition_state = TSTATE_PRESETUP;
	}

	player_animate();
	print_num(scrolls_x[0] >> 16, 0, 0); print_num(scrolls_x[0] & 0xffff, 0, 10);
	print_num(scrolls_y[0] >> 16, 1, 0); print_num(scrolls_y[0] & 0xffff, 1, 10);
	print_num(scrolls_x[1] >> 16, 2, 0); print_num(scrolls_x[1] & 0xffff, 2, 10);
	print_num(scrolls_y[1] >> 16, 3, 0); print_num(scrolls_y[1] & 0xffff, 3, 10);
}

void player_animate() {
	if (player.animTimer > 0) player.animTimer--;
	
	if (player.state == STATE_LEFT) {
		player.mirror = MIRROR_HORIZ;
	}
	else if (player.state != STATE_NULL) {
		player.mirror = 0;
	}
	
	if (player.animTimer == 0) {
		player.animTimer = 10;
		switch (player.state) {
			case STATE_UP:
			case STATE_UPLEFT:
			case STATE_UPRIGHT:
				player.char_num = player_up[anim_cursor];
			break;
			case STATE_DOWN:
			case STATE_DOWNLEFT:
			case STATE_DOWNRIGHT:
				player.char_num = player_down[anim_cursor];
			break;
			case STATE_LEFT:
				player.char_num = player_side[anim_cursor];
			break;
			case STATE_RIGHT:
				player.char_num = player_side[anim_cursor];
			break;
		}
		anim_cursor == 3 ? anim_cursor = 0 : anim_cursor++;
	}
}
//allows me to treat the player sprite like any other sprite while only moving the screen
//around it
void player_draw() {
	Fixed32 player_x = player.xPos;
	Fixed32 player_y = player.yPos;

	player.xPos = PLAYER_SPRITE_X;
	player.yPos = PLAYER_SPRITE_Y;
	sprite_draw(&player);
	if (scroll_transition_state == TSTATE_NULL) { //if we're transitioning between levels, let that routine control the
	                                       //scaling instead of this one
		scroll_set(0, player_x - PLAYER_SPRITE_X, player_y - PLAYER_SPRITE_Y);
		// multiplying it by 3/4
		scroll_set(1, ((player_x - PLAYER_SPRITE_X) * 3) >> 2, ((player_y - PLAYER_SPRITE_Y) * 3) >> 2);
	}
	player.xPos = player_x;
	player.yPos = player_y;
}
