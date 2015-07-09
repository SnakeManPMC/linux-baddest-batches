#
# Makefile --- Baddest Batches (c) PMC 
#

CC	= gcc
LIBDIR = /home/bbs/lib/
MAINDIR = /dd/daydream-2.10_0429/main/
INCLUDE = /dd/daydream-2.10_0429/lib/

CFLAGS = -I$(MAINDIR) -I$(INCLUDE) -L$(LIBDIR) -ldd
#CFLAGS = -I$(MAINDIR) -I$(LIBDIR) -DUNIX -Wall -pipe -fomit-frame-pointer

LIBS	= 

all:		blt log

baddest:        baddest.c
	        $(CC) $(CFLAGS) -o $@ $<

blt:            blt.c
		$(CC) -o $@ $<

log:		log.c
		$(CC) -o $@ $<
