all : compile link
  
CC       = gcc
CFLAGS   = -m64 -O2 -Wall -I..
CPATH    = .


%.o : $(CPATH)/%.c  
	$(CC) -c $(CFLAGS) $<


OBJECTS2 = test_moments.o
OBJECTS5 = test_histogram.o
OBJECTS6 = test_timings.o
OBJECTS7 = test_jumps.o
OBJECTS8 = test_pcg64dxsm.o

compile : $(OBJECTS2) $(OBJECTS5) $(OBJECTS6) $(OBJECTS7) $(OBJECTS8)

link :
	$(CC) -o test_timings.exe $(OBJECTS6) -lm
	$(CC) -o test_moments.exe $(OBJECTS2) -lm
	$(CC) -o test_histogram.exe $(OBJECTS5) -lm
	$(CC) -o test_jumps.exe $(OBJECTS7) -lm
	$(CC) -o test_pcg64dxsm.exe $(OBJECTS8) -lm

clean :
	rm *.o
	rm *.exe
