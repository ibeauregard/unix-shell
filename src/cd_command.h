#ifndef CD_COMMAND_H
#define CD_COMMAND_H

#include "command.h"
#include "string_list.h"

extern const struct cd_command {
    Command* (*fromArguments)(StringList* arguments);
} CdCommand;

#endif
