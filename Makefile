.PHONY: all clean

CC=gcc
CFLAGS=

BINDIR=bin
OBJDIR=obj

SRCS=$(wildcard *.c)
OBJS=$(patsubst %,$(OBJDIR)/%,$(patsubst %.c,%.o,${SRCS}))
LIBS=

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

clean:
	@echo "   RM  ${BINDIR}"
	@rm -rf ${BINDIR}
	@echo "   RM  ${OBJDIR}"
	@rm -rf ${OBJDIR}
	