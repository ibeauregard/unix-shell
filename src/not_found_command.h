#ifndef NOT_FOUND_COMMAND_H
#define NOT_FOUND_COMMAND_H

#include "command.h"

extern const struct not_found_command {
    Command* (*withName)(char* name);
} NotFoundCommand;

#endif
