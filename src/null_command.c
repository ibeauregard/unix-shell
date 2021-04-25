#include "null_command.h"
#include <stdlib.h>

static Command* with_args(StringList* arguments);
const struct null_command NullCommand = {
        .withArgs = &with_args
};

static void execute(Command* this);
Command* with_args(StringList* arguments)
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
