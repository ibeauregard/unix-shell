#ifndef CONCRETE_COMMAND_CLASS_H
#define CONCRETE_COMMAND_CLASS_H

#include "command.h"
#include "string_list.h"

typedef struct concrete_command_class {
    Command* (*fromArguments)(StringList* arguments);
} ConcreteCommandClass;

#endif
