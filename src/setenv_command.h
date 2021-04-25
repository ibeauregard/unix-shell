#ifndef SETENV_H
#define SETENV_H

#include "command.h"
#include "string_list.h"

extern const struct setenv_command {
    Command* (*fromArguments)(StringList* arguments);
} SetenvCommand;

#endif
