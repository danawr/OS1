//		commands.cpp
//********************************************
#include "commands.h"
using namespace std;
std::deque<std::string> cmd_history;
std::vector<Job> Jobs; //This represents the list of Jobs. Please change to a preferred type (e.g array of char*)
char* last_dir;
extern int GPID;

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString)
{
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters =(char*) " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
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
        char* curr_dir;
        curr_dir = new char [MAX_LINE_SIZE];
        getcwd(curr_dir, MAX_LINE_SIZE);

        if( args[1]==NULL ) //"cd" doesn't recieve arguments
		{
            std::cout<<"'cd' command must recieve a path or '-'"<<std::endl;
		}
		else if ( !strcmp(args[1], "-") ) //if we need to change to the last dir
		{
            if (strcmp(last_dir, "no last dir")!=0)
            {
               if(!chdir(last_dir))
                    strcpy(last_dir,curr_dir);
            }
		}
		else if ( args[1]!=NULL )//if there's a path
		{
            if ( chdir(args[1])) //returns 0 for success
            {
               std::cerr << "smash error: > " << args[1] << " - path not found" << std::endl;
            }
            else //we succeeeded the move - need to update last_dir
            {
                strcpy(last_dir,curr_dir);
            }
		}

        delete[] curr_dir;
	}

	/*************************************************/
	else if (!strcmp(cmd, "pwd"))
	{
            char* curr_dir;
            curr_dir = new char [MAX_LINE_SIZE];
            getcwd(curr_dir, MAX_LINE_SIZE);
            //std::string str(curr_dir);
            printf("%s\n",curr_dir );
            delete[] curr_dir;
	}

	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
        std::deque<std::string>::iterator it= cmd_history.begin(); //from the new (last) to the old (first in line)
        while(it!=(cmd_history.end()-1))
        {
            std::cout << *it << endl;
            it++;
        }
	}
	/*************************************************/

	else if (!strcmp(cmd, "jobs"))
	{
        for (unsigned int i=0;i<Jobs.size(); i++)
        {
            int waiting_in_bg_time= (int)(time(NULL)) - Jobs[i].get_bg_arriving_time();
            std::cout<< "[" << i << "]" <<" "<< Jobs[i].get_name() <<" "<< Jobs[i].get_pid() <<" "<< waiting_in_bg_time << "secs" << endl;
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{
        std::cout<< "smash pid is" << getpid() << endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))  //wake a sleeping bg process up AND WAIT FOR HIM
	{
        int ind=0;
        if ( args[1] )// if there's an argument
        {
            ind= atoi(args[1]);
        }
        else
        {
            ind= Jobs.size()-1; // the newest bg Job.
        }
        //std::cout << Jobs[Jobs.begin()+ind].get_name() << std::endl;
        std::cout << Jobs[ind].get_name() << std::endl;
        if ( Jobs[ind].is_stopped() ) //if that process is asleep
        {
            //need to send signal to wake him up
            kill(Jobs[ind].get_pid(), SIGCONT);
            Jobs[ind].resume(); //this is just the flag. we should probably include this in the signal handler.
        }
            // TODO  - need to figure out how to make the smash wait for the process to end - fork?
        // move to fg = have the fg process (the shell, which we are in) wait for this process to end.
        if ( waitpid(Jobs[ind].get_pid(), NULL, 0) <0  )
            {
             // TODO - if the waitpid fails - need to handle it
            }
        //fg is for processes that are already in the Jobs list. we just need to wake them up, they will remove themselves out (BgCmd).
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg")) //wake a sleeping bg process up
	{
        int ind=0;
        if ( args[1] )// if there's an argument
        {
            ind= atoi(args[1]);
        }
        else
        {
            ind= find_last_sleeping_Job();
        }
        if (ind<0)
        {
            printf("there's no sleeping Job -> we'll do nothing.\n");
            return 0;
        }
        std::cout << Jobs[ind].get_name() << std::endl;
        if (Jobs[ind].is_stopped()) //if that process is asleep
        {
            //need to send signal to wake him up
            kill(Jobs[ind].get_pid(), SIGCONT);
            Jobs[ind].resume(); //this is just the flag. we should probably include this in the signal handler.
        }


	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
        if ( args[1] )// if there's an argument => quit kill
        {
             while (Jobs.size() > 0 ) // counting on the fact that the signal handlers remove the processes from the Jobs list!
            {
                int curr_pid=Jobs[0].get_pid();
                printf("Sending SIGTERM...");
                kill(curr_pid, SIGTERM);
                int start_time = (int) (time(NULL));
                while (((int) time(NULL) - start_time) < 5) {} // wait five seconds
                if ( curr_pid==Jobs[0].get_pid() ) // it's still on the Jobs list => hasn't died
                {
                    printf( "(5 sec passed) Sending SIGKILL…" );
                    kill(curr_pid, SIGKILL);
                }
                printf("Done \n");
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
	if (illegal_cmd == true)
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
                	GPID = pID;
                    waitpid(pID, NULL, 0); // the bgCmd will take care of bg Jobs off all kinds.
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
        std::string str(lineSize);
        args[1]=lineSize;
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
                    waitpid(pID, NULL, 0); // the bgCmd will take care of bg Jobs off all kinds.
                    break;
        }
        return 0;
	}
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to Jobs
// Parameters: command string, pointer to Jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize)
{

	//char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';

    char* cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
   	args[0] = cmd;
   	int num_arg=0;
	for (int i=1; i<MAX_ARG; i++)
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
               		// add to Jobs list
               		int child_pid=getpid();
                    //std::string str(cmd);
               		Job curr_Job= Job(cmd,child_pid);
               		Jobs.push_back(curr_Job);
               		//((std::vector<Job>)(*Jobs)).push_back(curr_Job);
               		// execute
               		if(ExeComp(lineSize)) //if it fails
                        ExeExternal(args,cmd);
                    // once completed, we want to remove from Jobs list. the actual waiting process is inside ExeExternal (and it waited in there)
                    int ind= find_ind_by_id(child_pid);
                    if (ind<0)
                    {
                        printf("child pid is not in the Jobs vector");
                        break;
                    }
                    Jobs.erase(Jobs.begin()+ind);
                    exit(0);
                    break;
                //default:
                    // WE DONT WANT THE SHELL TO WAIT
                  //  return 0;
                    //break;
            }


	}
	return -1;
}

int find_ind_by_id( int child_pid)
{
    for (unsigned int  i=0; i< Jobs.size(); i++)
    {
        if (Jobs[i].get_pid()==child_pid)
            return i;
    }
    return -1;
};


int find_last_sleeping_Job()
{
    for (int i= Jobs.size()-1; i>-1; i--)
    {
        if (Jobs[i].is_stopped())
            return i;
    }
    return -1;
};
