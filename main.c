#include	<string.h> //memcpy
#include	<machine.h>
#define		_SPR2_
#include	<sega_spr.h>
#include	<sega_scl.h> 
#include	<sega_mth.h>

#include	"vblank.h"
#include	"sprite.h"
#include	"scroll.h"
#include    "player.h"
#include    "print.h"

#include	"graphicrefs.h"

#define CommandMax    300
#define GourTblMax    300
#define LookupTblMax  100
#define CharMax       32 //CHANGE WHEN YOU INCREASE TILES BEYOND THIS POINT
#define DrawPrtyMax   256
SPR_2DefineWork(work2D, CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)


int main() {
	int count, i;

	SCL_Vdp2Init();
	SPR_2Initial(&work2D);
	count = 0;
	SCL_SetColRamMode(SCL_CRM24_1024);
	
	SetVblank(); //setup vblank routine
	set_imask(0);
	
	SPR_2FrameChgIntr(1); //wait until next frame to set color mode
	SCL_DisplayFrame();
	for (i = 0; i < 19 * 2; i += 2) {
		SPR_2SetChar((Uint16)count, COLOR_5, 0, dimensions[i], dimensions[i + 1], (char *)tiles[count]);
		count++;
	}
	SCL_DisplayFrame();
	
	scroll_init(wood_chr, map1, map2, wood_pal);
	player_init();
	print_init();

	SCL_SetPriority(SCL_NBG0,7); //set layer priorities
	SCL_SetPriority(SCL_SPR,7);
	SCL_SetSpriteMode(SCL_TYPE5,SCL_MIX,SCL_SP_WINDOW);
	
	while(1) {
		player_input();
		SPR_2OpenCommand(SPR_2DRAW_PRTY_OFF);
			player_draw();
			print_display();
		SPR_2CloseCommand();
		
		SCL_DisplayFrame();
	}
	return 0;
}
