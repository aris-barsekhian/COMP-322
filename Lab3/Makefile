CC = gcc
CFLAGS  = -g -Wall

default: catcher

catcher:  catcher.o
	$(CC) $(CFLAGS) -o catcher catcher.o

catcher.o:  catcher.c 
	$(CC) $(CFLAGS) -c catcher.c

clean:
	$(RM) catcher *.o *~
