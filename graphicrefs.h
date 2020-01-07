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
//---background tilemaps---
//map.c
extern Uint16 map_width;
extern Uint16 map_height;
extern char map_name[];

//hill_map.c
extern Uint16 hill_map_width;
extern Uint16 hill_map_height;
extern char hill_map_name[];

//---sprite graphics---
//font.c
extern Uint16 font_num;
extern Uint16 font_size;
extern Uint16 font_width;
extern Uint16 font_height;
extern char font_name[];
extern Uint32 font_pal[];
//guy.c
extern Uint16 guy_num;
extern Uint16 guy_size;
extern Uint16 guy_width;
extern Uint16 guy_height;
extern char guy_name[];
extern Uint32 guy_pal[];

#endif
