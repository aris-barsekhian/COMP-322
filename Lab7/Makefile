all: clean matrix_gen matrix_add

matrix_gen:
	gcc -g -Wall matrix_gen.c -o matrix_gen  -lrt
matrix_add:
	gcc -g -Wall matrix_add.c -o matrix_add -lrt
	
clean:
	rm -rf matrix_gen matrix_add
