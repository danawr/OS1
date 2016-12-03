/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#include "Job.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20
#define MAX_CMD_HISTORY_SIZE 50


char* L_Fg_Cmd;
int GPID;
char lineSize[MAX_LINE_SIZE];
extern char* last_dir;
extern std::deque<std::string> cmd_history;
char* fgets_ret_value = NULL;
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];



	//signal declaretions
	sighandler(SIGTSTP, &ctrl_z);
	sighandler(SIGINT, &ctrl_c);

	struct sigaction childAct;
	childAct.sa_flags = SA_SIGINFO;
	childAct.sa_sigaction = &remove_dead_bg_jobs;
	sigaction(SIGCHLD, &childAct, NULL);

	/************************************/
	// Init globals
    last_dir = new char [MAX_LINE_SIZE]; //////////////need to free
	strcpy(last_dir, "no last dir");
	//SMASH_PID = getpid();
	GPID=-1;
    //jobs = NULL;

	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL)
			exit (-1);
	L_Fg_Cmd[0] = '\0';




    	while (1)
    	{
	 	printf("smash > ");
	 	while (fgets_ret_value == NULL)
             fgets_ret_value =fgets(lineSize, MAX_LINE_SIZE, stdin);
        fgets_ret_value = NULL;
		strcpy(cmdString, lineSize);
		cmdString[strlen(lineSize)-1]='\0';
		//if (!strcmp(cmdString, "\n"))
          //  std::cout << "cmdString is enter" << std::endl;
	/*	if (!strcmp(cmdString, "\n"))
		{
            lineSize[0]='\0';
            cmdString[0]='\0';
            continue;
		}
	*/

        //std::cout <<"cmdString is: " << cmdString << std::endl;
		//history maintainance
		cmd_history.push_back(cmdString);
		//std::cout <<"cmd_history back is: " << cmd_history.back() << std::endl;
		if ( cmd_history.size()>=50 )
        {
            cmd_history.pop_front(); //the oldest.
        }

					// perform a complicated Command
		if(!ExeComp(lineSize)) continue;
		//strcpy(cmdString, lineSize);
		//cmdString[strlen(lineSize) - 1] = '\0';
					// background command
	 	if(!BgCmd(lineSize))
	 	{
            //lineSize[0]='\0';
            //cmdString[0]='\0';
            continue;
	 	}
	 	//strcpy(cmdString, lineSize);
		//cmdString[strlen(lineSize) - 1] = '\0';
					// built in commands
		ExeCmd(lineSize, cmdString);

		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}

	delete[] last_dir;
    return 0;
}

