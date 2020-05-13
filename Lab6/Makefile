CC = gcc
CFLAGS  = -g -Wall

default: spiritd mole

catcher:  spiritd.o
	$(CC) $(CFLAGS) -o spiritd spiritd.o

mole:  mole.o
	$(CC) $(CFLAGS) -o mole mole.o

spiritd.o:  spiritd.c 
	$(CC) $(CFLAGS) -c spiritd.c
mole.o:  mole.c 
	$(CC) $(CFLAGS) -c mole.c
clean:
	$(RM) spiritd mole *.o *~
