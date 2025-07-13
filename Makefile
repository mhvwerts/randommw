all : compile link
  
CC       = gcc
CFLAGS   = -m64 -O2 -Wall -I.
CPATH    = .


%.o : $(CPATH)/%.c  
	$(CC) -c $(CFLAGS) $<


OBJECTS4 = example_randommw.o
OBJECTS7 = genzignor.o

compile : $(OBJECTS4) $(OBJECTS7)

link :
	$(CC) -o genzignor.exe $(OBJECTS7) -lm
	$(CC) -o example_randommw.exe $(OBJECTS4) -lm

clean :
	rm *.o
	rm *.exe
