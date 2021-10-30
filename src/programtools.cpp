#include <string>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <csignal>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

// = = = = = = = = = = Author: Kurt Wilson = = = = = = = = = = 
char** split_args(std::vector<char *> &args_out, std::string s)
{
    std::vector<std::string> args;
    size_t pos;
    std::string token;
    std::string delim = " ";

    while ((pos = s.find(delim)) != std::string::npos)
    {
        token = s.substr(0, pos);
        args.push_back(token);
        s.erase(0, pos + delim.length());
    }
    args.push_back(s);

    for(int i = 0; i < args.size(); i++)
    {
        // = = = = = = = = = = Author: Joseph Terribile = = = = = = = = = =
        // ha ha ha ha const cast go brrrrrrrrrr
        args_out.push_back(const_cast<char*>(args[i].c_str()));
        // = = = = = = = = = = END Author: Joseph Terribile = = = = = = = = = =
    }
    args_out.push_back(NULL);
    return &(args_out[0]);
}
// = = = = = = = = = = END Author: Kurt Wilson = = = = = = = = = = 

// = = = = = = = = = = Author: Joseph Terribile = = = = = = = = = =
pid_t startProgram(std::string command_string, std::filesystem::path &cwd)
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

    pid_t pid = fork();
    if (pid != 0)
    {
        return pid;
    }

    if (chdir(cwd.c_str()) == -1)
    {
        std::cout << std::strerror(errno) << std::endl;
        // std::cout << "Bailing out, you are on your own!" << std::endl;
        exit(-1);
    }
    
    // process the args in a complexly fun way
    std::vector<char *> argVec;
    char** argArr = split_args(argVec, command_string);

    // run it with exec
    execvp(exe.c_str(), argArr);
    
    // if we are here we can basically assume that exec failed
    std::cout << std::strerror(errno) << std::endl;
    // std::cout << "Bailing out, you are on your own!" << std::endl;
    exit(-1);
}

void startProgramAndWait(std::string &command_string, std::filesystem::path &cwd)
{
    pid_t pid = startProgram(command_string, cwd);
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
    int result = kill(pid, SIGKILL);
    // should we test this with a root-level task?
    if (result == -1)
    {
        std::cout << "Problem killing " << std::to_string(pid) << ": " << std::strerror(errno) << std::endl;
        return;
    }

    int status;
    waitpid(pid, &status, 0);
}
// = = = = = = = = = = END Author: Joseph Terribile = = = = = = = = = =