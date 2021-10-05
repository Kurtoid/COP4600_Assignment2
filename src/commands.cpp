#include "commands.hpp"
#include <iostream>
#include <algorithm>
#include <string>

std::string toLower(std::string data)
{
    // TODO: this is passing by value, not reference, which could be slow on large inputs
    std::transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return data;
}

Command getCommandFromPrompt(std::string &args)
{
    std::cout << "# ";
    std::string cmd_str;
    if (!std::getline(std::cin, cmd_str))
    {
        return Command::BYEBYE;
    }

    // find first space
    std::size_t arg_pos = cmd_str.find(" ");
    if (arg_pos != std::string::npos)
    {
        // this is a copy
        args = cmd_str.substr(arg_pos + 1, cmd_str.length());
    }

    std::string cmd_str_lower = toLower(cmd_str.substr(0, arg_pos));
    return findCommand(cmd_str_lower);
}
Command findCommand(std::string s)
{
    if (s == "movetodir")
    {
        return Command::MOVETODIR;
    }
    else if (s == "whereami")
    {
        return Command::WHEREAMI;
    }
    else if (s == "history")
    {
        return Command::HISTORY;
    }
    else if (s == "byebye")
    {
        return Command::BYEBYE;
    }
    else if (s == "replay")
    {
        return Command::REPLAY;
    }
    else if (s == "start")
    {
        return Command::START;
    }
    else if (s == "background")
    {
        return Command::BACKGROUND;
    }
    else if (s == "dalek")
    {
        return Command::DALEK;
    }
    else if (s == "repeat")
    {
        return Command::REPEAT;
    }
    else if (s == "dalekall")
    {
        return Command::DALEKALL;
    }
    else
    {
        return Command::NONE;
    }
}
std::string commandToString(Command command)
{
    switch (command)
    {
    case Command::MOVETODIR:
        return "movetodir";
    case Command::WHEREAMI:
        return "whereami";
    case Command::HISTORY:
        return "history";
    case Command::BYEBYE:
        return "byebye";
    case Command::REPLAY:
        return "replay";
    case Command::START:
        return "start";
    case Command::BACKGROUND:
        return "background";
    case Command::DALEK:
        return "dalek";
    case Command::REPEAT:
        return "repeat";
    case Command::DALEKALL:
        return "dalekall";
    case Command::NONE:
    default:
        return "NOSUCHCOMMAND";
    }
}