#pragma once
#ifndef RATCH_SCHEDULER_H
#define RATCH_SCHEDULER_H

#include <queue>


struct Task {
    std::string name;
    int time_req;
    int mem_req;
    int io_delay;
    int io_req;
    int execution_time;
};


class Scheduler {
    int tick_count = 0;
    int step = 4;
    int total_memory = 4;
    int used_memory = 0;

    std::queue<Task> active_tasks;
    std::queue<Task> finished_tasks;

public:

};


#endif //RATCH_SCHEDULER_H
