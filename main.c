#include	<string.h> //memcpy
#define		_SPR2_
#include	<sega_spr.h>
#include	<sega_scl.h> 
#include	<sega_mth.h>

#include    "cd.h"
#include	"sprite.h"
#include	"scroll.h"
#include    "player.h"
#include    "print.h"

#include	"graphicrefs.h"

Uint32 frame = 0;

Uint32 buf[2];

int main() {
	cd_init();
	sprite_init();
	// SCROLL_DATA scroll;
	// scroll.playfield_tiles = wood_chr;
	// scroll.playfield_tiles_num = 40;
	// scroll.playfield_palette = wood_pal;
	// scroll.levels = levels;
	// scroll.bg_tiles = bg0_chr;
	// scroll.bg_tiles_num = 44;
	// scroll.bg_palette = bg0_pal;
	// scroll.bg2_tilemap = bg0_2map;
	// scroll.bg3_tilemap = bg0_3map;
	scroll_init();
	print_init();
	player_init();
	SCL_SetSpriteMode(SCL_TYPE5,SCL_MIX,SCL_SP_WINDOW);
	print_string("The quick brown fox jumps over a lazy", 2, 0);
	print_string("dog.", 3, 0);
	while(1) {
		frame++;
		player_input();
		print_num(frame, 0, 0);
		// scroll_move(2, FIXED(-1.2), FIXED(-1.2));
		// scroll_move(3, FIXED(-0.8), FIXED(-0.8));

		SPR_2OpenCommand(SPR_2DRAW_PRTY_OFF);
			player_draw();
		// 	sprite_draw_all();
			print_display();
		SPR_2CloseCommand();
		
		SCL_DisplayFrame();
	}
	return 0;
}
