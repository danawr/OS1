#include "job.h"




job::job()
{
    //ctor
    pid_=-1;
}

job::job(char* name, char* arguments, int pid)
{
    name_=str(name);
    pid_=pid;
    arguments_=str(arguments);
    arriving_time_=(int) (time(NULL));
    stopped_=0;
}

job::~job()
{
    //dtor
}
string job::get_name()
{
    return name_;
}

int job::get_pid()
{
    return pid_;
}

string job::get_arguments()
{
    return arguments_;
}

int job::get_arriving_time()
{
    return arriving_time;
}

int job::get_bg_arriving_time()
{
    return bg_arriving_time;
}

bool job::is_stopped()
{
    return stopped_;
}

void job::stop()
{
    stopped_=1;
}

void job::resume()
{
    stopped_=0;
}

