#ifndef JOB_H
#define JOB_H
#include <string>

class job
{
    public:
        job();
        job(char* name, char* argument, int pid);
        virtual ~job();

        std::string get_name();
        int get_pid();
        std::string get_arguments();
        int get_arriving_time();
        int get_bg_arrivng_time();
        bool is_stopped();

        void stop();
        void resume();

    //protected:
    private:
        std::string name_;
        int pid_;
        std::string arguments;
        int arriving_time_;
        int bg_arrivng_time_;
        bool stopped_;

};

#endif // JOB_H
