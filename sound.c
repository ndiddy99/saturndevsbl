#include <SEGA_CDC.H>
#include <SEGA_INT.H>
#include <SEGA_SND.H>

#include "cd.h"

#define	SDDRV_NAME	"SDDRVS.TSK"
#define	SDMAP_NAME	"SMPSND5.MP"
#define	SDDRV_ADDR	0x6080000
#define	SDDRV_SIZE	0x6000
#define	SDMAP_ADDR	0x6086000
#define	SDMAP_SIZE	0x14

SndIniDt snd_init;

static void sound_interrupt() {
    SND_RESET_INT();
}

//must be called after cd_init
void sound_init() {
    cd_load(SDDRV_NAME, (void *)SDDRV_ADDR, SDDRV_SIZE);
    cd_load(SDMAP_NAME, (void *)SDMAP_ADDR, SDMAP_SIZE);
    set_imask(0);
    INT_ChgMsk(INT_MSK_NULL, INT_MSK_SND);
    INT_SetScuFunc(INT_SCU_SND, sound_interrupt);
    INT_ChgMsk(INT_MSK_SND,INT_MSK_NULL);

    SND_INI_PRG_ADR(snd_init) = (Uint16 *)SDDRV_ADDR;
    SND_INI_PRG_SZ(snd_init)  = (Uint16)  SDDRV_SIZE;
    SND_INI_ARA_ADR(snd_init) = (Uint16 *)SDMAP_ADDR;
    SND_INI_ARA_SZ(snd_init)  = (Uint16)  SDMAP_SIZE;
    SND_Init(&snd_init);
    SND_ChgMap(0);
    SND_SetCdDaLev(7, 7);
    SND_SetCdDaPan(0, 0);
    SND_SetTlVl(15);
    SND_RESET_INT();
}

void sound_cdda(int track) {
    CdcPly ply;
    CDC_PLY_STYPE(&ply) = CDC_PTYPE_TNO ;       /* CDC_CdPlayÇÃÉpÉâÉÅÅ[É^ê›íË*/
	CDC_PLY_STNO(&ply)  = track ;
	CDC_PLY_SIDX(&ply) = 1 ;
	CDC_PLY_ETYPE(&ply) = CDC_PTYPE_TNO ;
	CDC_PLY_ETNO(&ply)  = track + 1 ;
	CDC_PLY_EIDX(&ply) = 99 ;
	CDC_PLY_PMODE(&ply) = CDC_PM_DFL + 1;
    CDC_CdPlay(&ply) ;
}
