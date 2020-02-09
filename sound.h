#ifndef SOUND_H
#define SOUND_H
//must be called after cd_init
void sound_init(void);
//play an audio track
void sound_cdda(int track);
#endif
