#pragma once
#ifndef SCHEDULER_H

#include <queue>
#include <string>
#include <utility>

class Scheduler
{
    int tick_count = 0;
    int total_memory = 4;
    int used_memory = 0;
    int burst_size = 4;

    struct Task
    {
        std::string name;
        int time_req;
        int mem_req;
        int io_delay;
        int io_time_req;
        int execution_time;
    };

    std::queue<Task> active_tasks;
    std::queue<Task> waiting_tasks;

    // vector so I can iterate over these tasks
    std::vector<Task> finished_tasks;
    std::vector<Task> inactive_tasks;

    void update();

public:
    void start(std::string program_name);
    void step(int amt_time);
    void set_burst(int burst_size);
    void set_memory(int mem_size);
    int get_memory();
    void run();
    void addProgram(std::string program_name, int time_req, int mem_req, int io_delay = 0, int io_time_req = 0);
};

#endif