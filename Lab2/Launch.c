#include <stdio.h>  
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h> 
#include <stdlib.h>


int main(int argc, char *argv[]){
    pid_t  pid = fork();

    if (pid == 0)//child process
    {  
       
        execve(argv[1], argv + 1, NULL);
    }
    else if (pid == -1) // Fork failed
    {
        
       
    fprintf(stderr, "Fork failed , error %d\n", errno);

    exit(EXIT_FAILURE);     

    }
 else //parent process 
   {

       int position;
       fprintf(stderr, "%s: $$ = %d\n", argv[1], pid);
       waitpid(pid, &position, 0);
       if(WIFEXITED(position) != 0)
       {
         fprintf(stderr, "%s: $? = %d\n", argv[1], WEXITSTATUS(position));
       }
   
   }
    return 0;
}