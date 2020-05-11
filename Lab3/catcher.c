#include<stdio.h> 
#include<signal.h> 
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <time.h>


//string Array for the signal name
const char* my_sys_signame[] = { 

	"SIGHUP",
	"SIGINT",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGABRT",
	"SIGIOT",
	"SIGBUS",
	"SIGFPE",
	"SIGUSR1",
	"SIGSEGV",
	"SIGUSR2",
	"SIGPIPE",
	"SIGALRM",
	"SIGTERM",
	"SIGSTKFLT",
	"SIGCHLD",
	"SIGCONT",
	"SIGTSTP",
	"SIGTTIN",
	"SIGTTOU",
	"SIGURG",
	"SIGXCPU",
	"SIGXFSZ",
	"SIGVTALRM",
	"SIGPROF",
	"SIGWINCH" };

//int Array for the signal number
const int all_signals[] = {
	SIGHUP,
	SIGINT,
	SIGQUIT,
	SIGILL,
	SIGTRAP,
	SIGABRT,
	SIGIOT,
	SIGBUS,
	SIGFPE,
	SIGUSR1,
	SIGSEGV,
	SIGUSR2,
	SIGPIPE,
	SIGALRM,
	SIGTERM,
	SIGSTKFLT,
	SIGCHLD,
	SIGCONT,
	SIGTSTP,
	SIGTTIN,
	SIGTTOU,
	SIGURG,
	SIGXCPU,
	SIGXFSZ,
	SIGVTALRM,
	SIGPROF,
	SIGWINCH};

int done = 0;
static int count = 0;
int sig_counter = 0;

// Handler for signals provided on command line
void handle_allsig(int sig) 
{ 

	signal(sig, handle_allsig); // Unreliable signals should register handler again

	time_t seconds_now;	// Initialise time_t for the current epoch time

	sig_counter+=1;		// Total signal counter

	for(int i = 0;i<27;i++)
	{
		// Comparing signal number with our integer signal numbr array(all_signals[]) to get the signal name from (my_sys_signame[])
		if(all_signals[i] == sig) 
		{
			printf("\n%s caught at %ld\n", my_sys_signame[i],time(&seconds_now) );
		}
	}
//Checking if signal is SIGTERM or not
	if(sig ==15)
	{
		count+=1;
		// IF 3 successive SIGTERM then we are done
		if(count==3)
			done = 1;
	}
	else
	{
		count = 0;	
	}
} 


int main(int argc,char* argv[]) 
{ 
	int counter; 

	//USAGE message if no command line parameters are found
	if(argc==1) 
	{
		printf("\nUSAGE: catcher <TERM> <USR1> <USR2>\n"); 
		return 0;
	}

	if(argc>=2) 
	{ 

		//Loop through all the arguments from command line
		for(counter=1;counter<argc;counter++) 
		{
			for(int i = 0; i <27;i++)
			{
				//Checking each argument is related to which signal by strstr() -> SIGTERM -> TERM -->strstr("SIGTERM","TERM")
				if(strstr(my_sys_signame[i], argv[counter]) != NULL) 
				{

					//Creating signal handler for each signal provided on commandline
					signal(all_signals[i],handle_allsig);
				}
			}


		}

	} 

	// Log message with process id
	fprintf(stderr, "\ncatcher: $$ = %d\n", getpid()); 

	// Pause the process until signal raised
	while (!done)
	{
		pause();
		//sleep(1);
	}

	//Final total signal count log message
	fprintf(stderr, "\ncatcher: Total signals count = %d\n",sig_counter );

	return 0;
} 
