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

    struct Task
    {
        std::string name;
        int time_req;
        int mem_req;
        std::queue<std::pair<int, int>> io_pairs;

        int remaining_time;
        // time unit when this task actually finished running
        int finished_time;

        Task(std::string name, int time_req, int mem_req, std::queue<std::pair<int, int>> io_pairs = {}) : name(name), time_req(time_req), mem_req(mem_req), io_pairs(io_pairs), remaining_time(time_req), finished_time(0) {}
    };

    std::deque<Task> active_tasks;
    std::deque<Task> waiting_tasks;

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
    void addProgram(std::string program_name, int time_req, int mem_req, std::queue<std::pair<int, int>> io_pairs);
};

#endif