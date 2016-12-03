// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

extern int GPID;
extern char lineSize[MAX_LINE_SIZE];

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

   // std::cout << "using ctrl_c" << std::endl;
    if(GPID == -1)//no process in fg
        return;

	send_signal(GPID, SIGKILL);
/*	if (kill(GPID, SIGKILL))
	{
		perror("Error: ");
		printf("Error failed to send SIGKILL to %d\n", GPID);
		return;
	}

	printf("signal SIGKILL was sent to pid %d\n", GPID);
*/
	GPID=-1;
	return;

}

//********************************************
// function name: ctrl_z
// Description: SIGSTP handler function
// Parameters: int signum - signal
// Returns: void
//********************************************

void ctrl_z(int signum) {
	//int currPid = getpid();
	//char* delimiters = " \t\n";
	//char* cmd = strtok(lineSize, delimiters);

    //std::cout << "using ctrl_z" << std::endl;
    if(GPID == -1)//no process in fg
        return;

/*	if (kill(GPID, SIGTSTP))
	{
		perror("Error: ");
		printf("Error failed to send SIGTSTP to %d\n", GPID);
		return;
	}
*/
    send_signal(GPID, SIGTSTP);

    int ind=find_ind_by_id(GPID);
    Jobs[ind].stop();

	//printf("signal SIGTSTP was sent to pid %d\n", GPID);
    GPID=-1;
	return;

}



//********************************************
// function name: remove_dead_bg_jobs
// Description: removes dead child processes onc receiving SIGCHLD
// Parameters: int signum - signal, siginfo_t * sigInfo - to know which process died, void* context NULL
// Returns: void
//********************************************
void remove_dead_bg_jobs(int signum, siginfo_t * sigInfo, void* context)
{
  /*  std::cout << "remove_dead_bg_jobs is being used" << std::endl;
    std::cout << "si code is: " << sigInfo->si_code << std::endl;
    std::cout << "CLD_KILLED code is: " << CLD_KILLED << std::endl;
    std::cout << "CLD_EXITED code is: " << CLD_EXITED << std::endl;
    std::cout << "CLD_DUMPED code is: " << CLD_DUMPED << std::endl;
  */
    int jobInd = find_ind_by_id(sigInfo->si_pid);
	if (jobInd == -1)
	{
        //std::cout << "couldn't find pid in vector" << std::endl;
        return;
	}

    if (sigInfo->si_code==CLD_KILLED || sigInfo->si_code==CLD_EXITED|| sigInfo->si_code==CLD_DUMPED)
        {
           // std::cout << "trying to erase ind: "<< jobInd << std::endl;
            Jobs.erase(Jobs.begin()+find_ind_by_id(sigInfo->si_pid));
        }


}

//********************************************
// function name: send_signal
// Description: sends signal with the appropreate message
// Parameters: int pid, int signal
// Returns: void
//********************************************
void send_signal(int pid, int signal)
{
	std::cout << "signal " << sig_num_2_name(signal) << " was sent to pid " << pid << std::endl;
	if (kill(pid, signal))
	{
		perror("smash error: > kill failure");
	}
}

//********************************************
// function name: sig_num_2_name
// Description: translates for prining demands
// Parameters: int signum - signal number
// Returns: void
//********************************************
std::string sig_num_2_name(int signum)
{
	switch (signum)
	{
        case 1:
            return "SIGHUP";
        case 2:
            return "SIGINT";
        case 3:
            return "SIGQUIT";
        case 4:
            return "SIGILL";
        case 5:
            return "SIGTRAP";
        case 6:
            return "SIGABRT";
        case 7:
            return "SIGBUS";
        case 8:
            return "SIGFPE";
        case 9:
            return "SIGKILL";
        case 10:
            return "SIGUSR1";
        case 11:
            return "SIGSEGV";
        case 12:
            return "SIGUSR2";
        case 13:
            return "SIGPIPE";
        case 14:
            return "SIGALRM";
        case 15:
            return "SIGTERM";
        case 16:
            return "SIGSTKFLT";
        case 17:
            return "SIGCHLD";
        case 18:
            return "SIGCONT";
        case 19:
            return "SIGSTOP";
        case 20:
            return "SIGTSTP";
        case 21:
            return "SIGTTIN";
        case 22:
            return "SIGTTOU";
        case 23:
            return "SIGURG";
        case 24:
            return "SIGXCPU";
        case 25:
            return "SIGXFSZ";
        case 26:
            return "SIGVTALRM";
        case 27:
            return "SIGPROF";
        case 28:
            return "SIGWINCH";
        case 29:
            return "SIGPOLL";
        case 30:
            return "SIGPWR";
        case 31:
            return "SIGSYS";
        default:
            return "UNKNOWN";
	}
}
