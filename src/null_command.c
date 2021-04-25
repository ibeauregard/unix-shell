#include "null_command.h"

static Command* from_arguments(StringList* arguments);
const ConcreteCommandClass NullCommandClass = {
        .fromArguments = &from_arguments
};

static void execute(Command* this);
Command* from_arguments(StringList* arguments)
{
    (void) arguments;
    static Command null_command = {.execute = &execute};
    return &null_command;
}

void execute(Command* this)
{
    (void) this;
}
