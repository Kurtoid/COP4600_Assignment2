#include <string>
#include <sys/types.h>
#include <filesystem>

pid_t startProgram(std::string command_string, std::filesystem::path &cwd);

void startProgramAndWait(std::string &command_string, std::filesystem::path &cwd);

void killProcess(pid_t pid);