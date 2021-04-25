#ifndef NULL_COMMAND_H
#define NULL_COMMAND_H

#include "command.h"
#include "string_list.h"

extern const struct null_command {
    Command* (*withArgs)(StringList* arguments);
} NullCommand;

#endif
