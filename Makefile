#
#������ makefile �̗��p�����
# 1. ���̃t�@�C�����T���v���̃f�B���N�g���ɃR�s�[���Ă��������B
# 2. �f�B���N�g���̐ݒ�𒲐����Ă��������B
#    �}�N�� GCC, SEGALIB, SEGASMP ���`���Ă��������B
# 3. �R���t�B�M�����[�V�����t�@�C�� sample.cfg ���쐬���Ă��������B
#    �}�N�� TARGET �� OBJS ���`���Ă��������B
# 4. make �����s���ăT���v���v���O�������쐬���Ă��������B
#

#
#�����̃t�@�C���Ŏg���Ă���}�N���̐���
#  CC           �b�R���p�C�����w�肵�܂��B�i���s�\�Ȍ`�ŋL�q���Ă��������j
#  CFLAGS       �b�R���p�C���̃I�v�V�������w�肵�܂��B
#  AS           �A�Z���u�����w�肵�܂��B�i���s�\�Ȍ`�ŋL�q���Ă��������j
#  ASFLAGS      �A�Z���u���̃I�v�V�������w�肵�܂��B
#  LDFLAGS      �����J�̃I�v�V�������w�肵�܂��B
#  SATURN       �T�^�[���p�̃z�[���f�B���N�g�����w�肵�܂��B
#  GCC          �f�m�t�̂���f�B���N�g�����w�肵�܂��B
#  SEGASMP      �r�a�k�̃T���v���̂���f�B���N�g�����w�肵�܂��B
#  SEGALIB      �r�a�k�̃��C�u�����̂���f�B���N�g�����w�肵�܂��B
#  LOCATE_FILE  �������̃��P�[�V�������L�q�����t�@�C���ł��B
#  ENTRY_POINT  �T���v���v���O�����̎��s�J�n�A�h���X���w�肵�܂��B
#  LIBS         �����N���郉�C�u�������w�肵�܂��B
#  
#  �i�R���t�B�M�����[�V�����t�@�C�� sample.cfg �̒��Œ�`���܂��j
#  TARGET       �T���v���v���O�����̖��O���w�肵�܂��B
#               �i�t�@�C���̃v���C�}���l�[���ɂȂ�܂��j
#  OBJS         �����N����I�u�W�F�N�g�t�@�C�����w�肵�܂��B
#

CC = sh-coff-gcc
AS = gasm

CFLAGS  = -g -O2 -Wall -std=gnu99 -m2 -DMODEL_S -I$(GCC)/include -I$(SEGALIB)/include -I$(SEGASMP)/per/smpclib
ASFLAGS =
LDFLAGS = -T $(LOCATE_FILE) -e $(ENTRY_POINT) -nostartfiles

SATURN  = ../sbl6
GCC     = c:/saturn/SH_COFF/sh-coff
SEGASMP = $(SATURN)/segasmp
SEGALIB = $(SATURN)/segalib

LOCATE_FILE = saturn.lnk
ENTRY_POINT = START
CONFIG_FILE = sample.cfg

LIBS=	$(SEGALIB)/lib/sega_int.a\
	$(SEGALIB)/lib/sega_spr.a\
	$(SEGALIB)/lib/sega_scl.a\
	$(SEGALIB)/lib/sega_mth.a\
	$(SEGALIB)/lib/sega_cdc.a\
	$(SEGALIB)/lib/sega_adp.a\
	$(SEGALIB)/lib/sega_per.a\
	$(SEGALIB)/lib/sega_sat.a

include	$(CONFIG_FILE)

.SUFFIXES:
.SUFFIXES: .cof .o .src .c

all: $(TARGET).cof $(TARGET).bin

$(TARGET).cof:	$(OBJS)
	$(CC) $(LDFLAGS) $(_LDFLAGS) -o $(TARGET).cof -Xlinker -Map -Xlinker $(TARGET).map $(OBJS) $(LIBS)

$(TARGET).bin:	$(OBJS)
	sh-coff-objcopy -O binary $(TARGET).cof out.bin

%.o: %.c
	$(CC) -c $(CFLAGS) $(_CFLAGS) -o $@ $<

%.o: %.src
	cmd //c $(AS) $< $(ASFLAGS) $(_ASFLAGS) -o $@



