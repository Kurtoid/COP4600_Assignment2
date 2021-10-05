#include "commands.hpp"
#include <string>
struct HistoryEntry
{
    Command command;
    std::string args;
    std::string toString();
    static HistoryEntry loadFromString(std::string s);
};
void dumpHistoryToFile(std::string path, std::vector<HistoryEntry> entries);

std::vector<HistoryEntry> getHistoryFromFile(std::string path);