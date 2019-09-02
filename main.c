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
	scroll.bg_tiles = wood_chr;
	scroll.num_bg_tiles = 40;
	scroll.levels = levels;
	scroll.bg_palette = wood_pal;
	scroll.bg2_tiles = cloud_chr;
	scroll.num_bg2_tiles = 24;
	scroll.bg2_tilemap = map_cloud;
	scroll.bg2_palette = cloud_pal;
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
