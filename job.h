#ifndef JOB_H
#define JOB_H


class job
{
    public:
        job();
        job(char* name, char* argument, int pid);
        virtual ~job();

        string get_name();
        int get_pid();
        string get_arguments();
        int get_arriving_time();
        int get_bg_arrivng_time();
        bool is_stopped();

        void stop();
        void resume();

    //protected:
    private:
        string name_;
        int pid_;
        string arguments;
        int arriving_time_;
        int bg_arrivng_time_;
        bool stopped_;

};

#endif // JOB_H
