#ifndef Job_H
#define Job_H
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>

class Job
{
    public:
        Job();
        Job(char* name, int pid);
        virtual ~Job();

        std::string get_name();
        int get_pid();
        //std::string get_arguments();
        //nt get_arriving_time();
        int get_bg_arriving_time();
        int get_last_stopping_time();
        bool is_stopped();

        void stop();
        void resume();

    //protected:
    private:
        std::string name_;
        int pid_;
        //std::string arguments;
        //int arriving_time_;
        int bg_arriving_time_;
        int last_stopping_time_;
        bool stopped_;

};

#endif // Job_H
