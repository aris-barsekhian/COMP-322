# Name:Aris Barsekhian
# Project: Launch-Tube

EXECUTABLES=launch tube
SRC=launch.c tube.c
OBJ=launch.o tube.o

CC=gcc
CFLAGS=-g

all: $(EXECUTABLES)

	$(EXECUTABLES): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

launch: launch.c
	gcc -g -c launch.c
	gcc -g -o $@ launch.o

tube: tube.c
	gcc -g -c tube.c
	gcc -g -o $@ tube.o

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~