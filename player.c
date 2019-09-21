#include <sega_def.h>
#include <sega_mth.h>
#include <sega_scl.h>
#include "bullet.h"
#include "circle.h"
#include "collision.h"
#include "player.h"
#include "print.h"
#include "scroll.h"
#include "sprite.h"
#include "vblank.h"

#define DIAGONAL_MULTIPLIER (MTH_FIXED(0.8))
#define PLAYER_SPEED (MTH_FIXED(2))
#define PLAYER_TOP (MTH_FIXED(0))
#define PLAYER_SIDE (MTH_FIXED(15))
#define PLAYER_BOTTOM (MTH_FIXED(31))
#define PLAYER_WIDTH (MTH_FIXED(31))

extern Uint32 frame;
//maps to the D-Pad bitmap provided by the Saturn hardware
//what state the character should be set to given the d-pad's input
const Uint16 states[] = {
	                  // RLDU
	SPRITE_NULL,      // 0000
	SPRITE_UP,        // 0001
	SPRITE_DOWN,  	  // 0010
	SPRITE_NULL,      // 0011
	SPRITE_LEFT,      // 0100
	SPRITE_UPLEFT,    // 0101
	SPRITE_DOWNLEFT,  // 0110
	SPRITE_LEFT,      // 0111
	SPRITE_RIGHT,     // 1000 
	SPRITE_UPRIGHT,   // 1001
	SPRITE_DOWNRIGHT, // 1010
	SPRITE_RIGHT,     // 1011 
	SPRITE_NULL,      // 1100
	SPRITE_UP,        // 1101
	SPRITE_DOWN,      // 1110
	SPRITE_NULL       // 1111
};
const Uint16 player_down[] = {11, 10, 12, 10}; //frames for when the player's walking down
const Uint16 player_up[] = {14, 13, 15, 13};
const Uint16 player_side[] = {17, 16, 18, 16};
int anim_cursor = 0;
SPRITE_INFO player;
//if you hold the fire button down, this keeps firing in the direction you were 
//moving initially (so you can fire and move in different directions)
Uint16 bullet_direction;
Uint32 bullet_lastframe = 0;
//how long after firing you have to wait before you can fire again
#define BULLET_DELAY (10) 

void player_init() {
	sprite_make(10, MTH_FIXED(48) + PLAYER_SPRITE_X, MTH_FIXED(16) + PLAYER_SPRITE_Y, &player);
	player.speed = PLAYER_SPEED;
	scroll_set(0, MTH_FIXED(48), MTH_FIXED(16));
	circle_make(player.xPos, player.yPos);
}

void player_input() {
	Uint16 PadData1EW = PadData1E;
	PadData1E = 0;
	//if state changed, reset animation
	if (player.state != states[PadData1 >> 12]) {
		player.state = states[PadData1 >> 12];
		if (player.state != SPRITE_NULL) {
			player.facing = player.state;
		}
		player.animTimer = 0;
	}
	sprite_move(&player, 1);

	if (over_air(&player)) {
		scroll_transition_state = TSTATE_PRESETUP;
	}

	//B button fires in the direction the player is currently facing
	if (PadData1 & PAD_B && frame - bullet_lastframe > BULLET_DELAY) {
		bullet_direction = player.facing;
		bullet_lastframe = frame;
		bullet_make(player.xPos + (player.xSize >> 1),
					player.yPos + (player.ySize >> 1),
					bullet_direction);
	}
	//A button fires in the opposite direction the player is facing (behind)
	else if (PadData1 & PAD_A && frame - bullet_lastframe > BULLET_DELAY) {
		bullet_direction = 0;
		if (player.facing & SPRITE_UP)    { bullet_direction |= SPRITE_DOWN; }
		if (player.facing & SPRITE_DOWN)  { bullet_direction |= SPRITE_UP; }
		if (player.facing & SPRITE_LEFT)  { bullet_direction |= SPRITE_RIGHT; }
		if (player.facing & SPRITE_RIGHT) { bullet_direction |= SPRITE_LEFT; }
		bullet_lastframe = frame;
		bullet_make(player.xPos + (player.xSize >> 1),
				    player.yPos + (player.ySize >> 1),
					bullet_direction);
	}
	//C button keeps fire locked in the same direction after you hold it
	else if (PadData1 & PAD_C && frame - bullet_lastframe > BULLET_DELAY) {
		//if button is pressed, set firing direction to where the player's facing
		if (PadData1EW & PAD_C) {
			bullet_direction = player.facing;
		}
		bullet_lastframe = frame;
		bullet_make(player.xPos + (player.xSize >> 1),
					player.yPos + (player.ySize >> 1),
					bullet_direction);
	}


	player_animate();
	print_num(scrolls_x[0] >> 16, 0, 0); print_num(scrolls_x[0] & 0xffff, 0, 10);
	print_num(scrolls_y[0] >> 16, 1, 0); print_num(scrolls_y[0] & 0xffff, 1, 10);
	print_num(scrolls_x[1] >> 16, 2, 0); print_num(scrolls_x[1] & 0xffff, 2, 10);
	print_num(scrolls_y[1] >> 16, 3, 0); print_num(scrolls_y[1] & 0xffff, 3, 10);
}

void player_animate() {
	if (player.animTimer > 0) player.animTimer--;
	
	if (player.state == SPRITE_LEFT) {
		player.mirror = MIRROR_HORIZ;
	}
	else if (player.state != SPRITE_NULL) {
		player.mirror = 0;
	}
	
	if (player.animTimer == 0) {
		player.animTimer = 10;
		switch (player.state) {
			case SPRITE_UP:
			case SPRITE_UPLEFT:
			case SPRITE_UPRIGHT:
				player.char_num = player_up[anim_cursor];
			break;
			case SPRITE_DOWN:
			case SPRITE_DOWNLEFT:
			case SPRITE_DOWNRIGHT:
				player.char_num = player_down[anim_cursor];
			break;
			case SPRITE_LEFT:
				player.char_num = player_side[anim_cursor];
			break;
			case SPRITE_RIGHT:
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
