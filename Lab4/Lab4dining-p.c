#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>


sem_t *chopsticks[2];
sem_t *openSemaphor;


int seats,position;
/* keep a count of the number of full eat-think cycles*/
int count = 0;


void think();
void eat();
void handler();



int main(int argc, char *argv[]) {
	int i;
	char buffer[10];
	if (argc > 2) {
		seats = atoi(argv[1]);
		position = atoi(argv[2]);
		position--;
		/*You can create an additional semaphore that is used to 
		create a critical section around the allocation of system 
		resources (i.e., allocation of the chopstick)*/
		openSemaphor = sem_open("/open", O_CREAT, 0666, 1);
		sem_wait(openSemaphor);
		sprintf(buffer, "/ph%d", position);
		chopsticks[0] = sem_open(buffer, O_CREAT, 0666, 1);
		sprintf(buffer, "/ph%d", (position+1)%seats);		
		chopsticks[1] = sem_open(buffer, O_CREAT, 0666, 1);
		sem_post(openSemaphor);
		sem_close(openSemaphor);
		sem_unlink("/open");
		signal(15, handler);
		do {
			sem_wait(chopsticks[0]);
			sem_wait(chopsticks[1]);
			eat();
			sem_post(chopsticks[0]);
			sem_post(chopsticks[1]);
			think();
			count++;
		} while (1);
	}
	else {
		printf("incorrect usage: Not enough parameters\n");
	}
	return 0;
}


void eat() {
	fprintf(stdout,"Philosopher %d is eating\n", position+1);
	usleep((rand() % 3 + 1) * 1000000);
}

void think() {
	fprintf(stdout, "Philosopher %d is thinking\n", position+1);
	usleep((rand() % 3+1) * 1000000);	
}

void handler() {
	char buffer[10];
	fprintf(stdout,"Philosopher #%d completed %d cycles\n", position+1, count);
	sem_close(chopsticks[0]);
	sprintf(buffer, "/ph%d", position);
	sem_unlink(buffer);
	sem_close(chopsticks[1]);
	sprintf(buffer, "/ph%d", (position + 1) % seats);
	sem_unlink(buffer);
	exit(0);
}
