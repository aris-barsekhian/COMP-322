#include <time.h>
#include <sys/times.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
int main(){

	struct tms tmsValue;
	clock_t C_Time;
	pid_t pid;
	pid_t childId;
	int retval;
    ///Fork a new process
	pid = fork();

////++++++++++++++++++++++++PID =0 =>> It is the child
	if(pid == 0){
    ///Print Id's
		printf("PPID: %d, PID: %d\n",getppid(),getpid());
	}

 ////++++++++++++++++++++++++PID >0  =>> It is parent
	else if (pid>0)
	{ 
		C_Time = times(&tmsValue);
    ///Time 
		printf("Start: %d\n",(int)C_Time);
  childId =  waitpid(pid ,&retval,0);  //Wait for child
    ///print PID,PPID, childId,return retval
  printf("PPID: %d, PID: %d, CID: %d, RETVAL:%d\n",(int)getppid(),(int)getpid(),(int)childId, retval); 
  ///Print User,System,Cuser,and Csys time
  C_Time = times(&tmsValue);
  printf("USER: %d, SYS:%d\nCUSER: %d, CSYS: %d\n",(int)tmsValue.tms_utime,(int)tmsValue.tms_stime,
  	    (int)tmsValue.tms_cutime,(int)tmsValue.tms_cstime);

  printf("STOP: %d\n",(int)C_Time);
}
//++++++++++++++++++++++++PID =-1
else if(pid==-1){
	///Reports the abnormal termination of the program.
	exit(1);


}
	return 0;
}
