#include <string>
#include <sys/types.h>

pid_t startProgram(std::string command_string);

void startProgramAndWait(std::string &command_string);

void killProcess(pid_t pid);