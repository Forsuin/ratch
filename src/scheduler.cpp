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
            if (used_memory + (*it).mem_req < total_memory)
            {
                // add task to active queue and remove from inactive
                active_tasks.push(*it);
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

    while (!active_tasks.empty())
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

    Task current_task = active_tasks.front();

    if (!current_task.io_pairs.empty())
    {
        if (current_task.io_pairs.front().first == 0)
        {
            waiting_tasks.push(current_task);
        }
    }

    if (!waiting_tasks.empty() && waiting_tasks.front().io_pairs.front().second == 0)
    {
        waiting_tasks.front().io_pairs.pop();
        active_tasks.push(waiting_tasks.front());
        waiting_tasks.pop();
    }

    if (current_task.remaining_time == 0)
    {
        current_task.finished_time = elapsed_time;
        finished_tasks.push_back(current_task);
    }

    fmt::println("Current time <{}>", elapsed_time);

    elapsed_time++;
    current_task.remaining_time--;
    if (!waiting_tasks.empty())
    {
        waiting_tasks.front().io_pairs.front().second--;
    }
}