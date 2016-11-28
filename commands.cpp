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
        std::cout << jobs[jobs.begin()+ind].get_name() << std::endl;
        if (jobs[jobs.begin()+ind].stopped) //if that process is asleep
        {
            //TODO - need to send signal to wake him up
            jobs[jobs.begin()+ind].resume; //this is just the flag. we should probably include this in the signal handler.
        }
            // TODO  - need to figure out how to make the smash wait for the process to end - fork?
        // move to fg = have the fg process (the shell, which we are in) wait for this process to end.
        if ( waitpid(jobs[jobs.begin()+ind].get_pid, Null, 0) <0  )
            {
             // TODO - if the waitpid fails - need to handle it
            }
        jobs.erase[jobs.begin()+ind];
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{
        int ind=0;
        if ( arg[1] )// if there's an argument
        {
            ind= atoi(arg[1]);
        }
        else
        {
            ind= find_last_sleeping_job(&jobs); // TODO - find the newest sleeping job.
        }
        if (ind<0)
        {
            printf("ther's no sleeping job -> we'll do nothing.\n");
            return 0;
        }
        std::cout << jobs[jobs.begin()+ind].get_name() << std::endl;
        if (jobs[jobs.begin()+ind].stopped) //if that process is asleep
        {
            //TODO - need to send signal to wake him up
            jobs[jobs.begin()+ind].resume; //this is just the flag. we should probably include this in the signal handler.
        }

        //need to remove from bg line.
        jobs.erase[jobs.begin()+ind];
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
        if ( arg[1] )// if there's an argument => quit kill
        {
             while (jobs.size() > 0 ) // counting on the fact that the signal handlers remove the processes from the jobs list!
            {
                int curr_pid=jobs[0].get_pid();
                printf("Sending SIGTERM...");
                kill(curr_pid, SIGTERM);
                int start_time = (int) (time(NULL));
                while (((int) time(NULL) - start_time) < 5) {} // wait five seconds
                if ( curr_pid==jobs[0].get_pid() ) // it's still on the jobs list => hasn't died
                {
                    printf( "(5 sec passed) Sending SIGKILL…" );
                    kill(curr_pid, SIGKILL);
                }
                printf("Done \n")
            }
        }
        kill( getpid(), 9 );
	}
	/*************************************************/
	else // external command
	{
 		if (!getpid())// if this is a son, meaning the shell was forked (bgCmd for built in cmd)
            return -2;
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
					std::cout << "smash error: could not fork for external command" << std::endl;
					break;
        	case 0 :
                	// Child Process
               		setpgrp();
                    execvp(cmdString, args);
                    exit(1);
                    break;
			default:
                	// Add your code here
                    waitpid(pID, NULL, 0); // the bgCmd will take care of bg jobs off all kinds.
                    break;
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
        args[0]="-fc";
        args[1]=str(lineSize);
        int pID;
    	switch(pID = fork())
        {
    		case -1:
					std::cout << "smash error: could not fork for external command" << std::endl;
					break;
        	case 0 :
                	// Child Process
               		setpgrp();
                    execvp("csh", args);
                    exit(1);
                    break;
			default:
                	// Add your code here
                    waitpid(pID, NULL, 0); // the bgCmd will take care of bg jobs off all kinds.
                    break;
        }
        return 0;
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

    char* cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}

		// Add your code here (execute a in the background - only for external commands)
            int pID;
            switch(pID = fork())
            {
                case -1:
                    std::cout << "smash error: could not fork for bg command" << std::endl;
					break;
                case 0 :
                	// Child Process
               		setpgrp();
               		// add to jobs list
               		int child_pid=getpid();
               		job curr_job=job(cmd,child_id);
               		jobs.push_back(&curr_job);
               		// execute
               		if(ExeComp(lineSize)) //if it fails
                        ExeExternal(args,cmd);
                    // once completed, we want to remove from jobs list. the actual waiting process is inside ExeExternal (and it waited in there)
                    int ind= find_ind_by_id(&jobs, child_pid);
                    if (ind<0)
                    {
                        printf("child pid is not in the jobs vector");
                        break;
                    }
                    jobs.erase(jobs.begin()+ind);
                    break;
                default:
                    // WE DONT WANT THE SHELL TO WAIT
                    return 0;
                    break;
            }


	}
	return -1;
}

int find_ind_by_id(std::vector &jobs, int child_pid)
{
    for (int i=0; i< jobs.size(); i++)
    {
        if (jobs[jobs.begin()+i].get_pid==child_pid)
            return i;
    }
    return -1;
};


int find_last_sleeping_job(std::vector &jobs)
{
    for (int i= jobs.size()-1; i>-1; i--)
    {
        if (jobs[jobs.begin()+i].is_stopped)
            return i;
    }
    return -1;
};
