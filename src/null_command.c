#include "null_command.h"
#include <stdlib.h>

static Command* from_arguments(StringList* arguments);
const struct null_command NullCommand = {
        .fromArguments = &from_arguments
};

static void execute(Command* this);
Command* from_arguments(StringList* arguments)
{
    (void) arguments;
    Command* this = malloc(sizeof (Command));
    this->execute = &execute;
    return this;
}

void execute(Command* this)
{
    free(this);
}
