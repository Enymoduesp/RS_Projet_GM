IDIR = include
SDIR = src
ODIR = obj
BDIR = bin

CC = gcc
CFLAGS = -Wall -Wextra -g -I$(IDIR) -U _DEBUG_
LFLAGS = -lm

PROG = $(BDIR)/mpm
INPUT = test.txt

SRCS = $(wildcard $(SDIR)/*.c)
OBJS = $(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(SRCS))
DEPS = $(wildcard $(IDIR)/*.h)

.PHONY: all run clean delete dirs

all: dirs $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

dirs:
	@mkdir -p $(ODIR)
	@mkdir -p $(BDIR)

run: all
	./$(PROG) $(ARGS)
#  ARGS une variable Makefile vide par défaut, et tu la remplis au moment d'appeler


clean:
	rm -rf $(ODIR)

delete: clean
	rm -rf $(BDIR)
