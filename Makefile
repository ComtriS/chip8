.PHONY: all clean

CC=gcc
CFLAGS=

BINDIR=bin
OBJDIR=obj

SRCS=$(wildcard *.c)
OBJS=$(patsubst %,$(OBJDIR)/%,$(patsubst %.c,%.o,${SRCS}))
LIBS=

${BINDIR}/chip8: ${OBJS} ${BINDIR}
	@echo " MAKE  $@"

${BINDIR} ${OBJDIR}:
	@echo "MKDIR  $@"
	@mkdir -p $@

${OBJDIR}/%.o: %.c ${OBJDIR}
	@echo "   CC  $< => $@"
	@$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

all: ${BINDIR}/chip8
	@echo " MAKE  $@"

clean:
	@echo "   RM  ${BINDIR}"
	@rm -rf ${BINDIR}
	@echo "   RM  ${OBJDIR}"
	@rm -rf ${OBJDIR}
	