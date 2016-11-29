#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <deque>
#include "job.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAX_ARG 20

//typedef enum { FALSE , TRUE } bool;
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, void* jobs);
int ExeCmd(std::vector<job> &jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
extern char* last_dir;
int find_ind_by_id(std::vector<job>&jobs, int child_pid);
int find_last_sleeping_job(std::vector<job> &jobs);
#endif

