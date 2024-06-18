CC = gcc
CFLAGS = -I./include -Wall -Ofast -flto
LIBS = -lm
DEPS = zmg.h pcg_variants.h
RM = /bin/rm
TARGETS = zmgf.o zmgd.o
%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $<

all: $(TARGETS)

clean:
	$(RM) $(TARGETS)
