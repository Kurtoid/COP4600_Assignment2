#include <iostream>
#include <filesystem>
#include <vector>
#include "commands.hpp"
#include "history.hpp"
#include "foldertools.hpp"
#include "programtools.hpp"

struct systemState
{
    std::filesystem::path current_path;
    std::vector<HistoryEntry> history;
    std::vector<pid_t> runing_programs;
};

void executeCommand(HistoryEntry e, systemState &current_state);
int main()
{
    systemState current_state;
    current_state.current_path = std::filesystem::current_path();
    current_state.history = getHistoryFromFile("history.txt");
    // TODO: read history
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
        if (command != Command::NONE && command != Command::REPLAY)
        {
            current_state.history.push_back(e);
        }
        executeCommand(e, current_state);
        lastCommand = command;
    } while (lastCommand != Command::BYEBYE);

    dumpHistoryToFile("history.txt", current_state.history);
}

void executeCommand(HistoryEntry e, systemState &current_state)
{
    Command command = e.command;
    std::string args = e.args;
    // keep these in order, if possible
    switch (command)
    {
    case Command::MOVETODIR:
    {
        bool success = moveToFolder(args, current_state.current_path);
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
        HistoryEntry previous = current_state.history[current_state.history.size() - idx - 1];

        executeCommand(previous, current_state);
        break;
    }
    case Command::START:
        startProgramAndWait(args);
        break;
    case Command::BACKGROUND:
    {
        pid_t new_program = startProgram(args);
        if(new_program != -1)
        {
            std::cout << "started with pid: " << std::to_string(new_program) << std::endl;
            current_state.runing_programs.push_back(new_program);
        }
        break;
    }
    case Command::DALEK:
    {
        int pid = std::stoi(args);
        killProcess(pid);
        break;
    }
    case Command::BYEBYE:
        break;
    default:
        std::cout << "Command unknown" << std::endl;
        break;
    }
}