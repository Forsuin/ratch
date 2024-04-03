#define FMT_HEADER_ONLY

#include <fmt/core.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "scheduler.h"

int main()
{

    Scheduler scheduler;

    std::string input;
    bool shouldClose = false;

    while (!shouldClose)
    {

        // Input tokenizer
        input.clear();

        fmt::print("Command> ");
        std::getline(std::cin, input, '\n');

        std::stringstream ss(input);
        std::string word;
        std::vector<std::string> tokens;

        while (ss >> word)
        {
            tokens.push_back(word);
        }

        if (tokens.size() == 0)
        {
            shouldClose = true;
            continue;
        }

        if (tokens[0] == "start")
        {
            if (tokens.size() != 2)
            {
                fmt::println("Invalid number of arguments");
                continue;
            }

            scheduler.start(tokens[1]);
        }
        else if (tokens[0] == "step")
        {
            if (tokens.size() != 2)
            {
                fmt::println("Invalid number of arguments");
                continue;
            }

            int stepAmt = std::stoi(tokens[1]);

            scheduler.step(stepAmt);
        }
        else if (tokens[0] == "setBurst")
        {
            if (tokens.size() != 2)
            {
                fmt::println("Invalid number of arguments");
                continue;
            }

            int burst_size = std::stoi(tokens[1]);

            scheduler.set_burst(burst_size);
        }
        else if (tokens[0] == "setMemory")
        {
            if (tokens.size() != 2)
            {
                fmt::println("Invalid number of arguments");
                continue;
            }

            int mem_size = std::stoi(tokens[1]);

            scheduler.set_memory(mem_size);
        }
        else if (tokens[0] == "getMemory")
        {
            if (tokens.size() != 1)
            {
                fmt::println("Invalid number of arguments");
                continue;
            }

            scheduler.get_memory();
        }
        else if (tokens[0] == "run")
        {
            if (tokens.size() != 1)
            {
                fmt::println("Invalid number of arguments");
                continue;
            }

            scheduler.run();
        }
        else if (tokens[0] == "addProgram")
        {
            // Check to make sure we have enough arguments and everything is a pair
            if (tokens.size() < 4 || tokens.size() % 2 != 0)
            {
                fmt::println("Invalid number of arguments");
                continue;
            }

            std::string name = tokens[1];
            int time_req = std::stoi(tokens[2]);
            int mem_req = std::stoi(tokens[3]);

            std::queue<std::pair<int, int>> pairs;

            if (tokens.size() > 4)
            {
                for (auto it = tokens.begin() + 4; it != tokens.end(); it += 2)
                {
                    int first = std::stoi(*it);
                    int second = std::stoi(*(it + 1));

                    pairs.push({first, second});
                }
            }

            scheduler.addProgram(name, time_req, mem_req, pairs);
        }
        else if (tokens[0] == "quit")
        {
            shouldClose = true;
        }
        else
        {
            fmt::println("Invalid commands");
        }
    }
}