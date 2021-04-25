#ifndef CONCRETE_COMMAND_CLASS_SELECTOR_H
#define CONCRETE_COMMAND_CLASS_SELECTOR_H

#include "concrete_command_class.h"

extern const struct concrete_command_class_selector {
    const ConcreteCommandClass* (*fromCommandName)(char* name);
} ConcreteCommandClassSelector;

#endif
