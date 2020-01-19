#include <string.h> //memcpy
#define	_SPR2_
#include <sega_spr.h>
#include <sega_scl.h> 
#include <sega_mth.h>

#include "cd.h"
#include "enemylist.h"
#include "graphicrefs.h"
#include "sprite.h"
#include "scroll.h"
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
	cd_init();
	sprite_init();
	LEVEL level1;
	level1.player_startx = MTH_FIXED(64);
	level1.player_starty = MTH_FIXED(176);

	layer_init(&(level1.playfield), bg_name, bg_num, bg_pal, map_name, map_width, map_height);
	layer_init(&(level1.bg_near), cloud_name, cloud_num, cloud_pal, cloudmap_name, cloudmap_width, cloudmap_height);
	layer_init(&(level1.bg_far), hills_name, hills_num, hills_pal, hill_map_name, hill_map_width, hill_map_height);

	scroll_init(&level1);
	print_init();
	player_init(&level1);
	SCL_SetSpriteMode(SCL_TYPE5,SCL_MIX,SCL_SP_WINDOW);
	enemylist_spawn(0);
	while(1) {
		frame++;
		player_input();
		print_num(frame, 0, 0);
		

		SPR_2OpenCommand(SPR_2DRAW_PRTY_OFF);
			player_draw();
			sprite_draw_all();
			print_display();
		SPR_2CloseCommand();
		
		SCL_DisplayFrame();
	}
	return 0;
}
