#ifndef CD_H
#define CD_H

//init cd stuff
void cd_init(void);

/**
 * read data off the cd's root directory
 * filename: duh
 * dataBuf: where you want to copy the data to
 * read_size: # of bytes to read 
 */
void cd_open(const char *filename, char *dataBuf, int read_size);

#endif
