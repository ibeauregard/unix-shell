#include "exit_command.h"
#include <stdlib.h>

static Command* from_arguments(StringList* arguments);
const ConcreteCommandClass ExitCommandClass = {
        .fromArguments = &from_arguments
};

struct internals {
    unsigned char status;
};

static void execute(Command* this);
Command* from_arguments(StringList* arguments)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = malloc(sizeof (struct internals));
    free(arguments->next(arguments)); // discard command name ("exit")
    if (!arguments->isEmpty(arguments)) {
        char* argument = arguments->next(arguments);
        this->_internals->status = atoi(argument);
        free(argument);
    } else {
        this->_internals->status = 0;
    }
    this->execute = &execute;
    return this;
}

void execute(Command* this)
{
    exit(this->_internals->status);
}
