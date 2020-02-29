#include <stdio.h>
#include <time.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
	printf("START: %ld\n", time(NULL));
	//fork(); 	
	pid_t cpid;
	pid_t child;
	int retval;
	struct tms tmsValue;
	child=fork();	
	if (child == 0)
	{
		printf("PPID: %d, PID: %d\n", getppid(), getpid());
		exit(0);           /* terminate child */
	}
	else
	{
		times(&tmsValue);
		child=wait(&retval);
		printf("PPID: %d, PID: %d,CPID %d,RETVAL %d\n", getppid(), getpid(),
			child, retval);
		printf("USER: %ld, SYS: %ld\nCUSER: %ld, CSYS: %ld\n", tmsValue.tms_utime,
			tmsValue.tms_stime, tmsValue.tms_cutime, tmsValue.tms_cstime);

	}
	
 		printf("STOP: %ld\n", time(NULL));
	return 0;
}

