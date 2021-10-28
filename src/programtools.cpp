#include <string>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <csignal>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

char** split_args(std::string s){
    std::vector<std::string> args;
    std::vector<char *> args_out;
    size_t pos;
    std::string token;
    std::string delim = " ";
    while ((pos = s.find(delim)) != std::string::npos)
    {
        token = s.substr(0, pos);
        args.push_back(token);
        s.erase(0, pos + delim.length());
    }
    for(int i = 0; i < args.size(); i++)
    {
        // ha ha ha ha const cast go brrrrrrrrrr
        args_out.push_back(const_cast<char*>(args[i].c_str()));
    }
    return &(args_out[0]);
}

pid_t startProgram(std::string command_string)
{
    size_t exeEnd = command_string.find(' ');
    std::string exe = command_string.substr(0, exeEnd);
    std::string args;
    if (exeEnd == std::string::npos)
    {
        args = "";
    }
    else
    {
        args = command_string.substr(exeEnd + 1, command_string.length());
    }

    // verify that the exe exists
    if (!std::filesystem::exists(exe))
    {
        std::cout << "Couldn't run " << exe << std::endl;
        return -1;
    }

    pid_t pid = fork();
    if (pid != 0)
    {
        return pid;
    }
    
    // run it with exec
    char** argArr = split_args(args);
    execvp(exe.c_str(), argArr);
    
    // if we are here we can basically assume that exec failed
    // this obviously is bad
    std::cout << std::strerror(errno) << std::endl;
    std::cout << "Bailing out, you are on your own!" << std::endl;
    exit(-1);
}

void startProgramAndWait(std::string &command_string)
{
    pid_t pid = startProgram(command_string);
    if (pid == -1)
    {
        return;
    }
    int status;
    do
    {
        waitpid(pid, &status, 0);
        // waitpid will return on _any_ status change, but we're only concerned about exits
    } while (!WIFEXITED(status));
    // int return_status = WEXITSTATUS(status);
}

void killProcess(pid_t pid)
{
    // TODO testing/validation
    kill(pid, SIGKILL);
}