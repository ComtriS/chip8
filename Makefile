SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,SRCS)

all: chip8

chip8:
	gcc ${SRCS} -o chip8