CC = gcc
CFLAGS = -I.. -Wall -Ofast -flto
LIBS = -lm
FLOATOBJ = ../zmgf.o
DOUBLOBJ = ../zmgd.o
RM = /bin/rm
%.o: %.c
	$(CC) -c $(CFLAGS) $<

TARGETS = histof histod momentf momentd speedf speedd normalplotf normalplotd ks

all: $(TARGETS)

histof: histof.o $(FLOATOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
histod: histod.o $(DOUBLOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
momentf: momentf.o $(FLOATOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
momentd: momentd.o $(DOUBLOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
normalplotf: normalplotf.o $(FLOATOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
normalplotd: normalplotd.o $(DOUBLOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
speedf: speedf.c $(FLOATOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
speedd: speedd.o $(DOUBLOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
ks: ks.o $(FLOATOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) *.o
veryclean: clean
	$(RM) $(TARGETS)
