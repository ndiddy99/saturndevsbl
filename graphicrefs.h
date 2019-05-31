#ifndef GRAPHICREFS_H
#define GRAPHICREFS_H

//sprite.c
extern Uint16 dimensions[];
extern Uint16 *tiles[];

//bg.c
extern Uint32 test_pal[];
extern Uint8 test_chr[];

//map.c
extern Uint16 map[];

#define TEST_PAL_SIZE 256
#define TEST_CHR_SIZE (64 * 64)

#endif
