#include "Job.h"




Job::Job()
{
    //ctor
    pid_=-1;
}

Job::Job(char* name,  int pid)
{
    std::string str(name);
    name_=name;
    pid_=pid;
    //arguments_=str(arguments);
    bg_arriving_time_=(int) (time(NULL)); // we make the job when we enter it.
    last_stopping_time_=-1;
    stopped_=0;
}

Job::~Job()
{
    //dtor
}
std::string Job::get_name()
{
    return name_;
}

int Job::get_pid()
{
    return pid_;
}


int Job::get_last_stopping_time()
{
    return last_stopping_time_;
}

int Job::get_bg_arriving_time()
{
    return bg_arriving_time_;
}

bool Job::is_stopped()
{
    return stopped_;
}

void Job::stop()
{
    last_stopping_time_=(int) (time(NULL));
    stopped_=1;
}

void Job::resume()
{
    last_stopping_time_=-1;
    stopped_=0;
}

