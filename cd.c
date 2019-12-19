#include <sega_gfs.h>
#include "print.h"

// max num files that can be opened simultaneously
#define MAX_OPEN        1

// max number of files per directory
#define MAX_DIR         100

// size of one sector in bytes
#define SECT_SIZE       2048

// number of sectors to read
#define RD_SIZE         1

Uint32 lib_work[GFS_WORK_SIZE(MAX_OPEN) / sizeof(Uint32)]; //library work area

GfsDirTbl directory_table; //directory info management struct
GfsDirName dirname[MAX_DIR]; //list of all filenames

Uint32 buf[SECT_SIZE * RD_SIZE / sizeof(Uint32)];

// char print_buf[100];


void cd_init(void) {
    GFS_DIRTBL_TYPE(&directory_table) = GFS_DIR_NAME;
    GFS_DIRTBL_DIRNAME(&directory_table) = dirname;
    GFS_DIRTBL_NDIR(&directory_table) = MAX_DIR;
    GFS_Init(MAX_OPEN, lib_work, &directory_table);
}

/**
 * read data off the cd's root directory
 * filename: duh
 * dataBuf: where you want to copy the data to
 * read_size: # of bytes to read 
 */
void cd_open(const char *filename, char *dataBuf, int read_size) {
    GfsHn gfs = GFS_Open(GFS_NameToId(filename));
    // Sint32 fid, fnum, fsize, atr;
    // GFS_GetFileInfo(gfs, &fid, &fnum, &fsize, &atr);
    // print_num(fsize, 5, 0);
    // Uint32 num;
    // GFS_Fread(gfs, 1, &num, sizeof(num));
    // print_num(num, 6, 0);
    //make sure we read at least one sector
    // GFS_Fread(gfs, 1, dataBuf, 768);

    GFS_Fread(gfs, 1, dataBuf, read_size);
    GFS_Close(gfs);
}








