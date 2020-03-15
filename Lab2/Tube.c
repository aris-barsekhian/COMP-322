#include <string.h>
#include <stdio.h>  
#include <sys/wait.h>
#include <unistd.h> 
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h> 


int main(int argc, char *argv[]){
    int PipeFile[2];
    pipe(PipeFile);
    int x = 0;
    int position;
    
    for(x = 0;x < argc;x++){
        if(strcmp(argv[x] , ",") == 0){
            argv[x] = NULL;
            position = x + 1;
            break;
        }
    }

    pid_t  pid = fork();

    if (pid == 0) //child process
    {    
          dup2(PipeFile[1], 1);
          execve(argv[1], argv + 1, NULL);
    }
    else if (pid == -1) // Fork failed
    {
        
        fprintf(stderr, "Fork failed , error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    else
    {
        pid_t Pid2 = fork();
        if (Pid2 > 0)
        {
             fprintf(stderr, "%s: $$ = %d\n", argv[1], pid);
             fprintf(stderr, "%s: $$ = %d\n", argv[4], Pid2);
        }
        else if (Pid2 == 0)
        {
            dup2(PipeFile[0], 0);
            execve(argv[position], argv + position, NULL);
            exit(0);
        }
         close(PipeFile[1]);
         close(PipeFile[0]);        
         int location1;
         int Location2;
         waitpid(pid, &location1, 0);
         waitpid(Pid2, &Location2, 0);
         fprintf(stderr, "%s: $? = %d\n", argv[1], location1);
         fprintf(stderr, "%s: $? = %d\n", argv[1], Location2);
    }
    return 0;
}