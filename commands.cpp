//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(std::vector<job> &jobs, char* lineSize, char* cmdString)
{
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") )
	{
        char* curr_dir=getcwd();
		if ( !strcmp(arg[1], "-") ) //if we need to change to the last dir
		{
            if (last_dir)
            {
                chdir(last_dir);
                last_dir=curr_dir;
            }
		}
		else if ( arg[1]!=NULL )//if there's a path
		{
            if ( chdir(arg[1])) //returns 0 for success
            {
                std::cerr << "smash error: > " << args[1] << " - path not found" << std::endl;
            }
            else //we succeeeded the move - need to update last_dir
            {
                last_dir=curr_dir;
            }

		}
	}

	/*************************************************/
	else if (!strcmp(cmd, "pwd"))
	{
            printf("%s\n", str(getcwd()) );
	}

	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
        std::deque<string>iterator it= cmd_history.end(); //from the new (last) to the old (first in line)
        while(it!=cmd_history.begin())
        {
            std::cout << *it << endl;
            it--;
        }
	}
	/*************************************************/

	else if (!strcmp(cmd, "jobs"))
	{
        for (int i=0;i<jobs.size(); i++)
        {
            int waiting_in_bg_time= (int)(time(NULL)) - jobs[i].get_bg_arriving_time();
            std::cout<< "[" << i << "]" <<" "<< jobs[i].get_name() <<" "<< jobs[i].get_pid() <<" "<< waiting_in_bg_time << "secs" << endl;
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{
        std::cout<< "smash pid is" << getpid() << endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
        int ind=0;
        if ( arg[1] )// if there's an argument
        {
            ind= atoi(arg[1]);
        }
        else
        {
            ind= jobs.size()-1; // the newest bg job.
        }
        std::cout << jobs[jobs.begin()+ind].get_name() << endl;
        jobs[ind].resume;
            // TODO  - need to figure out how to make the smash wait for the process to end - fork?
            // TODO - if the process fails - need to handle it
        jobs.erase[jobs.begin()+ind];
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{

	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork())
	{
    		case -1:
					// Add your code here (error)

					/*
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();

			        // Add your code here (execute an external command)

					/*
					your code
					*/

			default:
                	// Add your code here

					/*
					your code
					*/
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)

		/*
		your code
		*/
	}
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)

		/*
		your code
		*/

	}
	return -1;
}

