.PHONY: all run clean

CC=gcc
CFLAGS=-Wall -Werror

BINDIR=bin
OBJDIR=obj
ROMDIR=roms

SRCS=$(wildcard *.c)
OBJS=$(patsubst %,$(OBJDIR)/%,$(patsubst %.c,%.o,${SRCS}))
LIBS=
ARGS=
ifeq ($(DEBUG),1)
	ARGS+=-D
endif
ifeq ($(DUMP),1)
	ARGS+=-d
endif
ifeq ($(STEP),1)
	ARGS+=-s
endif

${BINDIR}/chip8: ${OBJS} | ${BINDIR}
	@echo "   CC  $@"
	@${CC} -o $@ ${OBJS} ${CFLAGS} $(LIBS)

${BINDIR} ${OBJDIR}:
	@echo "MKDIR  $@"
	@mkdir -p $@

${OBJDIR}/%.o: %.c | ${OBJDIR}
	@echo "   CC  $< => $@"
	@$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

all: ${BINDIR}/chip8

run: ${BINDIR}/chip8
	${BINDIR}/chip8 "${ROMDIR}/Chip8 Picture.ch8" ${ARGS}

clean:
	@echo "   RM  ${BINDIR}"
	@rm -rf ${BINDIR}
	@echo "   RM  ${OBJDIR}"
	@rm -rf ${OBJDIR}
	