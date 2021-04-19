#ifndef UNSETENV_COMMAND_H
#define UNSETENV_COMMAND_H

#include "command.h"
#include "string_list.h"

extern const struct unsetenv_command {
    Command* (*withArgs)(StringList* arguments);
} UnsetenvCommand;

#endif
