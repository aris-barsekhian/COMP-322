

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <aio.h>

int COUNTER = 0;

/**
 * Prepare for reading data from buffer
 */
void prepareReadData(struct aiocb* block_data, int offset, off_t buffer_length) {
    memset(block_data, 0, sizeof(struct aiocb));
    block_data->aio_fildes = STDIN_FILENO;
    block_data->aio_offset = offset;
    block_data->aio_buf = malloc(buffer_length);
    block_data->aio_nbytes = buffer_length;
}

/**
 * Prepare for writing data from buffer
 */
void prepareWriteData(struct aiocb* block_data, int offset, off_t buffer_length) {
    block_data->aio_fildes = STDOUT_FILENO;
    block_data->aio_offset = offset;
    block_data->aio_nbytes = buffer_length;
}

/**
 * Check block for reading
 */
int checkBlockForReading(struct aiocb* block_data, int block_size, int buffer_length, int size) {
    int result = 0;
    block_size /= 4;
    if (((COUNTER % block_size) + block_size) >= size) {
        prepareReadData(block_data, 0, buffer_length + 1);
        result = 1;
    } else
        prepareReadData(block_data, 0, buffer_length);
    return result;
}

/**
 * Add two matrix
 */
void matrixAddProcess(struct aiocb* block, struct aiocb* new, int size, int scalar) {
    signed int hold, extra = 0;
    char buffer[5], line_buffer[2] = { '\n' };
    for (unsigned int bytes = 0; bytes < block->aio_nbytes; bytes += 4) {
        COUNTER++;
        memset(buffer, '\0', sizeof(buffer));
        memmove(buffer, (void*) block->aio_buf + bytes, 4);
        hold = atoi(buffer) + scalar;
        memset(buffer, '\0', sizeof(buffer));
        sprintf(buffer, "%d", hold);
        memmove((void*) new->aio_buf + (bytes + extra), buffer, 4);
        if (COUNTER % size == 0) {
            extra++;
            memmove((void*) new->aio_buf + (bytes + extra), line_buffer, 1);
        }
    }
}

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: matrix_add size blocks\n");
        printf(
                "\tPerforms matrix addition on an integer matrix with a random scalar value. "
                        "The operation is performed on the matrix in bites of size \"size x size\"\n");
        printf(
                "Example: matrix_add size blocks < matrix_file.size-1 >matrix_file.size-2\n");
        return 1;
    }
    int matrix_size, block_number, block_size, bytes, scalar, offset1 = 0, offset2 = 0, new_line = 0;
    struct aiocb current_block, next, last, new;
    //int start_time = time(NULL);
    clock_t start_time = clock();
    srand(time(NULL));
    matrix_size = atoi(argv[1]);
    block_number = atoi(argv[2]);
    if (block_number == 0) {
        printf("Error: blocks is 0 \n");
        return 1;
    }
    block_size = matrix_size / block_number;
    block_size = block_size * 4;
    bytes = block_size * block_size;
    scalar = rand() % 201;
    if (scalar <= 100)
        scalar = scalar * -1;
    else if (scalar > 100)
        scalar -= 100;
    prepareReadData(&current_block, offset1, bytes);
    aio_read(&current_block);
    while (aio_error(&current_block) == EINPROGRESS);
    aio_return(&current_block);
    for (offset1 = bytes; offset1 < ((matrix_size * matrix_size) * 4); offset1 = offset1 + bytes) {
        prepareReadData(&next, offset1, bytes);
        aio_read(&next);
        new_line = checkBlockForReading(&new, block_size, bytes, matrix_size);
        matrixAddProcess(&current_block, &new, matrix_size, scalar);
        memcpy(&last, &new, sizeof(struct aiocb));
        while (aio_error(&next) == EINPROGRESS);
        prepareWriteData(&last, offset2, bytes);
        aio_write(&last);
        offset2 = offset1 + bytes + new_line;
        new_line = 0;
        while (aio_error(&last) == EINPROGRESS);
        aio_return(&last);
        memcpy(&current_block, &next, sizeof(struct aiocb));
    }

    checkBlockForReading(&new, block_size, bytes, matrix_size);
    matrixAddProcess(&current_block, &new, matrix_size, scalar);
    prepareWriteData(&new, offset2, bytes);
    aio_write(&new);
    while (aio_error(&new) == EINPROGRESS);
    aio_return(&new);
    if (aio_fsync(O_SYNC, &new) == -1) {
        printf("Error: Fail to SYNC\n");
    }
    clock_t end_time = clock();
    double time_execute = ((double) (end_time - start_time) / CLOCKS_PER_SEC);
    printf("\nTime execute: %f seconds\n", time_execute);
    return 0;
}

