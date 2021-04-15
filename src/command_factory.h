#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include "command_line.h"
#include "command.h"

extern const struct command_factory {
    Command* (*fromCommandLine)(CommandLine* line);
} CommandFactory;

#endif
