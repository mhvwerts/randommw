all : compile link
  
CC       = gcc
CFLAGS   = -m64 -O2 -Wall -I.
CPATH    = .


%.o : $(CPATH)/%.c  
	$(CC) -c $(CFLAGS) $<


OBJECTS0 = randommw.o
OBJECTS1 = zigtimer.o
OBJECTS4 = example_randommw.o
OBJECTS7 = genzignor.o
OBJECTS8 = melg19937-64.o

compile : $(OBJECTS0) $(OBJECTS1) $(OBJECTS4) $(OBJECTS7) $(OBJECTS8)

link :
	$(CC) -o genzignor.exe $(OBJECTS0) $(OBJECTS1) $(OBJECTS7) -lm
	$(CC) -o example_randommw.exe $(OBJECTS0) $(OBJECTS4) -lm
	$(CC) -o melg19937-64.exe $(OBJECTS8) -lm

clean :
	rm *.o
	rm *.exe
