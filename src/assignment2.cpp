#include <iostream>
#include <filesystem>
#include "commands.hpp"

#define DEBUG

struct systemState
{
    std::filesystem::path current_path;
};

int main()
{
    systemState current_state;
    current_state.current_path = std::filesystem::current_path();
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
}