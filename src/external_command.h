#ifndef EXTERNAL_COMMAND_H
#define EXTERNAL_COMMAND_H

#include "command.h"
#include "string_list.h"

extern const struct external_command {
    Command* (*fromArguments)(StringList* arguments);
} ExternalCommand;

#endif
