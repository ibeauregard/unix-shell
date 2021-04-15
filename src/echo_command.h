#ifndef ECHO_COMMAND_H
#define ECHO_COMMAND_H

#include "command.h"

extern const struct echo_command {
    Command* (*withArgs)(char** argArray);
} EchoCommand;

#endif
