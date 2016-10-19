# Karen Lee kglee
# pa01
# Makefile
# ===================================================================

CC     = gcc
CFLAGS = -g -Wall -O0 -std=c99 -D_SVID_SOURCE

graph01:	graph01.o  intList.o
	${CC}  -o  graph01  ${CFLAGS} graph01.o intList.o

intList.o:	intList.c  intList.h
	${CC}  -c  ${CFLAGS}  intList.c

graph01.o:	graph01.c  intList.h
	${CC}  -c  ${CFLAGS}  graph01.c
