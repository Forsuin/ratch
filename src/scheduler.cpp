#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include "scheduler.h"

void Scheduler::start(std::string program_name)
{
    // find task in inactive array

    for (auto it = inactive_tasks.begin(); it != inactive_tasks.end(); it++)
    {
        if ((*it).name == program_name)
        {
            if (used_memory + (*it).mem_req <= total_memory)
            {
                used_memory += (*it).mem_req;

                // add task to active queue and remove from inactive
                active_tasks.push_back(*it);
                inactive_tasks.erase(it);
                break;
            }
            else
            {
                fmt::println("Unable to start program, not enough memory");
            }
        }
    }
}

void Scheduler::step(int amt_time)
{
    fmt::println("Advancing the system for {} units", amt_time);

    for (int i = 0; i < amt_time; i++)
    {
        update();
    }
}

void Scheduler::set_burst(int burst_size)
{
    this->burst_size = burst_size;

    for (auto &t : active_tasks)
    {
        t.burst_size = burst_size;
    }

    for (auto &t : waiting_tasks)
    {
        t.burst_size = burst_size;
    }

    for (auto &t : inactive_tasks)
    {
        t.burst_size = burst_size;
    }
}

void Scheduler::set_memory(int mem_size)
{
    total_memory = mem_size;
}

int Scheduler::get_memory()
{
    return total_memory;
}

void Scheduler::run()
{
    fmt::println("Advancing the system untill all jobs finished");

    while (!active_tasks.empty() || !waiting_tasks.empty())
    {
        update();
    }
}

void Scheduler::addProgram(std::string program_name, int time_req, int mem_req, std::queue<std::pair<int, int>> io_pairs = {})
{
    Task task(program_name, time_req, mem_req, io_pairs);

    inactive_tasks.push_back(task);
}

void Scheduler::update()
{
    /*
        First, if there is no currently running task, get the next task in queue
        Second, check if currently_running task needs to wait for IO
        Third, check if first task in waiting queue is finished waititng for IO
        Fourth, check if the currently running task is completely finished running
        Fifth, check if the currently running task has finished it's burst, but still has time left
        Sixth, increment elapsed time and decrement current tasks remaining time and waiting_task
    */

    if (active_tasks[0].remaining_time == 0)
    {
        active_tasks[0].finished_time = elapsed_time;
        used_memory -= active_tasks[0].mem_req;
        active_tasks[0].finished_time = elapsed_time;
        finished_tasks.push_back(active_tasks[0]);
        active_tasks.pop_front();
        active_tasks[0] = active_tasks[0];
    }

    if (!active_tasks[0].io_pairs.empty())
    {
        if (active_tasks[0].io_pairs.front().first == 0)
        {
            waiting_tasks.push_back(active_tasks[0]);
            active_tasks.pop_front();
            active_tasks[0] = active_tasks[0];
        }
    }

    if (!waiting_tasks.empty() && waiting_tasks.front().io_pairs.front().second == 0)
    {
        waiting_tasks.front().io_pairs.pop();
        active_tasks.push_back(waiting_tasks.front());
        waiting_tasks.pop_front();
    }

    if (active_tasks[0].burst_size == 0)
    {
        active_tasks[0].burst_size = burst_size;
        active_tasks.push_back(active_tasks.front());
        active_tasks.pop_front();
    }

    fmt::println("Current time <{}>", elapsed_time);

    if (!active_tasks.empty())
    {
        fmt::println("Running job {} has {} time units left and is using {} memory resources", active_tasks[0].name, active_tasks[0].remaining_time, active_tasks[0].mem_req);

        fmt::print("The queue is:");

        if (active_tasks.begin() + 1 == active_tasks.end())
        {
            fmt::println("empty");
        }

        int i = 1;
        for (auto it = active_tasks.begin() + 1; it != active_tasks.end(); it++, i++)
        {
            Task &t = (*it);
            fmt::print("\tPosition {}: job {} has {} time units and is using {} memory resources.\n", i, t.name, t.remaining_time, t.mem_req);
        }
    }
    else
    {
        fmt::println("Current job is empty");
        fmt::println("The queue is: empty");
    }

    if (!waiting_tasks.empty())
    {
        for (auto &t : waiting_tasks)
        {
            fmt::println("The process {} is obtaining IO and will be back in {} time unit.", t.name, t.io_pairs.front().second);
        }
    }

    if (!finished_tasks.empty())
    {
        fmt::println("Finished jobs are:");

        for (auto &t : finished_tasks)
        {
            fmt::println("\t{} {} {}", t.name, t.time_req, t.finished_time);
        }
    }

    elapsed_time++;
    active_tasks[0].remaining_time--;
    active_tasks[0].burst_size--;

    if (!active_tasks[0].io_pairs.empty())
    {
        active_tasks[0].io_pairs.front().first--;
    }

    if (!waiting_tasks.empty())
    {
        waiting_tasks.front().io_pairs.front().second--;
    }

    fmt::print("\n");
}