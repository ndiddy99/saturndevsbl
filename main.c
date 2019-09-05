#include	<string.h> //memcpy
#define		_SPR2_
#include	<sega_spr.h>
#include	<sega_scl.h> 
#include	<sega_mth.h>

#include	"sprite.h"
#include	"scroll.h"
#include    "player.h"
#include    "print.h"

#include	"graphicrefs.h"

Uint16 *levels[] = {map1, map2, map3};

int main() {

	sprite_init();
	SCROLL_DATA scroll;
	scroll.playfield_tiles = wood_chr;
	scroll.playfield_tiles_num = 40;
	scroll.playfield_palette = wood_pal;
	scroll.levels = levels;
	scroll.bg_tiles = bg0_chr;
	scroll.bg_tiles_num = 44;
	scroll.bg_palette = bg0_pal;
	scroll.bg2_tilemap = bg0_2map;
	scroll.bg3_tilemap = bg0_3map;
	scroll_init(&scroll);
	player_init();
	print_init();
	SCL_SetSpriteMode(SCL_TYPE5,SCL_MIX,SCL_SP_WINDOW);
	while(1) {
		if (scroll_transition_state == TSTATE_NULL) {
			player_input();
		}
		else {
			scroll_transition();
		}
		SPR_2OpenCommand(SPR_2DRAW_PRTY_OFF);
			player_draw();
			print_display();
		SPR_2CloseCommand();
		
		SCL_DisplayFrame();
	}
	return 0;
}
