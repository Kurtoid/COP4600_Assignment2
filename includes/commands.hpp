#include <string>
enum class Command
{
    MOVETODIR,
    WHEREAMI,
    HISTORY,
    BYEBYE,
    REPLAY,
    START,
    BACKGROUND,
    DALEK,
    REPEAT,
    DALEKALL,
    NONE,

};

Command findCommand(std::string s);

Command getCommandFromPrompt(std::string &string_result);