// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"


//********************************************
// function name: sighandler
// Description:  function that sets the signal handlers
// Parameters: int signum - signal
//			   SIG_HANDLER_PTR func_ptr - pointer to function that handles our signal
// Returns: void
//**************************************************************************************
void sighandler(int signum, SIG_HANDLER_PTR func_ptr) {
	struct sigaction act;
	act.sa_handler = func_ptr;
	if (sigaction(signum, &act, NULL)) //set out signal handler function to a given signal
	{
		perror("Error: ");
		exit(1);
	}
}

//********************************************
// function name: ctrl_c
// Description:  SIGINT handler function
// Parameters: int signum - signal
// Returns: void
//********************************************
void ctrl_c(int signum) {
	int currPid = getpid();

	if (currPid == SMASH_PID) //no process in foreground
		return;

	if (kill(currPid, SIGINT))
	{
		perror("Error: ");
		printf("Error failed to send SIGINT to %d\n", currPid);
		return;
	}

	printf("signal SIGINT was sent to pid %d\n", currPid);
	return;

}

//********************************************
// function name: ctrl_z
// Description: SIGSTP handler function
// Parameters: int signum - signal
// Returns: void
//********************************************

void ctrl_z(int signum) {
	int currPid = getpid();
	char* delimiters = " \t\n";
	char* cmd = strtok(lineSize, delimiters);

	if (currPid == SMASH_PID) //no process in foreground
		return;

	if (kill(currPid, SIGTSTP))
	{
		perror("Error: ");
		printf("Error failed to send SIGTSTP to %d\n", currPid);
		return;
	}

	/********put process to a job list*******/
	
	job newJob=job(cmd, currPid);
	newJob.stop();
	jobs.push_back(&newJob);

	printf("signal SIGTSTP was sent to pid %d\n", currPid);

	return;

}