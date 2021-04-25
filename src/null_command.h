#ifndef NULL_COMMAND_H
#define NULL_COMMAND_H

#include "command.h"
#include "string_list.h"

extern const struct null_command {
    Command* (*fromArguments)(StringList* arguments);
} NullCommand;

#endif
