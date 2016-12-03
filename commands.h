#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "Job.h"
#include "signals.h"
#include <vector>
#include <deque>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <limits>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
//typedef enum { FALSE , TRUE } bool;
int ExeComp(char* lineSize);
int BgCmd(char* lineSize);
int ExeCmd(char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
extern char* last_dir;
int find_ind_by_id(int child_pid);
int find_last_sleeping_Job();
extern std::deque<std::string> cmd_history;
extern std::vector<Job> Jobs; //This represents the list of Jobs. Please change to a preferred type (e.g array of char*)
#endif

