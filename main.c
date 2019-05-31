#include	<string.h> //memcpy
#include	<machine.h>
#define		_SPR2_
#include	<sega_spr.h>
#include	<sega_scl.h> 
#include	<sega_mth.h>

#include	"vblank.h"
#include	"graphic\cosmo2u.map"
#include	"sprite.h"
#include	"scroll.h"

#include	"graphicrefs.h"

#define CommandMax    300
#define GourTblMax    300
#define LookupTblMax  100
#define CharMax       16
#define DrawPrtyMax   256
SPR_2DefineWork(work2D, CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)


int main()
{
	Uint16 PadData1EW;
	SPRITE_INFO sprite;
	int count, i;
	Fixed32 scrollX = FIXED(0), scrollY = FIXED(0);

	SCL_Vdp2Init();
	SPR_2Initial(&work2D);
	SCL_SetColRamMode(SCL_CRM24_1024);
	
	SetVblank(); //setup vblank routine
	set_imask(0);
	
	SPR_2FrameChgIntr(1); //wait until next frame to set color mode
	SCL_DisplayFrame();
	SCL_DisplayFrame();
	
	init_scroll(test_chr, map, test_pal);
	

	SCL_SetPriority(SCL_NBG0,7); //set layer priorities
	SCL_SetPriority(SCL_SPR,7);
	SCL_SetSpriteMode(SCL_TYPE5,SCL_MIX,SCL_SP_WINDOW);
	
	count = 0;
	for (i = 0; i < 4; i += 2) {
		SPR_2SetChar((Uint16)count, COLOR_5, 0, dimensions[i], dimensions[i + 1], (char *)tiles[count]);
		count++;
	}
	make_sprite(0, FIXED(50), FIXED(20), &sprite);
	while(1) {
		PadData1EW = PadData1E;
		PadData1E = 0;
		sprite.mirror = 0;
		scrollX = 0;
		scrollY = 0;
		
		if(PadData1 & PAD_U){
			// SCL_SetColOffset(SCL_OFFSET_A,SCL_NBG0,start.red,start.green,start.blue);
			// SCL_SetAutoColOffset(SCL_OFFSET_A,1,10,&start,&end);
			// sprite.yPos -= MTH_FIXED(1);
			scrollY = FIXED(-4);
		}
		if(PadData1 & PAD_D){
			// SCL_SetColOffset(SCL_OFFSET_A,SCL_NBG0,end.red,end.green,end.blue);
			// SCL_SetAutoColOffset(SCL_OFFSET_A,1,10,&end,&start);
			// sprite.yPos += MTH_FIXED(1);
			scrollY = FIXED(4);
		}
		if (PadData1 & PAD_L) {
			// sprite.xPos -= MTH_FIXED(1);
			scrollX = FIXED(-4);
		}
		if (PadData1 & PAD_R) {
			// sprite.xPos += MTH_FIXED(1);
			scrollX = FIXED(4);
		}
		set_scroll(SCL_NBG0, scrollX, scrollY);
		if((PadData1 & PAD_X)) {
			sprite.angle += MTH_FIXED(3);
			if (sprite.angle > MTH_FIXED(180))
				sprite.angle -= MTH_FIXED(360);
		}
		if((PadData1 & PAD_Y)) {
			sprite.angle -= MTH_FIXED(3);
			if (sprite.angle < MTH_FIXED(-180))
				sprite.angle += MTH_FIXED(360);
		}
		if (PadData1 & PAD_Z)	sprite.mirror |= MIRROR_VERT; 
		if((PadData1 & PAD_A)) 	sprite.scale += MTH_FIXED(0.05);
		if((PadData1 & PAD_B)) 	sprite.scale -= MTH_FIXED(0.05);
		if (PadData1 & PAD_C)	sprite.mirror |= MIRROR_HORIZ;
		
		SPR_2OpenCommand(SPR_2DRAW_PRTY_OFF);
			draw_sprite(&sprite);
		SPR_2CloseCommand();
		
		SCL_DisplayFrame();
	}
	return 0;
}
