#include "echo_command.h"
#include <stdlib.h>
#include <stdio.h>

static Command* with_args(StringList* arguments);
const struct echo_command EchoCommand = {
        .withArgs = &with_args
};

struct internals {
    StringList* arguments;
};

static void execute(Command* this);
Command* with_args(StringList* arguments)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->arguments = arguments;
    this->execute = &execute;
    return this;
}

static void delete(Command** this);
void execute(Command* this)
{
    StringList* arguments = this->_internals->arguments;
    free(arguments->next(arguments)); // do not print built-in name
    while (!arguments->isEmpty(arguments)) {
        char* argument = arguments->next(arguments);
        printf("%s ", argument);
        free(argument);
    }
    puts("");
    delete(&this);
}

void delete(Command** this)
{
    free((*this)->_internals);
    free(*this); *this = NULL;
}
