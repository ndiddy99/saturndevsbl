/*----------------------------------------------------------------------------
 *  V_Blank.c -- V-Blank割り込み処理内ルーチンサンプル
 *  Copyright(c) 1994 SEGA
 *  Written by K.M on 1994-05-16 Ver.1.00
 *  Updated by K.M on 1994-09-21 Ver.1.00
 *
 *  UsrVblankStart()	：V-Blank開始割り込み処理サンプル
 *  UsrVblankEnd()	：V-Blank終了割り込み処理サンプル
 *
 *----------------------------------------------------------------------------
 */

#include	<machine.h>
#include	<sega_mth.h>
#include	<sega_spr.h>
#include	<sega_scl.h>
#include	<sega_xpt.h>
#include 	<sega_int.h>
#include	"per_x.h"
#include	"scroll.h"
#include    "graphicrefs.h"
#include "print.h"

volatile trigger_t	PadData1  = 0x0000;
volatile trigger_t	PadData1E = 0x0000;
volatile trigger_t	PadData2  = 0x0000;
volatile trigger_t	PadData2E = 0x0000;
SysPort	*__port = NULL;
void UsrVblankIn(void);
void UsrVblankOut(void);

void SetVblank(void) {
	
	__port = PER_OpenPort();
	
	/* V-Blank割り込みルーチンの登録 */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT);
	INT_SetScuFunc(INT_SCU_VBLK_IN,UsrVblankIn);
	INT_SetScuFunc(INT_SCU_VBLK_OUT,UsrVblankOut);
	INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT,INT_MSK_NULL);
}


void UsrVblankIn(void) {
	Uint16 *vram_ptr = (Uint16 *)NBG0_MAP_ADDR;
	int i;
	
	if (copy_mode & COPY_MODE_RCOL) {
		for (i = -1; i < SCREEN_TILES_Y + 1; i++) {
			vram_ptr[(((i + map_tile_y) % 32) * 32) + ((map_tile_x + SCREEN_TILES_X) % 32)] = 
				get_map_val(0, map_tile_x + SCREEN_TILES_X, map_tile_y + i);
		}
	}
	if (copy_mode & COPY_MODE_LCOL) {
		for (i = -1; i < SCREEN_TILES_Y + 1; i++) {
			vram_ptr[(((i + map_tile_y) % 32) * 32) + ((map_tile_x - 1) % 32)] = 
				get_map_val(0, map_tile_x - 1, map_tile_y + i);
		}		
	}
	if (copy_mode & COPY_MODE_BROW) {
		for (i = -1; i < SCREEN_TILES_X + 1; i++) {
			vram_ptr[(((map_tile_y + SCREEN_TILES_Y) % 32) * 32) + ((i + map_tile_x) % 32)] =
				get_map_val(0, map_tile_x + i, map_tile_y + SCREEN_TILES_Y);
		}
	}
	if (copy_mode & COPY_MODE_TROW) {
		for (i = -1; i < SCREEN_TILES_X + 1; i++) {
			vram_ptr[(((map_tile_y - 1) % 32) * 32) + ((i + map_tile_x) % 32)] =
				get_map_val(0, map_tile_x + i, map_tile_y - 1);
		}
	}

	
	SCL_VblankStart();
}

void UsrVblankOut(void) {
	SCL_VblankEnd();
	
	if( __port != NULL ){
		const SysDevice	*device;
		
		PER_GetPort( __port );
		
		if(( device = PER_GetDeviceR( &__port[0], 0 )) != NULL ){
			trigger_t	prev = PadData1;
			trigger_t	current = PER_GetTrigger( device );
			
			PadData1  = current;
			PadData1E = PER_GetPressEdge( prev, current );
		}
		else{
			PadData1 = PadData1E = 0;
		}
		
		if(( device = PER_GetDeviceR( &__port[1], 0 )) != NULL ){
			trigger_t	prev = PadData2;
			trigger_t	current = PER_GetTrigger( device );
			
			PadData2  = current;
			PadData2E = PER_GetPressEdge( prev, current );
		}
		else{
			PadData2 = PadData2E = 0;
		}
	}
}
