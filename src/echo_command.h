#ifndef ECHO_COMMAND_H
#define ECHO_COMMAND_H

#include "command.h"
#include "string_list.h"

extern const struct echo_command {
    Command* (*withArgs)(StringList* arguments);
} EchoCommand;

#endif
