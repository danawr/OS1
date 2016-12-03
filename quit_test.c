/*
 * quit_test.c
 *
 *  Created on: Nov 8, 2013
 *      Author: Yuval Bar-Tal
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(){
	sigset_t mask;
	sigset_t old_set;
	sigfillset(&mask);
	sigprocmask(SIG_SETMASK,&mask,&old_set);

	sleep (400);
	return 0;
}
