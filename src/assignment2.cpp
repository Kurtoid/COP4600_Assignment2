#include <iostream>
#include <filesystem>
#include <vector>
#include "commands.hpp"
#include "history.hpp"

struct systemState
{
    std::filesystem::path current_path;
    std::vector<HistoryEntry> history;
};

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
        current_state.history.push_back(e);
        std::cout << (int)command << std::endl;
        switch (command)
        {
        case Command::MOVETODIR:

            break;
        case Command::BYEBYE:

            // TODO: dump history

            break;
        default:
            std::cout << "Command unknown" << std::endl;
            break;
        }

        lastCommand = command;
    } while (lastCommand != Command::BYEBYE);

    dumpHistoryToFile("history.txt", current_state.history);
}