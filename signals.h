#ifndef _SIGS_H
#define _SIGS_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "Job.h"
#include "commands.h"

typedef void(*SIG_HANDLER_PTR)(int signum);

void sighandler(int signum, SIG_HANDLER_PTR func_ptr);
void ctrl_c(int signum);
void ctrl_z(int signum);
void remove_dead_bg_jobs(int signum, siginfo_t * sigInfo, void* context);
std::string sig_num_2_name(int signum);
void send_signal(int pid, int signal);
#endif

