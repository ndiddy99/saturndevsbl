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

int main() {

	sprite_init();
	scroll_init(wood_chr, map1, map2, wood_pal);
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
