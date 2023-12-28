all : compile link
  
CC       = gcc
CFLAGS   = -m64 -O2 -Wall -I..
CPATH    = .

%.o : $(CPATH)/%.c  
	$(CC) -c $(CFLAGS) $<

OBJECTS0 = zigrandom.o zignor.o
OBJECTS1 = zigtimer.o test_timings.o
OBJECTS2 = test_moments.o

compile : $(OBJECTS0) $(OBJECTS1) $(OBJECTS2)

link :
	$(CC) -o test_timings.exe $(OBJECTS0) $(OBJECTS1) -lm
	$(CC) -o test_moments.exe $(OBJECTS0) $(OBJECTS2) -lm

clean :
	rm *.o test_timings.exe test_moments.exe
