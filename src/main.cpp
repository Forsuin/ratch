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
        fmt::print("Command> ");
        std::getline(std::cin, input, '\n');

        std::stringstream ss(input);
        std::string word;
        std::vector<std::string> tokens;

        while (ss >> word)
        {
            tokens.push_back(word);
        }

        if (tokens[0] == "start")
        {
        }
        else if (tokens[0] == "step")
        {
        }
        else if (tokens[0] == "stepBurst")
        {
        }
        else if (tokens[0] == "setMemory")
        {
        }
        else if (tokens[0] == "getMemory")
        {
        }
        else if (tokens[0] == "run")
        {
        }
        else if (tokens[0] == "addProgram")
        {
        }
        else
        {
            fmt::println("Invalid commands");
        }

        if (tokens[0] == "quit")
        {
            shouldClose = true;
        }
    }
}