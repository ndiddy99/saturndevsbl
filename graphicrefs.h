#ifndef GRAPHICREFS_H
#define GRAPHICREFS_H


//---background tiles---
//bg.c
extern Uint16 bg_num;
extern char bg_name[];
extern Uint32 bg_pal[];
//hills.c
extern Uint16 hills_num;
extern char hills_name[];
extern Uint32 hills_pal[];
//cloud.c
extern Uint16 cloud_num;
extern char cloud_name[];
extern Uint32 cloud_pal[];
//---level tilemaps---
//map0.c
extern Uint16 map0_width;
extern Uint16 map0_height;
extern char map0_name[];
//---background tilemaps---
//hill_map.c
extern Uint16 hill_map_width;
extern Uint16 hill_map_height;
extern char hill_map_name[];

//cloudmap.c
extern Uint16 cloudmap_width;
extern Uint16 cloudmap_height;
extern char cloudmap_name[];

//---sprite graphics---
//font.c
#define GRAPHIC_FONT (0)
extern Uint16 font_num;
extern Uint16 font_size;
extern Uint16 font_width;
extern Uint16 font_height;
extern char font_name[];
extern Uint32 font_pal[];
//guy.c
#define GRAPHIC_GUY (font_num)
extern Uint16 guy_num;
extern Uint16 guy_size;
extern Uint16 guy_width;
extern Uint16 guy_height;
extern char guy_name[];
extern Uint32 guy_pal[];
//float.c
#define GRAPHIC_FLOAT (GRAPHIC_GUY + guy_num)
extern Uint16 float_num;
extern Uint16 float_size;
extern Uint16 float_width;
extern Uint16 float_height;
extern char float_name[];
extern Uint32 float_pal[];
//explosion.c
#define GRAPHIC_EXPLOSION (GRAPHIC_FLOAT + float_num)
extern Uint16 explosion_num;
extern Uint16 explosion_size;
extern Uint16 explosion_width;
extern Uint16 explosion_height;
extern char explosion_name[];
extern Uint32 explosion_pal[];
#endif
