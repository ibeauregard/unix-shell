#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include "command.h"
#include "string_list.h"

extern const struct exit_command {
    Command* (*withArgs)(StringList* arguments);
} ExitCommand;

#endif
