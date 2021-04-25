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

static const ConcreteCommandClass* from_command(char* command);
const struct concrete_command_class_factory ConcreteCommandClassFactory = {
        .fromCommand = &from_command
};

const ConcreteCommandClass* from_command(char* command)
{
    if (!command) {
        return &NullCommandClass;
    } if (!strcmp(command, "echo")) {
        return &EchoCommandClass;
    } if (!strcmp(command, "setenv")) {
        return &SetenvCommandClass;
    } if (!strcmp(command, "unsetenv")) {
        return &UnsetenvCommandClass;
    } if  (!strcmp(command, "cd")) {
        return &CdCommandClass;
    } if (!strcmp(command, "env")) {
        return &EnvCommandClass;
    } if (!strcmp(command, "exit")) {
        return &ExitCommandClass;
    } else {
        return &ExternalCommandClass;
    }
}
