BINDIR=bin
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,SRCS)

${BINDIR}/chip8: ${BINDIR}
	@gcc ${SRCS} -o $@

${BINDIR}:
	@mkdir -p $@

all: ${BINDIR}/chip8

clean:
	@rm -rf ${BINDIR}
