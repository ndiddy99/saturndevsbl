#include <string.h> //memcpy
#define	_SPR2_
#include <sega_spr.h>
#include <sega_scl.h> 
#include <sega_mth.h>
#include <sega_cdc.h>
#include <sega_sys.h>

#include "cd.h"
#include "enemylist.h"
#include "graphicrefs.h"
#include "scroll.h"
#include "sound.h"
#include "sprite.h"
#include "player.h"
#include "print.h"

static inline void layer_init(LAYER *layer, char *tiles_name, Uint16 tiles_num, Uint32 *palette, char *map_name, Uint16 map_width, Uint16 map_height) {
	layer->tile_name = tiles_name;
	layer->tile_num = tiles_num;
	layer->palette = palette;
	layer->map_name = map_name;
	layer->map_width = map_width;
	layer->map_height = map_height;
}

Uint32 frame = 0;

int main() {
	CdcStat cd_status;

	cd_init();
	sprite_init();
	LEVEL level1;
	level1.player_startx = MTH_FIXED(120);
	level1.player_starty = MTH_FIXED(400);

	layer_init(&(level1.playfield), bg_name, bg_num, bg_pal, map0_name, map0_width, map0_height);
	layer_init(&(level1.bg_near), cloud_name, cloud_num, cloud_pal, cloudmap_name, cloudmap_width, cloudmap_height);
	layer_init(&(level1.bg_far), hills_name, hills_num, hills_pal, hill_map_name, hill_map_width, hill_map_height);

	scroll_init(&level1);
	print_init();
	player_init(&level1);
	SCL_SetSpriteMode(SCL_TYPE5,SCL_MIX,SCL_SP_WINDOW);
	enemylist_spawn(0);
	sound_init();
	sound_cdda(2);

	while(1) {
		frame++;
		player_input();
		print_num(frame, 0, 0);
		//if the cd drive is opened, return to menu
		CDC_GetPeriStat(&cd_status);
		if ((cd_status.status & 0xF) == CDC_ST_OPEN) {
			SYS_EXECDMP();
		}

		SPR_2OpenCommand(SPR_2DRAW_PRTY_OFF);
			player_draw();
			sprite_draw_all();
			print_display();
		SPR_2CloseCommand();

		SCL_DisplayFrame();
	}
	return 0;
}
