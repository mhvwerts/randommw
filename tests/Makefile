all : compile link
  
CC       = gcc
CFLAGS   = -m64 -O2 -Wall -I..
CPATH    = .


%.o : $(CPATH)/%.c  
	$(CC) -c $(CFLAGS) $<


OBJECTS0 = ../randommw.o
OBJECTS1 = ../zigtimer.o
OBJECTS2 = test_moments.o
OBJECTS5 = test_histogram.o
OBJECTS6 = test_timings.o
OBJECTS8 = test_jumps.o

compile : $(OBJECTS0) $(OBJECTS1) $(OBJECTS2) $(OBJECTS5) $(OBJECTS6) $(OBJECTS8)

link :
	$(CC) -o test_timings.exe $(OBJECTS0) $(OBJECTS1) $(OBJECTS6) -lm
	$(CC) -o test_moments.exe $(OBJECTS0) $(OBJECTS2) -lm
	$(CC) -o test_histogram.exe $(OBJECTS0) $(OBJECTS5) -lm
	$(CC) -o test_jumps.exe $(OBJECTS8) $(OBJECTS0) $(OBJECTS1) -lm

clean :
	rm *.o
	rm *.exe
