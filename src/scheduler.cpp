#include "scheduler.h"

void Scheduler::start(std::string program_name)
{
    // find task in inactive array

    for (auto it = inactive_tasks.begin(); it != inactive_tasks.end(); it++)
    {
        if ((*it).name == program_name)
        {
            // add task to active queue and remove from inactive
            active_tasks.push(*it);
            inactive_tasks.erase(it);
            break;
        }
    }
}

void Scheduler::step(int amt_time)
{
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
    this->total_memory = mem_size;
}

void Scheduler::run()
{
    while (!active_tasks.empty())
    {
        step(1);
    }
}

void Scheduler::addProgram(std::string program_name, int time_req, int mem_req, int io_delay = 0, int io_time_req = 0)
{
    Task task;
    task.name = program_name;
    task.time_req = time_req;
    task.mem_req = mem_req;
    task.io_delay = io_delay;
    task.io_time_req = io_time_req;
    task.execution_time = 0;

    inactive_tasks.push_back(task);
}

void Scheduler::update()
{
}