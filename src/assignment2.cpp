#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>
#include <thread>
#include "commands.hpp"
#include "history.hpp"
#include "foldertools.hpp"
#include "programtools.hpp"

// = = = = = = = = = = Author: Kurt Wilson = = = = = = = = = = 
struct systemState
{
    std::filesystem::path current_path;
    std::vector<HistoryEntry> history;
    std::vector<pid_t> runing_programs;
};

void executeCommand(HistoryEntry &e, systemState &current_state);

int main()
{
    systemState current_state;
    current_state.current_path = std::filesystem::current_path();
    current_state.history = getHistoryFromFile("history.txt");
    Command lastCommand;
#ifdef DEBUG
    std::cout << "cwd: " << current_state.current_path.generic_string() << std::endl;
#endif
    do
    {
        std::string args;
        Command command = getCommandFromPrompt(args);
#ifdef DEBUG
        std::cout << "args: " << args << std::endl;
#endif
        HistoryEntry e;
        e.args = args;
        e.command = command;
        if (command != Command::NONE)
        {
            current_state.history.push_back(e);
        }
        executeCommand(e, current_state);
        lastCommand = e.command;
    } while (lastCommand != Command::BYEBYE);

    dumpHistoryToFile("history.txt", current_state.history);
}
// = = = = = = = = = = END Author: Kurt Wilson = = = = = = = = = = 

void executeCommand(HistoryEntry &e, systemState &current_state)
{
    // = = = = = = = = = = Author: Joseph Terribile = = = = = = = = = = 
    Command command = e.command;
    std::string args = e.args;
    switch (command)
    {
    case Command::MOVETODIR:
    {
        bool success = moveToFolder(args, current_state.current_path);
        if (!success)
        {
            std::cout << "Could not move to that directory." << std::endl;
            break;
        }
        break;
    }
    case Command::WHEREAMI:
        std::cout << current_state.current_path << std::endl;
        break;
    case Command::HISTORY:
        if (args == "-c")
        {
            current_state.history.clear();
        }
        else
        {
            printHistory(current_state.history);
        }
        break;
    case Command::REPLAY:
    {
        int idx = std::stoi(args);
        HistoryEntry previous = current_state.history[current_state.history.size() - idx];
        
        std::cout << commandToString(previous.command) << previous.args << std::endl;

        if (previous.command == Command::BYEBYE)
        {
            e.command = previous.command;
            return;
        }
        executeCommand(previous, current_state);
        break;
    }
    case Command::START:
        startProgramAndWait(args, current_state.current_path);
        break;
    case Command::BACKGROUND:
    {
        pid_t new_program = startProgram(args, current_state.current_path);
        if(new_program != -1)
        {
            std::cout << "Started with pid: " << std::to_string(new_program) << std::endl;
            current_state.runing_programs.push_back(new_program);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        break;
    }
    case Command::DALEK:
    {
        int pid = std::stoi(args);
        killProcess(pid);
        break;
    }
    case Command::REPEAT:
    {
        size_t numEnd = args.find(' ');
        int numTimes = std::stoi(args.substr(0, numEnd));

        // error check presence of program to repeat
        if (numEnd == std::string::npos)
        {
            std::cout << "Provide a program to repeat." << std::endl;
            break;
        }

        std::string command_string = args.substr(numEnd + 1, args.length());
        std::vector<pid_t> spawned;

        for (int i = 0; i < numTimes; i++)
        {
            pid_t pid = startProgram(command_string, current_state.current_path);
            if (pid != -1)
            {
                spawned.push_back(pid);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "PIDs: ";
        for (int i = 0; i < spawned.size() - 1; i++)
        {
            std::cout << std::to_string(spawned[i]) << ", ";
            current_state.runing_programs.push_back(spawned[i]);
        }
        std::cout << std::to_string(spawned[spawned.size() - 1]) << std::endl;
        current_state.runing_programs.push_back(spawned[spawned.size() - 1]);

        break;
    }
    case Command::DALEKALL:
    {
        if (current_state.runing_programs.size() == 0)
        {
            std::cout << "There are no running background processes" << std::endl;
            break;
        }
        
        std::cout << "Exterminating " << std::to_string(current_state.runing_programs.size()) << (current_state.runing_programs.size() == 1 ? " process: " : " processes: ");

        for (int i = 0; i < current_state.runing_programs.size(); i++)
        {
            pid_t pid = current_state.runing_programs[i];
            std::cout << std::to_string(pid);
            if (i != current_state.runing_programs.size() - 1)
            {
                std::cout << " ";
            }
            killProcess(pid);
        }
        std::cout << std::endl;
        current_state.runing_programs.clear();
        break;
    }
    case Command::BYEBYE:
        break;
    default:
        std::cout << "Command unknown" << std::endl;
        break;
    }
    // = = = = = = = = = = END Author: Joseph Terribile = = = = = = = = = =
}