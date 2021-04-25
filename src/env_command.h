#ifndef ENV_COMMAND_H
#define ENV_COMMAND_H

#include "command.h"
#include "string_list.h"

extern const struct env_command {
    Command* (*fromArguments)(StringList* arguments);
} EnvCommand;

#endif
