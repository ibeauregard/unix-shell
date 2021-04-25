#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include "concrete_command_class.h"
#include "command_line.h"

extern const struct command_factory {
    Command* (*fromCommandLine)(CommandLine* line);
} CommandFactory;

#endif
