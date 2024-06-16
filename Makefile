all : compile link
  
CC       = gcc
CFLAGS   = -m64 -O2 -Wall -I.
CPATH    = .


%.o : $(CPATH)/%.c  
	$(CC) -c $(CFLAGS) $<


OBJECTS0 = randommw.o
OBJECTS1 = zigtimer.o
OBJECTS2 = test_moments.o
OBJECTS3 = test_seed.o
OBJECTS4 = test_minimal.o
OBJECTS5 = test_histogram.o
OBJECTS6 = test_timings.o
OBJECTS7 = genzignor.o
OBJECTS8 = xoshiro_dev.o

compile : $(OBJECTS0) $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4) $(OBJECTS5) $(OBJECTS6) $(OBJECTS7) $(OBJECTS8)

link :
	$(CC) -o test_timings.exe $(OBJECTS0) $(OBJECTS1) $(OBJECTS6) -lm
	$(CC) -o test_moments.exe $(OBJECTS0) $(OBJECTS2) -lm
	$(CC) -o test_seed.exe $(OBJECTS0) $(OBJECTS3) -lm
	$(CC) -o test_minimal.exe $(OBJECTS0) $(OBJECTS4) -lm
	$(CC) -o test_histogram.exe $(OBJECTS0) $(OBJECTS5) -lm
	$(CC) -o genzignor.exe $(OBJECTS0) $(OBJECTS1) $(OBJECTS7) -lm
	$(CC) -o xoshiro_dev.exe $(OBJECTS8)

clean :
	rm *.o
	rm test_timings.exe test_moments.exe test_seed.exe test_minimal.exe test_histogram.exe genzignor.exe xoshiro_dev.exe
