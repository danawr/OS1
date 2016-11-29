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
    arriving_time_=(int) (time(NULL));
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


int Job::get_arriving_time()
{
    return arriving_time_;
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
    stopped_=1;
}

void Job::resume()
{
    stopped_=0;
}

