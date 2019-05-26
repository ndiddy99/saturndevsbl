void init_scroll(const Uint8 *tiles, const Uint16 *tilemap, const Uint32 *palette);
// void set_scroll(Fixed32 x, Fixed32 y);

/*
1. Copy one screen of tilemap into vram
2. If scroll pos passes a tile boundary, copy a row/col into vram
*/
