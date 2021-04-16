#ifndef EXTERNAL_COMMAND_H
#define EXTERNAL_COMMAND_H

#include "command.h"
#include "command_line.h"

extern const struct external_command {
    Command* (*fromCommandLine)(CommandLine* commandLine);
} ExternalCommand;

#endif
