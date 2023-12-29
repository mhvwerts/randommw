all : compile link
  
CC       = gcc
CFLAGS   = -m64 -O2 -Wall -I.
CPATH    = .

%.o : $(CPATH)/%.c  
	$(CC) -c $(CFLAGS) $<

OBJECTS0 = zigrandom.o zignor.o
OBJECTS1 = zigtimer.o test_timings.o
OBJECTS2 = test_moments.o
OBJECTS3 = test_seed.o
OBJECTS4 = test_minimal.o
OBJECTS5 = test_histogram.o

compile : $(OBJECTS0) $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4) $(OBJECTS5)

link :
	$(CC) -o test_timings.exe $(OBJECTS0) $(OBJECTS1) -lm
	$(CC) -o test_moments.exe $(OBJECTS0) $(OBJECTS2) -lm
	$(CC) -o test_seed.exe $(OBJECTS0) $(OBJECTS3) -lm
	$(CC) -o test_minimal.exe $(OBJECTS0) $(OBJECTS4) -lm
	$(CC) -o test_histogram.exe $(OBJECTS0) $(OBJECTS5) -lm

clean :
	rm *.o
	rm test_timings.exe test_moments.exe test_seed.exe test_minimal.exe test_histogram.exe
