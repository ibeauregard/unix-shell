#include "concrete_command_class_factory.h"
#include "null_command.h"
#include "echo_command.h"
#include "setenv_command.h"
#include "unsetenv_command.h"
#include "cd_command.h"
#include "env_command.h"
#include "exit_command.h"
#include "external_command.h"
#include <string.h>

static const ConcreteCommandClass* from_command_name(char* name);
const struct concrete_command_class_factory ConcreteCommandClassFactory = {
        .fromCommandName = &from_command_name
};

const ConcreteCommandClass* from_command_name(char* name)
{
    if (!name) {
        return &NullCommandClass;
    } if (!strcmp(name, "echo")) {
        return &EchoCommandClass;
    } if (!strcmp(name, "setenv")) {
        return &SetenvCommandClass;
    } if (!strcmp(name, "unsetenv")) {
        return &UnsetenvCommandClass;
    } if  (!strcmp(name, "cd")) {
        return &CdCommandClass;
    } if (!strcmp(name, "env")) {
        return &EnvCommandClass;
    } if (!strcmp(name, "exit")) {
        return &ExitCommandClass;
    } else {
        return &ExternalCommandClass;
    }
}
