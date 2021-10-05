#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "history.hpp"

std::string HistoryEntry::toString()
{
    std::string args_escaped;
    for (char &c : this->args)
    {
        if (c == '\"')
        {
            args_escaped += "\\\"";
        }
        else
        {
            args_escaped += c;
        }
    }
    std::cout << "command: " << std::to_string((int)this->command) << std::endl;
    return std::to_string((int)this->command) + " " + args_escaped;
}

HistoryEntry HistoryEntry::loadFromString(std::string s)
{
    size_t first_space_pos = s.find(' ');
    int command_idx = std::stoi(s.substr(0, first_space_pos));
    std::string args = s.substr(first_space_pos, s.length());
    HistoryEntry e;
    e.args = args;
    e.command = (Command)command_idx;
    return e;
}

void dumpHistoryToFile(std::string path, std::vector<HistoryEntry> entries)
{
    std::ofstream file;
    file.open(path);
    for (HistoryEntry entry : entries)
    {
        file << entry.toString() << std::endl;
    }
}
std::vector<HistoryEntry> getHistoryFromFile(std::string path)
{
    std::ifstream file;
    file.open(path);
    if (!file.is_open())
    {
        return std::vector<HistoryEntry>();
    }
    std::string line;
    std::vector<HistoryEntry> entries;
    while (getline(file, line))
    {
        HistoryEntry e = HistoryEntry::loadFromString(line);
        // std::cout << (int)e.command << " " << e.args << std::endl;
        entries.push_back(e);
    }
    return entries;
}