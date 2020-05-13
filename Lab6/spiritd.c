#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

static pid_t mole_pid;
static int Mole_arg;

// signal handler to handle signal and do processing as per document
static void signal_handler(int sig)
{

	FILE * logf = fopen("/tmp/log.txt","a+");
	fprintf(logf, "Caught signal = %d", sig);
	// Checking if signal is SIGTERM
	if (sig == SIGTERM)
	{

		// the program kills all child processes and shutdowns the daemon gracefully
		fprintf(logf, "Send SIGTERM to %d\n", (int)mole_pid);
		kill(mole_pid, SIGTERM);
		fprintf(logf, "Exiting\n");
		exit(0);
	}
	// Checking if signal is SIGUSR1
	else if (sig == SIGUSR1)
	{
		//kill child process #1 (mole1)
		//randomly create either mole1 or mole 2 if it does not already exists
		if (Mole_arg == 0 && mole_pid > 0)
		{
			fprintf(logf, "Send SIGCHLD to %d\n", (int)mole_pid);
			kill(mole_pid, SIGCHLD);
		}
		Mole_arg = rand() % 2;
		char snum[16];
		sprintf(snum, "%d", Mole_arg + 1);
		char *Mole_command[] = { "mole", snum, 0 };
		mole_pid = fork();
		fprintf(logf, "mole_pid = %d\n", (int)mole_pid);
		if (mole_pid == 0)
		{
			fprintf(logf, "Executing%s %s\n", Mole_command[0], Mole_command[1]);
			execv(Mole_command[0], Mole_command);
		}
		(void) signal(SIGUSR1, signal_handler);
	}
	// Checking if signal is SIGUSR2
	else if (sig == SIGUSR2)
	{

		//kill child process #1 (mole2)
		//randomly create either mole1 or mole 2 if it does not already exists
		if (Mole_arg == 1)
		{
			fprintf(logf, "Send SIGCHLD to %d\n", (int)mole_pid);

			kill(mole_pid, SIGCHLD);
		}
		//randomly determine the child process number (either 1 or 2)
		Mole_arg = rand() % 2;
		char snum[16];
		sprintf(snum, "%d", Mole_arg + 1);
		char *Mole_command[] = { "mole", snum, 0 };
		//fork a new process	
		mole_pid = fork();
		if (mole_pid == 0)
		{

			//exec the program mole, with the value of argv[0] set to either 1 or 2	
			fprintf(logf,"Executing %s %s\n", Mole_command[0], Mole_command[1]);
			execv(Mole_command[0], Mole_command);
		}
		(void) signal(SIGUSR2, signal_handler);
	}
	else
	{
		fprintf(logf,"received unexpected signal %d\n", sig);

	}
}

int main(int argc, char **argv)
{

	FILE * logf = fopen("/tmp/log.txt","a+");
	const char *logfile = "/tmp/log.txt";

	// Create child process
	pid_t process_id = fork();
	if (process_id < 0)

		fprintf(logf, "fork() failed");  // Indication of fork() failure

	if (process_id > 0)
	{
		// PARENT PROCESS exits successfully
		printf("process ID of child process %d \n", process_id);
		exit(0);
	}

	// unmask the file mode
	umask(0);
	// set new session
	pid_t sid = setsid();
	if (sid < 0)
		fprintf(logf, "setsid() failed\n");

	// Change the current working directory to root.
	//if (chdir("/") != 0)
	//fprintf(logf, "chdir() failed\n");


	// Redirect standard input from /dev/null
	close(STDIN_FILENO);
	if (open("/dev/null", O_RDONLY) < 0)
		fprintf(logf, "failed to open /dev/null\n");

	// Redirect standard output and standard error to a log file
	int fd1 = open(logfile, O_APPEND, 0644);
	if (fd1 < 0)
		fprintf(logf, "failed to open %s\n", logfile);

	if (dup2(fd1, STDOUT_FILENO) < 0)
		fprintf(logf, "failed to send standard output to log file %s\n", logfile);

	if (dup2(fd1, STDERR_FILENO) < 0)
		fprintf(logf, "failed to send standard output to log file %s\n", logfile);

	close(fd1);
	fprintf(logf, "Starting the process\n");

	// assign signal handlers
	(void) signal(SIGTERM, signal_handler);
	(void) signal(SIGUSR1, signal_handler);
	(void) signal(SIGUSR2, signal_handler);
	mole_pid = fork();
	while (1)
	{
		sleep(1);
		fprintf(logf, "Running %d...\n",mole_pid);
		if (mole_pid == 0)
		{

			int Mole_arg = rand() % 2;
			char snum[16];
			sprintf(snum, "%d", Mole_arg + 1);
			char *Mole_command[] = { "mole", snum, 0 };
			fprintf(logf, "Executing %s %s\n", Mole_command[0], Mole_command[1]);
			execv(Mole_command[0], Mole_command );

		}
	}

	return(0);
}
