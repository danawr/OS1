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
	//char pwd[MAX_LINE_SIZE];
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
        if (args[1]==NULL)
            illegal_cmd=true;
        else
        {
            if ( !strcmp(args[1], "-") ) //if we need to change to the last dir
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
            //TODO: check when "cd" doesn't recieve arguments
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
        std::deque<std::string>::iterator it= cmd_history.end()-1; //from the new (last) to the old (first in line)
        while(it!=cmd_history.begin()-1)
        {
            std::cout << *it << endl;
            it--;
        }
	}
	/*************************************************/

	else if (!strcmp(cmd, "jobs"))
	{
        for (unsigned int i=0;i<Jobs.size(); i++)
        {
            int waiting_in_bg_time= (int)(time(NULL)) - Jobs[i].get_bg_arriving_time();
            std::cout<< "[" << i << "]" <<" "<< Jobs[i].get_name() <<" : "<< Jobs[i].get_pid() <<" "<< waiting_in_bg_time << " secs" << endl;
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
        GPID = Jobs[ind].get_pid();
        if ( Jobs[ind].is_stopped() ) //if that process is asleep
        {
            //need to send signal to wake him up
            send_signal(Jobs[ind].get_pid(), SIGCONT);
            //std::cout << "signal SIGCONT was sent to pid " << Jobs[ind].get_pid() << std::endl;
            //kill(Jobs[ind].get_pid(), SIGCONT);
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
            //std::cout << "signal SIGCONT was sent to pid " << Jobs[ind].get_pid() << std::endl;
            //kill(Jobs[ind].get_pid(), SIGCONT);
            send_signal(Jobs[ind].get_pid(), SIGCONT);
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

                //int kill_ret_val=kill(curr_pid, SIGTERM);
                kill(curr_pid, SIGTERM);
                printf("Sending SIGTERM...");

                int start_time = (int) (time(NULL));
                while (((int) time(NULL) - start_time) < 5) {}

                if (Jobs.size()>0 && curr_pid==Jobs[0].get_pid() ) // it's still on the Jobs list => hasn't died
                {
                        printf( "(5 sec passed) Sending SIGKILL… " );
                        kill(curr_pid, SIGKILL);
                }
                //std::cout << "ret val is: "<< kill_ret_val << endl;
                /*printf("Sending SIGTERM...");
                if (kill_ret_val)
                {
                    //int start_time = (int) (time(NULL));
                    //while (((int) time(NULL) - start_time) < 5) {} // wait five seconds
                    if ( curr_pid==Jobs[0].get_pid() ) // it's still on the Jobs list => hasn't died
                    {
                        printf( "(5 sec passed) Sending SIGKILL…" );
                        kill(curr_pid, SIGKILL);
                    }
                    //Jobs.erase(0);
                }
                */
                printf("Done \n");
            }
        }
        kill( getpid(), 9 );
	}
	/*************************************************/
		else if (!strcmp(cmd, "kill"))
	{
        if ( args[1] && args[2])// if there are arguments
        {
            int signum= -atoi(args[1]); // atoi inturprets the char as negative :)
            //std::cout << "signum is" << signum << std::endl;
            if ((unsigned)atoi(args[2]) <=Jobs.size()-1)
                kill(Jobs[atoi(args[2])].get_pid(), signum);
        }
        else
            illegal_cmd=true;
	}
	/*************************************************/
	else // external command
	{
 		//if (!getpid())// if this is a son, meaning the shell was forked (bgCmd for built in cmd)
          //  return -2;
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
					std::cout << "smash error: > could not fork for external command" << std::endl;
					break;
        	case 0 :
                	// Child Process
               		setpgrp();
               		//std::cout << "in exe child" << std::endl;
                    execvp(cmdString, args);
                    perror("smash error: > execvp failure");
                    exit(1);
                    break;
			default:
                	// Add your code here
                	//std::cout << "waiting for exe child" << std::endl;
                	GPID = pID;
                    waitpid(pID, NULL, 0); // the bgCmd will take care of bg Jobs off all kinds.
                    //std::cout << "exe child is dead" << std::endl;
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
	//char ExtCmd[MAX_LINE_SIZE+2];
	char cmdString[MAX_LINE_SIZE];
	//char *args[MAX_ARG];
	char *csh_args[3];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
        csh_args[0]=(char*)"csh";
        csh_args[1]=(char*)"-fc";
        //csh_args[2]=(char*)"-c";
        //std::string str(lineSize);
        //str[str.size()-1]="\0";
        strcpy(cmdString, lineSize);
		cmdString[strlen(lineSize)-1]='\0';
        csh_args[2]=cmdString;
        //args[3]=NULL;
        int pID;
    	switch(pID = fork())
        {
    		case -1:
					std::cout << "smash error: > could not fork for external command" << std::endl;
					break;
        	case 0 :
                	// Child Process
               		setpgrp();
                    execvp("csh", csh_args);
                    perror("smash error: > execvp failure");
                    exit(1);
                    break;
			default:
                	// Add your code here
                	GPID = pID;
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
	char* delimiters = (char*)" \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';

    char* cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
    //cmd[strlen(lineSize)-1]='\0';
   	args[0] = cmd;

   	int num_arg=0;
	for (int i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}
		// Add your code here (execute a in the background - only for external commands)
            //std::vector<Job>* global_Jobs_ptr;
            //global_Jobs_ptr= &Jobs;
            int pID;
            switch(pID = fork())
            {
                case -1:
                    std::cout << "smash error: > could not fork for bg command" << std::endl;
					break;
                case 0 :
                	// Child Process
               		setpgrp();
               		// execute
               		//std::cout << "bg job name is: "<<(*global_Jobs_ptr)[0].get_name() << std::endl;
               		//if(ExeComp(lineSize)) //if it fails
                      // ExeExternal(args,cmd);
                    //(*global_Jobs_ptr).push_back(Job(cmd,getpid()));
                    //std::cout << "child Jobs[0] - in the global jobs - is: " << (*global_Jobs_ptr)[0].get_pid() << endl;
                    if (execvp(cmd, args) == -1)
                       perror("smash error: > execvp failure");
                    //std::cout << "back from exe, inside BbCmd" << std::endl;
                    exit(1);
                    break;
                default:
                    // WE DONT WANT THE SHELL TO WAIT
               		//Job curr_Job= Job(cmd,pID);
               		Jobs.push_back(Job(cmd,pID));
               		lineSize[0]='\0';
               		//std::cout << "job was just now added " << std::endl;
               		return 0;
               		//std::cout << "parent Jobs[0] is: " << Jobs[0].get_pid() << endl;
                    //break;
            }
            return 0;

	}
	return -1;
}

int find_ind_by_id( int child_pid)
{
    //std::cout << "child pid is" << child_pid << endl;
    for (unsigned int  i=0; i< Jobs.size(); i++)
    {
        //std::cout << "vec pid is" << Jobs[i].get_pid() << endl;
        if (Jobs[i].get_pid()==child_pid)
            return i;
    }
    return -1;
};


int find_last_sleeping_Job()
{
    if (Jobs.size() == 0)
        return -1;

    int max_ind=-1;
    int max_stopped_time=-1;
    for (unsigned int i= 0; i<Jobs.size(); i++)
    {
        //std::cout << "i is: " << i << " last stopping time: " << Jobs[i].get_last_stopping_time() << std::endl;
        if (  Jobs[i].get_last_stopping_time()>max_stopped_time )
        {
            max_stopped_time=Jobs[i].get_last_stopping_time();
            max_ind=i;
            //std::cout << "max ind changed"<<std::endl;
        }
    }
    //std::cout << "max ind is: " << max_ind << std::endl;
    return max_ind;
};
