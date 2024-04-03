#pragma once
#ifndef SCHEDULER_H

#include <deque>
#include <queue>
#include <string>
#include <utility>
#include <memory>

class Scheduler
{
    int elapsed_time = 0;
    int total_memory = 4;
    int used_memory = 0;
    int burst_size = 4;

    // Private to just this class, don't want to expose it
    struct Task
    {
        std::string name;
        int time_req;
        int mem_req;
        std::queue<std::pair<int, int>> io_pairs;

        int remaining_time;
        // time unit when this task actually finished running
        int finished_time;

        int burst_size;

        Task(std::string name, int time_req, int mem_req, std::queue<std::pair<int, int>> io_pairs = {}) : name(name), time_req(time_req), mem_req(mem_req), io_pairs(io_pairs), remaining_time(time_req), finished_time(0)
        {
            burst_size = this->burst_size;
        }
    };

    std::deque<Task> active_tasks;
    std::deque<Task> waiting_tasks;

    // vector so I can iterate over these tasks
    std::vector<Task> finished_tasks;
    std::vector<Task> inactive_tasks;

    /*
        Where all logic actually happens, runs thorugh a single iteration of the scheduler
        Params - none
        Returns - void
    */
    void update();

public:
    /*
        Starts a program if it has already been added to the system. If duplicate names exists, the first one added will start
        Params - program_name - name of the program to start
        Returns - void
    */
    void start(std::string program_name);

    /*
        Will execute a given number of time units of the scheduler
        Params - amt_time, how many time units to execute
        Returns - void
    */
    void step(int amt_time);

    /*
        Sets the burst size
        Params - number of time units a task can execute before swapping with the next task
        Returns - void
    */
    void set_burst(int burst_size);

    /*
        Sets the amount of system memory
        Params - amount of memory available to the system
        Returns - void
    */
    void set_memory(int mem_size);

    /*
        Gets the amount of system memory
        Params - none
        Returns - int - # of memory units
    */
    int get_memory();

    /*
        Runs until there are no more running tasks available
        Params - none
        Returns - void
    */
    void run();

    /*
        Adds a program to the system
        Params - program_name - name of program
               - time_req - number of time units this task needs to run until it is complete
               - mem-req - how much memory this task needs
               - io_pairs - describe when the task will read from IO
        Returns - void
    */
    void addProgram(std::string program_name, int time_req, int mem_req, std::queue<std::pair<int, int>> io_pairs);
};

#endif