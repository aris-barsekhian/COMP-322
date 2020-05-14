

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage:  matrix_gen size >matrix_file.size\n");
        printf(
                "\tGenerates a matrix of size \"size x size\" that contains a random set of integers from -100 .. 100.\n");
        return 1;
    }

    int size = atoi(argv[1]);
    int random_number;

    srand(time(NULL));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            random_number = rand() % 201;
            if (random_number <= 100)
                random_number *= -1;
            else if (random_number > 100)
                random_number -= 100;
            printf("%d ", random_number);
        }
        printf("\n");
    }
    return 0;
}

