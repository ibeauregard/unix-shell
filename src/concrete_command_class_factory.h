#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include "concrete_command_class.h"

extern const struct concrete_command_class_factory {
    const ConcreteCommandClass* (*fromCommand)(char* command);
} ConcreteCommandClassFactory;

#endif
