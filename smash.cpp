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
#include "job.h"

#define MAX_LINE_SIZE 80
#define MAXARGS 20
#define MAX_CMD_HISTORY_SIZE 50
char* L_Fg_Cmd;
int SMASH_PID;
std::vector<job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE];
extern std::deque<std::string> cmd_history;


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

	/************************************/
	// Init globals

	SMASH_PID = getpid();
    //jobs = NULL;
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL)
			exit (-1);
	L_Fg_Cmd[0] = '\0';




    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);
		cmdString[strlen(lineSize)-1]='\0';

		//history maintainance
		cmd_history.push_back(cmdString);
		if ( cmd_history.size()>=50 )
        {
            cmd_history.pop_front(); //the oldest.
        }

					// perform a complicated Command
		if(!ExeComp(lineSize)) continue;
					// background command
	 	if(!BgCmd(lineSize, jobs)) continue;
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);

		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

