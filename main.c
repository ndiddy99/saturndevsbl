#include	<string.h> //memcpy
#define		_SPR2_
#include	<sega_spr.h>
#include	<sega_scl.h> 
#include	<sega_mth.h>

#include    "cd.h"
#include	"graphicrefs.h"
#include	"sprite.h"
#include	"scroll.h"
#include    "player.h"
#include    "print.h"

Uint32 frame = 0;

int main() {
	cd_init();
	sprite_init();
	LEVEL level1;
	level1.player_startx = MTH_FIXED(64);
	level1.player_starty = MTH_FIXED(176);
	level1.playfield_tile_filename = bg_name;
	level1.playfield_tile_num = bg_num;
	level1.playfield_palette = bg_pal;
	level1.playfield_map_filename = map_name;
	level1.playfield_map_width = map_width;
	level1.playfield_map_height = map_height;
	scroll_init(&level1);
	print_init();
	player_init(&level1);
	SCL_SetSpriteMode(SCL_TYPE5,SCL_MIX,SCL_SP_WINDOW);
	while(1) {
		frame++;
		player_input();
		print_num(frame, 0, 0);
		

		SPR_2OpenCommand(SPR_2DRAW_PRTY_OFF);
			player_draw();
		// 	sprite_draw_all();
			print_display();
		SPR_2CloseCommand();
		
		SCL_DisplayFrame();
	}
	return 0;
}
