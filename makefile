CC = gcc
CC_FLAGS = -g -pedantic -Wall -W -std=c89

FILES = zeroone.c 
assign1: $(FILES)
	gcc $(CC_FLAGS) -o zero-one $(FILES) 

clean:
	rm -f *.o core *.exe *~ *.out *.stackdump

rebuild: clean build
